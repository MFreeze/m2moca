#!/bin/sh
./build.sh
bibtex target/main.aux
./build.sh

