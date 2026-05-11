#ifndef DEADLOCKHANDLER_H
#define DEADLOCKHANDLER_H

#include "Common.h"

const int MAX_PROCESS = 100;
const int MAX_RESOURCE = 100;

void printSafetyTable(int n, int m, vector<vector<int>> allocation, vector<vector<int>> maxMatrix, vector<vector<int>> need, vector<int> available);
void printDetectionTable(int n, int m, vector<vector<int>> allocation, vector<vector<int>> request, vector<int> available);
bool isSafety(int n, int m, vector<vector<int>> allocation, vector<vector<int>> need, vector<int> available, vector<int>& safeSequence);
void inputNM(int& n, int& m);
void safetyAlgorithm();
void requestAlgorithm();
void deadlockDetection();

#endif