#include <stdio.h>
#include <algorithm>
#define N 40000
#define INF 1000000000
using namespace std;
int rt = 0, tot = 0;
int ch[N][2], val[N], sz[N], cnt[N], fa[N];
inline void up(int x) {
    sz[x] = sz[ch[x][0]] + sz[ch[x][1]] + cnt[x];
}
inline bool get(int x) {
    return x == ch[fa[x]][1];
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
void splay(int x) {
    int f = fa[x];
    while (f) {
        if (fa[f]) rotate(get(x) == get(f) ? f : x);
        rotate(x);
        f = fa[x];
    }
    rt = x;
}
void insert(int x) {
    int cur = rt, f = 0;
    while (cur) {
        f = cur;
        if (x < val[cur]) cur = ch[cur][0];
        else if (x > val[cur]) cur = ch[cur][1];
        else {
            cnt[cur]++;
            sz[cur]++;
            break;
        }
    }
    if (!cur) {
        cur = ++tot;
        sz[cur] = cnt[cur] = 1;
        val[cur] = x;
        fa[cur] = f;
        ch[f][x > val[f]] = cur;
        if (f) sz[f]++;
    }
    splay(cur);
}
int pre(int x) {
    int ans = -INF, cur = rt;
    while (cur) {
        if (x < val[cur]) cur = ch[cur][0];
        else if (x > val[cur]) {
            ans = val[cur];
            cur = ch[cur][1];
        } else return x;
    }
    return ans;
}
int nxt(int x) {
    int ans = INF, cur = rt;
    while (cur) {
        if (x > val[cur]) cur = ch[cur][1];
        else if (x < val[cur]){
            ans = val[cur];
            cur = ch[cur][0];
        } else return x;
    }
    return ans;
}
void print(int cur) {
    if (!cur) return;
    print(ch[cur][0]);
    printf("%d ");
    print(ch[cur][1]);
}
int main() {
    int n, x, ans;
    scanf("%d", &n);
    scanf("%d", &ans);
    insert(ans);
    for (int i = 2; i <= n; i++) {
        scanf("%d", &x);
        int s1 = pre(x), s2 = nxt(x);
        insert(x);
        // printf("%d %d %d\n", x, s1, s2);
        // printf("%d %d\n", s1, s2);
        ans += min(x - s1, s2 - x);
    }
    printf("%d\n", ans);
    return 0;
}