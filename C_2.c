#include <stdio.h>
#include <math.h>

double f1(double x, double b) { return x*x*x + 2.0*x - b; }
double df1(double x) { return 3.0*x*x + 2.0; }

double newton1(double b)
{
    double x = 0.0;
    for (int i = 0; i < 1000; i++) {
        double y = f1(x,b);
        double dy = df1(x);
        if (fabs(dy) < 1e-12) break;
        x = x - y/dy;
    }
    return x;
}

double f2(double x, double b)
{
    return 2.0*x*x*x + 3.0*x*x - 12.0*x - b;
}

double df2(double x)
{
    return 6.0*x*x + 6.0*x - 12.0;
}

int newton2(double b, double start, double *root_out)
{
    double x = start;
    for (int i = 0; i < 200; i++) {
        double y = f2(x,b);
        double dy = df2(x);
        if (fabs(dy) < 1e-9) break;
        double x_next = x - y/dy;
        if (!isfinite(x_next)) break;
        x = x_next;
    }
    if (!isfinite(x)) return 0;
    if (fabs(f2(x,b)) < 0.01) {
        *root_out = x;
        return 1;
    }
    return 0;
}

int main(void)
{
    double b;
    printf("Input b:");
    if (scanf("%lf", &b) != 1) return 1;

    double sol1 = newton1(b);

    printf("Solution of x^3+2x=%f: %f\n", b, sol1);

    double cands[50];
    int nc = 0;
    for (int s = -10; s <= 10; s++) {
        double r;
        if (newton2(b, (double)s, &r)) {
            int exists = 0;
            for (int k = 0; k < nc; k++) {
                if (fabs(cands[k] - r) < 0.05) {
                    exists = 1;
                    break;
                }
            }
            if (!exists) {
                cands[nc++] = r;
            }
        }
    }

    for (int i = 0; i < nc; i++) {
        for (int j = i+1; j < nc; j++) {
            if (cands[j] < cands[i]) {
                double tmp = cands[i];
                cands[i] = cands[j];
                cands[j] = tmp;
            }
        }
    }

    printf("Solution(s) of 2x^3+3x^2-12x=%f:", b);
    for (int i = 0; i < nc; i++) {
        printf(" %f", cands[i]);
    }
    printf("\n");

    return 0;
}
