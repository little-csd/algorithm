#include <stdio.h>
#include <stdlib.h>
using namespace std;
int main() {
    freopen("data.in", "w", stdout);
    int n = 500;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++)
            printf("%d ", rand() % 10);
        printf("\n");
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++)
            printf("%d ", rand() % 10);
        printf("\n");
    }
    return 0;
}