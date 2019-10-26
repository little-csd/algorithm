#include <stdio.h>
#include <algorithm>
#define N 50111
#define MAXN (N<<7)
#define mid(l,r) ((l+r)>>1)
#define INF 2147483647
using namespace std;
int type[N], qa[N], qb[N], qc[N];
int n, m, a[N], b[N<<1];
int tot = 0;
int rt[N], L[MAXN], R[MAXN], sz[MAXN];
int tp1, tp2, sta1[100], sta2[100];
inline int lowbit(int x) {
    return x&-x;
}
void update(int pre, int& cur, int l, int r, int p, int type) {
    if (!cur) cur = ++tot;
    sz[cur] = sz[pre] + type;
    if (l == r) return;
    L[cur] = L[pre];
    R[cur] = R[pre];
    int mid = mid(l,r);
    if (p <= mid) update(L[pre], L[cur], l, mid, p, type);
    else update(R[pre], R[cur], mid+1, r, p, type);
}
void add(int i, int type) {
    int pos = lower_bound(b+1, b+m+1, a[i]) - b;
    while (i <= n) {
        update(rt[i], rt[i], 1, m, pos, type);
        i += lowbit(i);
    }
}
int rnk(int l, int r, int v) {
    int mid, ans = 0;
    while (l < r) {
        mid = mid(l,r);
        if (v <= b[mid]) {
            r = mid;
            for (int i = 1; i <= tp1; i++) sta1[i] = L[sta1[i]];
            for (int i = 1; i <= tp2; i++) sta2[i] = L[sta2[i]];
        } else {
            l = mid + 1;
            for (int i = 1; i <= tp1; i++) {
                ans -= sz[L[sta1[i]]];
                sta1[i] = R[sta1[i]];
            }
            for (int i = 1; i <= tp2; i++) {
                ans += sz[L[sta2[i]]];
                sta2[i] = R[sta2[i]];
            }
            
        }
    }
    return ans + 1;
}
int kth(int l, int r, int p) {
    int mid, lsz;
    while (l < r) {
        mid = mid(l,r);
        lsz = 0;
        for (int i = 1; i <= tp1; i++) lsz -= sz[L[sta1[i]]];
        for (int i = 1; i <= tp2; i++) lsz += sz[L[sta2[i]]];
        if (p <= lsz) {
            r = mid;
            for (int i = 1; i <= tp1; i++) sta1[i] = L[sta1[i]];
            for (int i = 1; i <= tp2; i++) sta2[i] = L[sta2[i]];
        } else {
            p -= lsz;
            l = mid + 1;
            for (int i = 1; i <= tp1; i++) sta1[i] = R[sta1[i]];
            for (int i = 1; i <= tp2; i++) sta2[i] = R[sta2[i]];
        }
    }
    return b[l];
}
int pre(int l, int r, int v) {
    if (l == r) {
        if (v <= b[l]) return INF;
        int ans = 0;
        for (int i = 1; i <= tp1; i++) ans -= sz[sta1[i]];
        for (int i = 1; i <= tp2; i++) ans += sz[sta2[i]];
        return ans > 0 ? b[l] : INF;
    }
    int mid = mid(l,r);
    if (v <= b[mid]) {
        for (int i = 1; i <= tp1; i++) sta1[i] = L[sta1[i]];
        for (int i = 1; i <= tp2; i++) sta2[i] = L[sta2[i]];
        return pre(l, mid, v);
    }
    else {
        int a1[25], a2[25];
        for (int i = 1; i <= tp1; i++) {
            a1[i] = L[sta1[i]];
            sta1[i] = R[sta1[i]];
        }
        for (int i = 1; i <= tp2; i++) {
            a2[i] = L[sta2[i]];
            sta2[i] = R[sta2[i]];
        }
        int ans = pre(mid+1, r, v);
        if (ans != INF) return ans;
        for (int i = 1; i <= tp1; i++) sta1[i] = a1[i];
        for (int i = 1; i <= tp2; i++) sta2[i] = a2[i];
        return pre(l, mid, v);
    }
}
int nxt(int l, int r, int v) {
    if (l == r) {
        if (v >= b[l]) return INF;
        int ans = 0;
        for (int i = 1; i <= tp1; i++) ans -= sz[sta1[i]];
        for (int i = 1; i <= tp2; i++) ans += sz[sta2[i]];
        return ans > 0 ? b[l] : INF;
    }
    int mid = mid(l,r);
    if (v >= b[mid]) {
        for (int i = 1; i <= tp1; i++) sta1[i] = R[sta1[i]];
        for (int i = 1; i <= tp2; i++) sta2[i] = R[sta2[i]];
        return nxt(mid+1, r, v);
    } else {
        int a1[25], a2[25];
        for (int i = 1; i <= tp1; i++) {
            a1[i] = R[sta1[i]];
            sta1[i] = L[sta1[i]];
        }
        for (int i = 1; i <= tp2; i++) {
            a2[i] = R[sta2[i]];
            sta2[i] = L[sta2[i]];
        }
        int ans = nxt(l, mid, v);
        if (ans != INF) return ans;
        for (int i = 1; i <= tp1; i++) sta1[i] = a1[i];
        for (int i = 1; i <= tp2; i++) sta2[i] = a2[i];
        return nxt(mid+1, r, v);
    }
}
int main() {
    int q;
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        b[i] = a[i];
    }
    m = n;
    for (int i = 1; i <= q; i++) {
        scanf("%d", &type[i]);
        if (type[i] == 3) {
            scanf("%d%d", &qa[i], &qb[i]);
            b[++m] = qb[i];
        } else scanf("%d%d%d", &qa[i], &qb[i], &qc[i]);
    }
    sort(b+1, b+m+1);
    m = unique(b+1, b+m+1) - (b+1);
    for (int i = 1; i <= n; i++) add(i, 1);
    for (int i = 1; i <= q; i++) {
        if (type[i] == 3) {
            add(qa[i], -1);
            a[qa[i]] = qb[i];
            add(qa[i], 1);
        } else {
            tp1 = tp2 = 0;
            int L = qa[i] - 1, R = qb[i];
            while (L) {
                sta1[++tp1] = rt[L];
                L -= lowbit(L);
            }
            while (R) {
                sta2[++tp2] = rt[R];
                R -= lowbit(R);
            }
            if (type[i] == 1) printf("%d\n", rnk(1, m, qc[i]));
            else if (type[i] == 2) printf("%d\n", kth(1, m, qc[i]));
            else if (type[i] == 4) printf("%d\n", pre(1, m, qc[i]));
            else printf("%d\n", nxt(1, m, qc[i]));
        }
    }
    return 0;
}

/*
5 1
1 -2 1 3 2
2 1 4 4
*/