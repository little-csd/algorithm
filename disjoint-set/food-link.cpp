#include <stdio.h>
#define N 50111
int n, mod = 3;
int fa[N], w[N];
void init() {
    for (int i = 1; i <= n; i++) {
        fa[i] = i;
        w[i] = 0;
    }
}

int find(int x) {
    if (x == fa[x]) return x;
    int fx = find(fa[x]);
    w[x] = (w[x] + w[fa[x]]) % mod;
    return fa[x] = fx;
}

bool process(int type, int x, int y) {
    if (x > n || y > n) return 0;
    if (x == y) return type == 1;
    int fx = find(x), fy = find(y);
    if (fx == fy) {
        if (type == 1) return w[x] == w[y];
        else return (w[x] - w[y] + 3) % 3 == 1;
    } else {
        fa[fx] = fy;
        w[fx] = (-w[x] + type - 1 + w[y] + 3) % 3;
        return 1;
    }
}

int main() {
    int k, type, x, y, ans = 0;
    scanf("%d%d", &n, &k);
    init();
    for (int i = 0; i < k; i++) {
        scanf("%d%d%d", &type, &x, &y);
        // printf("%d\n", process(type, x, y));
        ans += !process(type, x, y);
    }
    printf("%d", ans);
    return 0;
}