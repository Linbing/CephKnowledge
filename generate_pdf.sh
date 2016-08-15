#!/usr/bin/env bash

pandoc --template=default --latex-engine=xelatex $1.md -o $1.pdf -V mainfont="STHeitiSC-Medium"
