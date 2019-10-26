#include <stdio.h>
#include <algorithm>
#define N 511111
#define INF 0x3f3f3f3f
using namespace std;
int sta[N], tot;
int rt, a[N];
int fa[N], size[N], ch[N][2], val[N];
int set[N], rev[N], sum[N], lsum[N], rsum[N], msum[N];
inline bool get(int x) {
    return x == ch[fa[x]][1];
}
void up(int t) {
    int l = ch[t][0], r = ch[t][1], v = val[t];
    size[t] = size[l] + size[r] + 1;
    sum[t] = sum[l] + sum[r] + v;
    lsum[t] = max(lsum[l], max(sum[l] + v, sum[l] + v + lsum[r]));
    rsum[t] = max(rsum[r], max(sum[r] + v, sum[r] + v + rsum[l]));
    msum[t] = max(max(msum[l], msum[r]), max(rsum[l], 0) + v + max(lsum[r], 0));
}
inline void down(int t) {
    if (set[t] != INF) {
        int l = ch[t][0], r = ch[t][1], v = set[t];
        if (l) {
            val[l] = set[l] = v;
            sum[l] = size[l] * v;
            lsum[l] = rsum[l] = msum[l] = max(sum[l], v);
        }
        if (r) {
            val[r] = set[r] = v;
            sum[r] = size[r] * v;
            lsum[r] = rsum[r] = msum[r] = max(sum[r], v);
        }
        set[t] = INF;
    }
    if (rev[t]) {
        int l = ch[t][0], r = ch[t][1];
        rev[t] = 0;
        if (l) {
            rev[l] = !rev[l];
            swap(lsum[l], rsum[l]);
            swap(ch[l][0], ch[l][1]);
        }
        if (r) {
            rev[r] = !rev[r];
            swap(lsum[r], rsum[r]);
            swap(ch[r][0], ch[r][1]);
        }
    }
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
        if (fa[f] != k) rotate(get(x) == get(f) ? f : x);
        rotate(x);
        f = fa[x];
    }
    if (!k) rt = x;
}
void build(int& t, int l, int r) {
    t = sta[tot--];
    int mid = (l + r) >> 1;
    val[t] = a[mid];
    set[t] = INF;
    if (mid > l) {
        build(ch[t][0], l, mid-1);
        fa[ch[t][0]] = t;
    }
    if (r > mid) {
        build(ch[t][1], mid+1, r);
        fa[ch[t][1]] = t;
    }
    up(t);
}
void clear(int t) {
    if (!t) return;
    clear(ch[t][0]);
    clear(ch[t][1]);
    fa[t] = size[t] = ch[t][0] = ch[t][1] = val[t] = 0;
    rev[t] = sum[t] = lsum[t] = rsum[t] = msum[t] = 0;
    set[t] = INF;
    sta[++tot] = t;
}
int find(int x) {
    int t = rt;
    while (1) {
        down(t);
        int l = ch[t][0];
        if (x <= size[l]) t = l;
        else if (x == size[l] + 1) return t;
        else {
            x -= size[l] + 1;
            t = ch[t][1];
        }
    }
}
void insert(int pos, int n) {
    int s;
    build(s, 1, n);
    int s1 = find(pos);
    splay(s1, 0);
    int s2 = find(pos+1);
    splay(s2, rt);
    ch[s2][0] = s;
    fa[s] = s2;
    up(s2);up(rt);
}
void remove(int pos, int n) {
    int s1 = find(pos - 1);
    splay(s1, 0);
    int s2 = find(pos + n);
    splay(s2, rt);
    clear(ch[s2][0]);
    ch[s2][0] = 0;
    up(s2);up(rt);
}
void make_same(int pos, int n, int v) {
    int s1 = find(pos - 1);
    splay(s1, 0);
    int s2 = find(pos + n);
    splay(s2, rt);
    int t = ch[s2][0];
    val[t] = set[t] = v;
    sum[t] = size[t] * v;
    lsum[t] = rsum[t] = msum[t] = max(sum[t], v);
    up(s2);up(rt);
}
void reverse(int pos, int n) {
    int s1 = find(pos - 1);
    splay(s1, 0);
    int s2 = find(pos + n);
    splay(s2, rt);
    int t = ch[s2][0];
    rev[t] = !rev[t];
    swap(lsum[t], rsum[t]);
    swap(ch[t][0], ch[t][1]);
    up(s2);up(rt);
}
int get_sum(int pos, int n) {
    int s1 = find(pos - 1);
    splay(s1, 0);
    int s2 = find(pos + n);
    splay(s2, rt);
    return sum[ch[s2][0]];
}
inline int max_sum() {
    return msum[rt];
}
void init() {
    for (tot = 1; tot < N; tot++) sta[tot] = tot;
    tot--;
    lsum[0] = rsum[0] = msum[0] = -INF;
    size[0] = val[0] = sum[0] = 0;
    a[1] = a[2] = -INF;
    build(rt, 1, 2);
}
void print(int t) {
    if (!t) return;
    print(ch[t][0]);
    int x = val[t] == INF ? 0 : val[t];
    printf("%10d%10d%10d%10d\n", t, ch[t][0], ch[t][1], x);
    print(ch[t][1]);
}
int main() {
    init();
    int n, q;
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    insert(1, n);
    char s[10];
    int pos, num, c;
    for (int i = 1; i <= q; i++) {
        scanf("%s", s);
        if (s[0] == 'I') {
            scanf("%d%d", &pos, &num);
            for (int j = 1; j <= num; j++) scanf("%d", &a[j]);
            insert(pos+1, num);
        } else if (s[0] == 'D') {
            scanf("%d%d", &pos, &num);
            remove(pos+1, num);
        } else if (s[0] == 'R') {
            scanf("%d%d", &pos, &num);
            reverse(pos+1, num);
        } else if (s[0] == 'G') {
            scanf("%d%d", &pos, &num);
            printf("%d\n", get_sum(pos+1, num));
        } else {
            if (s[2] == 'K') {
                scanf("%d%d%d", &pos, &num, &c);
                make_same(pos+1, num, c);
            } else printf("%d\n", max_sum());
        }
        // print(rt);
    }
    return 0;
}