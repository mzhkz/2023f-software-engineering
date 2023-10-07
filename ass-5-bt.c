#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TREE_DEGREE 3

typedef struct KeyValue
{
    int key;
    int value;
    struct KeyValue *next;
} KeyValue;


typedef struct Node
{
    char *name;
    struct Node *next;
    struct Node *prev;
    struct Node *child;
    struct Node *parent;
    int is_leaf;

    struct KeyValue *keyvalue;
} Node;

typedef struct Tree
{
    Node *root;
} Tree;

int connectNodeAsPeer(Node *head, Node *q) {
    Node *p = head;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = q;
    q->prev = p;
    return 0;
}

int connectNodeAsChild(Node *head, Node *q) {
    if (head->child == NULL) {
        head->child = q;
    } else {
        Node *p = p->child;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = q;
    }
    q->parent = head;

    return 0;
}

int removeNodeAsChild(Node *head, Node *q) {
    if (head->child == NULL) {
        return 0;
    } else {
        Node *p = p->child;
        while (p->next != NULL) {
            if (p == q) {
                p->prev->next = p->next;
                p->next->prev = p->prev;
                return 0;
            }
            p = p->next;
        }
    }
    q->parent = NULL;

    return 0;
}

KeyValue* combineKeyValueStore(KeyValue *head, KeyValue *q) {
  KeyValue *p = head;

  // リストの末尾を見つけるためにループを使う必要はありません
  while (p->next != NULL) {
    p = p->next;
  }

  p->next = q;
  return head;
}



int kvs_length(KeyValue *head) {
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


KeyValue* quick_sort(KeyValue *s){

  if (s == NULL) {
    return NULL;
  }

  KeyValue *p = NULL, *p_start = NULL;
  KeyValue *q = NULL, *q_start = NULL;
  KeyValue *t = s->next;

  if (t == NULL)
  {
    return s;
  }

  while (1)
  {
    if (t == NULL) {
      break;
    }

    KeyValue *next = t->next;
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
  return combineKeyValueStore(combineKeyValueStore(quick_sort(q_start), s), quick_sort(p_start));
}

int insert_node(Tree *tree, Node *node, KeyValue *kvs, Node *childs_head, int is_backpropagation) {
    if (node->is_leaf == 1 && is_backpropagation == 0) {
        Node *child_head = node->childs; //子ノードの先頭を取得
        Node *appled_child= child_head;
        while (child_head != NULL) {
            if (child_head->kvs_head->key < kvs->key) { //子ノードの先頭のキーが挿入するキーより小さい場合は、そこに置く。
                if (appled_child->keyvalue->key < child_head->keyvalue->key) { //appled_child_nodeのキーがchild_node_headのキーより大きい場合は、appled_child_nodeを更新する。
                        appled_child = child_head;
                }
            } 
            child_head = child_head->next;
        }
        // 子ノードに橋渡し
        return insert_node(tree, appled_child_node, kvs);
    }

    // ノードに挿入
    node_kvs_length = kvs_length(node->keyvalue);

    // 対象のノードがあいていた場合
    if (kl + 1 < TREE_DEGREE ) {
        node->keyvalue = combineKeyValueStore(node->keyvalue, kvs);
        node->keyvalue = quick_sort(node->keyvalue);
        Node *child = childs_head;
        while (child != NULL) {
            connectNodeAsChild(node, child);
            child = child->next;
        }
        return 1;
    }

    // 対象のノードがいっぱいだった場合
    int mid_index = kl+1 / 2; //new_edgeを個足したので+1
    KeyValue *temp = node->keyvalue, *spliter = NULL, *mid_keyvalue = NULL;
    int cl = 1;
    while (1) {
        if (cl == mid_index - 1) {
            spliter = temp;
            mid = temp->next;
            break;
        }
        temp = temp->next;
        cl++;
    }

     // 二つに分割する。
    KeyValue *former_keyvalue_head = node->keyvalue;
    KeyValue *latter_keyvalue_head = mid;
    if (node->is_leaf) {
        latter = latter->next;
    }
    spliter->next = NULL; // リストを分割する。formerからlaterを切り離す。

    Node *new_parent_node = malloc(sizeof(Node));
    Node *new_child_former_node = malloc(sizeof(Node));
    Node *new_child_letter_node = malloc(sizeof(Node));

    new_parent_node.name = "";

    //子供にする
    connectNodeAsChild(new_parent_node, new_child_former_node);
    connectNodeAsChild(new_parent_node, new_child_letter_node);

    //チルド同士つんが得る
    connectNodeAsPeer(new_child_former_node, new_child_letter_node);

    // KV for new leaf (new parent)
    new_parent_node->keyvalue = mid_keyvalue;
    new_parent_node->is_leaf = 1;
    // KV for new child (new parent)
    new_child_former_node->keyvalue = former_keyvalue_head;
    new_child_letter_node->keyvalue = latter_keyvalue_head;
    new_parent_node->is_leaf = node->is_leaf;

    if (node->parent == NULL) {  // 継承先がルートノードだった場合は、ルートノードを更新する。
        tree->root = new_parent_node;
    } else { //すでにあればマージする。
        //親ノードの横関係を持たせる（nodeから継承）
        removeNodeAsChild(node->parent, node); //ループしちゃうので、一旦外す。nodeのkvsとchildのkvsは同じオブジェクトなので。。
        insert_node(tree, node->parent, node->keyvalue, new_parent_node->child, 1);
    }
}

void insert(Tree *tree, int key, int value) {
    printf("##### key: %d #######\n", key);
    // 初期状態
    if (tree->root == NULL) {
        Node *node = malloc(sizeof(Node)); // rootノードを作る
        node->keyvalue = NULL;
        node->parent = NULL; //rootノードなので親はいない。
        node->childs = NULL;
        node->next = NULL;
        node->prev = NULL;
        tree->root = node;
    }
    // 木の中に挿入する。
    KeyValue *keyvalue = malloc(sizeof(KVS));
    keyvalue->key = key;
    keyvalue->value = value;
    keyvalue->next = NULL;
    insert_kvs_to_node(tree, tree->root, keyvalue, NULL, 0);

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
     draw_tree(tree);
    insert(tree, 2, 8);
     draw_tree(tree);
    insert(tree, 3, 7);
     draw_tree(tree);
    insert(tree, 4, 7);
    draw_tree(tree);
    insert(tree, 5, 7);
    draw_tree(tree);
    return 0;
}
