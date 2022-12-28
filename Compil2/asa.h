#ifndef ASA_H
#define ASA_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ts.h"

typedef enum {typeNb, typeId, typeOp, typeinst, typeaffi, typelire, typesi, typesinon, typeTQ} typeNoeud;

typedef struct {
  int val;
} feuilleNb;

typedef struct {
  int tailleid;
  char *text;
} feuilleId;

typedef struct {
  int ope;
  struct asa * noeud[2];
} noeudOp;

typedef struct {
  struct asa * noeud[1];
} noeudaffi;

typedef struct {
  struct asa * noeud[1];
} noeudlire;

typedef struct {
  char nom[5];
  struct asa * noeud[2];
} noeudinsts;

typedef struct {
  struct asa * noeud[2];
} noeudsi;

typedef struct {
  struct asa * noeud[3];
} noeudsinon;

typedef struct {
  struct asa * noeud[2];
} noeudTQ;

typedef struct asa{
  typeNoeud type;
  int ninst;

  union {
    feuilleNb nb;
    noeudOp op;
    feuilleId id;
    noeudinsts insts;
    noeudaffi affi;
    noeudlire lire;
    noeudsi si;
    noeudsinon sinon;
    noeudTQ tq;
  };
} asa;

// fonction d'erreur utilisée également par Bison
void yyerror(const char * s);

/*
  Les fonctions creer_<type> construise un noeud de l'arbre
  abstrait du type correspondant et renvoie un pointeur celui-ci
 */
asa * creer_feuilleNb( int value );
asa * creer_feuilleId( char* nom_id );
asa * creer_noeudOp( int ope, asa * p1, asa * p2 );
asa * creer_noeudInsts(asa * p1, asa * p2);
asa * creer_noeud_affi(asa * p1);
asa * creer_noeud_lire(asa * p1);
asa * creer_noeud_si(asa * p1, asa * p2);
asa * creer_noeud_sinon(asa * p1, asa * p2, asa * p3);
asa * creer_noeudTQ(asa * p1, asa * p2);

void free_asa(asa *p);
void affic_asa(asa *p);

// produit du code pour la machine RAM à partir de l'arbre abstrait
// ET de la table de symbole
void codegen(asa *p);

extern ts * tsymb;

#endif
