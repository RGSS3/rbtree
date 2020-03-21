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

    int mark;
    Node *next;
} NILNODE, *NIL = &NILNODE;

Node objnode, *objlist = &objnode;

void gclean(void) {
    for (Node *p = objlist->next; p; p = p->next) {
        p->mark = 0;
    }
}

void gmark(Node *a) {
    if (a == NIL) {
        return ;
    }
    a->mark = 1;
    gmark(a->l);
    gmark(a->r);
}

void gfree(void) {
    int freecnt = 0;
    for (Node *p = objlist; p->next; ) {
        if (!p->next->mark) {
            Node *tmp = p->next;
            p->next = p->next->next;
            free(tmp);
            ++freecnt;
        } else {
            p = p->next;
        }
    }
    printf("Free Count:%d\n", freecnt);
}

Node *makeNode(Color c, Node *l, int val, Node *r) {
    Node *m = (Node *)malloc(sizeof(*m));
    *m = (Node) {
        c, l, val, r, 0, objlist->next
    };
    objlist->next = m;
    return m;
}

#define Run(cond, L, x, R)                       \
    if (_current != NIL && cond) {               \
       Node *save = _current;                    \
       x = _current->val;                        \
       {                                         \
            _current = save->l;                  \
            L                                    \
            _current = save->r;                  \
            R                                    \
            _current = save;                     \
       }                                         \
    } else {                                     \
        match = false;                           \
        break;                                   \
    }

#define B1(L, x, r) Run(_current->c == CB, L, x, r)
#define R1(L, x, r) Run(_current->c == CR, L, x, r)
#define R2(L, x, r) Run(_current->c == CR, L, x, r)
#define On(n, P, Q) \
    do {                   \
       bool match = true;  \
       Node *_current = n; \
       do {                \
           P               \
       } while (0);        \
       if (match) {        \
           Q               \
       }                   \
    } while(0);
#define PURE(a) a = _current;

Node *balance(Node *n) {
    Node *a, *b, *c, *d;
    int x, y, z;
#define A PURE(a)
#define B PURE(b)
#define C PURE(c)
#define D PURE(d)
#define RET return makeNode(CR, makeNode(CB, a, x, b), y, makeNode(CB, c, z, d));
    On(n, B1(R1(R2(A, x, B), y, C), z, D), RET)
    On(n, B1(R1(A, x, R2(B, y, C)), z, D), RET)
    On(n, B1(A, x, R1(R2(B, y, C), z, D)), RET)
    On(n, B1(A, x, R1(B, y, R2(C, z, D))), RET)
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
        return makeNode(CR, NIL, x, NIL);
    }
    NODE(c, y, l, r, t);
    if (x < y) {
        return balance(makeNode(c, ins(l, x), y, r));
    } else if (x > y) {
        return balance(makeNode(c, l, y, ins(r, x)));
    } else {
        return t;
    }
}

void gcstart(Node *root) {
    gclean();
    gmark(root);
    gfree();
}

Node *insert(Node *root, int x) {
    root = ins(root, x);
    gcstart(root);
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

int main(void) {
    Node *root = NIL;
    for (int i = 0; i < 100; ++i) {
        root = insert(root, rand() % 100);
        print(root, 0);
    }
    gcstart(NIL);
}

