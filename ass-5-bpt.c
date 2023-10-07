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
    int node_count;
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
        if (head->child == NULL) {
            head->child = q;
        } else {
            Node *tail = head->child;
            while (tail->next != NULL) {
                tail = tail->next;
            }
            tail->next = q;
        }
    }
    q->parent = head;

    return 0;
}

int removeNodeAsChild(Node *head, Node *q) {
    if (head->child == NULL) {
        return 0;
    } else {
        Node *p = head->child;
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
    if (head == NULL)
    {
        return q;
    }
    else if (q == NULL)
    {
        return head;
    }
    KeyValue *p = head;

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


void print_nlist(Node *s) {
  while (1)
  {
    if (s == NULL) {
      return;
    }
    printf("%d ", s->keyvalue->key);
    s = s->next;
  }
   printf("\n");
}

void print_list(KeyValue *s) {
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


int insert_node(Tree *tree, Node *node, KeyValue *kvs, Node *childs_head, int is_backpropagation) {
    if (node->is_leaf == 1 && is_backpropagation == 0) {
        Node *child_head = node->child; //子ノードの先頭を取得!
        Node *appled_child= child_head;
        while (child_head != NULL) {
            if (child_head->keyvalue->key < kvs->key) { //子ノードの先頭のキーが挿入するキーより小さい場合は、そこに置く。
                if (appled_child->keyvalue->key < child_head->keyvalue->key) { //appled_child_nodeのキーがchild_node_headのキーより大きい場合は、appled_child_nodeを更新する。
                        appled_child = child_head;
                }
            } 
            child_head = child_head->next;
        }
        // 子ノードに橋渡し
        return insert_node(tree, appled_child, kvs, NULL, 0);
    }

    // ノードに挿入
    int node_kvs_length = kvs_length(node->keyvalue);

    // 対象のノードがあいていた場合
    if (node_kvs_length + 1 < TREE_DEGREE ) {
        printf("insert;\n");
        node->keyvalue = combineKeyValueStore(node->keyvalue, kvs);
        node->keyvalue = quick_sort(node->keyvalue);
        Node *child = childs_head;
        while (child != NULL) {
            connectNodeAsChild(node, child);
            child = child->next;
        }
        return 1;
    }

    printf("retree;\n");

    // 対象のノードがいっぱいだった場合
    kvs->next = NULL;
    node->keyvalue = combineKeyValueStore(node->keyvalue, kvs);
    node->keyvalue = quick_sort(node->keyvalue);

    int mid_index = node_kvs_length+1 / 2; //new_edgeを個足したので+1
    KeyValue *temp = node->keyvalue, *spliter = NULL, *mid_keyvalue = NULL;
    int cl = 1;
    while (1) {
        if (cl == mid_index - 1) {
            spliter = temp;
            mid_keyvalue = temp->next;
            break;
        }
        temp = temp->next;
        cl++;
    }


    // 二つに分割する。
    KeyValue *former_keyvalue_head = node->keyvalue;
    KeyValue *latter_keyvalue_head = mid_keyvalue;
    if (node->is_leaf == 1) { //leafの場合はchildに自身を含まない。上位のleafに結合するだけ。
        latter_keyvalue_head = latter_keyvalue_head->next;
    }
    spliter->next = NULL; // リストを分割する。formerからlaterを切り離す。


    Node *new_parent_node = malloc(sizeof(Node));
    Node *new_child_former_node = malloc(sizeof(Node));
    Node *new_child_letter_node = malloc(sizeof(Node));

    // char name[50];
    // sprintf(name, "parent: %d", tree->node_count);
    // new_parent_node->name = name;

    // tree->node_count += 1;
    // sprintf(name, "child: %d for %s", tree->node_count, new_parent_node->name);
    // new_parent_node->name = name;
    // tree->node_count += 1;
    // sprintf(name, "child: %d for %s", tree->node_count, new_parent_node->name);
    // new_parent_node->name = name;
    // tree->node_count += 1;

    //init
    new_parent_node->parent = NULL;
    new_parent_node->child = NULL;

    // new_parent_node->name = "";

    //子供にする
    connectNodeAsChild(new_parent_node, new_child_former_node);
    connectNodeAsChild(new_parent_node, new_child_letter_node);



    //チルド同士つんが得る
    new_child_former_node->next = NULL;
    new_child_letter_node->next = NULL;
    connectNodeAsPeer(new_child_former_node, new_child_letter_node);
    if (node->prev != NULL) {
        node->prev->next = new_child_former_node;
    }

    // KV for new leaf (new parent)
    KeyValue *new_mid_keyvalue = malloc(sizeof(KeyValue));
    new_mid_keyvalue->key = mid_keyvalue->key;
    new_mid_keyvalue->value = mid_keyvalue->value;
    new_mid_keyvalue->next = NULL;

    new_parent_node->keyvalue = new_mid_keyvalue;
    new_parent_node->is_leaf = 1;

    // KV for new child (new parent)
    new_child_former_node->keyvalue = former_keyvalue_head;
    new_child_letter_node->keyvalue = latter_keyvalue_head;
    new_child_former_node->is_leaf = node->is_leaf;
    new_child_letter_node->is_leaf = node->is_leaf;

    if (node->parent == NULL) {  // 継承先がルートノードだった場合は、ルートノードを更新する。
        printf("updart root of tree;\n");
        tree->root = new_parent_node;
        return 1;
    } else { // すでにあればマージする。
        // 親ノードの横関係を持たせる（nodeから継承）
        printf("update parent node;\n");
        removeNodeAsChild(node->parent, node); //ループしちゃうので、一旦外す。nodeのkvsとchildのkvsは同じオブジェクトなので。。
        return insert_node(tree, node->parent, new_parent_node->keyvalue, new_parent_node->child, 1);
    }
}

void insert(Tree *tree, int key, int value) {
    printf("##### key: %d #######\n", key);
    // 初期状態
    if (tree->root == NULL) {
        Node *node = malloc(sizeof(Node)); // rootノードを作る
        node->is_leaf = 0;
        node->keyvalue = NULL;
        node->parent = NULL; //rootノードなので親はいない。
        node->child = NULL;
        node->next = NULL;
        node->prev = NULL;
        tree->root = node;
    }
    // 木の中に挿入する。
    KeyValue *keyvalue = malloc(sizeof(KeyValue));
    keyvalue->key = key;
    keyvalue->value = value;
    keyvalue->next = NULL;
    insert_node(tree, tree->root, keyvalue, NULL, 0);

    return;
}

void draw_tree(Tree *tree) {
    Node *node = tree->root;
    while (node != NULL) {
        printf(" [");
        KeyValue *kvs = node->keyvalue;
        while (kvs != NULL) {
            printf(" <key: %d (%d)> ", kvs->key, node->is_leaf);
            kvs = kvs->next;
        }
        printf("] ");
        if (node->next == NULL) {
            node = node->child;
            printf("\n\n");
        }
        else
        {
            node = node->next;
        }
    }
}



int main() {
    Tree *tree = malloc(sizeof(Tree));
    tree->node_count = 0;
    insert(tree, 1, 9);
    draw_tree(tree);
    insert(tree, 2, 8);
     draw_tree(tree);
    insert(tree, 3, 7);
     draw_tree(tree);
    insert(tree, 4, 7);
    draw_tree(tree);
    // insert(tree, 5, 7);
    // draw_tree(tree);
    return 0;
}
