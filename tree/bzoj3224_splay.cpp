#include <stdio.h>
#define N 100111
int rt, tot;
int fa[N], sz[N], ch[N][2], val[N], cnt[N];
// 更新操作
inline void maintain(int x) {
    sz[x] = sz[ch[x][0]] + sz[ch[x][1]] + cnt[x];
}
// 左儿子返回0，右儿子返回1
inline bool get(int x) {
    return x == ch[fa[x]][1];
}
inline void clear(int x) {
    fa[x] = sz[x] = ch[x][0] = ch[x][1] = val[x] = cnt[x] = 0;
}
void rotate(int x) {
    int y = fa[x], z = fa[y], k = get(x);
    ch[y][k] = ch[x][k^1];
    fa[ch[x][k^1]] = y;
    ch[x][k^1] = y;
    fa[y] = x;
    fa[x] = z;
    if (z) ch[z][y == ch[z][1]] = x;
    maintain(y);
    maintain(x);
}
void splay(int x) {
    int f = fa[x];
    while (f) {
        if (fa[f]) rotate(get(f) == get(x) ? f : x);
        rotate(x);
        f = fa[x];
    }
    rt = x;
}
void insert(int v) {
    if (!rt) {
        rt = ++tot;
        sz[rt] = cnt[rt] = 1;
        val[rt] = v;
        return;
    }
    int f = 0, cur = rt;
    while (1) {
        if (val[cur] == v) {
            cnt[cur]++;
            sz[cur]++;
            if (f) sz[f]++;
            splay(cur);
            break;
        }
        f = cur;
        cur = v < val[cur] ? ch[cur][0] : ch[cur][1];
        if (!cur) {
            cur = ++tot;
            val[tot] = v;
            cnt[tot] = sz[tot] = 1;
            ch[f][v > val[f]] = cur;
            fa[cur] = f;
            if (f) sz[f]++;
            splay(cur);
            break;
        }
    }
}
int rnk(int v) {
    int ans = 0, cur = rt;
    while (1) {
        if (v < val[cur]) cur = ch[cur][0];
        else if (v > val[cur]) {
            ans += sz[ch[cur][0]] + cnt[cur];
            cur = ch[cur][1];
        } else {
            ans += sz[ch[cur][0]] + 1;
            splay(cur);
            return ans;
        }
    }
}
int kth(int x) {
    int cur = rt;
    while (1) {
        int szl = sz[ch[cur][0]];
        if (x <= szl) cur = ch[cur][0];
        else if (x > szl + cnt[cur]) {
            x -= szl + cnt[cur];
            cur = ch[cur][1];
        } else return val[cur];
    }
}
int pre() {
    int cur = ch[rt][0];
    while (ch[cur][1]) cur = ch[cur][1];
    return cur;
}
int nxt() {
    int cur = ch[rt][1];
    while (ch[cur][0]) cur = ch[cur][0];
    return cur;
}
void remove(int x) {
    rnk(x);
    if (cnt[rt] > 1) {
        cnt[rt]--;
        sz[rt]--;
        return;
    }
    if (!ch[rt][0] && !ch[rt][1]) {
        clear(rt);
        rt = 0;
    } else if (!ch[rt][0]) {
        int cur = rt;
        rt = ch[rt][1];
        fa[rt] = 0;
        clear(cur);
    } else if (!ch[rt][1]) {
        int cur = rt;
        rt = ch[rt][0];
        fa[rt] = 0;
        clear(cur);
    } else {
        int p = pre(), cur = rt;
        splay(p);
        fa[ch[cur][1]] = p;
        ch[p][1] = ch[cur][1];
        clear(cur);
        maintain(rt);
    }
}
void print(int cur) {
    if (!cur) return;
    print(ch[cur][0]);
    printf("%d %d\n", cur, val[cur]);
    print(ch[cur][1]);
}
int main() {
    int n;
    int type, x;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d%d", &type, &x);
        // print(rt);
        switch (type)
        {
        case 1:
            insert(x);
            break;
        case 2:
            remove(x);
            break;
        case 3:
            printf("%d\n", rnk(x));
            break;
        case 4:
            printf("%d\n", kth(x));
            break;
        case 5:
            insert(x);
            printf("%d\n", val[pre()]);
            remove(x);
            break;
        default:
            insert(x);
            printf("%d\n", val[nxt()]);
            remove(x);
        }
    }
    return 0;
}