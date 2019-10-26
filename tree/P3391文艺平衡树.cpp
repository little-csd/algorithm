#include <stdio.h>
#include <algorithm>
#define T Node
using namespace std;
class Treap {
    struct Node {
        int v, size, flag;
        Node *l, *r;
        Node(int v, T* k):v(v),l(k),r(k){
            flag = 0;
            size = 1;
        }
        inline void up() {
            size = l->size + r->size + 1;
        }
        inline void down() {
            if (!flag) return;
            l->flag = !l->flag;
            r->flag = !r->flag;
            flag = !flag;
            T* swp = l;
            l = r;
            r = swp;
        }
    } *rt, *empty;

    void split(T* t, int v, T* &l, T* &r) {
        if (t == empty) {
            l = r = empty;
            return;
        }
        t->down();
        if (v <= t->l->size) {
            split(t->l, v, l, r);
            t->l = r;
            r = t;
        } else {
            split(t->r, v - t->l->size - 1, l, r);
            t->r = l;
            l = t;
        }
        t->up();
    }
    T* merge(T* a, T* b) {
        if (a == empty) return b;
        if (b == empty) return a;
        if (a->v > b->v) {
            a->down();
            a->r = merge(a->r, b);
            a->up();
            return a;
        } else {
            b->down();
            b->l = merge(a, b->l);
            b->up();
            return b;
        }
    }
    void print(T* t) {
        if (t == empty) return;
        t->down();
        print(t->l);
        printf("%d ", t->v);
        print(t->r);
    }
    void build(T* &t, int l, int r) {
        int mid = (l+r)>>1;
        t = new Node(mid, empty);
        if (mid > l) build(t->l, l, mid-1);
        if (r > mid) build(t->r, mid+1, r);
        t->up();
    }
public:    
    Treap() {
        empty = new Node(0, NULL);
        empty->size = 0;
        rt = empty;
    }
    void init(int n) {
        build(rt, 1, n);
    }
    void rot(int l, int r) {
        T *a, *b, *c, *d;
        split(rt, l-1, a, b);
        split(b, r-l+1, c, d);
        c->flag = !c->flag;
        rt = merge(a, merge(c, d));
    }
    void output() {
        print(rt);
    }
};

int main() {
    Treap* tree = new Treap();
    int n, m;
    scanf("%d%d", &n, &m);
    tree->init(n);
    int l, r;
    for (int i = 1; i <= m; i++) {
        scanf("%d%d", &l, &r);
        tree->rot(l, r);
    }
    tree->output();
    return 0;
}