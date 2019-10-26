#include <stdio.h>
#define N 30111
#define mid(l,r) ((l+r)>>1)
#define lson(d) (d<<1)
#define rson(d) (d<<1|1)
#define max(a,b) ((a)>(b)?(a):(b))
using namespace std;
int mod, a[N];
int edge_num = 0, link[N];
struct edge {
    int to, next;
} e[N<<1];
int fa[N], size[N], son[N], dep[N];
int tot = 0, top[N], tid[N], rnk[N];
struct seg_tree {
    int l, r;
    int sum, max;
} node[N<<2];

void add(int l, int r) {
    int pre_edge = link[l];
    e[++edge_num].to = r;
    e[edge_num].next = pre_edge;
    link[l] = edge_num;

    pre_edge = link[r];
    e[++edge_num].to = l;
    e[edge_num].next = pre_edge;
    link[r] = edge_num;
}

void dfs1(int rt) {
    son[rt] = 0;
    size[rt] = 1;
    for (int d = link[rt]; d; d = e[d].next) {
        int to = e[d].to;
        if (!dep[to]) {
            dep[to] = dep[rt] + 1;
            fa[to] = rt;
            dfs1(to);
            size[rt] += size[to];
            if (size[to] > size[son[rt]]) son[rt] = to;
        }
    }
}

void dfs2(int rt, int tp) {
    top[rt] = tp;
    tid[rt] = ++tot;
    rnk[tot] = rt;
    if (!son[rt]) return;
    dfs2(son[rt], tp);
    for (int d = link[rt]; d; d = e[d].next)
    if (e[d].to != son[rt] && e[d].to != fa[rt]) {
        dfs2(e[d].to, e[d].to);
    }
}

inline void maintain(int rt) {
    node[rt].sum = node[lson(rt)].sum + node[rson(rt)].sum;
    node[rt].max = max(node[lson(rt)].max, node[rson(rt)].max);
}

void build(int rt, int l, int r) {
    node[rt].l = l;
    node[rt].r = r;
    if (l == r) {
        node[rt].sum = node[rt].max = a[rnk[l]];
        return;
    }
    int mid = mid(l,r);
    build(lson(rt), l, mid);
    build(rson(rt), mid+1, r);
    maintain(rt);
}

void update(int rt, int l, int r, int pos, int v) {
    if (l == r) {
        node[rt].sum = node[rt].max = v;
        return;
    }
    int mid = mid(l,r);
    if (pos <= mid) update(lson(rt), l, mid, pos, v);
    else update(rson(rt), mid+1, r, pos, v);
    maintain(rt);
}

int query_max(int rt, int l, int r, int L, int R) {
    if (L <= l && r <= R) {
        return node[rt].max;
    }
    int mid = mid(l,r);
    if (R <= mid) return query_max(lson(rt), l, mid, L, R);
    else if (L > mid) return query_max(rson(rt), mid+1, r, L, R);
    else return max(query_max(lson(rt), l, mid, L, R), query_max(rson(rt), mid+1, r, L, R));
}

int query_sum(int rt, int l, int r, int L, int R) {
    if (L <= l && r <= R) {
        return node[rt].sum;
    }
    int mid = mid(l,r), ans = 0;
    if (L <= mid) ans += query_sum(lson(rt), l, mid, L, R);
    if (R > mid) ans += query_sum(rson(rt), mid+1, r, L, R);
    return ans;
}

int pre_max(int rt, int u, int v) {
    int tu = top[u], tv = top[v];
    int ans = -2147483647;
    while (tu != tv) {
        if (dep[tu] > dep[tv]) {
            ans = max(ans, query_max(rt, 1, tot, tid[tu], tid[u]));
            u = fa[tu];
            tu = top[u];
        } else {
            ans = max(ans, query_max(rt, 1, tot, tid[tv], tid[v]));
            v = fa[tv];
            tv = top[v];
        }
    }
    if (dep[u] > dep[v]) {
        int t = u;
        u = v;
        v = t;
    }
    return max(ans, query_max(rt, 1, tot, tid[u], tid[v]));
}

int pre_sum(int rt, int u, int v) {
    int tu = top[u], tv = top[v];
    int ans = 0;
    while (tu != tv) {
        if (dep[tu] > dep[tv]) {
            ans += query_sum(rt, 1, tot, tid[tu], tid[u]);
            u = fa[tu];
            tu = top[u];
        } else {
            ans += query_sum(rt, 1, tot, tid[tv], tid[v]);
            v = fa[tv];
            tv = top[v];
        }
    }
    if (dep[u] > dep[v]) {
        int t = u;
        u = v;
        v = t;
    }
    return ans + query_sum(rt, 1, tot, tid[u], tid[v]);
}

int main() {
    int n;
    int l, r;
    scanf("%d", &n);
    for (int i = 1; i < n; i++) {
        scanf("%d%d", &l, &r);
        add(l, r);
    }

    for (int i = 1; i <= n; i++) 
        scanf("%d", &a[i]);
    
    int root = 1;
    dep[root] = 1;
    dfs1(root);
    dfs2(root, root);

    // for (int i = 1; i <= n; i++)
        // printf("%d %d %d %d\n", i, top[i], tid[i], rnk[i]);

    build(1, 1, tot);
    int a, b, q;
    char type[10];
    scanf("%d", &q);
    for (int i = 1; i <= q; i++) {
        scanf("%s%d%d", type, &a, &b);
        if (type[0] == 'C') {
            update(root, 1, tot, tid[a], b);
        } else {
            if (type[1] == 'M') {
                printf("%d\n", pre_max(root, a, b));
            } else {
                printf("%d\n", pre_sum(root, a, b));
            }
        }
    }
    return 0;
}