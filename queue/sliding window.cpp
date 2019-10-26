#include <stdio.h>
#define N 1001111
int a[N], q[N], id[N], l = 1, r = 0;
int read() {
    int ans = 0, f = 1;
    char c = getchar();
    while (c < '0' || c > '9') {
        if (c == '-') f = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        ans = ans * 10 + c - '0';
        c = getchar();
    }
    return ans * f;
}
int main() {
    int n, k;
    while (scanf("%d%d", &n, &k) != EOF) {
        l = 1, r = 0;
        for (int i = 1; i < k && i <= n; i++) {
            a[i] = read();
            while (l <= r && a[i] <= q[r]) r--;
            q[++r] = a[i];
            id[r] = i;
        }
        for (int i = k; i <= n; i++) {
            a[i] = read();
            while (l <= r && a[i] <= q[r]) r--;
            q[++r] = a[i];
            id[r] = i;
            if (i - id[l] >= k) l++;
            printf("%d ", q[l]);
        }
        printf("\n");
        l = 1, r = 0;
        for (int i = 1; i < k && i <= n; i++) {
            while (l <= r && a[i] >= q[r]) r--;
            q[++r] = a[i];
            id[r] = i;
        }
        for (int i = k; i <= n; i++) {
            while (l <= r && a[i] >= q[r]) r--;
            q[++r] = a[i];
            id[r] = i;
            if (i - id[l] >= k) l++;
            printf("%d ", q[l]);
        }
        printf("\n");
    }
    return 0;
}