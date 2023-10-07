#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct kv {
  int key;
  int value;
  struct kv *next;
} KeyValue;

void print_kv(KeyValue *kv) {
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

int kvs_length(KeyValue *kv) {
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


int binary_search(KeyValue *kv, int key, int start, int end) {
  // リストをキーでソート済みと仮定
  if (start > end) {
      return -1; // キーが見つからなかった場合
  }
  KeyValue *temp = kv, *mid_kv = NULL;
  int mid = (start + end) / 2;
  if (start == end ) {
      mid_kv = kv;
  }
  else
  {
      int cl = 0;
      while (1)
      {
        if (start <= cl && cl <= end)
        {
          if (cl == mid)
          {
            mid_kv = temp->next;
            break;
          }
        }
        temp = temp->next;
        cl++;
      }
  }

  if (mid_kv->key == key) {
    return mid_kv->value; // キーが見つかった場合
  } else if (mid_kv->key < key) {
    // 右側を探索
    return binary_search(kv, key, mid + 1, end);
  } else {
    // 左側を探索
    return binary_search(kv, key, start, mid);
  }
}

int search_key(KeyValue *kv, int key) {
  int length = kvs_length(kv);
  return binary_search(kv, key, 0, length - 1);
}

int main() {
  int size = 100;
  KeyValue *kv = malloc(sizeof(KeyValue));
  KeyValue *head = kv;
  for (int i = 0; i < size; i++)
  {
    kv->key = i * 2;
    kv->value = i * 4;

    if (i  < size - 1) {
       KeyValue *next = malloc(sizeof(KeyValue));
       kv->next = next;
       kv = next;
    }
  }


  int result = search_key(head, 2);
  printf("### result ###\n");
  printf("%d\n", result);

  return 0;
}