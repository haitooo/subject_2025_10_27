#include <stdio.h>

#define W 12
#define H 8

void fill(int board[H][W], int x, int y)
{
    int target;
    int stack[H*W][2];
    int sp = 0;

    if (x < 0 || x >= W || y < 0 || y >= H)
        return ;

    target = board[y][x];
    if (target != 1)
        return ;
    stack[sp][0] = x;
    stack[sp][1] = y;
    sp++;
    while (sp > 0)
    {
        sp--;
        int cx = stack[sp][0];
        int cy = stack[sp][1];

        if (cx < 0 || cx >= W || cy < 0 || cy >= H)
            continue ;
        if (board[cy][cx] != 1)
            continue ;
        board[cy][cx] = 0;

        if (sp < H*W)
        {
            stack[sp][0] = cx+1;
            stack[sp][1] = cy;
            sp++;
        }
        if (sp < H*W)
        {
            stack[sp][0] = cx-1;
            stack[sp][1] = cy;
            sp++;
        }
        if (sp < H*W)
        {
            stack[sp][0] = cx;
            stack[sp][1] = cy+1;
            sp++;
        }
        if (sp < H*W)
        {
            stack[sp][0] = cx;
            stack[sp][1] = cy-1;
            sp++;
        }
    }
}

int main(void)
{
    int board[H][W];
    int x, y;

    printf("Input 12*8 board:\n");
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            if (scanf("%d", &board[i][j]) != 1)
                return (1);
            if (board[i][j] != 0 && board[i][j] != 1)
                board[i][j] = 0;
        }
    }

    printf("Input x:");
    if (scanf("%d", &x) != 1)
        return (1);
    printf("Input y:");
    if (scanf("%d", &y) != 1)
        return (1);
    fill(board, x, y);
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
            printf("%d ", board[i][j]);
        printf("\n");
    }
    return (0);
}
