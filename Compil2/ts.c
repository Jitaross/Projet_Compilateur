#include "ts.h"


ts * tsymb = NULL;
static int mem_offset = 33;

ts * ts_init_table(char * id, int size){
  tsymb = malloc(sizeof(struct ts));
  tsymb->id = malloc(strlen(id) + 1);;

  strcpy(tsymb->id,id);
  tsymb->adr = mem_offset;
  tsymb->size = size;
  mem_offset += size;
  tsymb->next = NULL;

  return tsymb;
}

void ts_ajouter_id(char *id, int size)
{
  ts *new = malloc(sizeof(ts));
  new->id = malloc(strlen(id)+1);
  strcpy(new->id, id);
  new->adr = mem_offset;
  new->size = size;
  mem_offset += size;
  new->next = tsymb;
  tsymb = new;

}

int ts_retrouver_id(ts *tsymb, const char * id){
  ts *current = tsymb;
  while (current != NULL){
    if (strcmp(current->id, id) == 0) return current->adr;
    current = current->next;
  }
  return -1;
}

void ts_free_table()
{
  ts *next, *t = tsymb;

  while (t!=NULL){
    next = t->next;
    free(t->id);
    free(t);
    t = next;
  }
}

void ts_print()
{
  ts *t = tsymb;
  if (t!=NULL){
    printf("{%s : %d}", t->id, t->adr);
    t = t->next;
  }
  while (t!=NULL){
    printf("-->{%s : %d}", t->id, t->adr);
    t = t->next;
  }

}
