# basic-gcc-like-compiler

Designing a C like compiler

## File Structure

-   `cmd` - All commands. **Must be run from root only**
-   `grammar` - Grammar rules and descriptions
-   `lexer` - Lex file. Reads the input and writes the tokens to `out/lex/<input filename>.tkl`
-   `parser` - Parser program files
-   `results` - Parsing table, Transition graph, Graph visualization
-   `symbol table` - Symbol table generating program and writes the tokens to `out/symbol table/<input filename>.csv`
-   `util` - Utility Cpp program to prepare for read and write to files
-   `out` - Output files from the lexer, parser, and symbol table

## Run

-   Use a bash like shell
-   Run `./run.sh`
-   Individual commands may be run as `./cmd/<command>.sh`

## Individual Commands in "cmd" directory

-   `lex.sh` - Must be supplied with a file to lex
-   `symboltable.sh` - Must be supplied with a file to generate the symbol table
-   `parse.sh` - Run the parser
-   `visualize.sh` - Visualize the transition graph (saved in `results/graph.svg`)

## Transitions

![](results/graph.svg)

