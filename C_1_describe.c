#include <stdio.h>

/*
 * 普通の int / long long では入りきらない巨大な積も扱えるようにする。
 *
 * 表現ルール：
 *   - BASE = 10000 (1要素が0〜9999の「1桁」だと思って扱う)
 *   - d[0] が一番下の桁 (= BASE^0 の位)
 *   - d[1] が次の桁 (= BASE^1 の位)
 *   - ... というふうに、下位から上位へ並べる
 *
 *   例: 12345678
 *   BASE=10000 なので、
 *   12345678 = 5678 * BASE^0 + 1234 * BASE^1
 *   → d[0] = 5678, d[1] = 1234, len=2
 *
 * sign:
 *   1 なら正、-1 なら負、0 なら「0そのもの」
 */

#define BASE    10000
#define MAXDIG  200   /* 桁(=配列長)の上限。適当に大きめに確保 */

typedef struct {
    int sign;        /* 1 / -1 / 0 */
    int len;         /* 有効な桁数 (d[0]..d[len-1]が有効) */
    int d[MAXDIG];   /* 各桁。d[0]が最下位桁 */
} BigInt;

/* 整数 x (long long の範囲) から BigInt を作る */
void bi_set_int(BigInt *a, long long x)
{
    if (x == 0) {
        /* 0 の場合は特別扱い */
        a->sign = 0;
        a->len = 0;
        return;
    }

    /* 符号を記録して、xは正にそろえる */
    if (x < 0) {
        a->sign = -1;
        x = -x;
    } else {
        a->sign = 1;
    }

    /* x を BASE(10000)で割りながら、桁ごとに入れる */
    a->len = 0;
    while (x > 0) {
        a->d[a->len] = (int)(x % BASE); /* 下位4桁を格納 */
        x /= BASE;                      /* xを4桁ぶん右シフトしたイメージ */
        a->len++;
    }
}

/*
 * a = a * b (aを上書き)
 *
 * やり方は、手計算の「筆算の掛け算」と同じ。
 * 各桁どうしをかけて、対応する位置に足しこんでいく。
 */
void bi_mul(BigInt *a, const BigInt *b)
{
    /* どっちかが0なら結果も0 */
    if (a->sign == 0 || b->sign == 0) {
        a->sign = 0;
        a->len = 0;
        return;
    }

    BigInt res;
    res.sign = a->sign * b->sign; /* 符号は掛け算ルール (例: (+)*(−)= (−) ) */

    /* 結果用バッファを0クリア */
    for (int i = 0; i < MAXDIG; i++) {
        res.d[i] = 0;
    }

    /* i桁目とj桁目を掛けると、(i+j)桁目に足し込まれるイメージ */
    for (int i = 0; i < a->len; i++) {
        long long carry = 0; /* 繰り上がり */
        for (int j = 0; j < b->len || carry > 0; j++) {

            long long cur = res.d[i + j]
                + (long long)a->d[i] * (j < b->len ? b->d[j] : 0)
                + carry;

            res.d[i + j] = (int)(cur % BASE); /* 下位4桁を入れる */
            carry = cur / BASE;               /* 繰り上がり部分 */
        }
    }

    /* 結果の桁数(res.len)を決める。末尾の余分な0を省く */
    int L = MAXDIG - 1;
    while (L > 0 && res.d[L] == 0) {
        L--;
    }
    res.len = L + 1;

    /* もし結果が0になってたら sign=0 にそろえる */
    if (res.len == 1 && res.d[0] == 0) {
        res.sign = 0;
        res.len = 0;
    }

    *a = res; /* a にコピーして終了 */
}

/*
 * BigInt を10進数として表示する。
 * ポイント：BASE=10000 なので、いちばん上の桁だけ普通にprintf("%d")
 * それより下の桁は4桁ゼロ埋め("%04d")で表示する。
 */
void bi_print(const BigInt *a)
{
    if (a->sign == 0) {
        printf("0");
        return;
    }

    if (a->sign < 0) {
        putchar('-');
    }

    /* いちばん上の桁(最高位)だけは普通に表示してOK */
    int i = a->len - 1;
    printf("%d", a->d[i]);

    /* 残りの下位桁は必ず4桁で埋めて表示する */
    for (i = i - 1; i >= 0; i--) {
        printf("%04d", a->d[i]);
    }
}

int main(void)
{
    long long nums[10];
    long long sum = 0; /* 普通のlong longで10個分の合計は余裕で入る */
    BigInt prod;       /* 積(多倍長) */
    BigInt tmp;        /* 一時的に各入力の値を格納する用 */

    printf("Input 10 natural numbers:");

    /* 10個まとめて読む */
    for (int i = 0; i < 10; i++) {
        if (scanf("%lld", &nums[i]) != 1) {
            return 1; /* 読めなかったらエラー終了 */
        }
    }

    /* 合計はふつうにたし算 */
    for (int i = 0; i < 10; i++) {
        sum += nums[i];
    }

    /* 積を求める。最初は1から始めて、順番にかけていく */
    bi_set_int(&prod, 1); /* prod = 1 */
    for (int i = 0; i < 10; i++) {
        bi_set_int(&tmp, nums[i]); /* tmp = nums[i] をBigInt化 */
        bi_mul(&prod, &tmp);       /* prod = prod * tmp */
    }

    printf("\nsum=%lld, prod=", sum);
    bi_print(&prod); /* 多倍長整数を表示 */
    printf("\n");

    return 0;
}
