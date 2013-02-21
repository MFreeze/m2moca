#!/bin/sh
if [[ $# -lt 1 ]]; then
    echo "$0 file.tex [directory_name]" 1>&2
fi

TEX_FILE=$1
TARGET_DIR=target

if [[ $# -eq 2 ]]; then
    TARGET_DIR=$2
fi

if [ ! -d $TARGET_DIR ]; then
    mkdir $TARGET_DIR
fi

pdflatex \
    -output-directory "$TARGET_DIR" \
    "$TEX_FILE"

