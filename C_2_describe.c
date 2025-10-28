#include <stdio.h>
#include <math.h>

/*
 * (a) x^3 + 2x = b
 *    f1(x) = x^3 + 2x - b
 *    f1'(x) = 3x^2 + 2
 *
 * ニュートン法:
 *   x_{n+1} = x_n - f1(x_n)/f1'(x_n)
 *
 * これを何回も繰り返すと、解に近づいていく（ことが多い）
 */
double f1(double x, double b)
{
    return x*x*x + 2.0*x - b;
}

double df1(double x)
{
    return 3.0*x*x + 2.0;
}

/* b が与えられたときの x^3+2x=b の解を探す */
double solve_eq1_newton(double b)
{
    double x = 0.0; /* 初期値。とりあえず0から始める */
    for (int i = 0; i < 1000; i++) {
        double y  = f1(x, b);  /* 今の x での f1(x) */
        double dy = df1(x);    /* 導関数 f1'(x) */

        if (fabs(dy) < 1e-12) {
            /* dy が小さすぎると割り算できないのでやめる */
            break;
        }

        x = x - y / dy; /* ニュートン法の更新式 */
    }
    return x;
}

/*
 * (b) 2x^3 + 3x^2 - 12x = b
 *    f2(x) = 2x^3 + 3x^2 - 12x - b
 *    f2'(x) = 6x^2 + 6x - 12
 *
 * これもニュートン法で解ける。同じく
 *   x_{n+1} = x_n - f2(x_n)/f2'(x_n)
 *
 * ただし複数の解がある場合もあるので、スタート地点をいろいろ変えて
 * 見つかった解を全部リストに入れる。
 */

double f2(double x, double b)
{
    return 2.0*x*x*x + 3.0*x*x - 12.0*x - b;
}

double df2(double x)
{
    return 6.0*x*x + 6.0*x - 12.0;
}

/*
 * 1つの初期値 start からニュートン法を回してみて、
 * 「ちゃんと解っぽい」と判断できれば 1 を返す。
 * 見つけた解は *root_out に入れる。
 * うまくいかなければ 0 を返す。
 */
int newton_for_f2(double b, double start, double *root_out)
{
    double x = start;

    for (int i = 0; i < 200; i++) {
        double y  = f2(x, b);
        double dy = df2(x);

        if (fabs(dy) < 1e-9) {
            /* 導関数が小さすぎると動きにくいので諦め */
            break;
        }

        double next = x - y / dy;

        if (!isfinite(next)) {
            /* 無限大とかNaNになったら諦め */
            break;
        }

        x = next;
    }

    /* 最終的な x が「ちゃんと方程式を満たしてるか？」を確認
     * |f2(x)| < 0.01 なら、x は十分いい近似解とみなす
     */
    if (!isfinite(x)) {
        return 0;
    }
    if (fabs(f2(x, b)) < 0.01) {
        *root_out = x;
        return 1;
    }
    return 0;
}

int main(void)
{
    double b;

    printf("Input b:");
    if (scanf("%lf", &b) != 1) {
        return 1;
    }

    /* (a) の方程式 x^3+2x=b を解く */
    double ans1 = solve_eq1_newton(b);

    printf("Solution of x^3+2x=%f: %f\n", b, ans1);

    /*
     * (b) の方程式 2x^3+3x^2-12x=b を解く
     * 複数の解があるかもしれないので、いろんなスタート値から試す。
     * ここでは -10, -9, ..., 0, ..., 10 を初期値として試す。
     */

    double roots[50]; /* 見つかった解を入れる配列 */
    int root_count = 0;

    for (int s = -10; s <= 10; s++) {
        double r;
        if (newton_for_f2(b, (double)s, &r)) {
            /* 近い解がすでに入っていないかチェック
             * (同じ解を何度も追加しないようにする)
             */
            int already = 0;
            for (int k = 0; k < root_count; k++) {
                if (fabs(roots[k] - r) < 0.05) {
                    /* ほぼ同じ値なら同じ解とみなす */
                    already = 1;
                    break;
                }
            }

            if (!already) {
                roots[root_count++] = r;
            }
        }
    }

    /* 小さい順に並べ替え（単純なバブルソート） */
    for (int i = 0; i < root_count; i++) {
        for (int j = i + 1; j < root_count; j++) {
            if (roots[j] < roots[i]) {
                double tmp = roots[i];
                roots[i] = roots[j];
                roots[j] = tmp;
            }
        }
    }

    printf("Solution(s) of 2x^3+3x^2-12x=%f:", b);
    for (int i = 0; i < root_count; i++) {
        printf(" %f", roots[i]);
    }
    printf("\n");

    return 0;
}
