#include <stdio.h>
#include <algorithm>
#define N 20111
#define MAXN (N<<5)
#define mid(l,r) ((l+r)>>1)
using namespace std;
int n, q[6];
int tot = 0, rt[N], lson[MAXN], rson[MAXN], sum[MAXN], lmax[MAXN], rmax[MAXN];
struct ele {
    int v, id;
    bool operator < (const ele& x) const {
        return v < x.v;
    }
} a[N];

void maintain(int rt) {
    int l = lson[rt], r = rson[rt];
    sum[rt] = sum[l] + sum[r];
    lmax[rt] = max(lmax[l], sum[l] + lmax[r]);
    rmax[rt] = max(rmax[r], sum[r] + rmax[l]);
}

void build(int& rt, int l, int r) {
    rt = ++tot;
    if (l == r) {
        sum[rt] = lmax[rt] = rmax[rt] = 1;
        return;
    }
    int mid = mid(l,r);
    build(lson[rt], l, mid);
    build(rson[rt], mid+1, r);
    maintain(rt);
}

void update(int pre, int& cur, int l, int r, int x) {
    cur = ++tot;
    if (l == r) {
        sum[cur] = lmax[cur] = rmax[cur] = -1;
        return;
    }
    lson[cur] = lson[pre];
    rson[cur] = rson[pre];
    int mid = mid(l,r);
    if (x <= mid) update(lson[pre], lson[cur], l, mid, x);
    else update(rson[pre], rson[cur], mid+1, r, x);
    maintain(cur);
}

int query_sum(int rt, int l, int r, int L, int R) {
    if (L <= l && r <= R) return sum[rt];
    int mid = mid(l,r);
    int ans = 0;
    if (L <= mid) ans += query_sum(lson[rt], l, mid, L, R);
    if (R > mid) ans += query_sum(rson[rt], mid+1, r, L, R);
    return ans;
}

int query_lmax(int rt, int l, int r, int L, int R) {
    if (L <= l && r <= R) return lmax[rt];
    int mid = mid(l,r);
    if (R <= mid) return query_lmax(lson[rt], l, mid, L, R);
    else if (L > mid) return query_lmax(rson[rt], mid+1, r, L, R);
    else return max(query_sum(lson[rt], l, mid, L, R) + query_lmax(rson[rt], mid+1, r, L, R),
            query_lmax(lson[rt], l, mid, L, R));
}

int query_rmax(int rt, int l, int r, int L, int R) {
    if (L <= l && r <= R) return rmax[rt];
    int mid = mid(l,r);
    if (R <= mid) return query_rmax(lson[rt], l, mid, L, R);
    else if (L > mid) return query_rmax(rson[rt], mid+1, r, L, R);
    else  return max(query_sum(rson[rt], mid+1, r, L, R) + query_rmax(lson[rt], l, mid, L, R),
        query_rmax(rson[rt], mid+1, r, L, R));
}

int check(int v) {
    // printf("%d %d %d %d\n", q[1], q[2], q[3], q[4]);
    int ans = 0;
    ans += query_rmax(rt[v], 1, n, q[1], q[2]);
    ans += query_lmax(rt[v], 1, n, q[3], q[4]);
    if (q[3] - q[2] > 1) ans += query_sum(rt[v], 1, n, q[2]+1, q[3]-1);
    return ans;
}

int main() {
    int qs;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i].v);
        a[i].id = i;
    }
    sort(a+1, a+n+1);
    build(rt[1], 1, n);
    for (int i = 2; i <= n; i++) update(rt[i-1], rt[i], 1, n, a[i-1].id);
    scanf("%d", &qs);
    int ans = 0;
/*
    printf("lmax:\n");
    for (int k = 1; k <= n; k++)
    for (int i = 0; i < n; i++) {
        printf("length: %d\n", i+1);
        for (int j = 1; j <= n - i; j++) {
            printf("[%d, %d]: %d\n", j, j+i, query_rmax(rt[k], 1, n, j, j+i));
        }
    }
*/
    for (int i = 1; i <= qs; i++) {
        scanf("%d%d%d%d", &q[1], &q[2], &q[3], &q[4]);
        q[1] = (q[1] + ans) % n + 1;
        q[2] = (q[2] + ans) % n + 1;
        q[3] = (q[3] + ans) % n + 1;
        q[4] = (q[4] + ans) % n + 1;
        sort(q+1,q+5);
        // printf("%d %d %d %d\n", q[1], q[2], q[3], q[4]);
        int L = 1, R = n;
        while (L <= R) {
            int mid = mid(L,R);
            if (check(mid) >= 0) ans = mid, L = mid + 1;
            else R = mid - 1;
        }
        ans = a[ans].v;
        printf("%d\n", ans);
    }
    return 0;
}