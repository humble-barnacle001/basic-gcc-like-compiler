#!/bin/bash
if [[ -z "$1" || ! -f ./out/lex/$1 ]]
then
    echo "File does not exist"
    exit
fi

echo "Start compiling parser..."
g++ ./parser/parser.cpp ./util/filereadwrite.cpp -o ./bin/parser.bin 
echo "Done compiling...running"
./bin/parser.bin ./out/lex/$1 > ./out/parser/parser.txt
echo "Done. Debug wrote to './out/parser/parser.txt'"
