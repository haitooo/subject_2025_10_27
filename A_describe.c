#include <stdio.h>

/*
 * 2つの整数 a と b の最大公約数 (Greatest Common Divisor = GCD)
 * を計算する関数です。
 *
 * アルゴリズム→「ユークリッドの互除法」
 * やってること：
 *   gcd(a, b) = gcd(b, a % b)
 * を、b が 0 になるまで繰り返すと、a に答えが残る。
 */
int gcd(int a, int b)
{
    int t;

    /* 念のため、マイナスが来たときには正にしておく 
    この二行は無くても動く*/
    if (a < 0) a = -a;
    if (b < 0) b = -b;

    /* b が 0 になるまで繰り返す */
    while (b != 0) {
        t = a % b;  /* a を b で割った余りをとる */
        a = b;      /* a <- b */
        b = t;      /* b <- 余り */
    }

    /* b が 0 になった時点で a が最大公約数 */
    return a;
}

int main(void)
{
    int x;
    int current_gcd = 0; /* これまでに読んだ数のGCD。まだ何も読んでない間は0 */
    int first = 1;       /* 「最初の数かどうか」のフラグ (1なら最初) */

    printf("Input natural numbers:");

    /*
     * scanf("%d", &x) は、キーボードから整数を1個読み取って x に入れる。
     * 戻り値が1なら整数が読めた、0やEOFなら読めなかった。
     */
    while (scanf("%d", &x) == 1) {
        if (x <= 0) {
            /* 0以下が入力されたので、もう入力を終わりにする */
            break;
        }

        if (first) {
            /*
             * 最初の正の整数は、そのまま current_gcd に入れる。
             * たとえば最初に 252 を読んだら、current_gcd = 252。
             */
            current_gcd = x;
            first = 0;
        } else {
            /*
             * 2個目以降の正の整数が来たら、
             * これまでの gcd と新しい数 x の gcd を取り直す。
             * 例: 今までのGCD=252、新しい数=1001 → gcd(252,1001)=7
             */
            current_gcd = gcd(current_gcd, x);
        }
    }

    printf("\ngcd = %d\n", current_gcd);

    return 0;
}
