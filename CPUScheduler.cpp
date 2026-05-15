#include "CPUScheduler.h"

void centerTextCPU(string s, int width) {
    int left = (width - s.size()) / 2;
    for (int i = 0; i < left; i++)
        cout << " ";
    cout << s;
}

// ----------------------------------------------
//  Arrow-key menu
// ----------------------------------------------
void showCPUMenu(int choice) {
    const vector<string> items = {
        "[ 1 ]  First-Come First-Serve   (FCFS)                    ",
        "[ 2 ]  Shortest Job First       (SJF)  -  Non-Preemptive  ",
        "[ 3 ]  Shortest Remaining Time  (SRTF) -  Preemptive      ",
        "[ 4 ]  Priority Scheduling             -  Non-Preemptive  ",
        "[ 5 ]  Priority Scheduling             -  Preemptive      ",
        "[ 6 ]  Round Robin              (RR)                      ",
        "[ 0 ]  Exit                                               "
    };

    const string border(60, '=');
    vector<int> width = { 140, 140, 140, 140, 140, 140, 140 };

    setColor(11);
    cout << "\n";
    centerTextCPU("================================================================================", 140);
    cout << "\n";
    centerTextCPU("CPU SCHEDULING SIMULATOR", 140);
    cout << "\n";
    centerTextCPU("================================================================================", 140);
    cout << "\n\n";

    for (int i = 0; i < items.size(); i++) {
        if (i == choice) setColor(14);
        else             setColor(10);
        centerTextCPU(items[i], width[i]);
        cout << "\n\n";
    }

    setColor(11);
    centerTextCPU("================================================================================", 140);
    cout << "\n\n";
    setColor(13);
    centerTextCPU("Use UP / DOWN arrow keys and ENTER to select", 140);
    cout << "\n";
    setColor(7);
}

// ----------------------------------------------
//  Output helpers
// ----------------------------------------------

void printGanttChart(const vector<GanttBlock>& gantt) {
    if (gantt.empty()) return;
    setColor(YELLOW);
    cout << "\n--- Gantt Chart ---\n";
    setColor(WHITE);

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

void printResults(vector<Process>& completed) {
    // Sort completed theo th? t? nh?p (P1, 2, 3, ...)
    for (int i = 0; i < completed.size() - 1; i++)
        for (int j = i + 1; j < completed.size(); j++)
            if (completed[i].inputOrder > completed[j].inputOrder)
                swap(completed[i], completed[j]);
    //
    setColor(YELLOW);
    cout << "\n--- Process Results ---\n";
    setColor(WHITE);
    setColor(CYAN);
    cout << left
        << setw(10) << "Process"
        << setw(6) << "AT"
        << setw(6) << "BT"
        << setw(6) << "ET"
        << setw(14) << "Turnaround"
        << setw(10) << "Waiting" << "\n";
    setColor(WHITE);
    cout << string(52, '-') << "\n";

    float totalTAT = 0, totalWT = 0;
    for (const auto& p : completed) {
        float tat = p.ET - p.AT;
        float wt = tat - p.BT;
        totalTAT += tat;
        totalWT += wt;
        cout << left
            << setw(10);
        setColor(YELLOW);
        cout << p.name;
        setColor(WHITE);
        cout << setw(6) << p.AT
            << setw(6) << p.BT
            << setw(6) << p.ET
            << setw(14) << tat
            << setw(10) << wt << "\n";
    }
    int n = (int)completed.size();
    cout << string(52, '-') << "\n";
    cout << fixed << setprecision(2);
    setColor(CYAN);
    cout << "Average Turnaround Time : ";
    setColor(WHITE);
    cout << totalTAT / n << "\n";
    setColor(CYAN);
    cout << "Average Waiting Time    : ";
    setColor(WHITE);
    cout << totalWT / n << "\n";
}

// ----------------------------------------------
//  Input helpers
// ----------------------------------------------

vector<Process> readProcesses(bool hasPriority) {
    vector<Process> list;
    Process p;
    int inputOrder = 0;
    int numOfProcess;
    setColor(YELLOW);
    cout << "Enter number of processes: ";
    setColor(WHITE);
    cin >> numOfProcess;
    setColor(WHITE);
    if (hasPriority) {
        setColor(YELLOW);
        cout << "Enter processes  [name  AT  BT  priority]  :\n";
        setColor(WHITE);
        for (int i = 0; i < numOfProcess; i++) {
            cin >> p.name >> p.AT >> p.BT >> p.priority;
            p.RT = p.BT; p.runTime = 0; p.ET = 0;
            p.inputOrder = inputOrder;
            list.push_back(p);
            inputOrder++;
        }
    }
    else {
        setColor(YELLOW);
        cout << "Enter processes  [name  AT  BT]  :\n";
        setColor(WHITE);
        for (int i = 0; i < numOfProcess; i++) {
            cin >> p.name >> p.AT >> p.BT;
            p.RT = p.BT; p.runTime = 0; p.ET = 0; p.priority = 0;
            p.inputOrder = inputOrder;
            list.push_back(p);
            inputOrder++;
        }
    }
    cin.clear();
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');  // Flush leftover input
    return list;
}

// ----------------------------------------------
//  Helper: push newly arrived processes into a
//  ready-queue (used by SRTF / Priority-P tick loops)
// ----------------------------------------------
void updateGantt(vector<GanttBlock>& gantt, const string& name, int time) {
    if (gantt.empty() || gantt.back().name != name)
        gantt.push_back({ name, time, time + 1 });
    else
        gantt.back().end_time = time + 1;
}

// ----------------------------------------------
//  1. FIRST-COME FIRST-SERVE  (non-preemptive)
// ----------------------------------------------
void runFCFS(vector<Process> procs) {
    setColor(CYAN);
    cout << " First-Come First-Serve  (FCFS)\n";
    setColor(WHITE);

    sort(procs.begin(), procs.end());   // Sort by AT

    vector<GanttBlock> gantt;
    vector<Process>    completed;
    float currentTime = 0;

    for (auto& p : procs) {
        if (currentTime < p.AT) {
            // CPU is idle until this process arrives
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

// ----------------------------------------------
//  2. SHORTEST JOB FIRST  (non-preemptive)
// ----------------------------------------------
void runSJF(vector<Process> procs) {
    setColor(CYAN);
    cout << " Shortest Job First  (SJF)  -  Non-Preemptive\n";
    setColor(WHITE);

    sort(procs.begin(), procs.end());

    vector<GanttBlock> gantt;
    vector<Process>    completed;
    vector<bool>       done(procs.size(), false);
    float currentTime = 0;
    int   completedCnt = 0;
    int   n = (int)procs.size();

    while (completedCnt < n) {
        int   bestIdx = -1;
        float bestBT = 1e9f;

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
            gantt.push_back({ "IDLE", (int)currentTime, (int)nextAT });
            currentTime = nextAT;
        }
        else {
            gantt.push_back({ procs[bestIdx].name,
                             (int)currentTime,
                             (int)(currentTime + procs[bestIdx].BT) });
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

// ----------------------------------------------
//  3. SHORTEST REMAINING TIME FIRST  (preemptive SJF)
// ----------------------------------------------
void runSRTF(vector<Process> procs) {
    setColor(CYAN);
    cout << " Shortest Remaining Time First  (SRTF) - Preemptive\n";
    setColor(WHITE);

    sort(procs.begin(), procs.end());

    vector<GanttBlock> gantt;
    vector<Process>    completed;
    float currentTime = 0;

    while (!procs.empty()) {
        int   bestIdx = -1;
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

// ----------------------------------------------
//  4. PRIORITY SCHEDULING  (non-preemptive)
//      Lower priority number = higher urgency
// ----------------------------------------------
void runPriorityNP(vector<Process> procs) {
    setColor(CYAN);
    cout << " Priority Scheduling - Non-Preemptive\n";
    cout << " (lower number = higher priority)\n";
    setColor(WHITE);

    sort(procs.begin(), procs.end());

    vector<GanttBlock> gantt;
    vector<Process>    completed;
    vector<bool>       done(procs.size(), false);
    float currentTime = 0;
    int   completedCnt = 0;
    int   n = (int)procs.size();

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
            for (int i = 0; i < n; ++i)
                if (!done[i]) nextAT = min(nextAT, procs[i].AT);
            gantt.push_back({ "IDLE", (int)currentTime, (int)nextAT });
            currentTime = nextAT;
        }
        else {
            gantt.push_back({ procs[bestIdx].name,
                             (int)currentTime,
                             (int)(currentTime + procs[bestIdx].BT) });
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

// ----------------------------------------------
//  5. PRIORITY SCHEDULING  (preemptive)
//      Structurally identical to SRTF but compares
//      priority instead of remaining time.
// ----------------------------------------------
void runPriorityP(vector<Process> procs) {
    setColor(CYAN);
    cout << " Priority Scheduling  -  Preemptive\n";
    cout << " (lower number = higher priority)\n";
    setColor(WHITE);

    sort(procs.begin(), procs.end());

    vector<GanttBlock> gantt;
    vector<Process>    completed;
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

// ----------------------------------------------
//  6. ROUND ROBIN  (preemptive, fixed time quantum)
// ----------------------------------------------
void runRoundRobin(vector<Process> procs, int quantum) {
    setColor(CYAN);
    cout << " Round Robin  (RR)  -  Quantum = " << quantum << "\n";
    setColor(WHITE);

    sort(procs.begin(), procs.end());   // Sort by AT

    int n = (int)procs.size();
    vector<GanttBlock> gantt;
    vector<Process>    completed;
    queue<int>         rq;               // Indices into procs[]
    vector<bool>       arrived(n, false);
    float currentTime = 0;
    int   completedCnt = 0;

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
            gantt.push_back({ "IDLE", (int)currentTime, (int)nextAT });
            currentTime = nextAT;
            enqueueArrivals();
            continue;
        }

        int idx = rq.front();
        rq.pop();

        // Run for min(quantum, remaining time)
        float slice = min((float)quantum, procs[idx].RT);
        int   start = (int)currentTime;
        procs[idx].RT -= slice;
        currentTime += slice;

        gantt.push_back({ procs[idx].name, start, (int)currentTime });

        // Enqueue any process that arrived during this slice
        enqueueArrivals();

        if (procs[idx].RT <= 0) {
            // Process finished
            procs[idx].ET = currentTime;
            completed.push_back(procs[idx]);
            ++completedCnt;
        }
        else {
            // Still has work left - goes to the back of the queue
            rq.push(idx);
        }
    }

    printGanttChart(gantt);
    printResults(completed);
}

// ----------------------------------------------
//  Main menu  (arrow-key navigation)
// ----------------------------------------------
void cpuSchedulingMenu() {
    // 7 menu items: indices 0-5 = algorithms, 6 = Exit
    const int MENU_SIZE = 7;
    int choice = 0;
    char key;

    while (true) {

        // -- Navigation loop -------------------------
        while (true) {
            system("cls");
            showCPUMenu(choice);

            key = _getch();

            // Arrow keys send two bytes: 0xE0 (224) or 0x00 followed by the code
            if (key == (char)224 || key == (char)0)
                key = _getch();

            if (key == 72)  choice = (choice - 1 + MENU_SIZE) % MENU_SIZE; // Up
            else if (key == 80)  choice = (choice + 1) % MENU_SIZE;             // Down
            else if (key == 13)  break;                                         // Enter
        }

        system("cls");

        // -- Exit ------------------------------------
        if (choice == 6) {
            setColor(RED);
            cout << "Exiting. Goodbye!\n";
            setColor(WHITE);
            return;
        }

        // -- Request Quantum Time before readProcesses() so it wouldn't take garbage value -------
        int quantum;
        if (choice == 5) {
            setColor(YELLOW);
            cout << "\nTime Quantum: ";
            setColor(WHITE);
            cin >> quantum;
            if (quantum <= 0) {
                setColor(RED);
                cout << "Quantum must be > 0.\n";
                setColor(WHITE);
                system("pause");
                continue;
            }
        }
        // -- Read processes --------------------------
        bool hasPriority = (choice == 3 || choice == 4);   // 0-indexed: options 4 & 5
        vector<Process> procs = readProcesses(hasPriority);

        if (procs.empty()) {
            setColor(RED);
            cout << "No processes entered.\n";
            setColor(WHITE);
            system("pause");
            continue;
        }

        // -- Run chosen algorithm --------------------
        switch (choice) {
        case 0: runFCFS(procs);       break;
        case 1: runSJF(procs);        break;
        case 2: runSRTF(procs);       break;
        case 3: runPriorityNP(procs); break;
        case 4: runPriorityP(procs);  break;
        case 5: runRoundRobin(procs, quantum); break;
        }

        cout << "\n";
        setColor(GREEN);
        system("pause");
        setColor(WHITE);
    }
}