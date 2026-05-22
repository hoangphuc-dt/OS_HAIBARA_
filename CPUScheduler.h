#ifndef CPUSCHEDULER_H
#define CPUSCHEDULER_H

#include "Common.h"
#include <algorithm>
#include <queue>
#include <climits>
#include <cmath>


struct Process {
    string name;
    float  AT = 0;   
    float  BT = 0;   
    float  RT = 0;   
    float  ET = 0;   
    int    priority = 0;   
    float  runTime = 0;   

    int    inputOrder;    
    bool operator<(const Process& other) const {
        return AT < other.AT;   
    }
};

struct GanttBlock {
    string name;
    int    start_time;
    int    end_time;
};


struct AlgorithmResult {
    string name;
    float  avgTAT = 0;
    float  avgWT = 0;
};


void cpuSchedulingMenu();

#endif