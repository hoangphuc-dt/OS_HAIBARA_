#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <string>
#include <climits>
#include <iomanip>
#include <cmath>

using namespace std;

// ----------------------------------------------
//  Data structures
// ----------------------------------------------

struct Process {
    string name;
    float  AT       = 0;   // Arrival Time
    float  BT       = 0;   // Burst Time
    float  RT       = 0;   // Remaining Time
    float  ET       = 0;   // Exit (completion) Time
    int    priority = 0;   // Lower number = higher urgency
    float  runTime  = 0;   // Accumulated run time (SRTF/Priority-P tracking)

    bool operator<(const Process& other) const {
        return AT < other.AT;   // Sort by arrival time
    }
};

struct GanttBlock {
    string name;
    int    start_time;
    int    end_time;
};

// ----------------------------------------------
//  Output helpers
// ----------------------------------------------

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
        int numDigits  = (int)to_string(b.start_time).length();
        int spaces     = (int)b.name.length() + 2 - numDigits + 1;
        for (int i = 0; i < spaces; ++i) cout << " ";
    }
    cout << gantt.back().end_time << "\n";
}

void printResults(const vector<Process>& completed) {
    cout << "\n--- Process Results ---\n";
    cout << left
         << setw(10) << "Process"
         << setw(6)  << "AT"
         << setw(6)  << "BT"
         << setw(6)  << "ET"
         << setw(14) << "Turnaround"
         << setw(10) << "Waiting" << "\n";
    cout << string(52, '-') << "\n";

    float totalTAT = 0, totalWT = 0;
    for (const auto& p : completed) {
        float tat = p.ET - p.AT;
        float wt  = tat  - p.BT;
        totalTAT += tat;
        totalWT  += wt;
        cout << left
             << setw(10) << p.name
             << setw(6)  << p.AT
             << setw(6)  << p.BT
             << setw(6)  << p.ET
             << setw(14) << tat
             << setw(10) << wt << "\n";
    }
    int n = (int)completed.size();
    cout << string(52, '-') << "\n";
    cout << fixed << setprecision(2);
    cout << "Average Turnaround Time : " << totalTAT / n << "\n";
    cout << "Average Waiting Time    : " << totalWT  / n << "\n";
}

// ----------------------------------------------
//  Input helpers
// ----------------------------------------------

vector<Process> readProcesses(bool hasPriority) {
    vector<Process> list;
    Process p;
    if (hasPriority) {
        cout << "Enter processes  [name  AT  BT  priority]  -  EOF when done:\n";
        while (cin >> p.name >> p.AT >> p.BT >> p.priority) {
            p.RT = p.BT; p.runTime = 0; p.ET = 0;
            list.push_back(p);
        }
    } else {
        cout << "Enter processes  [name  AT  BT]  -  EOF when done:\n";
        while (cin >> p.name >> p.AT >> p.BT) {
            p.RT = p.BT; p.runTime = 0; p.ET = 0; p.priority = 0;
            list.push_back(p);
        }
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Flush leftover input
    return list;
}

// ----------------------------------------------
//  Helper: push newly arrived processes into a
//  ready-queue (used by SRTF / Priority-P tick loops)
// ----------------------------------------------
void updateGantt(vector<GanttBlock>& gantt, const string& name, int time) {
    if (gantt.empty() || gantt.back().name != name)
        gantt.push_back({name, time, time + 1});
    else
        gantt.back().end_time = time + 1;
}

// ----------------------------------------------
//  1. FIRST-COME FIRST-SERVE  (non-preemptive)
// ----------------------------------------------
void runFCFS(vector<Process> procs) {
    cout << " First-Come First-Serve  (FCFS)\n";

    sort(procs.begin(), procs.end());   // Sort by AT

    vector<GanttBlock> gantt;
    vector<Process>    completed;
    float currentTime = 0;

    for (auto& p : procs) {
        if (currentTime < p.AT) {
            // CPU is idle until this process arrives
            gantt.push_back({"IDLE", (int)currentTime, (int)p.AT});
            currentTime = p.AT;
        }
        gantt.push_back({p.name, (int)currentTime, (int)(currentTime + p.BT)});
        currentTime += p.BT;
        p.ET = currentTime;
        completed.push_back(p);
    }

    printGanttChart(gantt);
    printResults(completed);
}

// ----------------------------------------------
//  2. SHORTEST JOB FIRST  (non-preemptive)
// ----------------------------------------------
void runSJF(vector<Process> procs) {
    cout << " Shortest Job First  (SJF)  -  Non-Preemptive\n";

    sort(procs.begin(), procs.end());

    vector<GanttBlock> gantt;
    vector<Process>    completed;
    vector<bool>       done(procs.size(), false);
    float currentTime   = 0;
    int   completedCnt  = 0;
    int   n             = (int)procs.size();

    while (completedCnt < n) {
        int   bestIdx = -1;
        float bestBT  = 1e9f;

        for (int i = 0; i < n; ++i)
            if (!done[i] && procs[i].AT <= currentTime && procs[i].BT < bestBT) {
                bestBT = procs[i].BT;
                bestIdx = i;
            }

        if (bestIdx == -1) {
            // CPU idle - jump to next arrival
            float nextAT = 1e9f;
            for (int i = 0; i < n; ++i)
                if (!done[i]) nextAT = min(nextAT, procs[i].AT);
            gantt.push_back({"IDLE", (int)currentTime, (int)nextAT});
            currentTime = nextAT;
        } else {
            gantt.push_back({procs[bestIdx].name,
                             (int)currentTime,
                             (int)(currentTime + procs[bestIdx].BT)});
            currentTime       += procs[bestIdx].BT;
            procs[bestIdx].ET  = currentTime;
            completed.push_back(procs[bestIdx]);
            done[bestIdx] = true;
            ++completedCnt;
        }
    }

    printGanttChart(gantt);
    printResults(completed);
}

// ----------------------------------------------
//  3. SHORTEST REMAINING TIME FIRST  (preemptive SJF)
// ----------------------------------------------
void runSRTF(vector<Process> procs) {
    cout << " Shortest Remaining Time First  (SRTF) - Preemptive\n";

    sort(procs.begin(), procs.end());

    vector<GanttBlock> gantt;
    vector<Process>    completed;
    float currentTime = 0;

    while (!procs.empty()) {
        int   bestIdx = -1;
        float bestRT  = 1e9f;

        for (int i = 0; i < (int)procs.size(); ++i)
            if (procs[i].AT <= currentTime && procs[i].RT < bestRT) {
                bestRT  = procs[i].RT;
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

// ----------------------------------------------
//  4. PRIORITY SCHEDULING  (non-preemptive)
//     Lower priority number = higher urgency
// ----------------------------------------------
void runPriorityNP(vector<Process> procs) {
    cout << " Priority Scheduling - Non-Preemptive\n";
    cout << " (lower number = higher priority)\n";

    sort(procs.begin(), procs.end());

    vector<GanttBlock> gantt;
    vector<Process>    completed;
    vector<bool>       done(procs.size(), false);
    float currentTime   = 0;
    int   completedCnt  = 0;
    int   n             = (int)procs.size();

    while (completedCnt < n) {
        int bestIdx  = -1;
        int bestPrio = INT_MAX;

        for (int i = 0; i < n; ++i)
            if (!done[i] && procs[i].AT <= currentTime && procs[i].priority < bestPrio) {
                bestPrio = procs[i].priority;
                bestIdx  = i;
            }

        if (bestIdx == -1) {
            float nextAT = 1e9f;
            for (int i = 0; i < n; ++i)
                if (!done[i]) nextAT = min(nextAT, procs[i].AT);
            gantt.push_back({"IDLE", (int)currentTime, (int)nextAT});
            currentTime = nextAT;
        } else {
            gantt.push_back({procs[bestIdx].name,
                             (int)currentTime,
                             (int)(currentTime + procs[bestIdx].BT)});
            currentTime       += procs[bestIdx].BT;
            procs[bestIdx].ET  = currentTime;
            completed.push_back(procs[bestIdx]);
            done[bestIdx] = true;
            ++completedCnt;
        }
    }

    printGanttChart(gantt);
    printResults(completed);
}

// ----------------------------------------------
//  5. PRIORITY SCHEDULING  (preemptive)
//     Structurally identical to SRTF but compares
//     priority instead of remaining time.
// ----------------------------------------------
void runPriorityP(vector<Process> procs) {
    cout << " Priority Scheduling  -  Preemptive\n";
    cout << " (lower number = higher priority)\n";

    sort(procs.begin(), procs.end());

    vector<GanttBlock> gantt;
    vector<Process>    completed;
    float currentTime = 0;

    while (!procs.empty()) {
        int bestIdx  = -1;
        int bestPrio = INT_MAX;

        for (int i = 0; i < (int)procs.size(); ++i)
            if (procs[i].AT <= currentTime && procs[i].priority < bestPrio) {
                bestPrio = procs[i].priority;
                bestIdx  = i;
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

// ----------------------------------------------
//  6. ROUND ROBIN  (preemptive, fixed time quantum)
// ----------------------------------------------
void runRoundRobin(vector<Process> procs, int quantum) {
    cout << " Round Robin  (RR)  -  Quantum = " << quantum << "\n";

    sort(procs.begin(), procs.end());   // Sort by AT

    int n = (int)procs.size();
    vector<GanttBlock> gantt;
    vector<Process>    completed;
    queue<int>         rq;               // Indices into procs[]
    vector<bool>       arrived(n, false);
    float currentTime   = 0;
    int   completedCnt  = 0;

    // Lambda: enqueue all processes that have now arrived
    auto enqueueArrivals = [&]() {
        for (int i = 0; i < n; ++i)
            if (!arrived[i] && procs[i].AT <= currentTime) {
                arrived[i] = true;
                rq.push(i);
            }
    };

    enqueueArrivals();   // Seed with t=0 arrivals

    while (completedCnt < n) {
        if (rq.empty()) {
            // CPU idle - jump to the next unarrived process
            float nextAT = 1e9f;
            for (int i = 0; i < n; ++i)
                if (!arrived[i]) nextAT = min(nextAT, procs[i].AT);
            gantt.push_back({"IDLE", (int)currentTime, (int)nextAT});
            currentTime = nextAT;
            enqueueArrivals();
            continue;
        }

        int idx = rq.front();
        rq.pop();

        // Run for min(quantum, remaining time)
        float slice    = min((float)quantum, procs[idx].RT);
        int   start    = (int)currentTime;
        procs[idx].RT -= slice;
        currentTime   += slice;

        gantt.push_back({procs[idx].name, start, (int)currentTime});

        // Enqueue any process that arrived during this slice
        enqueueArrivals();

        if (procs[idx].RT <= 0) {
            // Process finished
            procs[idx].ET = currentTime;
            completed.push_back(procs[idx]);
            ++completedCnt;
        } else {
            // Still has work left - goes to the back of the queue
            rq.push(idx);
        }
    }

    printGanttChart(gantt);
    printResults(completed);
}

// ----------------------------------------------
//  Main menu
// ----------------------------------------------
int main() {
    cout << "CPU Scheduling Simulator\n";
    cout << "  1. First-Come First-Serve (FCFS)\n";
    cout << "  2. Shortest Job First (SJF) - Non-Preemptive\n";
    cout << "  3. Shortest Remaining Time (SRTF) - Preemptive\n";
    cout << "  4. Priority Scheduling - Non-Preemptive\n";
    cout << "  5. Priority Scheduling - Preemptive\n";
    cout << "  6. Round Robin (RR)\n\n";
    cout << "Choice: ";

    int choice;
    if (!(cin >> choice) || choice < 1 || choice > 6) {
        cout << "Invalid choice.\n";
        return 1;
    }

    bool hasPriority = (choice == 4 || choice == 5);
    vector<Process> procs = readProcesses(hasPriority);

    if (procs.empty()) {
        cout << "No processes entered. Exiting.\n";
        return 1;
    }

    switch (choice) {
        case 1: runFCFS(procs);       break;
        case 2: runSJF(procs);        break;
        case 3: runSRTF(procs);       break;
        case 4: runPriorityNP(procs); break;
        case 5: runPriorityP(procs);  break;
        case 6: {
            int quantum;
            cout << "Time Quantum: ";
            cin >> quantum;
            if (quantum <= 0) { cout << "Quantum must be > 0.\n"; return 1; }
            runRoundRobin(procs, quantum);
            break;
        }
    }

    return 0;
}
