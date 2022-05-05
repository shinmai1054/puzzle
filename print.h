#ifndef PRINT_H
#define PRINT_H

#include <stdio.h>

#include "board.h"

// 関数リスト
extern void printpzl(pzl puzzle);
extern void print_move(pzl* itemF, pzl* itemB, int countF, int countB);

#endif
