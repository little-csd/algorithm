#include <stdio.h>
#include <algorithm>
#define N 100111
#define MAXN (N<<5)
#define lson(d) (d<<1)
#define rson(d) (d<<1|1)
#define mid(l,r) ((l+r)>>1)
using namespace std;
int a[N], b[N];
int fa[N], son[N], size[N], dep[N], top[N];
int edge_num = 0, m, head[N];
struct edge {
    int to, next;
} e[N<<1];
int tot = 0, tr[N], lson[MAXN], rson[MAXN], sum[MAXN];
void add(int u, int v) {
    e[++edge_num].to = v;
    e[edge_num].next = head[u];
    head[u] = edge_num;

    e[++edge_num].to = u;
    e[edge_num].next = head[v];
    head[v] = edge_num;
}

void update(int& cur, int pre, int l, int r, int v) {
    cur = ++tot;
    sum[cur] = sum[pre] + 1;
    if (l == r) return;
    int mid = mid(l,r);
    lson[cur] = lson[pre];
    rson[cur] = rson[pre];
    if (v <= b[mid]) update(lson[cur], lson[pre], l, mid, v);
    else update(rson[cur], rson[pre], mid+1, r, v);
}

int query(int l, int r, int u, int v, int lca, int f_lca, int k) {
    if (l == r) return l;
    int s = sum[lson[u]] + sum[lson[v]] - sum[lson[lca]] - sum[lson[f_lca]];
    int mid = mid(l,r);
    if (k <= s) return query(l, mid, lson[u], lson[v], lson[lca], lson[f_lca], k);
    else return query(mid+1, r, rson[u], rson[v], rson[lca], rson[f_lca], k - s);
}

int get_lca(int u, int v) {
    int tu = top[u], tv = top[v];
    while (tu != tv) {
        if (dep[tu] > dep[tv]) {
            u = fa[tu];
            tu = top[u];
        } else {
            v = fa[tv];
            tv = top[v];
        }
    }
    return dep[u]>dep[v]?v:u;
}

void dfs1(int rt) {
    size[rt] = 1;
    dep[rt] = dep[fa[rt]] + 1;
    update(tr[rt], tr[fa[rt]], 1, m, a[rt]);
    for (int d = head[rt]; d; d = e[d].next) {
        int to = e[d].to;
        if (!dep[to]) {
            fa[to] = rt;
            dfs1(to);
            size[rt] += size[to];
            if (size[to] > size[son[rt]]) son[rt] = to;
        }
    }
}

void dfs2(int rt, int tp) {
    top[rt] = tp;
    if (!son[rt]) return;
    dfs2(son[rt], tp);
    for (int d = head[rt]; d; d = e[d].next) {
        int to = e[d].to;
        if (to != son[rt] && to != fa[rt])
            dfs2(to, to);
    }
}

int main() {
    int n, q;
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        b[i] = a[i];
    }
    int l, r;
    for (int i = 1; i < n; i++) {
        scanf("%d%d", &l, &r);
        add(l,r);
    }
    sort(b+1, b+n+1);
    m = unique(b+1, b+n+1) - (b+1);
    int root = 1;
    dfs1(root);
    dfs2(root, root);
    int ans = 0;
    int u, v, k;
    for (int i = 1; i <= q; i++) {
        scanf("%d%d%d", &u, &v, &k);
        u = u^ans;
        int lca = get_lca(u, v);
        ans = b[query(1, m, tr[u], tr[v], tr[lca], tr[fa[lca]], k)];
        printf("%d\n", ans);
    }
    return 0;
}