#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#define N 200111
#define LL long long
#define MAXN (N << 7)
using namespace std;
int rt[N], tot = 0;
int ch[MAXN][2], w[MAXN], rev[MAXN], p[MAXN], sz[MAXN];
LL sum[MAXN];
int copyNode(int rt) {
    int cur = ++tot;
    ch[cur][0] = ch[rt][0];
    ch[cur][1] = ch[rt][1];
    sz[cur] = sz[rt];
    p[cur] = p[rt];
    rev[cur] = rev[rt];
    sum[cur] = sum[rt];
    w[cur] = w[rt];
    return cur;
}
void up(int rt) {
    int l = ch[rt][0], r = ch[rt][1];
    sum[rt] = sum[l] + sum[r] + w[rt];
    sz[rt] = sz[l] + sz[r] + 1;
}
void down(int rt) {
    int l = ch[rt][0], r = ch[rt][1];
    ch[rt][0] = ch[rt][1] = 0; // 重要。。。
    if (l) {
        int nr = copyNode(l);
        ch[rt][1] = nr;
        rev[nr] = !rev[nr];
    }
    if (r) {
        int nl = copyNode(r);
        ch[rt][0] = nl;
        rev[nl] = !rev[nl];
    }
    rev[rt] = 0;
}
void split(int rt, int k, int& l, int& r) {
    if (!rt) {
        l = r = 0;
        return;
    }
    int cur = copyNode(rt);
    if (rev[cur]) down(cur);
    int size = sz[ch[cur][0]];
    if (k <= size) {
        split(ch[cur][0], k, l, r);
        ch[cur][0] = r;
        up(cur);
        r = cur;
    } else {
        split(ch[cur][1], k - size - 1, l, r);
        ch[cur][1] = l;
        up(cur);
        l = cur;
    }
}
int merge(int a, int b) {
    if (!a || !b) return a | b;
    if (p[a] > p[b]) {
        if (rev[a]) down(a);
        ch[a][1] = merge(ch[a][1], b);
        up(a);
        return a;
    } else {
        if (rev[b]) down(b);
        ch[b][0] = merge(a, ch[b][0]);
        up(b);
        return b;
    }
}
void reverse(int pre, int& cur, int L, int R) {
    int a, b, c, d;
    split(pre, L-1, a, b);
    split(b, R-L+1, c, d);
    rev[c] ^= 1;
    cur = merge(a, merge(c, d));
}
LL query(int pre, int& cur, int L, int R) {
    int a, b, c, d;
    split(pre, L-1, a, b);
    split(b, R-L+1, c, d);
    LL ans = sum[c];
    cur = merge(a, merge(c, d));
    return ans;
}
void remove(int pre, int& cur, int pos) {
    int a, b, c, d;
    split(pre, pos-1, a, b);
    split(b, 1, c, d);
    cur = merge(a, d);
}
void insert(int pre, int& cur, int pos, int v) {
    int a, b;
    split(pre, pos, a, b);
    int c = ++tot;
    sz[c] = 1;
    p[c] = rand();
    sum[c] = w[c] = v;
    cur = merge(a, merge(c, b));
}
int main() {
    int n;
    int ver, opt;
    int p, x, l, r;
    LL ans = 0;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d%d", &ver, &opt);
        if (opt == 2) {
            scanf("%d", &x);
            x ^= ans;
            remove(rt[ver], rt[i], x);
        } else if (opt == 1) {
            scanf("%d%d", &p, &x);
            p ^= ans;
            x ^= ans;
            insert(rt[ver], rt[i], p, x);
        } else {
            scanf("%d%d", &l, &r);
            l ^= ans;
            r ^= ans;
            if (opt == 3) reverse(rt[ver], rt[i], l, r);
            else {
                // 这里不能写成rt[i] = rt[i-1]
                ans = query(rt[ver], rt[i], l, r);
                printf("%lld\n", ans);
            }
        }
    }
    return 0;
}