#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct list {
  struct list *next;
  int data;
};

int main() {
  struct list *s, *p, *q;
  p = malloc(sizeof(struct list));
  p->data = 0;
  s = p;
  int i;
  for (i = 1; i <= 100; i++) {
	q = malloc(sizeof(struct list));
	q->data = i;
	p->next = q;
	p = q; 
  }
  q->next = NULL;

  while(1) {
    printf("%d \n", s->data);
    if (s->next == NULL) {
      exit(0);
    }
    s = s->next;
  }
}
