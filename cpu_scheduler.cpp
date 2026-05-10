#include "cpu_scheduler.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <climits>
#include <iomanip>
#include <cmath>

using namespace std;

// ==============================================
//  HÀM HỖ TRỢ (OUTPUT HELPERS)
// ==============================================

void printGanttChart(const vector<GanttBlock>& gantt) {
    if (gantt.empty()) return;
    cout << "\n--- Gantt Chart ---\n";

    // Top border
    for (const auto& b : gantt) {
        cout << "+";
        for (size_t i = 0; i < b.name.length() + 2; ++i) cout << "-";
    }
    cout << "+\n";

    // Process names
    for (const auto& b : gantt)
        cout << "| " << b.name << " ";
    cout << "|\n";

    // Bottom border
    for (const auto& b : gantt) {
        cout << "+";
        for (size_t i = 0; i < b.name.length() + 2; ++i) cout << "-";
    }
    cout << "+\n";

    // Time markers
    for (const auto& b : gantt) {
        cout << b.start_time;
        int numDigits = (int)to_string(b.start_time).length();
        int spaces = (int)b.name.length() + 2 - numDigits + 1;
        for (int i = 0; i < spaces; ++i) cout << " ";
    }
    cout << gantt.back().end_time << "\n";
}

void printResults(const vector<Process>& completed) {
    cout << "\n--- Process Results ---\n";
    cout << left
        << setw(10) << "Process"
        << setw(6) << "AT"
        << setw(6) << "BT"
        << setw(6) << "ET"
        << setw(14) << "Turnaround"
        << setw(10) << "Waiting" << "\n";
    cout << string(52, '-') << "\n";

    float totalTAT = 0, totalWT = 0;
    for (const auto& p : completed) {
        float tat = p.ET - p.AT;
        float wt = tat - p.BT;
        totalTAT += tat;
        totalWT += wt;
        cout << left
            << setw(10) << p.name
            << setw(6) << p.AT
            << setw(6) << p.BT
            << setw(6) << p.ET
            << setw(14) << tat
            << setw(10) << wt << "\n";
    }
    int n = (int)completed.size();
    cout << string(52, '-') << "\n";
    cout << fixed << setprecision(2);
    cout << "Average Turnaround Time : " << totalTAT / n << "\n";
    cout << "Average Waiting Time    : " << totalWT / n << "\n";
}

// ==============================================
//  HÀM NHẬP (INPUT HELPERS)
// ==============================================

vector<Process> readProcesses(bool hasPriority) {
    vector<Process> list;
    Process p;
    if (hasPriority) {
        cout << "Enter processes  [name  AT  BT  priority]  -  EOF when done:\n";
        while (cin >> p.name >> p.AT >> p.BT >> p.priority) {
            p.RT = p.BT; p.runTime = 0; p.ET = 0;
            list.push_back(p);
        }
    }
    else {
        cout << "Enter processes  [name  AT  BT]  -  EOF when done:\n";
        while (cin >> p.name >> p.AT >> p.BT) {
            p.RT = p.BT; p.runTime = 0; p.ET = 0; p.priority = 0;
            list.push_back(p);
        }
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return list;
}

void updateGantt(vector<GanttBlock>& gantt, const string& name, int time) {
    if (gantt.empty() || gantt.back().name != name)
        gantt.push_back({ name, time, time + 1 });
    else
        gantt.back().end_time = time + 1;
}

// ==============================================
//  CÁC THUẬT TOÁN ĐIỀU PHỐI (6 THUẬT TOÁN)
// ==============================================

// 1. FCFS
void runFCFS(vector<Process> procs) {
    cout << " First-Come First-Serve  (FCFS)\n";
    sort(procs.begin(), procs.end());
    vector<GanttBlock> gantt;
    vector<Process> completed;
    float currentTime = 0;

    for (auto& p : procs) {
        if (currentTime < p.AT) {
            gantt.push_back({ "IDLE", (int)currentTime, (int)p.AT });
            currentTime = p.AT;
        }
        gantt.push_back({ p.name, (int)currentTime, (int)(currentTime + p.BT) });
        currentTime += p.BT;
        p.ET = currentTime;
        completed.push_back(p);
    }
    printGanttChart(gantt);
    printResults(completed);
}

// 2. SJF
void runSJF(vector<Process> procs) {
    cout << " Shortest Job First  (SJF)  -  Non-Preemptive\n";
    sort(procs.begin(), procs.end());
    vector<GanttBlock> gantt;
    vector<Process> completed;
    vector<bool> done(procs.size(), false);
    float currentTime = 0;
    int completedCnt = 0;
    int n = (int)procs.size();

    while (completedCnt < n) {
        int bestIdx = -1;
        float bestBT = 1e9f;
        for (int i = 0; i < n; ++i)
            if (!done[i] && procs[i].AT <= currentTime && procs[i].BT < bestBT) {
                bestBT = procs[i].BT;
                bestIdx = i;
            }
        if (bestIdx == -1) {
            float nextAT = 1e9f;
            for (int i = 0; i < n; ++i) if (!done[i]) nextAT = min(nextAT, procs[i].AT);
            gantt.push_back({ "IDLE", (int)currentTime, (int)nextAT });
            currentTime = nextAT;
        }
        else {
            gantt.push_back({ procs[bestIdx].name, (int)currentTime, (int)(currentTime + procs[bestIdx].BT) });
            currentTime += procs[bestIdx].BT;
            procs[bestIdx].ET = currentTime;
            completed.push_back(procs[bestIdx]);
            done[bestIdx] = true;
            ++completedCnt;
        }
    }
    printGanttChart(gantt);
    printResults(completed);
}

// 3. SRTF
void runSRTF(vector<Process> procs) {
    cout << " Shortest Remaining Time First (SRTF) - Preemptive\n";
    sort(procs.begin(), procs.end());
    vector<GanttBlock> gantt;
    vector<Process> completed;
    float currentTime = 0;

    while (!procs.empty()) {
        int bestIdx = -1;
        float bestRT = 1e9f;
        for (int i = 0; i < (int)procs.size(); ++i)
            if (procs[i].AT <= currentTime && procs[i].RT < bestRT) {
                bestRT = procs[i].RT;
                bestIdx = i;
            }
        if (bestIdx != -1) updateGantt(gantt, procs[bestIdx].name, (int)currentTime);
        else updateGantt(gantt, "IDLE", (int)currentTime);

        if (bestIdx != -1) {
            procs[bestIdx].runTime++;
            procs[bestIdx].RT--;
            if (procs[bestIdx].RT <= 0) {
                procs[bestIdx].ET = currentTime + 1;
                completed.push_back(procs[bestIdx]);
                procs.erase(procs.begin() + bestIdx);
            }
        }
        ++currentTime;
    }
    printGanttChart(gantt);
    printResults(completed);
}

// 4. PRIORITY (Non-Preemptive)
void runPriorityNP(vector<Process> procs) {
    cout << " Priority Scheduling - Non-Preemptive\n";
    sort(procs.begin(), procs.end());
    vector<GanttBlock> gantt;
    vector<Process> completed;
    vector<bool> done(procs.size(), false);
    float currentTime = 0;
    int completedCnt = 0;
    int n = (int)procs.size();

    while (completedCnt < n) {
        int bestIdx = -1;
        int bestPrio = INT_MAX;
        for (int i = 0; i < n; ++i)
            if (!done[i] && procs[i].AT <= currentTime && procs[i].priority < bestPrio) {
                bestPrio = procs[i].priority;
                bestIdx = i;
            }
        if (bestIdx == -1) {
            float nextAT = 1e9f;
            for (int i = 0; i < n; ++i) if (!done[i]) nextAT = min(nextAT, procs[i].AT);
            gantt.push_back({ "IDLE", (int)currentTime, (int)nextAT });
            currentTime = nextAT;
        }
        else {
            gantt.push_back({ procs[bestIdx].name, (int)currentTime, (int)(currentTime + procs[bestIdx].BT) });
            currentTime += procs[bestIdx].BT;
            procs[bestIdx].ET = currentTime;
            completed.push_back(procs[bestIdx]);
            done[bestIdx] = true;
            ++completedCnt;
        }
    }
    printGanttChart(gantt);
    printResults(completed);
}

// 5. PRIORITY (Preemptive)
void runPriorityP(vector<Process> procs) {
    cout << " Priority Scheduling - Preemptive\n";
    sort(procs.begin(), procs.end());
    vector<GanttBlock> gantt;
    vector<Process> completed;
    float currentTime = 0;

    while (!procs.empty()) {
        int bestIdx = -1;
        int bestPrio = INT_MAX;
        for (int i = 0; i < (int)procs.size(); ++i)
            if (procs[i].AT <= currentTime && procs[i].priority < bestPrio) {
                bestPrio = procs[i].priority;
                bestIdx = i;
            }
        if (bestIdx != -1) {
            updateGantt(gantt, procs[bestIdx].name, (int)currentTime);
            procs[bestIdx].runTime++;
            procs[bestIdx].RT--;
            if (procs[bestIdx].RT <= 0) {
                procs[bestIdx].ET = currentTime + 1;
                completed.push_back(procs[bestIdx]);
                procs.erase(procs.begin() + bestIdx);
            }
        }
        else updateGantt(gantt, "IDLE", (int)currentTime);
        ++currentTime;
    }
    printGanttChart(gantt);
    printResults(completed);
}

// 6. ROUND ROBIN
void runRoundRobin(vector<Process> procs, int quantum) {
    cout << " Round Robin (RR) - Quantum = " << quantum << "\n";
    sort(procs.begin(), procs.end());
    int n = (int)procs.size();
    vector<GanttBlock> gantt;
    vector<Process> completed;
    queue<int> rq;
    vector<bool> arrived(n, false);
    float currentTime = 0;
    int completedCnt = 0;

    auto enqueueArrivals = [&]() {
        for (int i = 0; i < n; ++i)
            if (!arrived[i] && procs[i].AT <= currentTime) {
                arrived[i] = true;
                rq.push(i);
            }
        };

    enqueueArrivals();
    while (completedCnt < n) {
        if (rq.empty()) {
            float nextAT = 1e9f;
            for (int i = 0; i < n; ++i) if (!arrived[i]) nextAT = min(nextAT, procs[i].AT);
            gantt.push_back({ "IDLE", (int)currentTime, (int)nextAT });
            currentTime = nextAT;
            enqueueArrivals();
            continue;
        }
        int idx = rq.front(); rq.pop();
        float slice = min((float)quantum, procs[idx].RT);
        int start = (int)currentTime;
        procs[idx].RT -= slice;
        currentTime += slice;
        gantt.push_back({ procs[idx].name, start, (int)currentTime });
        enqueueArrivals();
        if (procs[idx].RT <= 0) {
            procs[idx].ET = currentTime;
            completed.push_back(procs[idx]);
            ++completedCnt;
        }
        else rq.push(idx);
    }
    printGanttChart(gantt);
    printResults(completed);
}