#!/bin/bash
if ! command -v lex &> /dev/null
then
    sudo apt install -y flex
fi

if [[ -z "$1" || -z "$2" || ! -f "$1" || ! -f "./lexer/lexer.l" ]]
then
    echo "File does not exist"
    exit
fi

if [ -f "./bin/lexer.yy.c" ]
then
rm ./bin/lexer.yy.c
fi

if [ -f "./bin/lexer.bin" ]
then
rm ./bin/lexer.bin
fi

lex -o ./bin/lexer.yy.c ./lexer/lexer.l
cc ./bin/lexer.yy.c -o ./bin/lexer.bin
./bin/lexer.bin $1 ./out/lex/$2
[ $? -eq 0 ] && echo "Lexing Success"

