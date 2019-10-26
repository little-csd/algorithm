#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#define N 500011
#define T Node
#define INF 2000000000
using namespace std;
int read() {
    int x = 0, f = 1;
    char c = getchar();
    while (c != '-' && (c < '0' || c > '9')) c = getchar();
    if (c == '-') f = -1, c = getchar();
    while (c >= '0' && c <= '9') x = x * 10 + c - '0', c = getchar();
    return x * f;
}
struct Treap {
    struct Node {
        int v, p, size;
        int sum, msum, lsum, rsum;
        int rev, set;
        Node *l, *r;
        Node(int v, T* k):v(v),l(k),r(k),sum(v),msum(v),lsum(v),rsum(v) {
            rev = 0;
            set = INF;
            p = rand();
            size = 1;
        }
        void up() {
            size = l->size + r->size + 1;
            sum = l->sum + r->sum + v;
            lsum = max(l->lsum, max(l->sum + v, l->sum + v + r->lsum));
            rsum = max(r->rsum, max(r->sum + v, r->sum + v + l->rsum));
            msum = max(max(l->msum, r->msum), max(l->rsum, 0) + v + max(r->lsum, 0));
        }
        void down() {
            // set: 当前层更新好，下一层待更新
            if (set != INF) {
                if (l->size) {
                    l->set = l->v = set;
                    l->sum = l->size * set;
                    l->msum = l->lsum = l->rsum = max(l->sum, set);
                }
                if (r->size) {
                    r->v = r->set = set;
                    r->sum = r->size * set;
                    r->msum = r->lsum = r->rsum = max(r->sum, set);
                }
                set = INF;
            }
            // rev: 当前层已更新，下一层待更新
            if (rev) {
                rev = !rev;
                if (l->size) {
                    l->rev = !l->rev;
                    swap(l->l, l->r);
                    swap(l->lsum, l->rsum);
                }
                if (r->size) {
                    r->rev = !r->rev;
                    swap(r->l, r->r);
                    swap(r->lsum, r->rsum);
                }
            }
        }
    } *rt, *null;
    Treap() {
        null = new Node(0, NULL);
        null->size = 0;null->sum = 0;
        null->lsum = null->rsum = null->msum = -INF;
        rt = null;
    }
    void split(T* t, int x, T* &l, T* &r) {
        if (t == null) {
            l = r = null;
            return;
        }
        t->down();
        if (x <= t->l->size) {
            split(t->l, x, l, r);
            t->l = r;
            r = t;
        } else {
            split(t->r, x - t->l->size - 1, l, r);
            t->r = l;
            l = t;
        }
        t->up();
    }
    T* merge(T* a, T* b) {
        if (a == null) return b;
        if (b == null) return a;
        if (a->p > b->p) {
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
    void build(T* &t, int l, int r, int* in) {
        int mid = (l+r) >> 1;
        t = new Node(in[mid], null);
        if (mid > l) build(t->l, l, mid-1, in);
        if (r > mid) build(t->r, mid+1, r, in);
        t->up();
    }
    void insert(int pos, int* in, int size) {
        T* t = null;
        build(t, 0, size-1, in);
        T *a, *b;
        split(rt, pos, a, b);
        rt = merge(a, merge(t, b));
    }
    void remove(int pos, int tot) {
        T *a, *b, *c, *d;
        split(rt, pos-1, a, b);
        split(b, tot, c, d);
        del(c);
        rt = merge(a, d);
    }
    void reverse(int pos, int tot) {
        T *a, *b, *c, *d;
        split(rt, pos-1, a, b);
        split(b, tot, c, d);
        c->rev = !c->rev;
        swap(c->l, c->r);
        swap(c->lsum, c->rsum);
        rt = merge(a, merge(c, d));
    }
    void set(int pos, int tot, int v) {
        T *a, *b, *c, *d;
        split(rt, pos-1, a, b);
        split(b, tot, c, d);
        c->v = c->set = v;
        c->sum = c->size * v;
        if (v > 0) c->msum = c->lsum = c->rsum = c->sum;
        else c->msum = c->lsum = c->rsum = v;
        rt = merge(a, merge(c, d));
    }
    int query(int pos, int tot) {
        T *a, *b, *c, *d;
        split(rt, pos-1, a, b);
        split(b, tot, c, d);
        int ans = c->sum;
        rt = merge(a, merge(c, d));
        return ans;
    }
    int max_sum() {
        return rt->msum;
    }
    void del(T* t) {
        if (t == null) return;
        del(t->l);
        del(t->r);
        delete t;
    }
} tree;
int a[N];
int main() {
    int n = read(), m = read();
    for (int i = 0; i < n; i++) a[i] = read();
    tree.insert(0, a, n);
    char s[15];
    int pos, tot, tt;
    for (int i = 1; i <= m; i++) {
        scanf("%s", s);
        if (s[0] == 'I') {
            pos = read(), tot = read();
            for (int j = 0; j < tot; j++) a[j] = read();
            tree.insert(pos, a, tot);
        } else if (s[0] == 'D') {
            pos = read(), tot = read();
            tree.remove(pos, tot);
        } else if (s[0] == 'R') {
            pos = read(), tot = read();
            tree.reverse(pos, tot);
        } else if (s[0] == 'G') {
            pos = read(), tot = read();
            printf("%d\n", tree.query(pos, tot));
        } else if (s[2] == 'K') {
            pos = read(), tot = read(), tt = read();
            tree.set(pos, tot, tt);
        } else printf("%d\n", tree.max_sum());
    }
    return 0;
}