#!/bin/sh

if [[ $# -lt 1 ]]; then
    echo "$0 file.tex [directory_name]" 1>&2
    exit 1
fi

TEX_FILE=$1
TARGET_DIR=target

if [[ $# -eq 2 ]]; then
    TARGET_DIR=$2
fi

if [ ! -e build.sh ]; then
    echo '#!/bin/sh
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

pdflatex -output-directory $TARGET_DIR $TEX_FILE
' > build.sh
fi

chmod 755 build.sh

./build.sh $TEX_FILE $TARGET_DIR
bibtex "$TARGET_DIR/$(echo $TEX_FILE | awk -F '.' '{print$1}')"
./build.sh $TEX_FILE $TARGET_DIR
./build.sh $TEX_FILE $TARGET_DIR
