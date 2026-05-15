#ifndef CPUSCHEDULER_H
#define CPUSCHEDULER_H

#include "Common.h"
#include <algorithm>
#include <queue>
#include <climits>
#include <cmath>

// ----------------------------------------------
//  Data structures
// ----------------------------------------------

struct Process {
    string name;
    float  AT = 0;   // Arrival Time
    float  BT = 0;   // Burst Time
    float  RT = 0;   // Remaining Time
    float  ET = 0;   // Exit (completion) Time
    int    priority = 0;   // Lower number = higher urgency
    float  runTime = 0;   // Accumulated run time (SRTF/Priority-P tracking)

    int    inputOrder;    // Sort the completed list by input order
    bool operator<(const Process& other) const {
        return AT < other.AT;   // Sort by arrival time
    }
};

struct GanttBlock {
    string name;
    int    start_time;
    int    end_time;
};

// Hàm khởi chạy chính
void cpuSchedulingMenu();

#endif