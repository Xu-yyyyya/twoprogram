#include "pch.h"
#include "../sudoku/sudotest.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			char* args[] = { "D:\\C++\\C++practice\\sudoku\\x64\\Debug\\sudoku.exe","-c","20" };
			Assert::AreEqual(0, RunMySudoku(3, args));
			
		}
		TEST_METHOD(TestMethod2)
		{
			char* args[] = { "D:\\C++\\C++practice\\sudoku\\x64\\Debug\\sudoku.exe","-n","20" };
			Assert::AreEqual(0, RunMySudoku(3, args));
		}
		TEST_METHOD(TestMethod3)
		{
			char* args[] = { "D:\\C++\\C++practice\\sudoku\\x64\\Debug\\sudoku.exe","-s","game.txt" };
			Assert::AreEqual(0, RunMySudoku(3, args));
		}
		TEST_METHOD(TestMethod4)
		{
			char* args[] = { "D:\\C++\\C++practice\\sudoku\\x64\\Debug\\sudoku.exe","-n","10","-m","1"};
			Assert::AreEqual(0, RunMySudoku(5, args));
		}
		TEST_METHOD(TestMethod5)
		{
			char* args[] = { "D:\\C++\\C++practice\\sudoku\\x64\\Debug\\sudoku.exe","-n","10","-m","2"};
			Assert::AreEqual(0, RunMySudoku(5, args));
		}
		TEST_METHOD(TestMethod6)
		{
			char* args[] = { "D:\\C++\\C++practice\\sudoku\\x64\\Debug\\sudoku.exe","-n","10","-m","3" };
			Assert::AreEqual(0, RunMySudoku(5, args));
		}
		TEST_METHOD(TestMethod7)
		{
			char* args[] = { "D:\\C++\\C++practice\\sudoku\\x64\\Debug\\sudoku.exe","-n","20","-r","20~50"};
			Assert::AreEqual(0, RunMySudoku(5, args));
		}
		TEST_METHOD(TestMethod8)
		{
			char* args[] = { "D:\\C++\\C++practice\\sudoku\\x64\\Debug\\sudoku.exe","-n","10","-u"};
			Assert::AreEqual(0, RunMySudoku(4, args));
		}
		TEST_METHOD(TestMethod9)
		{
			std::map<char, string> map1,map2;
			map1['r'] = "20~28";
			char* args[] = { "D:\\C++\\C++practice\\sudoku\\x64\\Debug\\sudoku.exe","-r","20~28"};
			map2 = MyParse(3, args);
			Assert::AreEqual(map1['r'], map2['r']);
		}
		TEST_METHOD(TestMethod10)
		{
			
			Assert::AreEqual(1, test());
		}
	};
}
