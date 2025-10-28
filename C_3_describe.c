#include <stdio.h>

#define W 12  /* 横幅(列数) */
#define H  8  /* 高さ(行数) */

/*
 * 指定したマス (x, y) を起点に、
 * そこが白(=1)なら、その白い領域を全部 黒(=0) に塗りつぶす。
 *
 * やりかた（フラッドフィル）：
 *   1. 最初に (x, y) をスタックに入れる
 *   2. スタックから1つ取り出す → そこがまだ白(1)なら黒(0)に変える
 *   3. その上下左右をスタックに追加
 *   4. スタックが空になるまで2〜3を繰り返す
 *
 * 再帰で書くこともできるんだけど、今回は手動スタック配列でやる。
 */
void flood_fill_white_to_black(int board[H][W], int start_x, int start_y)
{
    /* スタックとして使う配列。
     * 最大で盤面全体(W*H)を積むことがあるので、そのくらい確保する。
     */
    int stack[W * H][2];
    int sp = 0; /* stack pointer: 今の積み上げ数 */

    /* まず、開始点が盤面内かチェック */
    if (start_x < 0 || start_x >= W || start_y < 0 || start_y >= H) {
        return; /* 盤面外なら何もしない */
    }

    /* 開始点が白(=1)じゃなければ、塗りつぶす必要なし */
    if (board[start_y][start_x] != 1) {
        return;
    }

    /* ここから塗りつぶし開始。
     * 最初のマスをスタックに入れる。
     */
    stack[sp][0] = start_x;
    stack[sp][1] = start_y;
    sp++;

    while (sp > 0) {
        /* スタックの最後の要素を取り出す（LIFO） */
        sp--;
        int x = stack[sp][0];
        int y = stack[sp][1];

        /* 盤面外ならスキップ */
        if (x < 0 || x >= W || y < 0 || y >= H) {
            continue;
        }

        /* すでに黒(0)ならやることなし。
         * 白(1)じゃないならスキップ
         */
        if (board[y][x] != 1) {
            continue;
        }

        /* このマスを黒(0)に塗る */
        board[y][x] = 0;

        /* 隣り合う4マス(上下左右)をスタックに追加する。
         * (まだ白なら後で黒にする)
         *
         * スタックがあふれないか最低限の確認をしつつ詰める。
         */
        if (sp < W*H) { stack[sp][0] = x + 1; stack[sp][1] = y;     sp++; }
        if (sp < W*H) { stack[sp][0] = x - 1; stack[sp][1] = y;     sp++; }
        if (sp < W*H) { stack[sp][0] = x;     stack[sp][1] = y + 1; sp++; }
        if (sp < W*H) { stack[sp][0] = x;     stack[sp][1] = y - 1; sp++; }
    }
}

int main(void)
{
    int board[H][W];
    int x, y;

    printf("Input 12*8 board:\n");

    /*
     * 8行×12列の盤面の色を読み取る。
     * 0 が黒、1 が白。
     * board[行][列] の形で格納するので
     *   行ループ(iがy方向)、列ループ(jがx方向)
     * で読み込む。
     */
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (scanf("%d", &board[i][j]) != 1) {
                return 1; /* 読み取り失敗したら終了 */
            }
            /* 念のため、0か1以外の値が来たら0に直しておく */
            if (board[i][j] != 0 && board[i][j] != 1) {
                board[i][j] = 0;
            }
        }
    }

    /* 塗り始める位置 (x, y) を読み取る */
    printf("Input x:");
    if (scanf("%d", &x) != 1) {
        return 1;
    }
    printf("Input y:");
    if (scanf("%d", &y) != 1) {
        return 1;
    }

    /*
     * flood_fill_white_to_black を呼び出すことで、
     * もし (x,y) が白(1)なら、その連結している白い領域を
     * ぜんぶ黒(0)に塗ってくれる。
     */
    flood_fill_white_to_black(board, x, y);

    /*
     * 結果を表示する。
     * 課題の出力例は行ごとに値のあとスペースが入っていたので、
     * printf("%d ", ...) の形にしている。
     */
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }

    return 0;
}
