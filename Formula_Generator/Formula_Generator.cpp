#include<cmath>
#include<string>
#include<vector>
#include<cstring>
#include<fstream>
#include<iomanip>
#include<iostream>
//使用FG头文件的函数需要声明域FG
#include"Formula_Generator.h"
//使用FC头文件的函数需要声明域FC
#include"Formula_Checker.h"

using namespace std;


int main(int argc, char* argv[]) {
    //maxn表示自然数的最大值
    //n表示题目数量
    int maxn = -1, n = -1;
    //File_Ex表示题目文件路径
    //File_Ans表示答案文件路径
    string File_Ex, File_Ans;
    for (int i = 1; i < argc; i++) {
        if (argv[i] == "-r" && i + 1 < argc) {
            if (maxn == -1)
                maxn = atoi(argv[i + 1]);
            else {
                cout << "[ERROR][-r]There are multiple definitions of the range of the maximum number." << endl << endl;
                system("pause");
                return 0;
            }
            if (maxn <= 0) {
                cout << "[ERROR][-r]There must be a number that greater than 0." << endl << endl;
                system("pause");
                return 0;
            }
            i++;
            continue;
        }
        if (argv[i] == "-r" && i + 1 >= argc) {
            cout << "[ERROR][-r]There must be a parameter after \"-r\"." << endl << endl;
            system("pause");
            return 0;
        }

        if (argv[i] == "-n" && i + 1 < argc) {
            if (n == -1)
                n = atoi(argv[i + 1]);
            else {
                cout << "[ERROR][-n]There are multiple definitions of the number of the equations." << endl << endl;
                system("pause");
                return 0;
            }
            if (maxn <= 0) {
                cout << "[ERROR][-n]There must be a number that greater than 0." << endl << endl;
                system("pause");
                return 0;
            }
            i++;
            continue;
        }
        if (argv[i] == "-n" && i + 1 >= argc) {
            cout << "[ERROR][-n]There must be a parameter after \"-n\"." << endl << endl;
            system("pause");
            return 0;
        }

        if (argv[i] == "-e" && i + 1 < argc) {
            if (File_Ex.empty())
                File_Ex = argv[i + 1];
            else {
                cout << "[ERROR][-e]There are too much files to read." << endl << endl;
                system("pause");
                return 0;
            }
            i++;
            continue;
        }
        if (argv[i] == "-e" && i + 1 >= argc) {
            cout << "[ERROR][-e]There must be a parameter after \"-e\"." << endl << endl;
            system("pause");
            return 0;
        }

        if (argv[i] == "-a" && i + 1 < argc) {
            if (File_Ans.empty())
                File_Ans = argv[i + 1];
            else {
                cout << "[ERROR][-a]There are too much files to read." << endl << endl;
                system("pause");
                return 0;
            }
            i++;
            continue;
        }
        if (argv[i] == "-a" && i + 1 >= argc) {
            cout << "[ERROR][-a]There must be a parameter after \"-e\"." << endl << endl;
            system("pause");
            return 0;
        }
    }

    if (n > 0 && maxn <= 0) {
        cout << "[ERROR][-r]There must be a definition of the range of the number after \"-r\"." << endl << endl;
        system("pause");
        return 0;
    }
    if (n <= 0 && maxn > 0) {
        cout << "[ERROR][-n]There must be a definition of the number of the exersice after \"-n\"." << endl << endl;
        system("pause");
        return 0;
    }

    if (!File_Ex.empty() && File_Ans.empty()) {
        cout << "[ERROR][-e]There must be a file after \"-e\"." << endl << endl;
        system("pause");
        return 0;
    }
    if (File_Ex.empty() && !File_Ans.empty()) {
        cout << "[ERROR][-e]There must be a file after \"-a\"." << endl << endl;
        system("pause");
        return 0;
    }

    if (n > 0 && maxn > 0)
        //可以在这里修改传入参数
        Formula_Generator(n,maxn);

    if (!File_Ans.empty() && !File_Ex.empty())
        FC::Formula_Checker();

    cout << "The program ends here." << endl << endl;
    system("pause");

    return 0;
}