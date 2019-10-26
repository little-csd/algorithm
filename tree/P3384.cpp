#include <stdio.h>
#define N 100111
#define mid(l,r) ((l+r)>>1)
#define lson(d) (d<<1)
#define rson(d) (d<<1|1)
using namespace std;
int mod, a[N];
int edge_num, link[N];
struct edge {
    int to, next;
} e[N<<1];
int fa[N], size[N], son[N], dep[N];
int tot, top[N], tid[N], rnk[N];
struct seg_tree {
    int l, r;
    int sum, tag;
} node[N<<2];

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

void pushdown(int rt, int l, int r) {
    int t = node[rt].tag;
    node[rt].tag = 0;

    int lson = lson(rt), rson = rson(rt), mid = mid(l,r);
    node[lson].tag = (node[lson].tag + t) % mod;
    node[lson].sum = (node[lson].sum + (long long)(mid - l + 1) * t) % mod;
    node[rson].tag = (node[rson].tag + t) % mod;
    node[rson].sum = (node[rson].sum + (long long)(r - mid) * t) % mod;
}

void build(int rt, int l, int r) {
    node[rt].l = l;
    node[rt].r = r;
    if (l == r) {
        node[rt].sum = a[rnk[l]];
        return;
    }
    int mid = mid(l,r);
    build(lson(rt), l, mid);
    build(rson(rt), mid+1, r);
    node[rt].sum = (node[lson(rt)].sum + node[rson(rt)].sum) % mod;
}

void update(int rt, int l, int r, int L, int R, int v) {
    if (L <= l && r <= R) {
        node[rt].sum = (node[rt].sum + (long long)(r - l + 1) * v) % mod;
        node[rt].tag = (node[rt].tag + v) % mod;
        return;
    }
    if (node[rt].tag) pushdown(rt, l, r);
    int mid = mid(l,r);
    if (L <= mid) update(lson(rt), l, mid, L, R, v);
    if (R > mid) update(rson(rt), mid+1, r, L, R, v);
    node[rt].sum = ((long long) node[lson(rt)].sum + node[rson(rt)].sum) % mod;
}

int query(int rt, int l, int r, int L, int R) {
    if (L <= l && r <= R) {
        return node[rt].sum;
    }
    if (node[rt].tag) pushdown(rt, l, r);
    int mid = mid(l,r);
    if (R <= mid) return query(lson(rt), l, mid, L, R);
    else if (L > mid) return query(rson(rt), mid+1, r, L, R);
    else return ((long long) query(lson(rt), l, mid, L, R) + query(rson(rt), mid+1, r, L, R)) % mod;
}

void update_path(int u, int v, int value) {
    int tu = top[u], tv = top[v];
    while (tu != tv) {
        if (dep[tu] > dep[tv]) {
            update(1, 1, tot, tid[tu], tid[u], value);
            u = fa[tu];
            tu = top[u];
        } else {
            update(1, 1, tot, tid[tv], tid[v], value);
            v = fa[tv];
            tv = top[v];
        }
    }
    if (dep[u] > dep[v]) {
        int t = u;
        u = v;
        v = t;
    }
    update(1, 1, tot, tid[u], tid[v], value);
}

void update_tree(int rt, int value) {
    int siz = size[rt];
    // printf("%d %d\n", rt, tid[rt]);
    update(1, 1, tot, tid[rt], tid[rt] + siz - 1, value);
}

long long query_path(int u, int v) {
    int tu = top[u], tv = top[v];
    long long ans = 0;
    while (tu != tv) {
        if (dep[tu] > dep[tv]) {
            // printf("%d %d\n", tid[tu], tid[u]);
            ans = (ans + query(1, 1, tot, tid[tu], tid[u])) % mod;
            u = fa[tu];
            tu = top[u];
        } else {
            // printf("%d %d\n", tid[tv], tid[v]);
            ans = (ans + query(1, 1, tot, tid[tv], tid[v])) % mod;
            v = fa[tv];
            tv = top[v];
        }
    }
    if (dep[u] > dep[v]) {
        int t = u;
        u = v;
        v = t;
    }
    ans = (ans + query(1, 1, tot, tid[u], tid[v])) % mod;
    return ans;
}

long long query_tree(int rt) {
    int siz = size[rt];
    return query(1, 1, tot, tid[rt], tid[rt] + siz - 1);
}

int main() {
    int n, m, root;
    n = read(), m = read(), root = read(), mod = read();
    for (int i = 1; i <= n; i++) 
        a[i] = read();
    int l, r, x;
    for (int i = 1; i < n; i++) {
        l = read(), r = read();
        add(l, r);
    }
    dep[root] = 1;
    dfs1(root);
    dfs2(root, root);

    // for (int i = 1; i <= n; i++) printf("%d %d %d %d %d %d %d\n", i, dep[i], son[i], size[i], fa[i], top[i], tid[i]);
    build(1, 1, tot);
    int type, v;
    for (int i = 1; i <= m; i++) {
        type = read();
        switch(type) {
            case 1:
                l = read(), r= read(), v = read();
                update_path(l, r, v);
                break;
            case 2:
                l = read(), r = read();
                printf("%lld\n", query_path(l, r));
                break;
            case 3:
                x = read(), v = read();
                update_tree(x, v);
                break;
            default:
                x = read();
                printf("%lld\n", query_tree(x));
        }
        // printf("%lld\n", query_path(6, 1));
    }
    return 0;
}