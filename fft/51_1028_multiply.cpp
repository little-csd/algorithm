#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <cstring>
#define MAXN 266666
using namespace std;
const double PI = acos(-1.0);
struct Complex
{
    double r, c;
    Complex(double r = 0.0, double c = 0.0) : r(r), c(c) {}
    Complex operator - (const Complex &b) const {
        return Complex(r - b.r, c - b.c);
    }
    Complex operator + (const Complex &b) const {
        return Complex(r + b.r, c + b.c);
    }
    Complex operator * (const Complex &b) const {
        return Complex(r * b.r - c * b.c, r * b.c + c * b.r);
    }
} x1[MAXN], x2[MAXN];
void read(char* str, Complex* x, int len)
{
    for (int i = 0; i < len; i++)
    {
        x[i] = Complex(str[len-i-1] - '0', 0);
    }
}
int sum[MAXN];
void print(Complex* x, int len)
{
    for (int i = 0; i < len; i++)
    {
        sum[i] += (int)(x[i].r + 0.5);
        if (sum[i] >= 10) {
            sum[i+1] += sum[i] / 10;
            sum[i] %= 10;
        }
    }
    while (sum[len] == 0 && len > 0) len--;
    while (len >= 0)
    {
        putchar(sum[len] + '0');
        len--;
    }
}
void change(Complex* x, int len)
{
    int j = len>>1;
    for (int i = 1; i < len-1; i++) {
        if (i < j) swap(x[i], x[j]);
        int k = len >> 1;
        while (j >= k) {
            j -= k;
            k >>= 1;
        }
        // j < k 可以不加上吗？ 可以
        if (j < k) j += k;
    }
}
void fft(Complex* x, int len, int inv)
{
    change(x, len);
    for (int h = 2; h <= len; h<<=1) {
        Complex wn(cos(-inv * 2 * PI / h), sin(-inv * 2 * PI / h));
        for (int j = 0; j < len; j += h) {
            Complex w(1, 0);
            for (int k = j; k < j + h / 2; k++) {
                Complex u = x[k];
                Complex v = w * x[k + h / 2];
                x[k] = u + v;
                x[k + h / 2] = u - v;
                w = w * wn;
            }
        }
    }
    if (inv == -1) {
        for (int i = 0; i < len; i++)
            x[i].r /= len;
    }
}
char str[MAXN/2];
int main()
{
    int len1, len2, len = 1;
    scanf("%s", str);
    len1 = strlen(str);
    read(str, x1, len1);
    scanf("%s", str);
    len2 = strlen(str);
    read(str, x2, len2);
    len1<<=1;
    len2<<=1;
    while (len < len1 || len < len2) len<<=1;
    fft(x1, len, 1);
    fft(x2, len, 1);
    for (int i = 0; i < len; i++)
        x1[i] = x1[i] * x2[i];
    fft(x1, len, -1);

    print(x1, len);
    return 0;
}