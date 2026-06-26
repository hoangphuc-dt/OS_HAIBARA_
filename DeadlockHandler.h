#ifndef DEADLOCK_HANDLER_H
#define DEADLOCK_HANDLER_H

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <conio.h>
#include "Common.h"

using namespace std;

// Định nghĩa các hằng số (nếu có)
const int MAX_PROCESS = 10;
const int MAX_RESOURCE = 10;


// Khai báo hàm bổ trợ đồ họa
void setColor(int color); // Hàm này tùy thuộc vào cách bạn định nghĩa (như dùng SetConsoleTextAttribute)
void centerTextBanker(string s, int width);
void showBanker(int choice);
void clearInput();

// Khai báo các hàm nhập liệu
void inputNM(int& n, int& m);
void inputAllocation(vector<vector<int>>& allocation, int n, int m);
void inputMax(vector<vector<int>>& maxMatrix, vector<vector<int>>& allocation, int n, int m);
void inputInstance(vector<int>& instance, vector<vector<int>>& allocation, int n, int m);
void inputRequest(vector<vector<int>>& request, int n, int m);

// Khai báo các hàm hiển thị và thuật toán
void printSafetyTable(int n, int m, vector<vector<int>> allocation, vector<vector<int>> maxMatrix, vector<vector<int>> need, vector<int> available, int highlightProcess = -1);
void printDetectionTable(int n, int m, vector<vector<int>> allocation, vector<vector<int>> request, vector<int> available);
bool isSafety(int n, int m, vector<vector<int>> allocation, vector<vector<int>> need, vector<int> available, vector<int>& safeSequence);

void safetyAlgorithm();
void requestAlgorithm();
void deadlockDetection();
void showBankerMenu();

#endif