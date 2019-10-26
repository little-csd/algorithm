#include <stdio.h>
#include <algorithm>
using namespace std;
#define N 30111
#define MAXN (N<<5)
#define mid(l,r) ((l+r)>>1)
int rt[N], lson[MAXN], rson[MAXN], sum[MAXN];
int a[N], b[N], tot = 0;
void build(int& x, int l, int r) {
    x = ++tot;

}

int main() {
    int n, q;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        b[i] = a[i];
    }
    sort(b+1,b+n+1);
    int m = unique(b+1, b+n+1) - (b+1);
    build(rt[0], 1, m);
    scanf("%d", &q);
    int l, r;
    while (q--) {
        scanf("%d%d", &l, &r);

    }
    return 0;
}