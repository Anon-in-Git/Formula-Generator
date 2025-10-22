#pragma once
#include<algorithm>
#include<cmath>
#include<string>
#include<vector>
#include<fstream>
#include<iostream>
#include<utility>

typedef std::pair<int, int> PII;

class FC {
public:
    static int mygcd(int a, int b) {
        if (a > b)
            std::swap(a, b);
        if(a == 0)
			return b;
		return mygcd(b % a, a);
    }

    static void Recognize(const std::string& SingleEx, std::vector<int>& mydata) {
        int i = 0;
        while (i < SingleEx.size() && SingleEx[i] != '.')
            i++;
        i++;
        for (; i < SingleEx.size();) {
            while (SingleEx[i] == ' ' && i < SingleEx.size())
                i++;
            if (std::isdigit(SingleEx[i])) {
                int num = atoi(SingleEx.c_str() + i);
                mydata.emplace_back(num);
                while (std::isdigit(SingleEx[i]) && i < SingleEx.size())
                    i++;
            }
            switch (SingleEx[i]) {
            case '+': mydata.emplace_back(-1); i++; break;
            case '-': mydata.emplace_back(-2); i++; break;
            case '*': mydata.emplace_back(-3); i++; break;
            case '/': mydata.emplace_back(-4); i++; break;
            case '(': mydata.emplace_back(-5); i++; break;
            case ')': mydata.emplace_back(-6); i++; break;
            case '=': mydata.emplace_back(-7); i++; break;
            case '\'': mydata.emplace_back(-8); i++; break;
            default: i++;
            }
        }
    }

    static bool Check(std::vector<int>& mydata, const int& idx) {
        int cnt = 0;
        for (int i = 0; i < mydata.size(); i++) {
            if (mydata[i] == -5)
                cnt++;
            if (mydata[i] == -6)
                cnt--;
            if (cnt < 0) {
                std::cout << "[Error][-e]The NO." << idx + 1 << " exercise includes an ILLEGAL balanced bracket sequence." << std::endl << std::endl;
                system("pause");
                return false;
            }

            if (mydata[i] <= -1 && mydata[i] >= -4 && (i + 1 >= mydata.size() || (mydata[i + 1] <= -1 && mydata[i + 1] >= -4))) {
                std::cout << "[Error][-e]The NO." << idx + 1 << " exercise includes illegal arithmetic operator." << std::endl << std::endl;
                system("pause");
                return false;
            }

            if (mydata[i] == -4 && mydata[i + 1] == 0) {
                std::cout << "[Error][-e]The NO." << idx + 1 << " exercise involves division by zero." << std::endl << std::endl;
                system("pause");
                return false;
            }

            if (mydata[i] == -8) {
                if (i - 1 >= 0 && mydata[i - 1] > 0 && i + 3 < mydata.size() && mydata[i + 1] > 0 && mydata[i + 2] == -4 && mydata[i + 3] > 0) {
                    mydata[i + 1] += mydata[i - 1] * mydata[i + 3];

                    mydata[i - 1] = -5;
                    mydata[i] = mydata[i + 1];
                    mydata[i + 1] = mydata[i + 2];
                    mydata[i + 2] = mydata[i + 3];
                    mydata[i + 3] = -6;

                    i += 3;
                }
                else {
                    std::cout << "[Error][-e]The NO." << idx + 1 << " exercise includes ILLEGAL arithmetic operator." << std::endl << std::endl;
                    system("pause");
                    return false;
                }
            }
        }

        if (cnt != 0) {
            std::cout << "[Error][-e]The NO." << idx + 1 << " exercise includes an ILLEGAL balanced bracket sequence." << std::endl << std::endl;
            system("pause");
            return false;
        }
        return true;
    }

    static PII Calculate(const std::vector<int>& Ex, int dep, int& idx) {
        std::vector<int> res, div;
        int tmp = 1, mag = 1, op = 1;
        bool init = true, rt = false;
        for (int i = idx; i < Ex.size(); i++) {
            if (init && Ex[i] >= 0) {
                init = false;
                tmp = Ex[i] * op;
                mag = 1;
                continue;
            }
            switch (Ex[i]) {
            case -1:
                if (!init) {
                    res.emplace_back(tmp);
                    div.emplace_back(mag);
                }
                init = true;
                op = 1;
                break;
            case -2:
                if (!init) {
                    res.emplace_back(tmp);
                    div.emplace_back(mag);
                }
                init = true;
                op = -1;
                break;
            case -3:
                op = 3;
                break;
            case -4:
                op = 4;
                break;
            default:
                if (Ex[i] == -5) {
                    i++;
                    auto t = Calculate(Ex, dep + 1, i);
                    // std::cout << i << ": " << t.first << " " << t.second << std::endl;
                    if (init) {
                        init = false;
                        tmp = t.first * op;
                        mag = t.second;
                    }
                    else {
                        if (op == 3) {
                            tmp *= t.first;
                            mag *= t.second;
                            int divi = mygcd(tmp, mag);
                            if (divi > 0) {
                                tmp /= divi;
                                mag /= divi;
                            }
                        }
                        if (op == 4) {
                            tmp *= t.second;
                            mag *= t.first;
                            int divi = mygcd(tmp, mag);
                            if (divi > 0) {
                                tmp /= divi;
                                mag /= divi;
                            }
                        }
                    }
                    break;
                }
                if (Ex[i] == -6) {
                    rt = true;
                    break;
                }
                if (Ex[i] >= 0) {
                    if (op == 3) {
                        tmp *= Ex[i];
                        int divi = mygcd(tmp, mag);
                        if (divi > 0) {
                            tmp /= divi;
                            mag /= divi;
                        }
                    }
                    if (op == 4 && Ex[i] > 0) {
                        mag *= Ex[i];
                        int divi = mygcd(tmp, mag);
                        if (divi > 0) {
                            tmp /= divi;
                            mag /= divi;
                        }
                    }
                }
            }
            if (rt) {
                idx = i;
                break;
            }
        }
        if (!init) {
            res.emplace_back(tmp);
            div.emplace_back(mag);
        }
        PII ans = { 0, 1 };
        for (int i = 0; i < res.size(); i++) {
            ans.first = ans.first * div[i] + ans.second * res[i];
            ans.second = ans.second * div[i];
            int divi = mygcd(ans.first, ans.second);
            if (divi == 0)
                continue;
            ans.first /= divi;
            ans.second /= divi;
        }
        return ans;
    }

    static void Formula_Checker(const std::string& File_Ex, const std::string& File_Ans) {
        // std::cout << "File_Ex: " << File_Ex << std::endl;
        std::ifstream fin(File_Ex);
        std::ofstream fout("Grade.txt");
        std::vector<std::string> Ex;
        std::vector<std::string> Ans;
        std::vector<int> Correct, Wrong;
        if (!fin.is_open()) {
            std::cout << "[Error][-e]File Not Found." << std::endl << std::endl;
            system("pause");
            return;
        }
        std::string t;
        while (std::getline(fin, t)) {
            Ex.push_back(t);
            if (t.empty() || std::count(t.begin(), t.end(), ' ') == t.size())
                Ex.pop_back();
        }
        fin.close();

        fin.open(File_Ans);
        if (!fin.is_open()) {
            std::cout << "[Error][-a]File Not Found." << std::endl << std::endl;
            system("pause");
            return;
        }
        while (std::getline(fin, t)) {
            Ans.push_back(t);
            if (t.empty() || std::count(t.begin(), t.end(), ' ') == t.size())
                Ans.pop_back();
        }
        fin.close();

        for (int i = 0; i < Ex.size(); i++) {
            std::vector<int> mydata, LeftEx, RightEx;
            Recognize(Ex[i], LeftEx);
            Recognize(Ans[i], RightEx);

            if (!Check(LeftEx, i) || !Check(RightEx, i))
                continue;

            int idx1 = 0, idx2 = 0;
            PII ans1 = Calculate(LeftEx, 0, idx1), ans2 = Calculate(RightEx, 0, idx2);
            // std::cout << ans1.first << " " << ans1.second << std::endl;
            // std::cout << ans2.first << " " << ans2.second << std::endl;
            if (ans1 == ans2 || (ans1.first == ans2.first && ans1.first == 0))
                Correct.emplace_back(i + 1);
            else
                Wrong.emplace_back(i + 1);
        }

        if (!fout.is_open()) {
            std::cout << "[Error][-o]Output Error." << std::endl << std::endl;
            system("pause");
            return;
        }
        fout << "Correct: " << Correct.size() << " (";
        for (int i = 0; i < Correct.size(); i++) {
            if (i)
                fout << ", ";
            fout << Correct[i];
        }
        fout << ")" << std::endl;
        fout << "Wrong: " << Wrong.size() << " (";
        for (int i = 0; i < Wrong.size(); i++) {
            if (i)
                fout << ", ";
            fout << Wrong[i];
        }
        fout << ")" << std::endl;
        fout.close();
    }
};