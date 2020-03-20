#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
typedef enum Color { CR, CB } Color;

typedef struct Node Node;
struct Node {
    enum Color c;
    Node *l;
    int val;
    Node *r;
} NILNODE, *NIL = &NILNODE;


Node *makeNode(Node *node, Color c, Node *l, int val, Node *r) {
    Node *m = node == NULL ? (Node *)malloc(sizeof(*m)) : node;
    *m = (Node) {
        c, l, val, r
    };
    return m;
}

#define Run(t, cond, L, x, R)                    \
    if (_current != NIL && cond) {               \
       t = _current;                             \
       Node *save = _current;                    \
       x = _current->val;                        \
       {                                         \
            _current = save->l;                  \
            L;                                   \
            _current = save->r;                  \
            R;                                   \
            _current = save;                     \
       }                                         \
    } else {                                     \
        match = false;                           \
        break;                                   \
    }

#define B1(t, L, x, r) Run(t, _current->c == CB, L, x, r)
#define R1(t, L, x, r) Run(t, _current->c == CR, L, x, r)
#define R2(t, L, x, r) Run(t, _current->c == CR, L, x, r)
#define On(n, P, Q) \
    do {                   \
       bool match = true;  \
       Node *_current = n; \
       do {                \
           P;              \
       } while (0);        \
       if (match) {        \
           Q;              \
       }                   \
    } while(0);
#define PURE(a) a = _current;

Node *balance(Node *n) {
    typeof(Node *) a, b, c, d, t1, t2, t3;
    int x, y, z;
#define A PURE(a)
#define B PURE(b)
#define C PURE(c)
#define D PURE(d)
#define RET return makeNode(t1, CR, makeNode(t2, CB, a, x, b), y, makeNode(t3, CB, c, z, d))
    On(n, B1(t1, R1(t2, R2(t3, A, x, B), y, C), z, D), RET);
    On(n, B1(t1, R1(t2, A, x, R2(t3, B, y, C)), z, D), RET);
    On(n, B1(t1, A, x, R1(t2, R2(t3, B, y, C), z, D)), RET);
    On(n, B1(t1, A, x, R1(t2, B, y, R2(t3, C, z, D))), RET);
#undef RET
#undef D
#undef C
#undef B
#undef A
    return n;
}

#define NODE(c, y, l, r, n) \
    Color c = n->c;   \
    int y = n->val; \
    Node *l  = n->l;   \
    Node *r  = n->r;

Node* ins(Node *t, int x) {
    if (t == NIL) {
        return makeNode(NULL, CR, NIL, x, NIL);
    }
    NODE(c, y, l, r, t);
    if (x < y) {
        return balance(makeNode(t, c, ins(l, x), y, r));
    } else if (x > y) {
        return balance(makeNode(t, c, l, y, ins(r, x)));
    } else {
        return t;
    }
}


Node *insert(Node *root, int x) {
    root = ins(root, x);
    root->c = CB;
    return root;
}

void print(Node *root, int indent) {
    if (root == NIL) {
        printf("%*sNIL\n", indent, "");
        return;
    }
    printf("%*s%d%c\n", indent, "", root->val, root->c == CB ? 'B' : 'R');
    print(root->l, indent + 1);
    print(root->r, indent + 1);
}

void freenode(Node *root) {
    if (root != NIL) {
        NODE(c, y, l, r, root);
        freenode(l);
        freenode(r);
        free(root);
    }
}

int main(void) {
    Node *root = NIL;
    for (int i = 0; i < 100; ++i) {
        root = insert(root, rand() % 100);
        print(root, 0);
    }
    freenode(root);
}

