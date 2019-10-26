#include <stdio.h>
#include <algorithm>
#define N 100111
#define H 19
#define mid(l,r) ((l+r)>>1)
using namespace std;
int a[N];
int part[H][N], cnt[H][N];
int getLen(int x) {
    int ans = 0;
    while (x) {
        ans++;
        x>>=1;
    }
    return ans;
}

void build(int dp, int l, int r) {
    if (l == r) return;
    int mid = mid(l,r), v = a[mid], ndp = dp + 1, scnt = (mid - l + 1);
    for (int i = l; i <= r; i++) if (part[dp][i] < v) scnt--;
    int ptl = l, ptr = mid + 1;
    for (int i = l; i <= r; i++) {
        if (part[dp][i] < v) {
            part[ndp][ptl++] = part[dp][i];
            cnt[dp][i] = cnt[dp][i-1] + 1;
        } else if (part[dp][i] == v) {
            if (scnt) {
                scnt--;
                part[ndp][ptl++] = part[dp][i];
                cnt[dp][i] = cnt[dp][i-1] + 1;
            } else {
                part[ndp][ptr++] = part[dp][i];
                cnt[dp][i] = cnt[dp][i-1];
            }
        } else {
            part[ndp][ptr++] = part[dp][i];
            cnt[dp][i] = cnt[dp][i-1];
        }
    }
    build(dp+1, l, mid);
    build(dp+1, mid+1, r);
}

int query(int dth, int l, int r, int L, int R, int k) {
    if (L == R) return part[dth][L];
    int mid = mid(l,r);
    int toleft1 = cnt[dth][L-1] - cnt[dth][l-1], toright1 = L - l - toleft1;
    int toleft2 = cnt[dth][R] - cnt[dth][L-1], toright2 = R - L + 1 - toleft2;
    if (k <= toleft2) return query(dth+1, l, mid, l + toleft1, l + toleft1 + toleft2 - 1, k);
    else return query(dth+1, mid+1, r, mid + 1 + toright1, mid + toright1 + toright2 , k - toleft2);
}

int main() {
    int n, q;
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &part[0][i]);
        a[i] = part[0][i];
    }
    sort(a+1, a+n+1);
    build(0, 1, n);
/*
    int dth = getLen(n - 1);
    for (int i = 0; i <= dth; i++) {
        printf("Depth %d: \n", i);
        for (int p = 1; p <= n; p++)
        printf("%d ", part[i][p]);
        printf("\n");
    }
    for (int i = 0; i <= dth; i++) {
        printf("Depth %d: \n", i);
        for (int p = 1; p <= n; p++)
        printf("%d ", cnt[i][p]);
        printf("\n");
    }
    return 0;
*/
    int l, r, k;
    for (int i = 1; i <= q; i++) {
        scanf("%d%d%d", &l, &r, &k);
        printf("%d\n", query(0, 1, n, l, r, k));
    }
    return 0;
}