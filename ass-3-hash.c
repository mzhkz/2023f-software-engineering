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
  int pos = key % lengh; //挿入するポジションを計算
  while (1)
  {
    if (cl >lengh) { //リストの長さを超えたら、挿入失敗
      return -1;
    }
    if (cl == pos) { //挿入するポジションに到達したら、挿入
      if (temp->key == -1 || temp->key == key) { //キーが空の場合、挿入
        temp->key = key;
        temp->value = value;
        return 1;
      }
      pos += delta; //キーが空でない場合、ポジションをdelta分更新
    }
    temp = temp->next;
    cl++;
  }
}

int read(KV *kv, int key, int delta) {
  int lengh = kv_length(kv);
  KV *temp = kv;
  int cl = 0;
  int pos = key % lengh; //挿入されていると思われるポジションを計算
  while (1)
  {
    if (cl >lengh) { //リストの長さを超えたら、検索失敗
      return -1;
    }
    if (cl == pos) { //挿入されていると思われるポジションに到達したら、キーが一致するか確認
      if (temp->key == key) { //キーが一致したら、値を返す
        return temp->value;
      }
      pos += delta; //キーが一致しなかったら、ポジションをdelta分更新
    }
    temp = temp->next;
    cl++;
  }
}

int main() {
  int size = 10; //このサイズを変更することで、Bucketサイズを変更できる。
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