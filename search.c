#include "search.h"

// 見たかどうかリスト 3進数で盤面を表す
static uint32_t checkList[4782969] = {}; // 3**14

// となり合うマス
static const int neighbor[] = {
  0x0002, // N: [M]
  0x0005, // M: [LN]
  0x080a, // L: [CKM]
  0x0014, // K: [JL]
  0x0028, // J: [IK]
  0x0050, // I: [HJ]
  0x0020, // H: [I]
  0x0100, // G: [F]
  0x0280, // F: [EG]
  0x0500, // E: [DF]
  0x0a00, // D: [CE]
  0x1404, // C: [BDL]
  0x2800, // B: [AC]
  0x1000  // A: [B]
};

static int search_eachcolor(pzl* item, int* count, int ID, int color, int backward, int N)
{
  pzl result;
  int target, occupied, movable;
  int checkID, pow3;
  int t_num = 0, m_num = 0;

  // 今から見る盤面
  target = item[ID].placement[color];

  // あきマス
  occupied = ~(item[ID].placement[0] | item[ID].placement[1]);

  // targetのコマを反復
  for(int t_count = 0; t_count < SQUARE_N; ++t_count) {
    movable = occupied & neighbor[t_count]; // targetが動ける場所
    if(((target >> t_count) & 1) && movable) {
      if(N < (t_num++)) break;
      // targetが動ける場所を反復
      for(int m_count = 0; m_count < SQUARE_N; ++m_count) {
        if((movable >> m_count) & 1) {
          if(N < (m_num++)) break;
          // 動かしたあと
          result.placement[color] = (target & ~(1 << t_count)) | (1 << m_count);
          result.placement[!color] = item[ID].placement[!color];
          result.parent = ID;

          checkID = 0;
          pow3 = 1;
          for(int kk = 0; kk < SQUARE_N; ++kk) {
            checkID += (((result.placement[0] >> kk) & 1) * 2 + ((result.placement[1] >> kk) & 1)) * pow3;
            pow3 *= 3;
          }

          // 見たことあれば追加しない
          if(checkList[checkID]) {
            if((checkList[checkID] >> 31) == backward) continue;
            else {
              item[(*count)++] = result;
              return checkList[checkID] & 0x7fffffff;
            }
          }

          // リストに追加
          item[(*count)++] = result;
          checkList[checkID] = (*count) | (backward << 31);

          if(*count >= S_LIMIT) {
            printf("Reached the search limit!\n");
            exit(1);
          }
        }
      }
    }
  }
  return 0;
}


// 探索
int search(pzl* item, int* count, int* process_head, int backward, int* N)
{
  int new_count = *count, ret;

  // キューを反復
  for(int i = *process_head; i < *count; i++) {
    // each color
    for(int j = 0; j < 2; j++){
      ret = search_eachcolor(item, &new_count, i, j, backward, N[j]);
      if(ret) {
        *count = new_count;
        return ret;
      }
    }
  }
  // 動きがなかったらエラー終了
  if(*count == new_count) {
      printf("Can't move!\n");
      exit(1);
  }
  *process_head = *count;
  *count = new_count;
  return 0;
}
