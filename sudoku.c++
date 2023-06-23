#include "getopt.h"
#include <iostream>
#include <vector>
#include <utility>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <map>
#include <io.h>

using namespace std;



typedef vector<vector<char> > Board;

const int N = 9;

class SudokuPlayer
{
private:
    // 使用位运算来表示某个数是否出现过（例：row_used_[0]=(二进制)111000表示第一行4/5/6出现过）
    int row_used_[N];
    int column_used_[N];
    int block_used_[N];

public:
    vector<Board> result_;
    vector<pair<int, int> > spaces_;

public:
    SudokuPlayer()
    {
        init_state();
    }

    void init_state()
    {
        memset(row_used_, 0, sizeof(row_used_));
        memset(column_used_, 0, sizeof(column_used_));
        memset(block_used_, 0, sizeof(block_used_));
        spaces_.clear();
        result_.clear();
    }

    void add_result(Board& board)
    {
        vector<vector<char> > obj(board);
        result_.push_back(obj);
    }

    //对i行、j列以及[i,j]所在的3*3块进行存在digit+1的更新（取反）
    void flip(int i, int j, int digit)
    {
        row_used_[i] ^= (1 << digit);
        column_used_[j] ^= (1 << digit);
        block_used_[(i / 3) * 3 + j / 3] ^= (1 << digit);
    }
};