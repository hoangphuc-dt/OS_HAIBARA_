#pragma once
#include <vector>
#include <string>
#include <iostream>

// --- Data structures ---
struct Process {
    std::string name;
    float AT = 0;
    float BT = 0;
    float RT = 0;
    float ET = 0;
    int priority = 0;
    float runTime = 0;

    bool operator<(const Process& other) const {
        return AT < other.AT;
    }
};

struct GanttBlock {
    std::string name;
    int start_time;
    int end_time;
};

// --- Khai báo các hàm để main.cpp có thể gọi ---
void runFCFS(std::vector<Process> procs);
void runSJF(std::vector<Process> procs);
void runSRTF(std::vector<Process> procs);
void runPriorityNP(std::vector<Process> procs);
void runPriorityP(std::vector<Process> procs);
void runRoundRobin(std::vector<Process> procs, int quantum);

// Hàm hỗ trợ nhập liệu
std::vector<Process> readProcesses(bool hasPriority);