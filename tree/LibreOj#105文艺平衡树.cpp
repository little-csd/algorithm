#include <stdio.h>
#include <algorithm>
#define N 100111
using namespace std;
int rt = 0, tot = 0;
int lazy[N], ch[N][2], val[N], sz[N], fa[N];
int read() {
    int x = 0;
    char c = getchar();
    while (c < '0' || c > '9') c = getchar();
    while (c >= '0' || c <= '9') x = x * 10 + c - '0', c = getchar();
    return x;
}
inline void up(int x) {
    sz[x] = sz[ch[x][0]] + sz[ch[x][1]] + 1;
}
inline void down(int x) {
    lazy[x] = 0;
    swap(ch[x][0], ch[x][1]);
    lazy[ch[x][0]] = !lazy[ch[x][0]];
    lazy[ch[x][1]] = !lazy[ch[x][1]];
}
inline bool get(int x) {
    return x == ch[fa[x]][1];
}
void build(int& t, int l, int r, int f) {
    t = ++tot;
    int mid = (l+r) >> 1;
    fa[t] = f;
    val[t] = mid;
    if (mid > l) build(ch[t][0], l, mid-1, t);
    if (r > mid) build(ch[t][1], mid+1, r, t);
    up(t);
}
void rotate(int x) {
    int y = fa[x], z = fa[y], k = get(x);
    ch[y][k] = ch[x][k^1];
    fa[ch[x][k^1]] = y;
    ch[x][k^1] = y;
    fa[y] = x;
    fa[x] = z;
    if (z) ch[z][y == ch[z][1]] = x;
    up(y);up(x);
}
void splay(int x, int k) {
    int f = fa[x];
    while (f != k) {
        if (fa[f] != k) rotate(get(f) == get(x) ? f : x);
        rotate(x);
        f = fa[x];
    }
    if (!k) rt = x;
}
int find(int x) {
    int cur = rt;
    while (1) {
        if (lazy[cur]) down(cur);
        if (x <= sz[ch[cur][0]]) cur = ch[cur][0];
        else if (x == sz[ch[cur][0]] + 1) break;
        else {
            x -= sz[ch[cur][0]] + 1;
            cur = ch[cur][1];
        }
    }
    return cur;
}
void print(int x) {
    if (!x) return;
    if (lazy[x]) down(x);
    print(ch[x][0]);
    printf("%d ", val[x]);
    print(ch[x][1]);
}
int main() {
    int n, q;
    scanf("%d%d", &n, &q);
    build(rt, 1, n, 0);
    // print(rt);
    int l, r, a, b;
    for (int i = 1; i <= q; i++) {
        scanf("%d%d", &l, &r);
        if (l == 1 && r == n) lazy[rt] = !lazy[rt];
        else if (l == 1) {
            b = find(r+1);
            splay(b, 0);
            lazy[ch[rt][0]] = !lazy[ch[rt][0]];
        } else if (r == n) {
            a = find(l-1);
            splay(a, 0);
            lazy[ch[rt][1]] = !lazy[ch[rt][1]];
        } else {
            a = find(l-1);
            b = find(r+1);
            splay(a, 0);
            splay(b, a);
            int x = ch[ch[rt][1]][0];
            lazy[x] = !lazy[x];
        }
    }
    print(rt);
    return 0;
}