#!/usr/bin/env bash

pandoc --template=chinese.tex.bug --latex-engine=xelatex $1.md -o $1.pdf
