%{
int nlines, nchars, scope = 0;
void variables(int l, int c, int scope, const char *s);
%}


b "\{"
sp [\t" "\n]
d "int"|"real"|"char"
ignore "main"|"if"|"then"|"else"|"while"|"cout"|"cin"
v [a-zA-Z][a-zA-Z0-9]*
int [0-9]+
real [0-9]+"."[0-9]+
char \'.\'
n (\-?{int}|{real})|{char}

%%
\n {
    nchars=0;
    nlines++;
}

{ignore} {
    nchars+=yyleng;
}

{b} {
    nchars+=yyleng;
    scope++;
}


{d}{sp}+{v}({sp}*,{sp}*{v})*\; {
    variables(nlines+1, nchars+1, scope, yytext);
    nchars+=yyleng;
}

{n} {
    fprintf(yyout, "%d,%d,%s,constant,\n", nlines+1, nchars+1, yytext);
    nchars+=yyleng;
}

. {
    nchars++;
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
    fprintf(yyout, "Line,Column,Value,Type,Scope\n");
    yylex();

    return 0;
}

void variables(int l, int c, int scope, const char *s){
    char *dt, *var;
    dt = (char *)malloc(5 * sizeof(char));
    var = (char *)malloc(33 * sizeof(char));
    int i = 0, k = 0;
    while(s[i] != ' '){
        dt[i] = s[i];
        i++;
    }
    dt[i] = '\0';
    while(s[i] == ' ' || s[i] == '\t' || s[i] == '\n'){
        i++;
    }
    while(s[i] != '\0'){
        if(s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == ',' || s[i] == ';'){
            if(k != 0){
                var[k] = '\0';
                fprintf(yyout, "%d,%d,%s,%s,%d\n", l, c+i-k, var, dt, scope);
            }
            k = 0;
        }
        else{
            var[k++] = s[i];
        }
        i++;
    }
}
