#include <stdio.h>
#include <algorithm>
#include <map>
using namespace std;
#define N 100111
#define MAXN (N<<5)
#define mid(l,r) ((l+r)>>1)
int a[N], b[N];
int tot = 0;
int rt[N], lson[MAXN], rson[MAXN], sum[MAXN];

void build(int& x, int l, int r) {
    x = ++tot;
    sum[x] = 0;
    if (l == r) return;
    int mid = mid(l,r);
    build(lson[x], l, mid);
    build(rson[x], mid+1, r);
}

void update(int& x, int lrt, int l, int r, int pos) {
    x = ++tot;
    sum[x] = sum[lrt] + 1;
    if (l == r) return;
    lson[x] = lson[lrt];
    rson[x] = rson[lrt];
    int mid = mid(l,r);
    if (pos <= mid) update(lson[x], lson[lrt], l, mid, pos);
    else update(rson[x], rson[lrt], mid+1, r, pos);
}

int query(int L, int R, int l, int r, int k) {
    if (l == r) return l;
    int mid = mid(l,r);
    int cnt = sum[lson[R]] - sum[lson[L]];
    if (k <= cnt) return query(lson[L], lson[R], l, mid, k);
    else return query(rson[L], rson[R], mid+1, r, k - cnt);
}

int main() {
    int n, qs;
    scanf("%d%d", &n, &qs);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        b[i] = a[i];
    }
    sort(b+1, b+n+1);
    int m = unique(b+1, b+n+1) - (b+1);
    build(rt[0], 1, m);
    for (int i = 1; i <= n; i++) {
        int pos = lower_bound(b+1, b+m+1, a[i]) - b;
        update(rt[i], rt[i-1], 1, m, pos);
    }
    int l, r, k;
    while (qs--) {
        scanf("%d%d%d", &l, &r, &k);
        int ans = query(rt[l-1], rt[r], 1, m, k);
        printf("%d\n", b[ans]);
    }
    return 0;
}