#!/usr/bin/env bash

pandoc --template=chinese.tex --latex-engine=xelatex $1.md -o $1.pdf
