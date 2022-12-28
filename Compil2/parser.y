%{
  #include <stdio.h>
  #include <ctype.h>
  #include <unistd.h>

  #include "asa.h"
  #include "ts.h"

  extern int yylex();

%}

%union{
  int nb;
  char id[32];
  struct asa * noeud;
 };

%define parse.error verbose

%token DEBUT FIN VAR AFFI LIR SI ALORS FSI SINON ALGO ENTREE TQ FAIRE FTQ
%token <nb> NB
%token <id> ID

%type <noeud> EXP INSTS INST ENTRE CHAI


%left AFF
%left '+' '-'
%left '*' '/' '%'

%start PROG

%%

PROG : DEBUT INSTS FIN                        { codegen($2); printf("STOP\n"); }
| ALGO ID COMS ENTRE DEBUT INSTS FIN          { codegen($4); codegen($6); printf("STOP\n"); }
| ALGO ID ENTRE DEBUT INSTS FIN               { codegen($3); codegen($5); printf("STOP\n"); }
| ALGO ID COMS DEBUT INSTS FIN                { codegen($5); printf("STOP\n"); }
| ALGO ID DEBUT INSTS FIN                     { codegen($4); printf("STOP\n"); }
| COMS ENTRE DEBUT INSTS FIN                  { codegen($2); codegen($4); printf("STOP\n"); }
| ENTRE DEBUT INSTS FIN                       { codegen($1); codegen($3); printf("STOP\n"); }
;

COMS : COMS COM
| COM
;

COM : '#' ID
;

ENTRE : ENTREE CHAI        { $$ = $2; }
;

CHAI : CHAI ID             { if(tsymb == NULL) ts_init_table($2, 1);
                             else ts_ajouter_id($2, 1);
                             $$ = creer_noeudInsts($1, creer_noeud_lire(creer_feuilleId($2))); }
| ID                       { if(tsymb == NULL) ts_init_table($1, 1);
                             else ts_ajouter_id($1, 1);
                             $$ = creer_noeudInsts(creer_noeud_lire(creer_feuilleId($1)), NULL); }
;

INSTS : INSTS INST         { $$ = creer_noeudInsts($1, $2); }
|INST                      { $$ = creer_noeudInsts($1, NULL); }
;

INST: EXP ';'              { $$ = creer_noeudInsts($1, NULL); }
| VAR ID ';'               { if(tsymb == NULL) ts_init_table($2, 1);
                             else ts_ajouter_id($2, 1);
                             $$ = creer_noeudInsts(creer_feuilleId($2), NULL); }
| AFFI EXP ';'             { $$ = creer_noeud_affi($2); }
| SI EXP ALORS INSTS FSI';'{ $$ = creer_noeud_si($2, $4); }
| SI EXP ALORS INSTS SINON INSTS FSI';' { $$ = creer_noeud_sinon($2,$4,$6); }
| TQ EXP FAIRE INSTS FTQ ';' { $$ = creer_noeudTQ($2, $4); }
;

EXP : NB                   { $$ = creer_feuilleNb(yylval.nb); }
| EXP '+' EXP              { $$ = creer_noeudOp('+', $1, $3); }
| EXP '-' EXP              { $$ = creer_noeudOp('-', $1, $3); }
| EXP '*' EXP              { $$ = creer_noeudOp('*', $1, $3); }
| EXP '/' EXP              { $$ = creer_noeudOp('/', $1, $3); }
| EXP '%' EXP              { $$ = creer_noeudOp('%', $1, $3); }
| EXP '<' EXP              { $$ = creer_noeudOp('<', $1, $3); }
| EXP '>' EXP              { $$ = creer_noeudOp('>', $1, $3); }
| '!' EXP                  { $$ = creer_noeudOp('!', $2, NULL); }
| EXP '=' EXP              { $$ = creer_noeudOp('=', $1, $3); }
//| EXP "<=" EXP             { $$ = creer_noeudOp('<=', $1, $3); }
//| EXP ">=" EXP             { $$ = creer_noeudOp('>=', $1, $3); }
| '(' EXP ')'              { $$ = $2; }
| ID AFF EXP               { $$ = creer_noeudOp('e', creer_feuilleId($1), $3); }
| ID                       { $$ = creer_feuilleId(yylval.id); }

;

%%

int main( int argc, char * argv[] ) {

  extern FILE *yyin;
  if (argc == 1){
    fprintf(stderr, "aucun fichier fourni\n");
    return 1;
  }
  yyin = fopen(argv[1],"r");
  yyparse();
  return 0;
}
