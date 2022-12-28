#include "asa.h"

int pile = 1;
int compteur = 0;
int cpttq, cptsinon;

asa * creer_feuilleNb(int val)
{
  asa *p;

  if ((p = malloc(sizeof(asa))) == NULL)
    yyerror("echec allocation mémoire creer_feuilleNb");

  p->type = typeNb;
  p->ninst = 1;
  p->nb.val = val;
  return p;
}

asa * creer_feuilleId( char* nom_id )
{
  asa *p;

  if((p = malloc(sizeof(asa))) == NULL) yyerror("echec de l'allocation memoire creer_feuilleId");

  p->type = typeId;
  p->ninst = 1;
  p->id.text = malloc(strlen(nom_id));
  p->id.tailleid = strlen(nom_id);
  strcpy(p->id.text, nom_id);
  return p;

}


asa * creer_noeudOp( int ope, asa * p1, asa * p2)
{
  asa * p;

  if ((p = malloc(sizeof(asa))) == NULL)
    yyerror("echec allocation mémoire creer_noeudOp");

  p->type = typeOp;
  p->op.ope = ope;
  p->op.noeud[0]=p1;
  p->op.noeud[1]=p2;
  p->ninst = p1->ninst + 2;
  if(p2) p->ninst += p2->ninst;
  switch(p->op.ope){
    case '<':
      p->ninst += 4;
      break;
    case '>':
      p->ninst += 4;
      break;
    case '=':
      p->ninst += 4;
      break;
    case '!':
      p->ninst += 3;
      break;
  }

  return p;
}

asa * creer_noeudInsts(asa * p1, asa * p2){
  asa * p;

  if((p = malloc(sizeof(asa))) == NULL) yyerror("echec alloc memoire creer_noeudInsts");

  p->type = typeinst;
  strcpy(p->insts.nom, "Insts");
  p->insts.noeud[0] = p1;
  p->insts.noeud[1] = p2;
  p->ninst = p1->ninst;
  if (p2) p->ninst += p2->ninst;

  return p;
}

asa * creer_noeud_affi(asa * p1){
  asa * p;
  
  if((p = malloc(sizeof(asa))) == NULL) yyerror("echec alloc memoire creer_noeud_affi");

  p->type = typeaffi;
  p->affi.noeud[0] = p1;
  p->ninst = p1->ninst + 1;
  if (p->affi.noeud[0]->type == typeId) p->ninst++;

  return p;
}

asa * creer_noeud_lire(asa * p1){
  asa * p;
  
  if((p = malloc(sizeof(asa))) == NULL) yyerror("echec alloc memoire creer_noeud_lire");

  p->type = typelire;
  p->affi.noeud[0] = p1;
  p->ninst = p1->ninst + 2;

  return p;
}

asa * creer_noeud_si(asa * p1, asa * p2){
  asa * p;

  if((p = malloc(sizeof(asa))) == NULL) yyerror("echec alloc memoire creer_noeud_si");

  p->type = typesi;
  p->si.noeud[0] = p1;
  p->si.noeud[1] = p2;
  p->ninst = p2->ninst + p1->ninst + 1;
  if(p1->type == typeId) p->ninst++;

  return p;
  
}

asa * creer_noeud_sinon(asa * p1, asa * p2, asa * p3){
  asa * p;

  if((p = malloc(sizeof(asa))) == NULL) yyerror("echec alloc memoire creer_noeud_si");

  p->type = typesinon;
  p->sinon.noeud[0] = p1;
  p->sinon.noeud[1] = p2;
  p->sinon.noeud[2] = p3;
  p->ninst = p2->ninst + p1->ninst + p3->ninst + 2;
  if(p1->type == typeId) p->ninst++;

  return p;
  
}

asa * creer_noeudTQ(asa * p1, asa * p2){
  asa * p;

  if((p = malloc(sizeof(asa))) == NULL) yyerror("echec alloc memoire creer_noeud_si");

  p->type = typeTQ;
  p->tq.noeud[0] = p1;
  p->tq.noeud[1] = p2;
  p->ninst = p2->ninst + p1->ninst + 2;
  if(p1->type == typeId) p->ninst++;

  return p;
  
}


void free_asa(asa *p)
{

  if (!p) return;
  switch (p->type) {
  case typeOp:
    free_asa(p->op.noeud[0]);
    free_asa(p->op.noeud[1]);
    break;
  default: break;
  }
  free(p);
}


void codegen(asa *p)
{
  if (!p) return;
  switch(p->type) {
  case typeNb:
    compteur+=2;
    printf("LOAD #%d\nSTORE %d\n", p->nb.val, pile);
    pile++;
    break;

  case typeOp:
    if (p->op.noeud[1]) codegen(p->op.noeud[1]);
    codegen(p->op.noeud[0]);
    if (p->op.noeud[0]->type == typeId){
      switch(p->op.ope){
        case '+':
          compteur += 3;
          printf("LOAD %d\nADD %d\nSTORE %d\n",ts_retrouver_id(tsymb, p->op.noeud[0]->id.text), pile - 1, pile - 2);
          pile--;
          break;
        case '-':
          compteur += 3;
          printf("LOAD %d\nSUB %d\nSTORE %d\n",ts_retrouver_id(tsymb, p->op.noeud[0]->id.text), pile - 1, pile - 2);
          pile--;
          break;
        case '*':
          compteur += 3;
          printf("LOAD %d\nMUL %d\nSTORE %d\n",ts_retrouver_id(tsymb, p->op.noeud[0]->id.text), pile - 1, pile - 2);
          pile--;
          break;
        case '/':
          compteur += 3;
          printf("LOAD %d\nDIV %d\nSTORE %d\n",ts_retrouver_id(tsymb, p->op.noeud[0]->id.text), pile - 1, pile - 2);
          pile--;
          break;
        case '%':
          compteur += 3;
          printf("LOAD %d\nMOD %d\nSTORE %d\n",ts_retrouver_id(tsymb, p->op.noeud[0]->id.text), pile - 1, pile - 2);
          pile--;
          break;
        case 'e':
          compteur += 2;
          printf("LOAD %d\nSTORE %d\n", pile - 1, ts_retrouver_id(tsymb, p->op.noeud[0]->id.text));
          break;
        case '<':
          compteur += 6;
          printf("LOAD %d\nSUB %d\nJUML %d\nLOAD #0\nJUMP %d\nLOAD #1\n", ts_retrouver_id(tsymb, p->op.noeud[0]->id.text), pile - 1, compteur - 1, compteur);
          break;
        case '>':
          compteur += 6;
          printf("LOAD %d\nSUB %d\nJUMG %d\nLOAD #0\nJUMP %d\nLOAD #1\n", ts_retrouver_id(tsymb, p->op.noeud[0]->id.text), pile - 1, compteur - 1, compteur);
          break;
        case '!':
          compteur += 5;
          printf("LOAD %d\nJUMG %d\nLOAD #1\nJUMP %d\nLOAD #0\n", ts_retrouver_id(tsymb, p->op.noeud[0]->id.text), compteur - 1, compteur);
          break;
        case '=':
          compteur += 6;
          printf("LOAD %d\nSUB %d\nJUMZ %d\nLOAD #0\nJUMP %d\nLOAD #1\n", ts_retrouver_id(tsymb, p->op.noeud[0]->id.text), pile - 1, compteur - 1, compteur);
          break;
    }
    break;
    }
    else {
      compteur += 2;
      switch(p->op.ope){
        case '+':
          printf("ADD %d\nSTORE %d\n",pile - 2, pile - 2);
          pile--;
          break;
        case '-':
          printf("SUB %d\nSTORE %d\n",pile - 2, pile - 2);
          pile--;
          break;
        case '*':
          printf("MUL %d\nSTORE %d\n",pile - 2, pile - 2);
          pile--;
          break;
        case '/':
          printf("DIV %d\nSTORE %d\n",pile - 2, pile - 2);
          pile--;
          break;
        case '%':
          printf("MOD %d\nSTORE %d\n",pile - 2, pile - 2);
          pile--;
          break;
        case '<':
          compteur += 4;
          printf("LOAD %d\nSUB %d\nJUML %d\nLOAD #0\nJUMP %d\nLOAD #1\n", pile - 2, pile - 1, compteur - 1, compteur);
          break;
        case '>':
          compteur += 4;
          printf("LOAD %d\nSUB %d\nJUMG %d\nLOAD #0\nJUMP %d\nLOAD #1\n", pile - 2, pile - 1, compteur - 1, compteur);
          break;
        case '!':
          compteur += 3;
          printf("LOAD %d\nJUMG %d\nLOAD #1\nJUMP %d\nLOAD #0\n", pile - 1, compteur - 1, compteur);
          break;
        case '=':
          compteur += 4;
          printf("LOAD %d\nSUB %d\nJUMZ %d\nLOAD #0\nJUMP %d\nLOAD #1\n", pile - 2, pile - 1, compteur - 1, compteur);
          break;
    }
    break;
    }
    
  case typeinst:
    codegen(p->insts.noeud[0]);
    if(!p->insts.noeud[1]) return;
    codegen(p->insts.noeud[1]);
    break;

  case typeaffi:
    codegen(p->affi.noeud[0]);
    if(p->affi.noeud[0]->type == typeId){
      printf("LOAD %d\n", ts_retrouver_id(tsymb, p->affi.noeud[0]->id.text));
      compteur++;
    } 
    printf("WRITE\n");
    compteur++;
    break;

  case typelire:
    compteur += 2;
    printf("READ\nSTORE %d\n", ts_retrouver_id(tsymb, p->lire.noeud[0]->id.text));
    break;

  case typesi:
    codegen(p->si.noeud[0]);
    if (p->si.noeud[0]->type == typeId) {
      printf("LOAD %d\nJUMZ %d\n", ts_retrouver_id(tsymb, p->si.noeud[0]->id.text), compteur + p->si.noeud[1]->ninst + 2);
      compteur++;
    }
    else printf("JUMZ %d\n", compteur + p->si.noeud[1]->ninst + 1);
    compteur++;
    codegen(p->si.noeud[1]);
    break;

  case typesinon:
    cptsinon = compteur;
    codegen(p->sinon.noeud[0]);
    if (p->sinon.noeud[0]->type == typeId) {
      printf("LOAD %d\nJUMZ %d\n", ts_retrouver_id(tsymb, p->sinon.noeud[0]->id.text), compteur + p->sinon.noeud[1]->ninst + 3);
      compteur++;
      }
    else printf("JUMZ %d\n", compteur + p->sinon.noeud[1]->ninst + 2);
    compteur ++;
    codegen(p->sinon.noeud[1]);
    printf("JUMP %d\n", cptsinon + p->ninst);
    compteur++;
    codegen(p->sinon.noeud[2]);
    break;

  case typeTQ:
    cpttq = compteur;
    codegen(p->tq.noeud[0]);
    if (p->tq.noeud[0]->type == typeId) {
      printf("LOAD %d\nJUMZ %d\n", ts_retrouver_id(tsymb, p->tq.noeud[0]->id.text), compteur + p->tq.noeud[1]->ninst + 3);
      compteur++;
      }
    else printf("JUMZ %d\n", compteur + p->tq.noeud[1]->ninst + 2);
    compteur++;
    codegen(p->tq.noeud[1]);
    printf("JUMP %d\n", cpttq);
    compteur++;
    break;

  default:
    break;
  }
}

void affic_asa(asa *p){
  if (!p) return;
  if(p->type == typeOp){
    affic_asa(p->op.noeud[0]);
    affic_asa(p->op.noeud[1]);
    printf("%c ", p->op.ope);
    return;
  }
  else{
    if (p ->type == typeNb) printf("%d ", p->nb.val);
    if (p->type == typeId) for(int i = 0; i < strlen(p->id.text); i++) printf("%c", p->id.text[i]);
    if(p->type == typeinst){
      printf("\n");
      for(int i = 0; i < strlen(p->insts.nom); i++) printf("%c", p->insts.nom[i]);
      printf("\n");
      affic_asa(p->op.noeud[0]);
      affic_asa(p->op.noeud[1]);
    }
    return;
  }
}

void yyerror(const char * s)
{
  fprintf(stderr, "%s\n", s);
  exit(0);
}
