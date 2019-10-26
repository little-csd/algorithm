#include <stdio.h>
#include <algorithm>
using namespace std;
#define N 300111
#define ls(x) ch[x][0]
#define rs(x) ch[x][1]
#define get(x) (x == ch[fa[x]][1])
#define isRoot(x) (ch[fa[x]][0] != x && ch[fa[x]][1] != x)
int ch[N][2], fa[N], sum[N], val[N], sz[N];
bool rev[N];
// 更新当前节点
void up(int rt) {
    sum[rt] = sum[ls(rt)] ^ sum[rs(rt)] ^ val[rt];
    sz[rt] = sz[ls(rt)] + sz[rs(rt)] + 1;
}
// 下传标记
void down(int rt) {
    rev[rt] = 0;
    int l  = ls(rt), r = rs(rt);
    if (l) {
        swap(ls(l), rs(l));
        rev[l] ^= 1;
    }
    if (r) {
        swap(ls(r), rs(r));
        rev[r] ^= 1;
    }
}
// 从上到下更新根节点到x路径上的点
void update(int x) {
    if (!isRoot(x)) update(fa[x]);
    if (rev[x]) down(x);
}
// 旋转操作
void rotate(int x) {
    int y = fa[x], z = fa[y], k = get(x);
    // 注意，此处应该写在前面
    if (!isRoot(y)) ch[z][ch[z][1] == y] = x;
    ch[y][k] = ch[x][k^1];
    fa[ch[x][k^1]] = y;
    ch[x][k^1] = y;
    fa[y] = x;
    fa[x] = z;
    up(y);up(x);
}
// splay操作，注意退出条件为x是当前splay树的根节点
void splay(int x) {
    update(x);
    int f = fa[x];
    while (!isRoot(x)) {
        if (!isRoot(f)) rotate(get(f) == get(x) ? f : x);
        rotate(x);
        f = fa[x];
    }
}
// 打通根节点到x的路径
void access(int x) {
    for (int p = 0; x; p = x, x = fa[x]) {
        splay(x);
        ch[x][1] = p;
        up(x);
    }
}
// 使x成为整个树的根
void mk_root(int x) {
    access(x);
    splay(x);
    swap(ls(x), rs(x));
    rev[x] ^= 1;
}
int find(int x) {
    access(x);
    splay(x);
    while (ls(x)) {
        if (rev[x]) down(x);
        x = ls(x);
    }
    splay(x);
    return x;
}
void link(int x, int y) {
    mk_root(x);
    if (find(y) != x) fa[x] = y;
}
void cut(int x, int y) {
    mk_root(x);
    if (find(y) != x || sz[x] > 2) return;
    fa[y] = rs(x) = 0;
    up(x);
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &val[i]);
        sum[i] = val[i];
        sz[i] = 1;
    }
    int opt, x, y;
    for (int i = 1; i <= m; i++) {
        scanf("%d%d%d", &opt, &x, &y);
        if (!opt) {
            mk_root(x);
            access(y);
            // 此处还是需要splay(y) access后x不一定是splay的根了
            splay(y);
            printf("%d\n", sum[y]);
        } else if (opt == 1) link(x, y);
        else if (opt == 2) cut(x, y);
        else if (opt == 3) {
            splay(x);
            val[x] = y;
        }
    }
    return 0;
}