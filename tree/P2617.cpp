#include <stdio.h>
#include <algorithm>
#define N 100111
#define MAXN (N*400)
#define lowbit(x) (x&-x)
#define mid(l,r) ((l+r)>>1)
using namespace std;
int tot = 0, rt[N], lson[MAXN], rson[MAXN], sum[MAXN];
int n, m, q;
int a[N], b[N<<1], qa[N], qb[N], qc[N];
int ql[10000], qr[10000], tl, tr;
int read() {
    int x = 0;
    char c = getchar();
    while (c < '0' || c > '9') c = getchar();
    while (c >= '0' && c <= '9') {
        x = x * 10 + c - '0';
        c = getchar();
    }
    return x;
}

void print(int x) {
    if (x > 9) print(x/10);
    putchar(x%10+'0');
}

void update(int pre, int& now, int l, int r, int v, int type) {
    now = ++tot;
    sum[now] = sum[pre] + type;
    if (l == r) return;
    lson[now] = lson[pre];
    rson[now] = rson[pre];
    int mid = mid(l,r);
    if (v <= mid) update(lson[pre], lson[now], l, mid, v, type);
    else update(rson[pre], rson[now], mid+1, r, v, type);
}

inline void add(int x, int type) {
    int pos = lower_bound(b+1, b+m+1, a[x]) - b;
    while (x <= n) {
        update(rt[x], rt[x], 1, m, pos, type);
        x += lowbit(x);
    }
}

int query(int l, int r, int k) {
    if (l == r) return l;
    int x = 0;
    for (int i = 1; i <= tl; i++) x -= sum[lson[ql[i]]];
    for (int i = 1; i <= tr; i++) x += sum[lson[qr[i]]];
    int mid = mid(l,r);
    if (k <= x) {
        for (int i = 1; i <= tl; i++) ql[i] = lson[ql[i]];
        for (int i = 1; i <= tr; i++) qr[i] = lson[qr[i]];
        return query(l, mid, k);
    } else {
        for (int i = 1; i <= tl; i++) ql[i] = rson[ql[i]];
        for (int i = 1; i <= tr; i++) qr[i] = rson[qr[i]];
        return query(mid+1, r, k - x);
    }
}

int main() {
    n = read(), q = read();
    for (int i = 1; i <= n; i++) {
        a[i] = read();
        b[i] = a[i];
    }
    char c;
    m = n;
    for (int i = 1; i <= q; i++) {
        c = getchar();
        while (c != 'Q' && c != 'C') c = getchar();
        if (c == 'Q') {
            qa[i] = read();
            qb[i] = read();
            qc[i] = read();
        } else {
            qa[i] = read();
            qb[i] = read();
            qc[i] = -1;
            b[++m] = qb[i];
        }
    }
    sort(b+1, b+m+1);
    m = unique(b+1, b+m+1) - (b+1);
    int l, r;
    for (int i = 1; i <= n; i++) add(i, 1);
    for (int i = 1; i <= q; i++) {
        if (qc[i] == -1) {
            add(qa[i], -1);
            a[qa[i]] = qb[i];
            add(qa[i], 1);
        } else {
            l = qa[i] - 1;
            r = qb[i];
            tl = tr = 0;
            while (l) {
                ql[++tl] = rt[l];
                l -= lowbit(l);
            }
            while (r) {
                qr[++tr] = rt[r];
                r -= lowbit(r);
            }
            int ans = query(1, m, qc[i]);
            print(b[ans]);
            putchar('\n');
        }
    }
    return 0;
}