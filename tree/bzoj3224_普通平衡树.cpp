#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#define T Node*
using namespace std;
struct Treap {
    struct Node {
        Node *left, *right;
        int v, w, size, p;
        Node(int v, T k):v(v) {
            left = right = k;
            p = rand();
            w = size = 1;
        }
        inline void up() {
            size = left->size + right->size + w;
        }
    } *rt, *empty;

    Treap() {
        empty = new Node(0, NULL);
        empty->w = empty->size = 0;
        empty->p = -2;
        rt = empty;
    }

    inline void rotL(T &t) {
        T t0 = t->left;
        t->left = t0->right;
        t0->right = t;
        t->up();t0->up();
        t = t0;
    }

    inline void rotR(T &t) {
        T t0 = t->right;
        t->right = t0->left;
        t0->left = t;
        t->up();t0->up();
        t = t0;
    }

    void insert(T &t, int v) {
        if (t == empty) t = new Node(v, empty);
        else {
            if (v == t->v) t->w++;
            else if (v < t->v) {
                insert(t->left, v);
                if (t->left->p > t->p) rotL(t);
            }
            else {
                insert(t->right, v);
                if (t->right->p > t->p) rotR(t);
            }
            t->up();
        }
    }

    void remove(T &t, int v) {
        if (v < t->v) remove(t->left, v);
        else if (v > t->v) remove(t->right, v);
        else {
            if (t->w > 1) t->w--;
            else {
                if (t->left != empty && t->right != empty) {
                    if (t->left->p > t->right->p) {
                        rotL(t);
                        remove(t->right, v);
                    } else {
                        rotR(t);
                        remove(t->left, v);
                    }
                    t->up();
                } else {
                    T t0 = t;
                    if (t->left == empty) t = t->right;
                    else t = t->left;
                    delete t0;
                }
            }
        }
        if (t != empty) t->up();
    }

    int rnk(T &t, int v) {
        int x = t->left->size;
        if (v < t->v) return rnk(t->left, v);
        else if (v > t->v) return x + t->w + rnk(t->right, v);
        else return x+1;
    }

    int kth(T &t, int v) {
        int x = t->left->size;
        if (x >= v) return kth(t->left, v);
        else if (x + t->w >= v) return t->v;
        else return kth(t->right, v - x - t->w);
    }

    int pre(int v) {
        T t = rt;
        int ans = 0;
        while (t != empty) {
            if (v > t->v) {
                ans = t->v;
                t = t->right;
            } else t = t->left;
        }
        return ans;
    }

    int post(int v) {
        T t = rt;
        int ans = 0;
        while (t != empty) {
            if (v < t->v) {
                ans = t->v;
                t = t->left;
            } else t = t->right;
        }
        return ans;
    }
} tree;

int main() {
    int n, type, v;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        // printf("%p\n", rt);
        scanf("%d%d", &type, &v);
        switch (type) {
            case 1:
            tree.insert(tree.rt, v);
            break;
            case 2:
            tree.remove(tree.rt, v);
            break;
            case 3:
            printf("%d\n", tree.rnk(tree.rt, v));
            break;
            case 4:
            printf("%d\n", tree.kth(tree.rt, v));
            break;
            case 5:
            printf("%d\n", tree.pre(v));
            break;
            default:
            printf("%d\n", tree.post(v));
        }
    }
    return 0;
}