#!/bin/sh
if [ ! -d target ]; then
	mkdir target
fi


if [[ $# == 1 && $1 == "bib" ]]; then
    pdflatex -output-directory target/ src/main.tex
    bibtex target/main
    pdflatex -output-directory target/ src/main.tex
fi

pdflatex -output-directory target/ src/main.tex

#if [ -f target/main.pdf ]; then
#	bibtex target/main.aux
#	pdflatex -output-directory target/ src/main.tex
#	pdflatex -output-directory target/ src/main.tex
#fi

