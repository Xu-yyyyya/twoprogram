#define _CRT_SECURE_NO_WARNINGS
//Author：Xu Yuang&Duan Chenrui
//
// 
//This brief piece of code is about to achive the little sudoku games,which contains how to get the final situations 
//and how to get the initial boards.Also it has the DFS methods to culculate a specify sukodu game. 
// 



#include "sudotest.h"



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

    //生成数独终盘，并从中挖去 digCount 个数（中间九宫格矩阵变换法）
    Board GenerateBoard(int digCount)
    {
        vector<vector<char> > board(N, vector<char>(N, '$'));
        // 生成0-8的数，位置随机
        vector<int> row = getRand9();
        for (int i = 0; i < 3; i++)
        {
            board[3][static_cast<std::vector<char, std::allocator<char>>::size_type>(i) + 3] = row[i] + '1';
            board[4][static_cast<std::vector<char, std::allocator<char>>::size_type>(i) + 3] = row[static_cast<std::vector<int, std::allocator<int>>::size_type>(i) + 3] + '1';
            board[5][static_cast<std::vector<char, std::allocator<char>>::size_type>(i) + 3] = row[static_cast<std::vector<int, std::allocator<int>>::size_type>(i) + 6] + '1';
        }
        // 以下四个步骤就是上面的矩阵变换
        CopySquare(board, 3, 3, true);
        CopySquare(board, 3, 3, false);
        CopySquare(board, 3, 0, false);
        CopySquare(board, 3, 6, false);

        while (digCount)
        {
            int x = rand() % 9;
            int y = rand() % 9;
            if (board[x][y] == '$')
                continue;
            char tmp = board[x][y];
            board[x][y] = '$';

            SolveSudoku(board);
            if (result_.size() == 1)
            {
                digCount--;
            }
            else
            {
                board[x][y] = tmp;
            }
        }
        // printBoard(board);
        // cout << "spaces " << player.spaces.size() << "\n";
        if (!CheckBoard(board))
        {
            cout << "wrong board" << endl;
        }

        return board;
    }

    // 获取包含0-8的随机数组
    vector<int> getRand9()
    {
        vector<int> result;
        int digit = 0;
        while (result.size() != 9)
        {
            int num = rand() % 9;
            if ((1 << num) & digit)
            {
                continue;
            }
            else
            {
                result.push_back(num);
                digit ^= (1 << num);
            }
        }
        return result;
    }

    //矩阵变换 将左上角坐标(src_x,src_y)的3x3矩阵进行列扩展或者行扩展。isRow 为是否是行扩展
    void CopySquare(Board& board, int src_x, int src_y, bool isRow)
    {//行列变换只有201/120两种
        int rand_tmp = rand() % 2 + 1;
        int order_first[3] = { 1, 2, 0 };
        int order_second[3] = { 2, 0, 1 };
        if (rand_tmp == 2)
        {
            order_first[0] = 2;
            order_first[1] = 0;
            order_first[2] = 1;
            order_second[0] = 1;
            order_second[1] = 2;
            order_second[2] = 0;
        }
        for (int i = 0; i < 3; i++)
        {
            if (isRow)
            {
                board[src_x][i] = board[static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(src_x) + order_first[0]][static_cast<std::vector<char, std::allocator<char>>::size_type>(src_y) + i];
                board[static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(src_x) + 1][i] = board[static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(src_x) + order_first[1]][static_cast<std::vector<char, std::allocator<char>>::size_type>(src_y) + i];
                board[static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(src_x) + 2][i] = board[static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(src_x) + order_first[2]][static_cast<std::vector<char, std::allocator<char>>::size_type>(src_y) + i];
                board[src_x][static_cast<std::vector<char, std::allocator<char>>::size_type>(i) + 6] = board[static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(src_x) + order_second[0]][static_cast<std::vector<char, std::allocator<char>>::size_type>(src_y) + i];
                board[static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(src_x) + 1][static_cast<std::vector<char, std::allocator<char>>::size_type>(i) + 6] = board[static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(src_x) + order_second[1]][static_cast<std::vector<char, std::allocator<char>>::size_type>(src_y) + i];
                board[static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(src_x) + 2][static_cast<std::vector<char, std::allocator<char>>::size_type>(i) + 6] = board[static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(src_x) + order_second[2]][static_cast<std::vector<char, std::allocator<char>>::size_type>(src_y) + i];
            }
            else
            {
                board[i][src_y] = board[static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(src_x) + i][static_cast<std::vector<char, std::allocator<char>>::size_type>(src_y) + order_first[0]];
                board[i][static_cast<std::vector<char, std::allocator<char>>::size_type>(static_cast<std::vector<char, std::allocator<char>>::size_type>(src_y)) + 1] = board[static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(src_x) + i][static_cast<std::vector<char, std::allocator<char>>::size_type>(src_y) + order_first[1]];
                board[i][static_cast<std::vector<char, std::allocator<char>>::size_type>(src_y) + 2] = board[static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(src_x)) + i][static_cast<std::vector<char, std::allocator<char>>::size_type>(src_y) + order_first[2]];
                board[static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(i) + 6][src_y] = board[static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(src_x)) + i][static_cast<std::vector<char, std::allocator<char>>::size_type>(src_y) + order_second[0]];
                board[static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(i)) + 6][static_cast<std::vector<char, std::allocator<char>>::size_type>(src_y) + 1] = board[static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(src_x) + i][static_cast<std::vector<char, std::allocator<char>>::size_type>(src_y) + order_second[1]];
                board[static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(i) + 6][static_cast<std::vector<char, std::allocator<char>>::size_type>(src_y) + 2] = board[static_cast<std::vector<std::vector<char, std::allocator<char>>, std::allocator<std::vector<char, std::allocator<char>>>>::size_type>(src_x) + i][static_cast<std::vector<char, std::allocator<char>>::size_type>(src_y) + order_second[2]];
            }
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

// 解析输入参数
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

void generateGame(int gameNumber, int gameLevel, vector<int> digCount, ofstream& outfile, SudokuPlayer& player)
{
    for (int i = 0; i < gameNumber; i++)
    {
        int cnt = 0;
        if (digCount.size() == 1)
        {
            cnt = digCount[0];
        }
        else
        {
            cnt = rand() % (digCount[1] - digCount[0] + 1) + digCount[0];
        }
        Board b = player.GenerateBoard(cnt);
        vector<Board> bs;
        bs.push_back(b);
        writeFile(bs, outfile);
    }
    outfile.close();
}


int RunMySudoku(int argc, char* argv[]) {
    srand((unsigned)time(NULL));
    SudokuPlayer player;

    map<char, string> params = MyParse(argc, argv);
    map<char, string>::iterator it, tmp;

    int opt = 0;

    vector<int> range;
    int gameNumber;
    int gameLevel = 0;
    int solution_count = 0;

    vector<Board> boards;
    ofstream outfile;

    it = params.begin();
    while (it != params.end())
    {
        switch (it->first)
        {
        case 'c':
            outfile.open("game.txt", ios::out | ios::trunc);
            range.push_back(0);
            generateGame(atoi(it->second.c_str()), 0, range, outfile, player);
            range.clear();
            break;

        case 's':
            outfile.open("sudoku.txt", ios::out | ios::trunc);
            boards = readFile(it->second);
            for (int i = 0; i < boards.size(); i++)
            {
                vector<Board> result = player.SolveSudoku(boards[i]);
                writeFile(result, outfile);
            }
            outfile.close();
            break;

        case 'n':
        case 'm':
        case 'r':
        case 'u':
            tmp = params.find('n');
            if (tmp == params.end())
            {
                printf("缺少参数 n \n");
                exit(0);
            }

            gameNumber = atoi(tmp->second.c_str());

            

            tmp = params.find('m');
            if (tmp != params.end())
            {
                gameLevel = atoi(tmp->second.c_str());
            }

            tmp = params.find('r');
            if (tmp != params.end())
            {
                char* p;
                char* pc = new char[100];
                strcpy(pc, tmp->second.c_str());
                p = strtok(pc, "~");
                while (p)
                {
                    range.push_back(atoi(p));
                    p = strtok(NULL, "~");
                }
            }
            else
            {
                // 根据不同级别采取挖空数量不同
                if (gameLevel == 1)
                {
                    range.push_back(20);
                    range.push_back(30);
                }
                else if (gameLevel == 2)
                {
                    range.push_back(30);
                    range.push_back(40);
                }
                else if (gameLevel == 3)
                {
                    range.push_back(40);
                    range.push_back(55);
                }
                else
                {
                    range.push_back(20);
                    range.push_back(55);
                }
            }

            outfile.open("game.txt", ios::out | ios::trunc);
            generateGame(gameNumber, gameLevel, range, outfile, player);
            range.clear();
            break;
        }
        // cout << it->first << ' ' << it->second << endl;
        it++;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    RunMySudoku(argc, argv);

    return 0;
}
