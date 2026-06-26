#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <windows.h>
#include <conio.h>
#include <string>

using namespace std;

#define GREEN 10
#define BLUE 9
#define YELLOW 14
#define RED 12
#define ORANGE 6
#define WHITE 15
#define PINK 13
#define CYAN 11

inline void setColor(int color) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, color);
}
#endif