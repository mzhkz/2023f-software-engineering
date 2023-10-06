#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct list {
  int data;
  struct list *next;
} List;

void print_list(List *s) {
  while (1)
  {
    if (s == NULL) {
      return;
    }
    printf("%d ", s->data);
    s = s->next;
  }
   printf("\n");
}


List* combine(List *p, List *q) {
  if (p == NULL)
  {
    return q;
  }
  else if (q == NULL)
  {
    return p;
  }

  List *s = p;

  // リストの末尾を見つけるためにループを使う必要はありません
  while (p->next != NULL) {
    p = p->next;
  }

  p->next = q;
  return s;
}


List* sort(List *s){

  if (s == NULL) {
    return NULL;
  }

  List *p = NULL, *p_start = NULL;
  List *q = NULL, *q_start = NULL;
  List *t = s->next;

  if (t == NULL)
  {
    return s;
  }

  while (1)
  {
    if (t == NULL) {
      break;
    }

    List *next = t->next;
    t->next = NULL;
    if (s->data >= t->data)
    {
      if (q == NULL) {
        q = t;
        q_start = q;
      }
      else
      {
        q->next = t;
        q = t;
      }
    } else {
      if (p == NULL) {
        p = t;
        p_start = p;
      } else {
        p->next = t;
        p = t;
      }
    }
    t = next;
  }
  s->next = NULL;
  return combine(combine(sort(q_start), s), sort(p_start));
}

int main(){
  List *s, *p, *q;
  p = malloc(sizeof(List));
  s = p;
  int size = 10;
  for (int i = 1; i < size; i++)
  {
    q = malloc(sizeof(List));
    p->data = size - i;
    p -> next = q;
    p = q;
  }
  q -> next = NULL;

  printf("\n### original ###\n");
  print_list(s);

  printf("\n### asc sorted ###\n");
  List *r = sort(s);
  print_list(r);

  return 0;
}