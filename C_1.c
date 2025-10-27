#include <stdio.h>

#define BASE 10000
#define MAXDIG 200

typedef struct {
    int sign;
    int len;
    int d[MAXDIG];
} BigInt;

void bi_set_int(BigInt *a, long long x)
{
    if (x == 0) {
        a->sign = 0;
        a->len = 0;
        return;
    }
    a->sign = (x < 0) ? -1 : 1;
    if (x < 0) x = -x;
    a->len = 0;
    while (x > 0) {
        a->d[a->len++] = (int)(x % BASE);
        x /= BASE;
    }
}

void bi_mul(BigInt *a, const BigInt *b)
{
    if (a->sign == 0 || b->sign == 0) {
        a->sign = 0;
        a->len = 0;
        return;
    }

    BigInt res;
    res.sign = a->sign * b->sign;
    for (int i = 0; i < MAXDIG; i++) res.d[i] = 0;

    for (int i = 0; i < a->len; i++) {
        long long carry = 0;
        for (int j = 0; j < b->len || carry > 0; j++) {
            long long cur = res.d[i + j]
                + (long long)a->d[i] * (j < b->len ? b->d[j] : 0)
                + carry;
            res.d[i + j] = (int)(cur % BASE);
            carry = cur / BASE;
        }
    }

    int L = MAXDIG - 1;
    while (L > 0 && res.d[L] == 0) L--;
    res.len = L + 1;
    if (res.len == 1 && res.d[0] == 0) {
        res.sign = 0;
        res.len = 0;
    }
    *a = res;
}

void bi_print(const BigInt *a)
{
    if (a->sign == 0) {
        printf("0");
        return;
    }
    if (a->sign < 0) {
        putchar('-');
    }
    int i = a->len - 1;
    printf("%d", a->d[i]);
    for (i = i - 1; i >= 0; i--) {
        printf("%04d", a->d[i]);
    }
}

int main(void)
{
    long long nums[10];
    long long sum = 0;
    BigInt prod;
    BigInt tmp;

    printf("Input 10 natural numbers:");
    for (int i = 0; i < 10; i++) {
        if (scanf("%lld", &nums[i]) != 1) {
            return 1;
        }
    }

    for (int i = 0; i < 10; i++) {
        sum += nums[i];
    }

    bi_set_int(&prod, 1);
    for (int i = 0; i < 10; i++) {
        bi_set_int(&tmp, nums[i]);
        bi_mul(&prod, &tmp);
    }

    printf("\nsum=%lld, prod=", sum);
    bi_print(&prod);
    printf("\n");
    return 0;
}
