#include <stdio.h>

int main(void)
{
    int N;

    while (1) {
        printf("Input N:");
        if (scanf("%d", &N) != 1) {
            return 1;
        }
        if (N >= 1 && N <= 30) {
            break;
        }
    }

    double r = N / 2.0;
    double cx = r;
    double cy = r;

    int width = 2 * N;

    for (int y = 0; y < N; y++) {
        for (int x = 0; x < width; x++) {
            double px = x * 0.5 + 0.25;
            double py = y * 1.0 + 0.5;
            double dx = px - cx;
            double dy = py - cy;
            double dist2 = dx * dx + dy * dy;
            if (dist2 <= r * r) {
                putchar('#');
            } else {
                putchar(' ');
            }
        }
        putchar('\n');
    }

    return 0;
}
