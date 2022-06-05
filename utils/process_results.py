import dataclasses
import json
import logging
from collections import defaultdict
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
import streamlit as st
from bokeh.models import ColumnDataSource
from bokeh.palettes import Spectral6
from bokeh.plotting import figure
from bokeh.transform import factor_cmap

CURRENT_FILE = Path(__file__).resolve()
CURRENT_DIRECTORY = CURRENT_FILE.parent.resolve()
TOP_DIRECTORY = CURRENT_DIRECTORY.parent.resolve()
RESULTS_DIRECTORY = TOP_DIRECTORY / "results"


def remove_prefix(text, prefix):
    if text.startswith(prefix):
        return text[len(prefix) :]
    return text


st.title("Vector Addition Benchmark Results")


@st.cache
def load_data():
    with open(RESULTS_DIRECTORY / "output.json", "r") as f:
        data = json.load(f)
    return data


benchmark_data = load_data()

st.caption(f"Builded as {benchmark_data['context']['library_build_type']}")

st.header("Full benchmark data")

with st.expander("See details"):
    st.json(benchmark_data)

st.header("CPU information")

st.markdown(f"`Hostname`: {benchmark_data['context']['host_name']}")
st.markdown(f"`N° cpu(s)`: {benchmark_data['context']['num_cpus']}")
st.markdown(f"`MHz/cpu`: {benchmark_data['context']['mhz_per_cpu']}")
st.markdown(
    f"`CPU Scaling enabled`: {benchmark_data['context']['cpu_scaling_enabled']}"
)

st.subheader("Caches info")

caches = benchmark_data["context"]["caches"]
caches_column = st.columns(len(caches))

# Get cache and index
for index, cache in enumerate(caches_column):
    cache.metric(
        f"Level {caches[index]['level']}",
        f"{caches[index]['type']}",
        f"{caches[index]['size']} bits",
    )


def separate_benchmark_data(benchmarks):
    """
    Separate the benchmark data into a dictonary.
    """

    separated_benchmarks = {}
    for benchmark in benchmarks:
        if "VectorAdd_" not in benchmark["run_name"]:
            continue
        if "aggregate_name" in benchmark or "repetition_index" not in benchmark:
            continue
        name, size, real_time = benchmark["run_name"].split("/")
        if real_time != "real_time":
            continue
        name = remove_prefix(name, "VectorAdd_")
        if "<" in name:
            parts = name.split("<")
            name = parts[0]
        if "alignment" in benchmark:
            name += f";alignment={int(benchmark['alignment'])}"
        if "tile_factor" in benchmark:
            name += f";tile_factor={int(benchmark['tile_factor'])}"
        if "unroll_factor" in benchmark:
            name += f";unroll_factor={int(benchmark['unroll_factor'])}"
        if "simd_width" in benchmark:
            name += f";simd_width={int(benchmark['simd_width'])}"
        size = int(remove_prefix(size, "log2(N):"))
        if name not in separated_benchmarks:
            separated_benchmarks[name] = {}

        if size not in separated_benchmarks[name]:
            separated_benchmarks[name][size] = []
        # print("aggregate_name" in benchmark, "  ", real_time, "   ", benchmark)
        separated_benchmarks[name][size].append(benchmark["bytes/s"] / (2**30))

    return separated_benchmarks


separated_benchmarks = separate_benchmark_data(benchmark_data["benchmarks"])

sizes = defaultdict(dict)

# st.subheader("Comparison")
# for benchmark_name, benchmark_data in separated_benchmarks.items():
#     for size, data in benchmark_data.items():
#         sizes[size][benchmark_name] = np.mean(data)
# chart_data = pd.DataFrame(sizes)
# st.dataframe(chart_data)

# p = figure(width=600, height=300, title="Years vs mpg without jittering")
# p.xgrid.grid_line_color = None
# p.xaxis.ticker = list(sizes.keys())
# source = ColumnDataSource(
#     data=dict(name=list(sizes.keys()), values=list(sizes.values()))
# )
# p.scatter(x="name", y="values", size=9, alpha=0.4, source=source)
# st.bokeh_chart(p, use_container_width=True)

# for size in sizes.keys():
#     st.subheader(f"Size {size}")
#     vals = sizes.get(size)
#     x = list(vals.keys())
#     y = list(vals.values())

#     data = ColumnDataSource(data=dict(name=x, values=y))
#     print(data)

#     p = figure(x_range=x, height=350, toolbar_location=None, title="Fruit Counts")

#     p.vbar(
#         x="name",
#         top="counts",
#         width=0.9,
#         source=data,
#         line_width=2,
#         legend_field="name",
#         line_color="white",
#         fill_color=factor_cmap("name", palette=Spectral6, factors=x),
#     )

#     st.bokeh_chart(p, use_container_width=True)

st.subheader("Benchmark results")
for benchmark_name, benchmark_data in separated_benchmarks.items():
    st.header(f"{benchmark_name}")

    # Create a chart for each operation where the x axis is the size of the matrix and the y axis is the time
    sizes = []
    bytes_per_sec = []

    for size, data in benchmark_data.items():
        sizes.append(int(size))

        operation_times = []
        for time in data:
            operation_times.append(time)

        bytes_per_sec.append(operation_times)

    chart_data = pd.DataFrame(bytes_per_sec, index=sizes)

    st.line_chart(chart_data)
