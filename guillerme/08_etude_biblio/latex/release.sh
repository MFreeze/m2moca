#!/bin/sh
./build.sh
bibtex target/rapport.aux
./build.sh
./build.sh

