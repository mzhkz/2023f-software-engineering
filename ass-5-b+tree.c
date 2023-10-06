#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TREE_DEGREE 3

typedef struct kvs
{
    int key;
    int value;
    struct kvs *next;
    int is_leaf;
} KVS;

typedef struct node 
{
    struct node *parent;
    struct node *childs;
    struct node *next;
    struct node *prev;
    KVS *kvs_head;
} Node;

typedef struct tree
{
    Node *root;
} Tree;


void print_list(KVS *s) {
  while (1)
  {
    if (s == NULL) {
      return;
    }
    printf("%d ", s->key);
    s = s->next;
  }
   printf("\n");
}


int kvs_length(KVS *head) {
  if (head == NULL) {
    return -1;
  }
  int cl = 1;
  while (head->next != NULL)
  {
    head = head->next;
    cl++;
  }
  return cl;
}

KVS* combine(KVS *p, KVS *q) {
  if (p == NULL)
  {
    return q;
  }
  else if (q == NULL)
  {
    return p;
  }

  KVS *s = p;

  // リストの末尾を見つけるためにループを使う必要はありません
  while (p->next != NULL) {
    p = p->next;
  }

  p->next = q;
  return s;
}


KVS* sort(KVS *s){

  if (s == NULL) {
    return NULL;
  }

  KVS *p = NULL, *p_start = NULL;
  KVS *q = NULL, *q_start = NULL;
  KVS *t = s->next;

  if (t == NULL)
  {
    return s;
  }

  while (1)
  {
    if (t == NULL) {
      break;
    }

    KVS *next = t->next;
    t->next = NULL;
    if (s->key >= t->key)
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


// B+tree

int insert_kvs_to_node(Tree *tree, Node *node, int key, int value) {
    int kl = kvs_length(node->kvs_head);
    // 初期状態 or 探索してエッジまでたどり着いた。
    if (node->kvs_head == NULL || node->kvs_head->is_leaf == 0) {
        if (kl + 1 < TREE_DEGREE ) { //一個足すので
            printf("insert to node directly\n");
            KVS *kvs = malloc(sizeof(KVS));
            kvs->key = key;
            kvs->value = value;
            kvs->next = NULL;
            kvs->is_leaf = 0;
            node->kvs_head = combine(node->kvs_head, kvs);
            return 1;
        } else {
            printf("make leaf and expand edges\n");
            // リーフにエッジを追加して、AEC順にソート
            KVS *new_edge = malloc(sizeof(KVS));
            new_edge->key = key;
            new_edge->value = value;
            new_edge->next = NULL;
            new_edge->is_leaf = 0;
            node->kvs_head = combine(node->kvs_head, new_edge);
            node->kvs_head = sort(node->kvs_head);

            // 中央値を取得
            int mid_key = kl+1 / 2; //new_edgeを個足したので+1
            KVS *temp = node->kvs_head, *spliter = NULL, *mid = NULL;
            int cl = 1;
            while (1) {
                if (cl == mid_key - 1) {
                    spliter = temp;
                    mid = temp->next;
                    break;
                }
                temp = temp->next;
                cl++;
            }

            // 二つに分割する。
            KVS *former = node->kvs_head;
            KVS *latter = mid;
            spliter->next = NULL; // リストを分割する。formerからlaterを切り離す。

            //横関係を保存する
            Node *before_parent = node->parent;
            Node *prev_node = node->prev;
            Node *next_node = node->next;
            //自身を子ノード(former child)にする。
            node->kvs_head = former;
            node->childs = NULL; //子ノードはないのでNULL

            //新しい子ノード(letter child)を作る
            Node *new_node_child_letter= malloc(sizeof(Node));
            new_node_child_letter->kvs_head = latter;
            new_node_child_letter->childs = NULL; //子ノードはないのでNULL

            //おやノード(リーフノード)を作る
            Node *new_node_parent = malloc(sizeof(Node));
            KVS *new_leaf = malloc(sizeof(KVS));
            new_leaf->key = mid->key; //中央値をリーフにする。
            new_leaf->is_leaf = 1; //リーフフラグを立てる。
            new_leaf->next = NULL;
            new_node_parent->kvs_head = new_leaf; //リーフKVSをおやノードに追加する。

            // 親と子を相互リンクさせる
            node->parent = new_node_parent;
            new_node_child_letter->parent = new_node_parent;
            new_node_parent->childs = node;
            new_node_parent->childs->next = new_node_child_letter;

            //子ノード横の関係を持たせる
            node->prev = NULL;
            node->next = new_node_child_letter; 
            new_node_child_letter->prev = node;
            new_node_child_letter->next = NULL;

            // 子ノードのchildを初期化
            node->childs = NULL;
            new_node_child_letter->childs = NULL;

            //親ノードの横関係を持たせる（nodeから継承）
            new_node_parent->next = prev_node;
            new_node_parent->prev = next_node;
            new_node_parent->parent = before_parent;

            // 継承先がルートノードだった場合は、ルートノードを更新する。
            if (before_parent == NULL) {
                tree->root = new_node_parent;
            }

            return 1;
        }
    } else { //エッジではないので適切なのノードに橋渡し。
        // まずは適切なノードを探す。
        printf("search edge\n");
        Node *child_node_head = node->childs; //子ノードの先頭を取得
        Node *appled_child_node = child_node_head;
        while (child_node_head != NULL) {
            if (child_node_head->kvs_head->key < key) { //子ノードの先頭のキーが挿入するキーより小さい場合は、そこに置く。
                if (appled_child_node->kvs_head->key < child_node_head->kvs_head->key) { //appled_child_nodeのキーがchild_node_headのキーより大きい場合は、appled_child_nodeを更新する。
                     appled_child_node = child_node_head;
                }
            } 
            child_node_head = child_node_head->next;
        }
        // 子ノードに橋渡し
        return insert_kvs_to_node(tree, appled_child_node, key, value);
    }
}


void insert(Tree *tree, int key, int value) {
    // 初期状態
    if (tree->root == NULL) {
        Node *node = malloc(sizeof(Node)); // rootノードを作る
        node->kvs_head = NULL;
        node->parent = NULL; //rootノードなので親はいない。
        node->childs = NULL;
        node->next = NULL;
        node->prev = NULL;
        tree->root = node;
    }
    // 木の中に挿入する。
    insert_kvs_to_node(tree, tree->root, key, value);

    return;
}

void draw_tree(Tree *tree) {
    Node *node = tree->root;
    while (node != NULL) {
        printf(" [");
        KVS *kvs = node->kvs_head;
        while (kvs != NULL) {
            printf(" <key: %d (%d)> ", kvs->key, kvs->is_leaf);
            kvs = kvs->next;
        }
        printf("] ");
        if (node->next == NULL) {
            node = node->childs;
            printf("\n\n");
        } else {
            node = node->next;
        }
    }
}


int main() {
    Tree *tree = malloc(sizeof(Tree));
    insert(tree, 1, 9);
    insert(tree, 2, 8);
    insert(tree, 3, 7);
    insert(tree, 4, 6);
    draw_tree(tree);
    return 0;
}