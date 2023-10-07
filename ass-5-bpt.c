#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 最小値を返すマクロ
#define min(a, b) ((a) < (b) ? (a) : (b))

// 最大値を返すマクロ
#define max(a, b) ((a) > (b) ? (a) : (b))

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
    struct Node *link;

    int edge_start;
    int edge_end;
    int is_leaf;

    struct KeyValue *keyvalue;
} Node;

typedef struct Tree
{
    Node *root;
    int node_count;
} Tree;

int connectNodeAsLink(Node *head, Node *q) {
    Node *p = head;
    while (p->link != NULL) {
        p = p->link;
    }
    p->link = q;
    return 0;
}

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


Node* removeNode(Node *head, Node *q) {
    if (head == NULL) {
        return NULL;
    } else {
        Node *p = head;
        while (p->next != NULL) {
        // while (p != NULL) {
            if (p == q) {
                if (p->prev == NULL) {
                    head = p->next;
                    p->next->prev = NULL;
                } else {
                    p->prev->next = p->next;
                    p->next->prev = p->prev;
                }
                return head;
            }
            p = p->next;
        }
    }
    return NULL;
}

KeyValue* removeKeyValue(KeyValue *head, int key) {
    if (head == NULL) {
        return 0;
    } else {
        KeyValue *p = head;
        KeyValue *b = NULL;
        while (p != NULL) {
            if (p->key == key) {
                if (b == NULL) {
                    head = p->next;
                }
                else
                {
                   b->next = p->next;
                }
                return head;
            }
            b = p;
            p = p->next;
        }
    }

    return NULL;
}

KeyValue* replaceKeyValue(KeyValue *head, int key, KeyValue *new_kv) {
    if (head == NULL) {
        return 0;
    } else {
        KeyValue *p = head;
        KeyValue *b = NULL;
        while (p->next != NULL) {
            if (p->key == key) {
                new_kv->next = p->next;
                if (b == NULL) {
                    head = new_kv;
                } else {
                    b->next = new_kv;
                }
                return head;
            }
            b = p;
            p = p->next;
        }
    }

    return NULL;
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

int nodes_length(Node *head) {
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

int binary_search(KeyValue *kv, int key, int start, int end) {
  // リストをキーでソート済みと仮定
  if (end - start == 0) {
     if (kv->key == key) {
        return kv->value; // キーが見つかった場合
     } else {
        return -1; // キーが見つからなかった場合
     }
  }
  if (start > end) {
      return -1; // キーが見つからなかった場合
  }
  KeyValue *temp = kv, *mid_kv = NULL;
  int mid = ((end - start) / 2) + start;
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
  printf("length: %d\n", length);
  return binary_search(kv, key, 0, length - 1);
}

int delete_from_node(Tree *tree, Node *node, int key, int is_backpropagation, Node *from) {
    if (node->is_leaf == 1 || is_backpropagation == 1) {
        if (is_backpropagation == 0) {
            Node *child_head = node->child; //子ノードの先頭を取得!
            Node *appled_child= child_head;
            while (child_head != NULL) {
                if (child_head->keyvalue->key <= key) { 
                    if (appled_child->keyvalue->key <= child_head->keyvalue->key) { 
                            appled_child = child_head;
                    }
                } 
                child_head = child_head->next;
            }
            // 子ノードに橋渡し
            return delete_from_node(tree, appled_child, key, 0, NULL);
        } else { // バックプロパゲーション
            Node *previous_parent = node->parent;

            // leafノードのchild1だった場合は、削除する。
            if (node->is_leaf == 1 && kvs_length(node->keyvalue) <= 1) {
                //親の子供（自分）を削除する
                node->parent->child = removeNode(node->parent->child, node);
                node->parent = NULL;

                // 親の子供が一つだった場合は、親の子供を親の親に継承する。
                if (nodes_length(node->parent->child) <= 1) {
                    Node *other_node = node->parent->child;
                    node->parent->child = removeNode(node->parent->child, other_node);
                    other_node->parent = NULL;
                    // 親の親のkvsを更新する。other leaf nodeと合体
                    node->parent->keyvalue = combineKeyValueStore(node->parent->keyvalue, other_node->keyvalue);

                    if (node->parent->parent == NULL) {
                        tree->root = node->parent;
                    } else {
                        // 親の親の子供を更新する。
                        Node *removed_node = removeNode(node->parent->parent->child, node->parent);
                        node->parent->parent->child = removed_node;
                        node->parent->parent = NULL;
                    }
                }
                // 子供の親を削除する。
                Node *f_child = node->child;
                while (f_child != NULL) {
                     // ループしちゃうので、一旦外す。nodeのkvsとchildのkvsは同じオブジェクトなので。。
                    Node* removed_node = removeNode(node->child, f_child);
                    node->child = removed_node;
                    f_child->parent = node->parent;
                    connectNodeAsChild(node->parent, f_child);
                    f_child = f_child->next;
                }
                node->child = NULL;
            }

            if (node->parent == NULL) {
                return 1; // ルートノードだった場合は、終了(一番topまで探索した)
            } else {
                return delete_from_node(tree, previous_parent, key, 1, node);
            }
        }
    } else {
        int is_exist = search_key(node->keyvalue, key);
        if (is_exist != -1) 
        {
            Node *previous_parent = node->parent;
            KeyValue *removed_kvs = removeKeyValue(node->keyvalue, key);
            node->keyvalue = removed_kvs;

            // 削除後のノードが空だった場合は、削除する。
            if (kvs_length(node->keyvalue) <= 0) {
                Node* removed_node = removeNode(node->parent->child, node);
                if (removed_node != NULL) {
                    node->parent->child = removed_node;
                }
                node->parent = NULL;
            }
            return delete_from_node(tree, previous_parent, key, 1, node);
        }
    }
    printf("error");
    return -1;
}


int read(Tree *tree, Node *node, int key) {
    if (node->is_leaf == 1) {
        Node *child_head = node->child; //子ノードの先頭を取得!
        Node *appled_child = child_head;
        int is_former = key < node->keyvalue->key;
        while (child_head != NULL) {
            if (child_head->keyvalue->key == key) {
                 appled_child = child_head;
                 break;
            }
            if (is_former == 1) {
                if (child_head->keyvalue->key <= appled_child->keyvalue->key) {
                    appled_child = child_head;
                }

            } else {
                if (child_head->keyvalue->key >= appled_child->keyvalue->key) {
                    appled_child = child_head;
                }
            }
            child_head = child_head->next;
        }
        // 子ノードに橋渡し
        printf("%d\n", appled_child->keyvalue->key);
        return read(tree, appled_child, key);
    } else {
        // return -1;
        return search_key(node->keyvalue, key);
    }
}


int insert_node(Tree *tree, Node *node, KeyValue *kvs, Node *childs_head, int is_backpropagation, Node *from) {
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
        if ( node->edge_start > kvs->key ) {
            node->edge_start = kvs->key;
        }
        if (node->edge_end < kvs->key ) {
            node->edge_end = kvs->key;
        }
        return insert_node(tree, appled_child, kvs, NULL, 0, NULL);
    }


    // ノードに挿入
    int node_kvs_length = kvs_length(node->keyvalue);

    // 対象のノードがあいていた場合
    if (node_kvs_length + 1 < TREE_DEGREE ) {
        node->keyvalue = combineKeyValueStore(node->keyvalue, kvs);
        node->keyvalue = quick_sort(node->keyvalue);
        node->edge_start = min(node->edge_start, kvs->key);
        node->edge_end = max(node->edge_end, kvs->key);

        Node *child = childs_head;
        while (child != NULL) {
            child->parent = node;

            node->edge_start = min(node->edge_start, child->edge_start);
            node->edge_end = max(node->edge_end, child->edge_end);

            child = child->next;
        }
        
        if (from != NULL) {
            node->edge_start = min(node->edge_start, from->edge_start);
            node->edge_end = max(node->edge_end, from->edge_end);
        }
        return 1;
    }

    // 対象のノードがいっぱいだった場合
    kvs->next = NULL;
    node->keyvalue = combineKeyValueStore(node->keyvalue, kvs);
    node->keyvalue = quick_sort(node->keyvalue);

    int mid_index = (node_kvs_length+1) / 2; //new_edgeを個足したので+1
    KeyValue *temp = node->keyvalue, *spliter = NULL, *mid_keyvalue = NULL;
    int cl = 0;
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

    //init
    new_parent_node->parent = NULL;
    new_parent_node->child = NULL;

    new_parent_node->edge_start = 0;
    new_parent_node->edge_end = 0;
    new_child_former_node->edge_start = 0;
    new_child_former_node->edge_end = 0;
    new_child_letter_node->edge_start = 0;
    new_child_letter_node->edge_end = 0;

    //子供にする
    connectNodeAsChild(new_parent_node, new_child_former_node);
    connectNodeAsChild(new_parent_node, new_child_letter_node);


    //チルド同士つんが得る
    new_child_former_node->next = NULL;
    new_child_letter_node->next = NULL;
    connectNodeAsPeer(new_child_former_node, new_child_letter_node);
    connectNodeAsLink(new_child_former_node, new_child_letter_node); //描写用のリンク
    if (node->prev != NULL)
    { // 前のノードがあれば、前のノードとつなぐ。
        node->prev->next = new_child_former_node;
        node->prev->link = new_child_former_node;
    }

    //親子関係を受け継ぐ
   if (node->is_leaf == 1) {
        int node_child_length = nodes_length(node->child);
        int n_mid_index = node_child_length / 2;
        Node *n_temp = node->child, *n_spliter = NULL, *mid_child = NULL;
        int ncl = 0;
        while (1) {
            if (ncl == n_mid_index - 1) {
                n_spliter = n_temp;
                mid_child = n_temp->next;
                break;
            }
            n_temp = n_temp->next;
            ncl++;
        }

        Node *former_child_head = node->child;
        Node *latter_child_head = mid_child;
        n_spliter->next = NULL; // リストを分割する。formerからlaterを切り離す。


        new_child_former_node->child = former_child_head;
        new_child_letter_node->child = latter_child_head;

        Node *f_child = new_child_former_node->child;
        while (f_child != NULL) {
            f_child->parent = new_child_former_node;

            new_child_former_node->edge_start = min(new_child_former_node->edge_start, f_child->edge_start);
            new_child_former_node->edge_end = max(new_child_former_node->edge_end, f_child->edge_end);

            f_child = f_child->next;
        }

        Node *l_child = new_child_letter_node->child;
        while (l_child != NULL) {
            l_child->parent = new_child_letter_node;

            new_child_letter_node->edge_start = min(new_child_letter_node->edge_start, l_child->edge_start);
            new_child_letter_node->edge_end = max(new_child_letter_node->edge_end, l_child->edge_end);

            l_child = l_child->next;
        }
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

    KeyValue *f_kvs = new_child_former_node->keyvalue;
    while (f_kvs != NULL) {
        new_child_former_node->edge_start = min(new_child_former_node->edge_start, f_kvs->key);
        new_child_former_node->edge_end = max(new_child_former_node->edge_end, f_kvs->key);
        f_kvs = f_kvs->next;
    }

    KeyValue *l_kvs = new_child_letter_node->keyvalue;
    while (l_kvs != NULL) {
        new_child_letter_node->edge_start = min(new_child_letter_node->edge_start, l_kvs->key);
        new_child_letter_node->edge_end = max(new_child_letter_node->edge_end, l_kvs->key);
        l_kvs = l_kvs->next;
    }

    new_parent_node->edge_start = min(new_child_former_node->edge_start, new_child_letter_node->edge_start);
    new_parent_node->edge_end = max(new_child_former_node->edge_end, new_child_letter_node->edge_end);

    if (node->parent == NULL) {  // 継承先がルートノードだった場合は、ルートノードを更新する。
        tree->root = new_parent_node;
        return 1;
    } else { // すでにあればマージする。
        // 親ノードの横関係を持たせる（nodeから継承）
        Node *previous_parent = node->parent;

        // ループしちゃうので、一旦外す。nodeのkvsとchildのkvsは同じオブジェクトなので。。
        Node* removed_node = removeNode(node->parent->child, node);
        if (removed_node != NULL) {
            node->parent->child = removed_node;
        }
        node->parent = NULL;
        return insert_node(tree, previous_parent, new_parent_node->keyvalue, new_parent_node->child, 1, node);
    }
}

int delete(Tree *tree, int key) {
    return delete_from_node(tree, tree->root, key, 0, NULL);
}

int insert(Tree *tree, int key, int value) {
    // 初期状態
    if (tree->root == NULL) {
        Node *node = malloc(sizeof(Node)); // rootノードを作る
        node->is_leaf = 0;
        node->edge_start = 0;
        node->edge_end = 0;
        node->keyvalue = NULL;
        node->parent = NULL; //rootノードなので親はいない。
        node->child = NULL;
        node->next = NULL;
        node->prev = NULL;
        node->link = NULL;
        tree->root = node;
    }
    // 木の中に挿入する。
    KeyValue *keyvalue = malloc(sizeof(KeyValue));
    keyvalue->key = key;
    keyvalue->value = value;
    keyvalue->next = NULL;
    return insert_node(tree, tree->root, keyvalue, NULL, 0, NULL);
}

void draw_tree(Tree *tree) {
    printf("##### Draw B+Tree #######\n\n");
    Node *node = tree->root;
    Node *layer_head = node;
    while (node != NULL) {
        printf(" [");
        KeyValue *kvs = node->keyvalue;
        while (kvs != NULL) {
            if (node->is_leaf == 1) {
                printf(" <key:%d, is_leaf)> ", kvs->key);
            } else {
                printf(" <key:%d, val:%d)> ", kvs->key, kvs->value);
            }
            kvs = kvs->next;
        }
        Node *child = node->child;
        if (child != NULL) {
            printf(" branch:");
            while (child != NULL) {
                if (child->keyvalue != NULL) {
                    printf("%d,", child->keyvalue->key);
                } else {
                    printf("NULL,");
                }
                child = child->next;
            }
            printf("");
        }
        printf("(%d-%d) ] ", node->edge_start, node->edge_end);
        if (node->link == NULL) {
            node = layer_head->child;
            layer_head = node;
            printf("\n\n");
        } else {
            node = node->link;
        }
    }
}



int main() {
    Tree *tree = malloc(sizeof(Tree));
    tree->node_count = 0;
    for (int i = 1; i < 4; i++) {
        insert(tree, i, i * 2);
    }
    // // draw_tree(tree);
    // for (int i = 1; i < 100; i++) {
    //     printf("find (key -> %d): %d\n", i, read(tree, tree->root, i));
    // }
    printf("find (key -> %d): %d\n", 33, read(tree, tree->root, 33));
    // delete(tree, 1);
     draw_tree(tree);
    return 0;
}
