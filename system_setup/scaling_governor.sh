#!/usr/bin/env bash

sudo cpupower frequency-set --governor performance
cpupower frequency-info -o proc
