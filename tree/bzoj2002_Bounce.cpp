#include <stdio.h>
#include <algorithm>
using namespace std;
#define N 200111
#define ls(x) ch[x][0]
#define rs(x) ch[x][1]
#define get(x) (x == ch[fa[x]][1])
#define nRoot(x) (x == ch[fa[x]][1] || x == ch[fa[x]][0])
int v[N], sz[N];
int fa[N], ch[N][2];
bool rev[N];
int read() {
    int x = 0;
    char c = getchar();
    while (c < '0' || c > '9') c = getchar();
    while (c >= '0' && c <= '9') x = x * 10 + c - '0', c = getchar();
    return x;
}
void up(int x) {
    sz[x] = sz[ls(x)] + sz[rs(x)] + 1;
}
void down(int x) {
    rev[x] = 0;
    int l = ls(x), r = rs(x);
    if (l) {
        swap(ls(l), rs(l));
        rev[l] ^= 1;
    }
    if (r) {
        swap(ls(r), rs(r));
        rev[r] ^= 1;
    }
}
void rotate(int x) {
    int y = fa[x], z = fa[y], k = get(x);
    if (nRoot(y)) ch[z][y == ch[z][1]] = x;
    ch[y][k] = ch[x][k^1];
    fa[ch[x][k^1]] = y;
    ch[x][k^1] = y;
    fa[y] = x;
    fa[x] = z;
    up(y);up(x);
}
void update(int x) {
    if (nRoot(x)) update(fa[x]);
    if (rev[x]) down(x);
}
void splay(int x) {
    update(x);
    int f = fa[x];
    while (nRoot(x)) {
        if (nRoot(f)) rotate(get(x) == get(f) ? f : x);
        rotate(x);
        f = fa[x];
    }
}
void access(int x) {
    for (int p = 0; x; p = x, x = fa[x]) {
        splay(x);
        ch[x][1] = p;
        up(x);
    }
}
void mk_root(int x) {
    access(x);
    splay(x);
    rev[x] ^= 1;
    swap(ls(x), rs(x));
}
void lnk(int x, int y) {
    mk_root(x);
    fa[x] = y;
}
void cut(int x, int y) {
    mk_root(x);
    access(y);
    splay(y);
    fa[x] = ls(y) = 0;
    up(y);
}
int main() {
    int q, n = read();
    for (int i = 1; i <= n; i++) {
        v[i] = read();
        sz[i] = 1;
    }
    sz[n+1] = 1;
    for (int i = 1; i <= n; i++)
        if (v[i] + i <= n) lnk(i, v[i] + i);
        else lnk(i, n+1);
    q = read();
    int opt, a, b;
    for (int i = 1; i <= q; i++) {
        opt = read();
        a = read() + 1;
        if (opt == 1) {
            mk_root(a);
            access(n+1);
            splay(n+1);
            printf("%d\n", sz[n+1] - 1);
        } else {
            b = read();
            if (a + v[a] > n) cut(a, n+1);
            else cut(a, a + v[a]);
            v[a] = b;
            if (a + v[a] > n) lnk(a, n+1);
            else lnk(a, a + v[a]);
        }
    }
    return 0;
}