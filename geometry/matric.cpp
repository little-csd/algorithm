#include <stdio.h>
#include <time.h>
#define N 511
int A[N][N], B[N][N], C[N][N];

int main() {
    int n = 500;
    freopen("data.in", "r", stdin);
    for (int i = 1; i <= n; i++)
    for (int j = 1; j <= n; j++)
        scanf("%d", &A[i][j]);
    for (int i = 1; i <= n; i++)
    for (int j = 1; j <= n; j++)
        scanf("%d", &B[i][j]);
    clock_t start, finish;
    start = clock();
    // jik
    for (int i = 1; i <= n; i++) {
        for (int k = 1; k <= n; k++) {
            for (int j = 1; j <= n; j++) {
                C[i][k] += A[i][j] * B[j][k];
            }
        }
    }
    /*
    for (int i = 1; i <= n; i++) {
        for (int k = 1; k <= n; k++) {
            int sum = 0;
            for (int j = 1; j <= n; j++) {
                C[i][k] += A[i][j] * B[j][k];
            }
            C[i][k] = sum;
        }
    }*/
    finish = clock();
    printf("%lf\n", (double)(finish - start) / CLOCKS_PER_SEC);
    return 0;
}