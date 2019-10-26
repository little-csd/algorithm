#include <stdio.h>
#include <algorithm>
#define N 500111
#define MAXN (N<<5)
#define mid(l,r) ((l+r)>>1)
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

void update(int& x, int lpr, int l, int r, int v) {
    x = ++tot;
    sum[x] = sum[lpr] + 1;
    if (l == r) return;
    lson[x] = lson[lpr];
    rson[x] = rson[lpr];
    int mid = mid(l,r);
    if (v <= b[mid]) update(lson[x], lson[lpr], l, mid, v);
    else update(rson[x], rson[lpr], mid+1, r, v);
}

int query(int pre, int cur, int l, int r, int t) {
    if (l == r) {
        int s = sum[cur] - sum[pre];
        return s > t ? l:0;
    }
    int sl = sum[lson[cur]] - sum[lson[pre]];
    int sr = sum[rson[cur]] - sum[rson[pre]];
    // printf("%d %d %d %d\n", l, r, sl, sr);
    int mid = mid(l,r);
    if (sl < sr) return query(rson[pre], rson[cur], mid+1, r, t);
    else if (sl > sr) return query(lson[pre], lson[cur], l, mid, t);
    else return 0;
}

int main() {
    int n = read(), q = read();
    for (int i = 1; i <= n; i++) {
        a[i] = read();
        b[i] = a[i];
    }
    sort(b+1, b+n+1);
    int m = unique(b+1, b+n+1) - (b+1);
    int l, r;
    for (int i = 1; i <= n; i++)
        update(rt[i], rt[i-1], 1, m, a[i]);
    for (int i = 1; i <= q; i++) {
        l = read(), r = read();
        int times = (r - l + 1) >> 1;
        // printf("%d\n", times);
        int ans = query(rt[l-1], rt[r], 1, m, times);
        printf("%d\n", b[ans]);
    }
    return 0; 
}