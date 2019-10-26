#include <stdio.h>
#include <algorithm>
#define N 80011
using namespace std;
int L[N], R[N], v[N], p[N], size[N], fa[N], rnk[N];
int tot = 0;
inline void up(int t) {
    size[t] = size[L[t]] + size[R[t]] + 1;
    if (L[t]) fa[L[t]] = t;
    if (R[t]) fa[R[t]] = t;
}
void build(int& t, int l, int r) {
    int mid = (l+r) >> 1;
    t=++tot;
    p[t] = rand();
    if (mid > l) build(L[t], l, mid - 1);
    scanf("%d", &v[t]);
    rnk[v[t]] = t;
    if (r > mid) build(R[t], mid+1, r);
    up(t);
}
void split(int t, int x, int& a, int& b) {
    if (!t) {
        a = b = 0;
        return;
    }
    if (x <= size[L[t]]) {
        split(L[t], x, a, b);
        L[t] = b;
        b = t;
    } else {
        split(R[t], x - size[L[t]] - 1, a, b);
        R[t] = a;
        a = t;
    }
    up(t);
}
int merge(int a, int b) {
    if (!a || !b) return a|b;
    if (p[a] > p[b]) {
        R[a] = merge(R[a], b);
        up(a);
        return a;
    } else {
        L[b] = merge(a, L[b]);
        up(b);
        return b;
    }
}
int get_size(int x, int rt) {
    if (x == rt) return size[L[x]] + 1;
    int ans = get_size(fa[x], rt);
    if (x == L[fa[x]]) return ans - size[R[x]] - 1;
    else return ans + size[L[x]] + 1;
}
void deal(int& x) {
    int s;
    if (L[x]) {
        s = L[x];
        L[x] = 0;L[s] = x;
    } else {
        s = R[x];
        R[x] = 0;R[s] = x;
    }
    size[x] = 1;size[s] = 2;
    fa[x] = s;fa[s] = 0;
    swap(p[x],p[s]);
    x = s;
}
int query(int t, int x) {
    if (t == 0) return 0;
    if (x <= size[L[t]]) return query(L[t], x);
    else if (x == 1 + size[L[t]]) return v[t];
    else return query(R[t], x - size[L[t]] - 1);
}
void print(int t) {
    if (L[t]) print(L[t]);
    printf("%d %d\n", v[t], size[t]);
    if (R[t]) print(R[t]);
}
int main() {
    int n, m;
    int rt = 0;
    scanf("%d%d", &n, &m);
    build(rt, 1, n);
    char q[10];
    int q1, q2;
    int a, b, c, d, pos;
    for (int i = 1; i <= m; i++) {
        scanf("%s", q);
        scanf("%d", &q1);
        switch (q[0])
        {
        case 'T':
            pos = get_size(rnk[q1], rt);
            split(rt, pos-1, a, b);
            split(b, 1, c, d);
            rt = merge(merge(c, a), d);
            break;
        case 'B':
            pos = get_size(rnk[q1], rt);
            split(rt, pos-1, a, b);
            split(b, 1, c, d);
            rt = merge(merge(a, d), c);
            break;
        case 'I':
            scanf("%d", &q2);
            if (q2 == 0) break;
            pos = get_size(rnk[q1], rt);
            if (q2 == 1) split(rt, pos-1, a, b);
            else split(rt, pos-2, a, b);
            split(b, 2, c, d);
            deal(c);
            rt = merge(merge(a, c), d);
            break;
        case 'A':
            pos = get_size(rnk[q1], rt);
            printf("%d\n", pos-1);
            break;
        default:
            printf("%d\n", query(rt, q1));
            break;
        }
    }
    return 0;
}