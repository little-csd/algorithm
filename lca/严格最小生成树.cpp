#include <stdio.h>
#include <algorithm>
using namespace std;
#define max(a,b) (a)>(b)?(a):(b)
#define swap(a,b); tt=a;a=b;b=tt;
#define N 100111
#define M 300111
#define B 17

struct edge {
    bool used;
    int x, y;
    int v;
    bool operator < (const edge o) {
        return v < o.v;
    }
} e[M];
struct lca {
    int f, m_fir, m_sec;
} fa[N][B];
int n, m, bit, cset[N], dep[N];

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

int getLog(int x) {
    int ans = 0, bit = 2;
    while (bit < x) {
        ans++;
        bit <<= 1;
    }
    return ans;
}

int find(int x) {
    if (cset[x] == x) return x;
    int fx = find(cset[x]);
    if (cset[x] == fx) return fx;
    dep[x] = dep[cset[x]] + dep[x];
    cset[x] = fx;
    return fx;
}

long long kruskal() {
    int used = 1;
    long long cost = 0;
    for (int i = 1; i <= n; i++) cset[i] = i;
    for (int i = 1; i <= m; i++) {
        int x = e[i].x, y = e[i].y;
        int fx = find(x), fy = find(y);
        if (fx == fy) continue;
        else {
            cset[fy] = fx;
            fa[fy][0].f = x;
            fa[fy][0].m_fir = e[i].v;
            fa[fy][0].m_sec = 0;
            dep[fy] = dep[x] + 1;
            used++;
            cost += e[i].v;
            e[i].used = true;
            if (used == n)
                return cost;
        }
    }
    return cost;
}

void lca_init() {
    int rt = find(1);
    fa[rt][0].f = rt;
    for (int i = 1; i <= n; i++) find(i);
    for (int j = 1; j <= bit; j++) 
    for (int i = 1; i <= n; i++) {
        lca lp = fa[i][j-1], rp = fa[lp.f][j-1];
        fa[i][j].f = rp.f;
        int lm1 = lp.m_fir, lm2 = lp.m_sec, rm1 = rp.m_fir, rm2 = rp.m_sec;
        if (lm1 > rm1) {
            fa[i][j].m_fir = lm1;
            fa[i][j].m_sec = max(lm2, rm1);
        } else if (lm1 < rm1) {
            fa[i][j].m_fir = rm1;
            fa[i][j].m_sec = max(rm2, lm1);
        } else {
            fa[i][j].m_fir = lm1;
            fa[i][j].m_sec = max(lm2, rm2);
        }
    }
}

void inline compare(int x, int bit, int& m_fir, int& m_sec) {
    lca cur = fa[x][bit];
    if (cur.m_fir > m_fir) {
        m_sec = max(m_fir, cur.m_sec);
        m_fir = cur.m_fir;
    } else if (cur.m_fir == m_fir) {
        if (cur.m_sec > m_sec) m_sec = cur.m_sec;
    } else {
        if (cur.m_fir > m_sec) m_sec = cur.m_fir;
    }
}

pair<int,int> lca_find(int x, int y) {
    int dx = dep[x], dy = dep[y], m_fir = 0, m_sec = 0;
    if (dx < dy) {
        int tt;
        swap(dx, dy);
        swap(x, y);
    }
    int dd = dx - dy, bit = 0;
    while (dd) {
        if (dd & 1) {
            compare(x, bit, m_fir, m_sec);
            x = fa[x][bit].f;
        }
        dd >>= 1;
        bit++;
    }
    if (x == y) return pair<int, int>(m_fir, m_sec);
    for (int i = bit; i >= 0; i--) {
        if (fa[x][i].f != fa[y][i].f) {
            compare(x, i, m_fir, m_sec);
            compare(y, i, m_fir, m_sec);
            x = fa[x][i].f;
            y = fa[y][i].f;
        }
    }
    compare(x, 0, m_fir, m_sec);
    compare(y, 0, m_fir, m_sec);
    return pair<int, int>(m_fir, m_sec);
}

long long compute() {
    int minn = 2147483647;
    for (int i = 1; i <= m; i++) {
        if (e[i].used) continue;
        int x = e[i].x, y = e[i].y;
        pair<int,int> maxn = lca_find(x, y);
        if (e[i].v == maxn.first) {
            if (e[i].v - maxn.second < minn)
                minn = e[i].v - maxn.second;
        } else {
            if (e[i].v - maxn.first < minn)
                minn = e[i].v - maxn.first;
        }
    }
    return minn;
}

void print() {
    for (int i = 1; i <= n; i++)
    for (int j = 0; j <= bit; j++)
    printf("%d's %d father is %d\n", i, 1<<j, fa[i][j].f);
}

int main() {
    n = read(), m = read();
    for (int i = 1; i <= m; i++) {
        e[i].x = read();
        e[i].y = read();
        e[i].v = read();
    }

    sort(e+1, e+m+1);
    bit = getLog(n);
    long long cost = kruskal();
    lca_init();
    // for (int i = 1; i <= n; i++) printf("%d's depth is %d \n", i, dep[i]);
    printf("%lld\n", cost + compute());
    return 0;
}
// 5 6
// 1 2 1
// 1 3 2
// 2 3 2
// 3 4 2
// 3 5 4
// 4 5 4