#ifndef BOARD_H
#define BOARD_H

#define S_LIMIT 210210 // 14C4 * 10C4
#define SQUARE_N 14 // A to N

// パズルの盤面
typedef struct {
  int parent;
  unsigned short int placement[2];  // 0が白、1が黒
} pzl;


#endif
