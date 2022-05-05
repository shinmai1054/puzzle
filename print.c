#include "print.h"

// 石の場所を表示
static void printplacement(int placement)
{
  for(int i = SQUARE_N - 1; i >= 0; i--) {
      if((placement >> i) & 1){
        printf("%c", -(i - 'N'));
      }
    }
    return;
}

// 盤面を表示
void printpzl(pzl puzzle)
{
  printf("white: ");
  printplacement(puzzle.placement[0]);
  printf(", black: ");
  printplacement(puzzle.placement[1]);
  printf("\n");
  return;
}

int traceback(pzl* item, int count, pzl* moveList)
{
  int moveCount = 0;
  int id = count - 1;

  while(id != -1) {
    moveList[moveCount++] = item[id];
    id = item[id].parent;
  }
  return moveCount - 1;
}

void printdiff(pzl target, pzl previous, int i, int last)
{
  int diff, diff_a, diff_b;

  printpzl(target);

  if(i != last) {
    if(target.placement[1] == previous.placement[1]) {
      // 白が変わった
      diff = target.placement[0] ^ previous.placement[0];
      diff_a = target.placement[0] & diff;
      diff_b = previous.placement[0] & diff;
      printf("   White: ");
    }
    else {
      // 黒が変わった
      diff = target.placement[1] ^ previous.placement[1];
      diff_a = target.placement[1] & diff;
      diff_b = previous.placement[1] & diff;
      printf("   Black: ");
    }
    printplacement(diff_a);
    printplacement(diff_b);
    printf("\n");
  }
}

// 盤面をトレースバック
void print_move(pzl* itemF, pzl* itemB, int countF, int countB)
{
  static pzl moveListF[S_LIMIT], moveListB[S_LIMIT];
  int moveCountF = 0, moveCountB = 0;

  moveCountF = traceback(itemF, countF, moveListF);
  moveCountB = traceback(itemB, countB, moveListB);

  for(int i = moveCountF; i > 0; i--) {
    printdiff(moveListF[i], moveListF[i-1], i, 0);
  }
  printdiff(moveListF[0], moveListB[1], 1, 0);

  for(int i = 1; i <= moveCountB; i++) {
    printdiff(moveListB[i], moveListB[i + 1], i, moveCountB);
  }
  printf("%d moves\n", moveCountF + moveCountB);
}
