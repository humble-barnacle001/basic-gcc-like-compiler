#!/bin/bash

if ! command -v dot &> /dev/null
then
    sudo apt install graphviz -y
fi

echo "Generating plots..."
g++ ./visualizer/visualize.cpp ./util/filereadwrite.cpp -o ./bin/visualize.bin
./bin/visualize.bin > ./out/parser/visualize.txt 
./visualizer/transition.py
echo "Done. Saved to './results/graph.dot'"
if ! command -v dot &> /dev/null
then
    echo "Graphviz not installed. Please install graphviz."
else
    echo "Building visulization..."
    dot -Tsvg ./results/graph.dot -o ./results/graph.svg
    echo "Done. Saved to './results/graph.svg'"
fi
