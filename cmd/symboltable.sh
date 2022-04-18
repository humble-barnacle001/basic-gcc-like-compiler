#!/bin/bash
if ! command -v lex &> /dev/null
then
    sudo apt install -y flex
fi

if [[ -z "$1" || -z "$2" || ! -f "$1" || ! -f "./symbol table/symboltable.l" ]]
then
    echo "File does not exist"
    exit
fi

if [ -f "./bin/symboltable.yy.c" ]
then
rm ./bin/symboltable.yy.c
fi

if [ -f "./bin/symboltable.bin" ]
then
rm ./bin/symboltable.bin
fi

lex -o ./bin/symboltable.yy.c "./symbol table/symboltable.l"
cc ./bin/symboltable.yy.c -o ./bin/symboltable.bin
./bin/symboltable.bin $1 "./out/symbol table/$2"

