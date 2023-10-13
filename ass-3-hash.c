#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct kv {
  int key;
  int value;
  struct kv *next;
} KV;


void print_kv(KV *kv) {
  while (1)
  {
    if (kv == NULL) {
      return;
    }
    printf("%d: %d\n", kv->key, kv->value);
    kv = kv->next;
  }
   printf("\n");
}

int kv_length(KV *kv) {
  if (kv == NULL) {
    return -1;
  }
  int cl = 1;
  while (kv->next != NULL)
  {
    kv = kv->next;
    cl++;
  }
  return cl;
}


int insert_and_update(KV *kv, int key, int value, int delta) {
  int lengh = kv_length(kv);
  KV *temp = kv;
  int cl = 0;
  int pos = key % delta;
  while (1)
  {
    if (cl >lengh) {
      return -1;
    }
    if (cl == pos) {
      if (temp->key == -1 || temp->key == key) {
        temp->key = key;
        temp->value = value;
        return 1;
      }
      pos += delta;
    }
    temp = temp->next;
    cl++;
  }
}

int read(KV *kv, int key, int delta) {
  int lengh = kv_length(kv);
  KV *temp = kv;
  int cl = 0;
  int pos = key % delta;
  while (1)
  {
    if (cl >lengh) {
      return -1;
    }
    if (cl == pos) {
      if (temp->key == key) {
        return temp->value;
      }
      pos += delta;
    }
    temp = temp->next;
    cl++;
  }
}

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

  print_kv(head);

  printf("insert_and_update\n");
  insert_and_update(head, 1, 1, 3);
  insert_and_update(head, 4, 2, 3);
  insert_and_update(head, 1, 99, 3);
  print_kv(head);
  printf("read: %d\n", read(head, 1, 3));

  return 0;
}