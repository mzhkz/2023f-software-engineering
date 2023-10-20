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


List* sort(List *s) {
    if (s == NULL) {
        return NULL;
    }

    List *start = s; //開始地点
    List *sorted = NULL; //ソート済みリスト

    while (start != NULL) {
        List *current = start; 
        start = start->next; 

        // 大小関係が逆、もしくは初期値だったらだったら
        if (sorted == NULL || current->data < sorted->data) {
            current->next = sorted;
            sorted = current;
        } else {
            List *temp = sorted;
            // 挿入する場所を探す
            while (temp->next != NULL && current->data >= temp->next->data) {
                temp = temp->next;
            }
            //順番を入れ替える
            current->next = temp->next;
            temp->next = current; 
        }
    }

    return sorted;
}


int main(){
  List *s, *p, *q;
  p = malloc(sizeof(List));
  s = p;
  int size = 10000 * 1000;
  for (int i = 1; i < size; i++)
  {
    q = malloc(sizeof(List));
    p->data = rand();
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