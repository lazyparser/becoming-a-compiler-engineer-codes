default: build check

build:
    g++ PL0-wordsyntax-analysis.cpp -o PL0-wordsyntax-analysis

check:
    ./PL0-wordsyntax-analysis

.PHONY: default build check