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


int binary_search(KV *kv, int key) {
  // aes sortは実装済みとする
  int lengh = kv_length(kv);
  if (lengh <= 1) {
    if (kv != NULL && kv->key == key) {
      return kv->value;
    }
    return -1;
  }

  int mid_key = lengh / 2;

  KV *temp = kv, *mid = NULL;
  int cl = 1;
  while (1)
  {
   if (cl == mid_key) {
     mid = temp->next;
     break;
   }
   temp = temp->next;
   cl++;
  }

  if (mid->key == key) {
    return mid->value;
  }
  
  int is_tail = key > mid->key;
  if (is_tail) {
    kv = mid;
  } else {
    temp->next = NULL;
  }
  printf("######\n");
  printf("mid: %d\n", mid_key);
  printf("is_tail: %d\n", is_tail);
  print_kv(kv);
  return binary_search(kv, key);
}

int main() {
  int size = 10;
  KV *kv = malloc(sizeof(KV));
  KV *head = kv;
  for (int i = 0; i < size; i++)
  {
    kv->key = i * 2;
    kv->value = i * 4;

    if (i  < size - 1) {
       KV *next = malloc(sizeof(KV));
       kv->next = next;
       kv = next;
    }
  }


  int result = binary_search(head, 1114);
  printf("### result ###\n");
  printf("%d\n", result);

  return 0;
}