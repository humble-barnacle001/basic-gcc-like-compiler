%{
    int rc = 0;
%}

m "main"
b "\{"
c "\}"
a "+"|"-"|"*"|"/"|"%"
r "<"|">"|"<="|">="|"=="|"!="
d "int"|"real"|"char"
i "if"
t "then"
e "else"
w "while"
f "("
g ")"
sp [\t" "\n]
p "cout"{sp}*"<<"
s "cin"{sp}*">>"
z ","
v [a-zA-Z][a-zA-Z0-9]*
int [0-9]+
real [0-9]+"."[0-9]+
char \'.\'
n (\-?{int}|{real})|{char}

%%
{a}{sp}* {
    fprintf(yyout, "%c", 'a');
}

{b}{sp}* {
    fprintf(yyout, "%c", 'b');
}

{c}{sp}* {
    fprintf(yyout, "%c", 'c');
}

{e}{sp}* {
    fprintf(yyout, "%c", 'e');
}

{f}{sp}* {
    fprintf(yyout, "%c", 'f');
}

{g}{sp}* {
    fprintf(yyout, "%c", 'g');
}

{i}{sp}* {
    fprintf(yyout, "%c", 'i');
}

{m}{sp}* {
    fprintf(yyout, "%c", 'm');
}

{p}{sp}* {
    fprintf(yyout, "%c", 'p');
}

{r}{sp}* {
    fprintf(yyout, "%c", 'r');
}

{s}{sp}* {
    fprintf(yyout, "%c", 's');
}

{t}{sp}* {
    fprintf(yyout, "%c", 't');
}

{w}{sp}* {
    fprintf(yyout, "%c", 'w');
}

{z}{sp}* {
    fprintf(yyout, "%c", 'z');
}

[;=] {
    fprintf(yyout, "%s", yytext);
}

{d} {
    fprintf(yyout, "%c", 'd');
}

{v} {
    fprintf(yyout, "%c", 'v');
}

{n} {
    fprintf(yyout, "%c", 'n');
}

{sp}* ;

. {
    rc = 1;
    printf("Error in lexing, deleting character: %s\n", yytext);
}
%%

int yywrap(void)  
{
    return 1;
}

int main(int argc, char*argv[]) 
{
    yyin=fopen(argv[1],"r");
    yyout=fopen(argv[2],"w");
    yylex();
    
    return rc;
}