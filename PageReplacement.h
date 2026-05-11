#ifndef PAGEREPLACEMENT_H
#define PAGEREPLACEMENT_H

#include "Common.h"

struct Step {
    int currentPage;
    vector<int> frame;
    bool isFault;
    int replacedPage;
    vector<int> refBits;
};

void printSpace(int n);
void printLine(int n, int margin = 0);
void printSubLine(int n, int margin = 0);
void centerText(string s, int width);
void showMenu(int choice);
vector<Step> FIFO(vector<int> pages, int f);
vector<Step> LRU(vector<int> pages, int f);
vector<Step> OPT(vector<int> pages, int f);
vector<Step> CLOCK(vector<int> pages, int f);
int countFault(vector<Step> rs);
void printResult(vector<Step> rs);
void printSimulation(vector<Step> rs, string name, int f);
void compareAlgorithms(vector<Step> fifoResult, vector<Step> lruResult, vector<Step> optResult, vector<Step> clockResult);

#endif