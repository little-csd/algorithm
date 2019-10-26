#include <stdio.h>
#include <algorithm>
#define ll long long
#define N 100111
#define MAXN (N<<5)
#define mid(l,r) ((l+r)>>1)
using namespace std;
int n, ver = 0;
int totNum[N];
int tot = 0, rt[N], lson[MAXN], rson[MAXN];
ll sum[MAXN], tag[MAXN], tsum[MAXN];
ll update(int pre, int& cur, int l, int r, int L, int R, int v) {
    cur = ++tot;
    lson[cur] = lson[pre];
    rson[cur] = rson[pre];
    tsum[cur] = tsum[pre];
    tag[cur] = tag[pre];
    if (L <= l && r <= R) {
        ll delta = (r - l + 1) * v;
        tag[cur] += v;
        tsum[cur] += delta;
        return delta;
    }
    int mid = mid(l,r);
    ll delta = 0;
    if (L <= mid) delta += update(lson[pre], lson[cur], l, mid, L, R, v);
    if (R > mid) delta += update(rson[pre], rson[cur], mid+1, r, L, R, v);
    tsum[cur] += delta;
    return delta;
}

ll query(int rt, int l, int r, int L, int R, ll tg) {
    if (L <= l && r <= R) {
        return tsum[rt] + tg * (r - l + 1);
    }
    tg += tag[rt];
    int mid = mid(l,r);
    if (R <= mid) return query(lson[rt], l, mid, L, R, tg);
    else if (L > mid) return query(rson[rt], mid+1, r, L, R, tg);
    else return query(lson[rt], l, mid, L, R, tg) + query(rson[rt], mid+1, r, L, R, tg);
}

int main() {
    int q, x;
    while (scanf("%d%d", &n, &q) != EOF) {
        tot = 0;
        ver = 0;
        for (int i = 1; i <= n; i++) {
            scanf("%d", &x);
            sum[i] = sum[i-1] + x;
        }
        char c[5];
        int l, r, d;
        for (int i = 1; i <= q; i++) {
            scanf("%s", c);
            switch (c[0]) {
                case 'Q':
                    scanf("%d%d", &l, &r);
                    printf("%lld\n", query(rt[ver], 1, n, l, r, 0) - sum[l-1] + sum[r]);
                    break;
                case 'H':
                    scanf("%d%d%d", &l, &r, &d);
                    printf("%lld\n", query(rt[d], 1, n, l, r, 0) - sum[l-1] + sum[r]);
                    break;
                case 'C':
                    scanf("%d%d%d", &l, &r, &d);
                    ver++;
                    update(rt[ver-1], rt[ver], 1, n, l, r, d);
                    totNum[ver] = tot;
                break;
                default:
                    scanf("%d", &d);
                    ver = d;
                    tot = totNum[ver];
            }
        }
    }
    return 0;
}