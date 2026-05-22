#include "CPUScheduler.h"

void centerTextCPU(string s, int width) {
    int left = (width - s.size()) / 2;
    for (int i = 0; i < left; i++)
        cout << " ";
    cout << s;
}


void showCPUMenu(int choice) {
    const vector<string> items = {
        "[ 1 ]  First-Come First-Serve   (FCFS)                    ",
        "[ 2 ]  Shortest Job First       (SJF)  -  Non-Preemptive  ",
        "[ 3 ]  Shortest Remaining Time  (SRTF) -  Preemptive      ",
        "[ 4 ]  Priority Scheduling             -  Non-Preemptive  ",
        "[ 5 ]  Priority Scheduling             -  Preemptive      ",
        "[ 6 ]  Round Robin              (RR)                      ",
        "[ 7 ]  Run All  &  Compare      (All Algorithms)          ",
        "[ 0 ]  Exit                                               "
    };

    const string border(60, '=');
    vector<int> width = { 140, 140, 140, 140, 140, 140, 140, 140 };

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



void printGanttChart(const vector<GanttBlock>& gantt) {
    if (gantt.empty()) return;
    setColor(YELLOW);
    cout << "\n--- Gantt Chart ---\n";
    setColor(WHITE);

   
    for (const auto& b : gantt) {
        cout << "+";
        for (size_t i = 0; i < b.name.length() + 2; ++i) cout << "-";
    }
    cout << "+\n";

    
    for (const auto& b : gantt)
        cout << "| " << b.name << " ";
    cout << "|\n";

    
    for (const auto& b : gantt) {
        cout << "+";
        for (size_t i = 0; i < b.name.length() + 2; ++i) cout << "-";
    }
    cout << "+\n";

   
    for (const auto& b : gantt) {
        cout << b.start_time;
        int numDigits = (int)to_string(b.start_time).length();
        int spaces = (int)b.name.length() + 2 - numDigits + 1;
        for (int i = 0; i < spaces; ++i) cout << " ";
    }
    cout << gantt.back().end_time << "\n";
}

AlgorithmResult printResults(vector<Process>& completed, const string& algoName = "") {
    
    for (int i = 0; i < (int)completed.size() - 1; i++)
        for (int j = i + 1; j < (int)completed.size(); j++)
            if (completed[i].inputOrder > completed[j].inputOrder)
                swap(completed[i], completed[j]);
    
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

    return AlgorithmResult{ algoName, (float)totalTAT / n, (float)totalWT / n };
}



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
    cin.ignore((numeric_limits<streamsize>::max)(), '\n'); 
    return list;
}

void updateGantt(vector<GanttBlock>& gantt, const string& name, int time) {
    if (gantt.empty() || gantt.back().name != name)
        gantt.push_back({ name, time, time + 1 });
    else
        gantt.back().end_time = time + 1;
}
AlgorithmResult runFCFS(vector<Process> procs) {
    setColor(CYAN);
    cout << " First-Come First-Serve  (FCFS)\n";
    setColor(WHITE);

    sort(procs.begin(), procs.end());   

    vector<GanttBlock> gantt;
    vector<Process>    completed;
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
    return printResults(completed, "FCFS");
}
AlgorithmResult runSJF(vector<Process> procs) {
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
    return printResults(completed, "SJF");
}

AlgorithmResult runSRTF(vector<Process> procs) {
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
    return printResults(completed, "SRTF");
}


AlgorithmResult runPriorityNP(vector<Process> procs) {
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
    return printResults(completed, "Priority (Non-Preemptive)");
}


AlgorithmResult runPriorityP(vector<Process> procs) {
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
    return printResults(completed, "Priority (Preemptive)");
}


AlgorithmResult runRoundRobin(vector<Process> procs, int quantum) {
    setColor(CYAN);
    cout << " Round Robin  (RR)  -  Quantum = " << quantum << "\n";
    setColor(WHITE);

    sort(procs.begin(), procs.end());   

    int n = (int)procs.size();
    vector<GanttBlock> gantt;
    vector<Process>    completed;
    queue<int>         rq;               
    vector<bool>       arrived(n, false);
    float currentTime = 0;
    int   completedCnt = 0;

    
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
            for (int i = 0; i < n; ++i)
                if (!arrived[i]) nextAT = min(nextAT, procs[i].AT);
            gantt.push_back({ "IDLE", (int)currentTime, (int)nextAT });
            currentTime = nextAT;
            enqueueArrivals();
            continue;
        }

        int idx = rq.front();
        rq.pop();

        float slice = min((float)quantum, procs[idx].RT);
        int   start = (int)currentTime;
        procs[idx].RT -= slice;
        currentTime += slice;

        gantt.push_back({ procs[idx].name, start, (int)currentTime });

       
        enqueueArrivals();

        if (procs[idx].RT <= 0) {
           
            procs[idx].ET = currentTime;
            completed.push_back(procs[idx]);
            ++completedCnt;
        }
        else {
            
            rq.push(idx);
        }
    }

    printGanttChart(gantt);
    return printResults(completed, "Round Robin (Q=" + to_string(quantum) + ")");
}


void runAll(vector<Process> procs, int quantum) {
    const string divider(62, '-');
    auto header = [&](const string& title) {
        setColor(CYAN);
        cout << "\n" << string(62, '=') << "\n";
        cout << "  " << title << "\n";
        cout << string(62, '=') << "\n";
        setColor(WHITE);
        };

    vector<AlgorithmResult> results;

    header("FIRST COME FIRST SERVE");
    results.push_back(runFCFS(procs));

    header("SHORTEST JOB FIRST");
    results.push_back(runSJF(procs));

    header("SHORTEST REMAINING TIME FIRST");
    results.push_back(runSRTF(procs));

    header("PRIORITY - NON-preemptive");
    results.push_back(runPriorityNP(procs));

    header("PRIORITY - PREEMPTIVE");
    results.push_back(runPriorityP(procs));

    header("ROUND ROBIN");
    results.push_back(runRoundRobin(procs, quantum));

    setColor(YELLOW);
    cout << "\n\n" << string(62, '=') << "\n";
    cout << "              ALGORITHM  COMPARISON  SUMMARY\n";
    cout << string(62, '=') << "\n";
    setColor(WHITE);

    setColor(CYAN);
    cout << fixed << setprecision(2);
    cout << left
        << setw(30) << "Algorithm"
        << setw(16) << "Avg TAT"
        << setw(14) << "Avg WT" << "\n";
    setColor(WHITE);
    cout << divider << "\n";

   
    float bestTAT = 1e9f, bestWT = 1e9f;
    for (const auto& r : results) {
        bestTAT = min(bestTAT, r.avgTAT);
        bestWT = min(bestWT, r.avgWT);
    }

    for (const auto& r : results) {
        bool isBestTAT = (r.avgTAT == bestTAT);
        bool isBestWT = (r.avgWT == bestWT);

        if (isBestTAT || isBestWT)
            setColor(GREEN);
        cout << left << setw(30) << r.name;

       
        setColor(isBestTAT ? GREEN : WHITE);
        cout << setw(16) << r.avgTAT;

        setColor(isBestWT ? GREEN : WHITE);
        cout << setw(14) << r.avgWT;

        cout << "\n";
        setColor(WHITE);
    }

    cout << divider << "\n";

   
    auto best = min_element(results.begin(), results.end(),
        [](const AlgorithmResult& a, const AlgorithmResult& b) {
            return (a.avgTAT + a.avgWT) < (b.avgTAT + b.avgWT);
        });

    setColor(YELLOW);
    cout << "\n  Best algorithm: ";
    setColor(GREEN);
    cout << best->name << "\n";
    setColor(WHITE);
}


void cpuSchedulingMenu() {
    const int MENU_SIZE = 8;
    int choice = 0;
    char key;

    while (true) {

      
        while (true) {
            system("cls");
            showCPUMenu(choice);

            key = _getch();

            
            if (key == (char)224 || key == (char)0)
                key = _getch();

            if (key == 72)       choice = (choice - 1 + MENU_SIZE) % MENU_SIZE;
            else if (key == 80)  choice = (choice + 1) % MENU_SIZE;             
            else if (key == 13)  break;                                         
        }

        system("cls");

        
        if (choice == 7) {
            setColor(RED);
            cout << "Exiting. Goodbye!\n";
            setColor(WHITE);
            return;
        }

       
        int quantum = 1;
        if (choice == 5 || choice == 6) {  
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
        
        bool hasPriority = (choice == 3 || choice == 4 || choice == 6);
        vector<Process> procs = readProcesses(hasPriority);

        if (procs.empty()) {
            setColor(RED);
            cout << "No processes entered.\n";
            setColor(WHITE);
            system("pause");
            continue;
        }

        
        switch (choice) {
        case 0: runFCFS(procs);         break;
        case 1: runSJF(procs);          break;
        case 2: runSRTF(procs);         break;
        case 3: runPriorityNP(procs);   break;
        case 4: runPriorityP(procs);    break;
        case 5: runRoundRobin(procs, quantum); break;
        case 6: runAll(procs, quantum); break;
        }

        cout << "\n";
        setColor(GREEN);
        system("pause");
        setColor(WHITE);
    }
}