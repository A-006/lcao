#!/bin/bash
cmake -B build
cmake --build build -j50
cd build
