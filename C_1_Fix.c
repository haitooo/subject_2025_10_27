#include <stdio.h>

#define BASE 10000
#define MAXDIG 200

int main(void)
{
    long long nums[10];
    long long sum = 0;

    int prod_sign, prod_len;
    int prod_d[MAXDIG];

    int tmp_sign, tmp_len;
    int tmp_d[MAXDIG];

    printf("Input 10 natural numbers:");
    for (int i = 0; i < 10; i++) {
        if (scanf("%lld", &nums[i]) != 1)
            return 1;
    }

    for (int i = 0; i < 10; i++) {
        sum += nums[i];
    }

    {
        long long x = 1;
        if (x == 0) {
            prod_sign = 0;
            prod_len = 0;
        } else {
            prod_sign = (x < 0) ? -1 : 1;
            if (x < 0) x = -x;
            prod_len = 0;
            while (x > 0) {
                prod_d[prod_len++] = (int)(x % BASE);
                x /= BASE;
            }
        }
    }

    for (int idx = 0; idx < 10; idx++) {
        long long x = nums[idx];
        if (x == 0) {
            tmp_sign = 0;
            tmp_len = 0;
        } else {
            tmp_sign = (x < 0) ? -1 : 1;
            if (x < 0) x = -x;
            tmp_len = 0;
            while (x > 0) {
                tmp_d[tmp_len++] = (int)(x % BASE);
                x /= BASE;
            }
        }

        if (prod_sign == 0 || tmp_sign == 0) {
            prod_sign = 0;
            prod_len = 0;
        } else {
            int res_d[MAXDIG];
            for (int i = 0; i < MAXDIG; i++)
                res_d[i] = 0;

            int res_sign = prod_sign * tmp_sign;

            for (int i = 0; i < prod_len; i++) {
                long long carry = 0;
                for (int j = 0; j < tmp_len || carry > 0; j++) {
                    long long cur =
                        (long long)res_d[i + j]
                        + (long long)prod_d[i] * (j < tmp_len ? tmp_d[j] : 0)
                        + carry;
                    res_d[i + j] = (int)(cur % BASE);
                    carry = cur / BASE;
                }
            }

            int L = MAXDIG - 1;
            while (L > 0 && res_d[L] == 0)
                L--;
            int res_len = L + 1;
            if (res_len == 1 && res_d[0] == 0) {
                res_sign = 0;
                res_len = 0;
            }

            prod_sign = res_sign;
            prod_len = res_len;
            for (int i = 0; i < MAXDIG; i++)
                prod_d[i] = res_d[i];
        }
    }

    printf("\nsum=%lld, prod=", sum);

    if (prod_sign == 0) {
        printf("0");
    } else {
        if (prod_sign < 0)
            putchar('-');
        int i = prod_len - 1;
        printf("%d", prod_d[i]);
        for (i = i - 1; i >= 0; i--)
            printf("%04d", prod_d[i]);
    }

    printf("\n");
    return 0;
}
