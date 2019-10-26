#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#define T Node*
#define MAXN 2147483647
#define MINN -2147483647
using namespace std;
class Treap {
private:
    struct Node {
        int v, p, w, size;
        Node *l, *r;
        Node(int v, T k):v(v) {
            w = size = 1;
            p = rand();
            l = r = k;
        }
        inline void up() {
            size = l->size + r->size + w;
        }
    } *rt, *empty;

    void split(T t, int v, T &l, T &r) {
        if (t == empty) {
            l = r = empty;
            return;
        }
        if (v <= t->v) {
            split(t->l, v, l, r);
            t->l = r;
            r = t;
        } else {
            split(t->r, v, l, r);
            t->r = l;
            l = t;
        } 
        t->up();
    }

    T merge(T a, T b) {
        if (a == empty) return b;
        if (b == empty) return a;
        if (a->p > b->p) {
            a->r = merge(a->r, b);
            a->up();
            return a;
        } else {
            b->l = merge(a, b->l);
            b->up();
            return b;
        }
    }

public:
    Treap() {
        empty = new Node(0, NULL);
        empty->size = empty->w = 0;
        empty->p = -1;
        rt = empty;
    }

    void ins(int v) {
        T a;T b;
        T c;T d;
        split(rt, v, a, b);
        split(b, v+1, c, d);
        if (c == empty) c = new Node(v, empty);
        else {
            c->w++;
            c->size++;
        }
        rt = merge(a, merge(c, d));
    }

    void rem(int v) {
        T a;T b;
        T c;T d;
        split(rt, v, a, b);
        split(b, v+1, c, d);
        if (c->w > 1) {
            c->w--;c->size--;
            d = merge(c, d);
        }
        rt = merge(a, d);
    }

    int rnk(int v) {
        T t = rt;
        int ans = 0;
        while (t != empty) {
            if (v < t->v) t = t->l;
            else if (v > t->v) {
                ans = ans + t->l->size + t->w;
                t = t->r;
            } else {
                ans = ans + t->l->size + 1;
                break;
            }
        }
        return ans;
    }

    int kth(int k) {
        T t = rt;
        while (t != empty) {
            if (k <= t->l->size) t = t->l;
            else if (k > t->l->size + t->w) {
                k = k - (t->l->size + t->w);
                t = t->r;
            } else break;
        }
        return t->v;
    }

    int pre_max(int v) {
        T t = rt;
        int ans = MINN;
        while (t != empty) {
            if (t->v >= v) t = t->l;
            else {
                ans = t->v;
                t = t->r;
            }
        }
        return ans;
    }

    int post_min(int v) {
        T t = rt;
        int ans = MAXN;
        while (t != empty) {
            if (t->v <= v) t = t->r;
            else {
                ans = t->v;
                t = t->l;
            }
        }
        return ans;
    }
};

int main() {
    int n, type, v;
    scanf("%d", &n);
    Treap* tree = new Treap();
    for (int i = 1; i <= n; i++) {
        scanf("%d%d", &type, &v);
        switch (type)
        {
        case 1:
            tree->ins(v);
            break;
        case 2:
            tree->rem(v);
            break;
        case 3:
            printf("%d\n", tree->rnk(v));
            break;
        case 4:
            printf("%d\n", tree->kth(v));
            break;
        case 5:
            printf("%d\n", tree->pre_max(v));
            break;
        default:
            printf("%d\n", tree->post_min(v));
            break;
        }
    }
    return 0;
}