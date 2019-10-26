#include <stdio.h>
#include <algorithm>
#define mid(l,r) ((l+r)>>1)
#define N 200111
#define MAXN (N<<5)
using namespace std;
int tot = 0, a[N], b[N];
int rt[N], lson[MAXN], rson[MAXN], sum[MAXN];
int read() {
    int ans = 0;
    char c = getchar();
    while (c < '0' || c > '9') c = getchar();
    while (c >= '0' && c <= '9') {
        ans = ans * 10 + c - '0';
        c = getchar();
    }
    return ans;
}

void build(int& x, int l, int r) {
    x = ++tot;
    sum[x] = 0;
    if (l == r) return;
    int mid = mid(l,r);
    build(lson[x], l, mid);
    build(rson[x], mid+1, r);
}

void update(int& x, int pre, int l, int r, int v) {
    x = ++tot;
    sum[x] = sum[pre] + 1;
    if (l == r) return;
    lson[x] = lson[pre];
    rson[x] = rson[pre];
    int mid = mid(l,r);
    if (v <= b[mid]) update(lson[x], lson[pre], l, mid, v);
    else update(rson[x], rson[pre], mid+1, r, v);
}

int query(int L, int R, int l, int r, int k) {
    if (l == r) return l;
    int s = sum[lson[R]] - sum[lson[L]];
    int mid = mid(l,r);
    if (k <= s) return query(lson[L], lson[R], l, mid, k);
    else return query(rson[L], rson[R], mid+1, r, k - s);
}

int main() {
    int n, q;
    n = read(), q = read();
    for (int i = 1; i <= n; i++) {
        a[i] = read();
        b[i] =  a[i];
    }
    sort(b+1, b+n+1);
    int m = unique(b+1, b+n+1) - (b+1);
    // build(rt[0], 1, m);
    // for (int i = 1; i <= m; i++) printf("%d\n", b[i]);
    for (int i = 1; i <= n; i++) {
        update(rt[i], rt[i-1], 1, m, a[i]);
    }
    int l, r, k;
    for (int i = 1; i <= q; i++) {
        l = read(), r = read(), k = read();
        int ans = query(rt[l-1], rt[r], 1, m, k);
        printf("%d\n", b[ans]);
    }
    return 0;
}