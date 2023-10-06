#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct kv {
  int key;
  int value;
  struct kv *next;
} KV;

int main() {
  int size = 10;
  KV *kv = malloc(sizeof(KV));
  KV *head = kv;
  for (int i = 0; i < size; i++)
  {
    kv->key = -1;
    kv->value = -1;

    if (i  < size - 1) {
       KV *next = malloc(sizeof(KV));
       kv->next = next;
       kv = next;
    }
  }
}