#!/bin/bash

set -x
if [[ -z "$1" || ! -f "$1" ]]
then
    echo "File does not exist"
    exit
fi


./cmd/lex.sh $1 ${1%.*}.tkl
./cmd/symboltable.sh $1 ${1%.*}.csv
./cmd/parse.sh ${1%.*}.tkl
./cmd/visualize.sh
