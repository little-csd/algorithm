#include <stdio.h>
#include <queue>
#include <math.h>
using namespace std;
#define swap(a,b); tt=a;a=b;b=tt;
#define N 500011
#define B 19
struct link_table {
    int to, next;
} e[2*N];
int cnt = 0, link[N];
int fa[N][B], dep[N];
queue<int> q;
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

void print(int x) {
    if (x > 9) print(x/10);
    putchar(x%10+'0');
}

void build(int rt) {
    q.push(rt);
    while (!q.empty()) {
        int d = q.front();
        q.pop();
        int edge = link[d];
        while (edge) {
            int to = e[edge].to;
            if (!dep[to]) {
                dep[to] = dep[d] + 1;
                fa[to][0] = d;
                q.push(to);
            }
            edge = e[edge].next;
        }
    }
}

void inline add(int x, int y) {
    e[++cnt].to = y;
    e[cnt].next = link[x];
    link[x] = cnt;

    e[++cnt].to = x;
    e[cnt].next = link[y];
    link[y] = cnt;
}

void init(int n, int bit) {
    for (int j = 1; j <= bit; j++)
    for (int i = 1; i <= n; i++) {
        fa[i][j] = fa[fa[i][j-1]][j-1];
    }
}

int lca(int x, int y, int bmax) {
    int dx = dep[x], dy = dep[y];
    if (dx < dy) {
        int tt;
        swap(dx,dy);
        swap(x,y);
    }
    // printf("%d %d\n", x, y);
    if (dx != dy) {
        int dd = dx - dy;
        int bit = 0;
        while (dd) {
            if (dd&1) x = fa[x][bit];
            bit++;
            dd >>= 1;
        }
    }
    if (x == y) return x;
    for (int i = bmax; i >= 0; i--) {
        if (fa[x][i] != fa[y][i]) {
            x = fa[x][i];
            y = fa[y][i];
        }
    }
    return fa[x][0];
}

int main() {
    int n, m, rt, x, y;
    scanf("%d%d%d", &n, &m, &rt);

    int bit = 0, t = 2;
    while (t < n) {
        bit++;
        t <<= 1;
    }
    int cnt = 0;
    for (int i = 1; i < n; i++) {
        x = read(), y = read();
        add(x,y);
    }
    fa[rt][0] = rt;
    dep[rt] = 1;
    build(rt);
    init(n, bit);
    for (int i = 1; i <= m; i++) {
        x = read(), y = read();
        print(lca(x, y, bit));
        putchar('\n');
    }
    return 0;
}