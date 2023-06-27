#include "getopt.h"
#include <iostream>
#include <vector>
#include <utility>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <map>
#include <io.h>
#include <unistd.h>
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

    vector<Board> SolveSudoku(Board board)
    {
        init_state();
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (board[i][j] == '$')
                {
                    spaces_.push_back(pair<int, int>(i, j));
                }
                else
                {
                    int digit = board[i][j] - '1';
                    flip(i, j, digit);
                }
            }
        }
        DFS(board, 0);
        return result_;
    }

    //从记录的第pos个挖空处开始递归挖空向量，直到递归结束。
    void DFS(Board& board, int pos)
    {
        if (pos == spaces_.size())
        {
            add_result(board);
            return;
        }
        int i = spaces_[pos].first;
        int j = spaces_[pos].second;
        //mask是记录了第i行、j列以及该空对应的3*3块所能被允许的填空的值（以位记录，例mask==(二进制)10000，则该空能填5）
        int mask = ~(row_used_[i] | column_used_[j] | block_used_[(i / 3) * 3 + j / 3]) & 0x1ff;
        int digit = 0;
        while (mask)
        {
            if (mask & 1)
            {
                flip(i, j, digit);
                board[i][j] = '1' + digit;
                DFS(board, pos + 1);
                //每次递归完要再一次取反，恢复上次递归前原状
                flip(i, j, digit);
            }
            mask = mask >> 1;
            digit++;
        }
    }


    //打印出所有可能的结果
    void get_result()
    {
        for (size_t i = 0; i < result_.size(); i++)
        {
            Board board = result_[i];
            PrintBoard(board);
        }
    }

    //检查board上的数据分布是否合法
    bool CheckBoard(Board& board)
    {
        init_state();
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if (board[i][j] != '$')
                {
                    int digit = board[i][j] - '1';
                    if ((row_used_[i] | column_used_[j] | block_used_[(i / 3) * 3 + j / 3]) & (1 << digit))
                    {
                        return false;
                    }
                    flip(i, j, digit);
                }
            }
        }
        return true;
    }

    //形式化打印棋盘
    void PrintBoard(Board& board)
    {
        for (int i = 0; i < board.size(); i++)
        {
            for (int j = 0; j < board[i].size(); j++)
            {
                cout << board[i][j] << " ";
            }
            cout << "\n";
        }
    }
};

char my_data[9][9] = {
    {'.', '4', '.', '.', '7', '.', '.', '2', '8'},
    {'.', '.', '.', '.', '.', '.', '9', '.', '.'},
    {'3', '.', '.', '.', '.', '.', '.', '5', '7'},
    {'.', '9', '4', '.', '.', '.', '8', '.', '.'},
    {'7', '.', '.', '8', '3', '.', '.', '.', '.'},
    {'.', '.', '.', '6', '.', '4', '.', '.', '5'},
    {'.', '.', '9', '1', '.', '.', '.', '.', '.'},
    {'.', '.', '1', '.', '2', '.', '4', '6', '.'},
    {'.', '8', '3', '.', '.', '6', '5', '7', '.'} };

int test()
{
    int right = 0;
    SudokuPlayer player;
    vector<vector<char> > board(N, vector<char>(N, '.'));

    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[i].size(); j++)
        {
            if (my_data[i][j] == '.')
                board[i][j] = '$';
            else
                board[i][j] = my_data[i][j];
        }
    }
    bool check = player.CheckBoard(board);
    if (check) {
        cout << "checked" << endl;
        right = 1;
    }

    player.SolveSudoku(board);
    player.get_result();

    cout << endl;
    return right;
}
vector<Board> readFile(string filePath)
{
    ifstream infile;
    vector<Board> boards;
    infile.open(filePath);
    char data[100];
    Board tmp;
    vector<char> row;
    while (!infile.eof())
    {
        infile.getline(data, 100);
        if (data[0] == '-')
        {
            boards.push_back(Board(tmp));
            tmp.clear();
            continue;
        }
        for (int i = 0; i < strlen(data); i++)
        {
            if (('1' <= data[i] && data[i] <= '9') || data[i] == '$')
            {
                row.push_back(data[i]);
            }
        }
        tmp.push_back(vector<char>(row));
        row.clear();
    }
    infile.close();
    return boards;
}

void writeFile(vector<Board> boards, ofstream& f)
{
    for (int k = 0; k < boards.size(); k++)
    {
        for (int i = 0; i < boards[k].size(); i++)
        {
            for (int j = 0; j < boards[k][i].size(); j++)
            {
                f << boards[k][i][j] << " ";
            }
            f << "\n";
        }
        f << "------- " << k << " -------" << endl;
    }
}

map<char, string> MyParse(int argc, char* argv[])
{
    map<char, string> params;
    int compeleteBoardCount, gameNumber, gameLevel;
    vector<int> range;
    string inputFile;
    char opt = 0;
    int countU = 0;
    while ((opt = getopt(argc, argv, "c:s:n:m:r:u")) != -1)
    {
        char* p1 = new char[20];
        switch (opt)
        {
        case 'c':
            compeleteBoardCount = atoi(optarg);
            if (compeleteBoardCount < 1 || compeleteBoardCount > 1000000)
            {
                printf("生成数独终盘数量范围在1～1000000之间\n");
                exit(0);
            }
            params[opt] = string(optarg);
            break;
        case 's':
            inputFile = string(optarg);
            if (_access(optarg, 0) == -1)
            {
                printf("file does not exist\n");
                exit(0);
            }
            params[opt] = string(optarg);
            break;
        case 'n':
            gameNumber = atoi(optarg);
            if (gameNumber < 1 || gameNumber > 10000)
            {
                printf("生成数独游戏数量范围在1～10000之间\n");
                exit(0);
            }
            params[opt] = string(optarg);
            break;
        case 'm':
            gameLevel = atoi(optarg);
            if (gameLevel < 1 || gameLevel > 3)
            {
                printf("生成游戏难度的范围在1～3之间\n");
                exit(0);
            }
            params[opt] = string(optarg);
            break;
        case 'r':
            char* p;
            
            strcpy(p1, optarg);
            p = strtok(p1, "~");
            while (p)
            {
                range.push_back(atoi(p));
                p = strtok(NULL, "~");
            }
            if (range.size() != 2)
            {
                printf("请输入一个范围参数\n");
                exit(0);
            }
            if ((range[0] >= range[1]) || range[0] < 20 || range[1] > 55)
            {
                printf("请输入合法范围20～55\n");
                exit(0);
            }
            params[opt] = string(optarg);
            break;
        case 'u':
            params[opt] = string();
            countU++;
            if (countU >=2)
                goto FLAG;
            break;
        default:
            printf("请输入合法参数\n");
            exit(0);
            break;
        }
    }
 FLAG:   return params;
}

int main(){
    test();
    return 0;
}