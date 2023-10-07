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
    struct Node *link;
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


int removeNode(Node *head, Node *q) {
    if (head == NULL) {
        return 0;
    } else {
        Node *p = head;
        //  while (p->next != NULL) {
        while (p != NULL) {
            if (p == q) {
                if (p->prev == NULL) {
                    head = p->next;
                    p->next->prev = NULL;
                } else {
                    p->prev->next = p->next;
                    p->next->prev = p->prev;
                }
                return 1;
            }
            p = p->next;
        }
    }
    return 0;
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
        } else {
            Node *peer_head = node;
            while (peer_head->prev != NULL) {
                peer_head = peer_head->prev;
            }
            while (peer_head != NULL)
            {
                int removed_kvs = search_key(peer_head->keyvalue, key);
                if (removed_kvs != -1) // 削除対象のキーがあった場合
                {
                    KeyValue *keyvalue = malloc(sizeof(KeyValue));
                    keyvalue->key = key;
                    keyvalue->value = from->keyvalue->value;
                    keyvalue->next = NULL;
                    peer_head->keyvalue = replaceKeyValue(peer_head->keyvalue, key, keyvalue); // 削除対象のキーを置き換える。
                    break; // 一つ見つかれば終了
                }
                peer_head = peer_head->next; // 次のノードに移動
            }
            if (node->parent == NULL) {
                return 1; // ルートノードだった場合は、終了(一番topまで探索した)
            } else {
                return delete_from_node(tree, node->parent, key, 1, node);
            }
        }
    } else {
        KeyValue *removed_kvs = removeKeyValue(node->keyvalue, key);
        if (removed_kvs != NULL) 
        {
            node->keyvalue = removed_kvs;
            return delete_from_node(tree, node->parent, key, 1, node);
        }
    }
    printf("error");
    return -1;
}


int read(Tree *tree, Node *node, int key) {
    if (node->is_leaf == 1) {
        Node *child_head = node->child; //子ノードの先頭を取得!
        Node *appled_child = child_head;
        while (child_head != NULL) {
            if (child_head->keyvalue->key <= key) { 
                if (appled_child->keyvalue->key <= child_head->keyvalue->key) { 
                        appled_child = child_head;
                }
            } 
            child_head = child_head->next;
        }
        // 子ノードに橋渡し
        return read(tree, appled_child, key);
    } else {
        printf("search: %d\n", node->keyvalue->key);
        return search_key(node->keyvalue, key);
    }
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
        node->keyvalue = combineKeyValueStore(node->keyvalue, kvs);
        node->keyvalue = quick_sort(node->keyvalue);
        Node *child = childs_head;
        while (child != NULL) {
            child->parent = node;
            child = child->next;
        }
        return 1;
    }

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

    //init
    new_parent_node->parent = NULL;
    new_parent_node->child = NULL;


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
        tree->root = new_parent_node;
        return 1;
    } else { // すでにあればマージする。
        // 親ノードの横関係を持たせる（nodeから継承）
        Node *previous_parent = node->parent;

        // ループしちゃうので、一旦外す。nodeのkvsとchildのkvsは同じオブジェクトなので。。
        removeNode(node->parent->child, node); 
        node->parent = NULL;
        return insert_node(tree, previous_parent, new_parent_node->keyvalue, new_parent_node->child, 1);
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
    return insert_node(tree, tree->root, keyvalue, NULL, 0);
}

void draw_tree(Tree *tree) {
    printf("##### Draw B+Tree #######\n\n");
    Node *node = tree->root;
    Node *layer_head = node;
    while (node != NULL) {
        printf(" [");
        KeyValue *kvs = node->keyvalue;
        while (kvs != NULL) {
            printf(" <key: %d (%d)> ", kvs->key, node->is_leaf);
            kvs = kvs->next;
        }
        Node *child = node->child;
        if (child != NULL) {
            printf(" (");
            while (child != NULL) {
                printf("%d,", child->keyvalue->key);
                child = child->next;
            }
            printf(" )");
        }
        printf("] ");
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
    insert(tree, 7, 1);
    insert(tree, 3, 2);
    insert(tree, 2, 3);
    insert(tree, 1, 3);
    insert(tree, 4, 3);
    draw_tree(tree);
    printf("read: %d\n", read(tree, tree->root, 1));
    // delete(tree, 1);
    draw_tree(tree);
    return 0;
}
