#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "board.h"
#include "search.h"
#include "print.h"

// 入力された盤面をビット列に
// ABCD... -> 0b00AB CDEF GHIJ KLMN
unsigned short int char2int(char* placement_c, int* N)
{
  int ret = 0;
  for(int i=0; placement_c[i]!='\0'; i++) {
    ret += 1 << (-(placement_c[i] - 'N'));
    if(N != NULL) (*N)++;
  }
  return ret;
}


int main(int argc, char** argv)
{
  static pzl searchItemF[S_LIMIT]; // 順方向探索結果リスト
  static pzl searchItemB[S_LIMIT]; // 逆方向探索結果リスト
  int countF = 0, countB = 0;      // 探索ノード数
  int process_headF = 0, process_headB = 0; // キューの先頭
  int resultF = 0, resultB = 0;    // 結合点
  int N[2] = {}, N2[2] = {};              // 石の数
  struct timespec start_time, end_time;              // 時間計測用
  unsigned int sec;
  int nsec;
  double d_sec;

  // 引数チェック
  if(argc != 5) {
    printf("Illegal argc\n");
    return 1;
  }

  // 最初の盤面
  pzl root = {-1, {char2int(argv[1], &(N[0])), char2int(argv[2], &(N[1]))}};
  searchItemF[countF++] = root;

  // 最終盤面
  pzl finish = {-1, {char2int(argv[3], &(N2[0])), char2int(argv[4], &(N2[1]))}};
  searchItemB[countB++] = finish;

  if(N[0] != N2[0] || N[1] != N2[1]) {
    printf("Illegal coin count!\n");
    return 1;
  }
  if((root.placement[0] & root.placement[1]) || (finish.placement[0] & finish.placement[1])) {
    printf("Illegal coin placement!\n");
    return 2;
  }

  // 探索開始
  clock_gettime(CLOCK_REALTIME, &start_time);
  while(!resultF) {
    // 順方向探索 マッチするとB側のノード番号が帰ってくる
    resultB = search(searchItemF, &countF, &process_headF, 0, N);
    if(resultB) {
      resultF = countF;
      printf("F\n");
      break;
    }
    // 逆方向探索
    resultF = search(searchItemB, &countB, &process_headB, 1, N);
    if(resultF)
      resultB = countB;
  }
  clock_gettime(CLOCK_REALTIME, &end_time);

  // 結果発表〜
  print_move(searchItemF, searchItemB, resultF, resultB);
  sec = end_time.tv_sec - start_time.tv_sec;
  nsec = end_time.tv_nsec - start_time.tv_nsec;
  d_sec = (double)sec * 1000. + (double)nsec / 1000000.;
  printf("Executed in %.3f ms\n", d_sec);

  return 0;
}
