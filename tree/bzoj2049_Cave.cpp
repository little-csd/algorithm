#include <stdio.h>
#include <algorithm>
#define ls(x) ch[x][0]
#define rs(x) ch[x][1]
#define N 10111
#define get(x) (ch[fa[x]][1] == x)
#define nRoot(x) (ch[fa[x]][0] == x || ch[fa[x]][1] == x)
using namespace std;
int ch[N][2], fa[N], rev[N];
int read() {
    int x = 0;
    char c = getchar();
    while (c < '0' || c > '9') c = getchar();
    while (c >= '0' && c <= '9') x = x * 10 + c - '0', c = getchar();
    return x;
}
void down(int x) {
    rev[x] = 0;
    swap(ls(x), rs(x));
    int l = ls(x), r = rs(x);
    if (l) rev[l] ^= 1;
    if (r) rev[r] ^= 1;
}
void rotate(int x) {
    int y = fa[x], z = fa[y], k = get(x);
    if (nRoot(y)) ch[z][y == ch[z][1]] = x;
    ch[y][k] = ch[x][k^1];
    fa[ch[x][k^1]] = y;
    ch[x][k^1] = y;
    fa[y] = x;
    fa[x] = z;
}
int st[N];
void splay(int x) {
    int y = x, tp = 0;
    st[++tp] = y;
    while (nRoot(y)) {
        y = fa[y];
        st[++tp] = y;
    }
    while (tp) {
        if (rev[st[tp]]) down(st[tp]);
        tp--;
    }
    int f = fa[x];
    while (nRoot(x)) {
        if (nRoot(f)) rotate(get(x) == get(f) ? f : x);
        rotate(x);
        f = fa[x];
    }
}
void access(int x) {
    int p = 0;
    while (x) {
        splay(x);
        rs(x) = p;
        p = x;
        x = fa[x];
    }
}
void mk_root(int x) {
    access(x);
    splay(x);
    rev[x] ^= 1;
}
void cut(int x, int y) {
    mk_root(x);
    access(y);
    splay(y);
    fa[x] = ls(y) = 0;
}
void conn(int x, int y) {
    mk_root(x);
    fa[x] = y;
}
bool query(int x, int y) {
    mk_root(x);
    access(y);
    splay(y);
    while (ls(y)) {
        if (rev[y]) down(y);
        y = ls(y);
    }
    return x == y;
}
int main() {
    int n = read(), m = read();
    char s[15];
    int x, y;
    for (int i = 1; i <= m; i++) {
        scanf("%s", s);
        x = read(), y = read();
        if (s[0] == 'Q') {
            if (query(x, y)) puts("Yes");
            else puts("No");
        }
        else if (s[0] == 'C') conn(x, y);
        else cut(x, y);
    }
    return 0;
}