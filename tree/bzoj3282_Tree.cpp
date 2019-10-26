#include <stdio.h>
#include <algorithm>
using namespace std;
#define ls(x) ch[x][0]
#define rs(x) ch[x][1]
#define N 300111
#define get(x) (ch[fa[x]][1] == x)
#define nRoot(x) (ch[fa[x]][1] == x || ch[fa[x]][0] == x)
int v[N], s[N], rev[N], fa[N], ch[N][2];
int read() {
    int x = 0;
    char c = getchar();
    while (c < '0' || c > '9') c = getchar();
    while (c >= '0' && c <= '9') x = x * 10 + c - '0', c = getchar();
    return x;
}
inline void up(int x) {
    s[x] = s[ls(x)] ^ s[rs(x)] ^ v[x];
}
inline void down(int x) {
    int l = ls(x), r = rs(x);
    rev[x] = 0;
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
        if (nRoot(f)) rotate(get(f) == get(x) ? f : x);
        rotate(x);
        f = fa[x];
    }
}
void access(int x) {
    int p = 0;
    while (x) {
        splay(x);
        ch[x][1] = p;
        up(x);
        p = x;
        x = fa[x];
    }
}
void mk_root(int x) {
    access(x);
    splay(x);
    swap(ls(x), rs(x));
    rev[x] ^= 1;
}
int find_root(int x) {
    access(x);
    splay(x);
    while (ls(x)) {
        if (rev[x]) down(x);
        x = ls(x);
    }
    splay(x);
    return x;
}
inline void link(int x, int y) {
    mk_root(x);
    if (find_root(y) != x) fa[x] = y;
}
inline void cut(int x, int y) {
    mk_root(x);
    access(y);
    splay(y);
    if (find_root(y) != x || fa[y] != x || ch[y][0]) return;
    fa[y] = ch[x][1] = 0;
    up(x);
}
inline void split(int x, int y) {
    mk_root(x);
    access(y);
    splay(y);
}
int main() {
    int n = read(), m = read();
    for (int i = 1; i <= n; i++) v[i] = read();
    int opt, x, y;
    for (int i = 1; i <= m; i++) {
        opt = read(), x = read(), y = read();
        if (!opt) {
            split(x, y);
            printf("%d\n", s[y]);
        } else if (opt == 1) link(x, y);
        else if (opt == 2) cut(x, y);
        else {
            splay(x);
            v[x] = y;
        } 
    }
    return 0;
}