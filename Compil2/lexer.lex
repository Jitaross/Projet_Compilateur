%{
#include <string.h>
#include "parser.h"
%}

%option nounput
%option noinput

NOMBRE   [1-9][0-9]*
OPE      [+\-*/%<>=!]
PA       [()]
IDE      [a-zA-Z][0-9a-z]*
BLANC    [ \t\n]

%%

"DEBUT"       { return DEBUT; }
"FIN"         { return FIN; }
";"           { return yytext[0]; }
"#"           { return yytext[0]; }
"ALGO"        { return ALGO; }
"VAR"         { return VAR; }
"AFFICHER"    { return AFFI; }
"ENTREE"      { return ENTREE; }
"TQ"          { return TQ; }
"TANT QUE"    { return TQ; }
"FAIRE"       { return FAIRE; }
"FTQ"         { return FTQ; }
{IDE}         { strcpy(yylval.id, yytext); return ID; }
"SI"          { return SI; }
"ALORS"       { return ALORS; }
"SINON"       { return SINON; }
"FSI"         { return FSI; }
"<-"          { return AFF; }
"<="          { return yytext[0]; }
">="          { return yytext[0]; }
{NOMBRE}      { yylval.nb = atoi(yytext); return NB; }
{OPE}         { return yytext[0]; }
{PA}          { return yytext[0]; }
{BLANC}       { }
.             { printf("lexer error : caractere inconnu : %c", yytext[0]); exit(1); }

%%
