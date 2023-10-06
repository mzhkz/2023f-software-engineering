/**

Input and Output:
Input: None
Output: リストの要素を改行して全件表示させる。
0
1
2
...
100

Explanation of my code:
pを前者のノード、qを後者のノードとし、for分を用いて値の設定とリンクの設定を行っ>ている。sは開始位置を保存するための変数である。

**/

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
    free(s);
  }
}
