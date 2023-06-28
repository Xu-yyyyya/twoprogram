#pragma once
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
int RunMySudoku(int argc, char* argv[]);
map<char, string> MyParse(int argc, char* argv[]);
int test();