#include "PageReplacement.h"

void printSpace(int n) {
    for (int i = 0; i < n; i++)
        cout << " ";
}

void printLine(int n, int margin) {
    printSpace(margin);
    for (int i = 0; i < n; i++)
        cout << "=";
    cout << "\n";
}

void printSubLine(int n, int margin) {
    printSpace(margin);
    for (int i = 0; i < n; i++)
        cout << "-";
    cout << "\n";
}

void centerText(string s, int width) {
    int left = (width - s.size()) / 2;
    for (int i = 0; i < left; i++)
        cout << " ";
    cout << s;
}

void showMenu(int choice) {
    vector<string> menu = {
        "[1] FIFO",
        "[2] LRU",
        "[3] OPT",
        "[4] CLOCK",
        "[5] RUN ALL",
        "[0] EXIT"
    };
    vector<int> width = { 140, 140, 140, 142, 143, 140 };

    setColor(11);
    cout << "\n";
    centerText("================================================================================", 140);
    cout << "\n";
    centerText("PAGE REPLACEMENT SIMULATOR", 140);
    cout << "\n";
    centerText("================================================================================", 140);
    cout << "\n\n";

    for (int i = 0; i < menu.size(); i++) {
        if (i == choice) setColor(14);
        else setColor(10);

        centerText(menu[i], width[i]);
        cout << "\n\n";
    }

    setColor(11);
    centerText("================================================================================", 140);
    cout << "\n\n";
    setColor(13);
    centerText("Use UP / DOWN arrow keys and ENTER to select", 140);
    cout << "\n";
    setColor(7);
}

vector<Step> FIFO(vector<int> pages, int f) {
    vector<Step> result;
    vector<int> frame(f, -1);
    int pointer = 0;

    for (int page : pages) {
        Step step;
        step.currentPage = page;
        step.replacedPage = -1;
        bool found = false;

        for (int x : frame) {
            if (x == page) { found = true; break; }
        }

        if (found) { step.isFault = false; }
        else {
            step.isFault = true;
            if (frame[pointer] != -1) step.replacedPage = frame[pointer];
            frame[pointer] = page;
            pointer = (pointer + 1) % f;
        }
        step.frame = frame;
        result.push_back(step);
    }
    return result;
}

vector<Step> LRU(vector<int> pages, int f) {
    vector<Step> result;
    vector<int> frame(f, -1);
    vector<int> lastUsed(f, -1);
    int timer = 0;

    for (int page : pages) {
        timer++;
        Step step;
        step.currentPage = page;
        step.replacedPage = -1;
        bool found = false;

        for (int i = 0; i < f; i++) {
            if (frame[i] == page) {
                found = true;
                lastUsed[i] = timer;
                break;
            }
        }

        if (found) { step.isFault = false; }
        else {
            step.isFault = true;
            int pos = -1;
            for (int i = 0; i < f; i++) {
                if (frame[i] == -1) { pos = i; break; }
            }
            if (pos == -1) {
                int minTime = lastUsed[0];
                pos = 0;
                for (int i = 1; i < f; i++) {
                    if (lastUsed[i] < minTime) { minTime = lastUsed[i]; pos = i; }
                }
                step.replacedPage = frame[pos];
            }
            frame[pos] = page;
            lastUsed[pos] = timer;
        }
        step.frame = frame;
        result.push_back(step);
    }
    return result;
}

vector<Step> OPT(vector<int> pages, int f) {
    vector<Step> result;
    vector<int> frame(f, -1);

    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];
        Step step;
        step.currentPage = page;
        step.replacedPage = -1;
        bool found = false;

        for (int x : frame) {
            if (x == page) { found = true; break; }
        }

        if (found) { step.isFault = false; }
        else {
            step.isFault = true;
            int pos = -1;
            for (int j = 0; j < f; j++) {
                if (frame[j] == -1) { pos = j; break; }
            }
            if (pos == -1) {
                int farthest = -1;
                pos = 0;
                for (int j = 0; j < f; j++) {
                    int nextUse = 1e9;
                    for (int k = i + 1; k < pages.size(); k++) {
                        if (frame[j] == pages[k]) { nextUse = k; break; }
                    }
                    if (nextUse > farthest) { farthest = nextUse; pos = j; }
                }
                step.replacedPage = frame[pos];
            }
            frame[pos] = page;
        }
        step.frame = frame;
        result.push_back(step);
    }
    return result;
}

vector<Step> CLOCK(vector<int> pages, int f) {
    vector<Step> result;
    vector<int> frame(f, -1);
    vector<int> ref(f, 0);
    int pointer = 0;

    for (int page : pages) {
        Step step;
        step.currentPage = page;
        step.replacedPage = -1;
        bool found = false;

        for (int i = 0; i < f; i++) {
            if (frame[i] == page) {
                found = true;
                ref[i] = 1;
                break;
            }
        }

        if (found) { step.isFault = false; }
        else {
            step.isFault = true;
            while (true) {
                if (ref[pointer] == 0) {
                    if (frame[pointer] != -1) step.replacedPage = frame[pointer];
                    frame[pointer] = page;
                    ref[pointer] = 1;
                    pointer = (pointer + 1) % f;
                    break;
                }
                else {
                    ref[pointer] = 0;
                    pointer = (pointer + 1) % f;
                }
            }
        }
        step.frame = frame;
        step.refBits = ref;
        result.push_back(step);
    }
    return result;
}

int countFault(vector<Step> rs) {
    int cnt = 0;
    for (Step s : rs) { if (s.isFault) cnt++; }
    return cnt;
}

void printResult(vector<Step> rs) {
    int faultCount = 0;
    int margin = 25;

    printLine(89, margin);
    printSpace(margin);
    setColor(11);
    cout << "|" << setw(10) << "PAGE" << "|" << setw(18) << "STATUS" << "|" << setw(25) << "FRAME" << "|" << setw(15) << "REPLACED" << "|" << setw(15) << "REF BIT" << "|";
    setColor(7);
    cout << "\n";
    printLine(89, margin);

    for (Step s : rs) {
        printSpace(margin);
        cout << "|" << setw(10) << s.currentPage << "|";

        if (s.isFault) { setColor(12); cout << setw(18) << "PAGE FAULT"; faultCount++; }
        else { setColor(10); cout << setw(18) << "PAGE HIT"; }

        setColor(7);
        cout << "|";

        string frameText = "";
        for (int x : s.frame) {
            if (x == -1) frameText += "- ";
            else frameText += to_string(x) + " ";
        }
        cout << setw(25) << frameText << "|";

        if (s.replacedPage == -1) cout << setw(15) << "-";
        else cout << setw(15) << s.replacedPage;
        cout << "|";

        string bitText = "";
        if (s.refBits.empty()) { bitText = "-"; }
        else {
            for (int b : s.refBits) bitText += to_string(b) + " ";
        }
        cout << setw(15) << bitText << "|";
        setColor(7);
        cout << "\n";
    }
    printLine(89, margin);
    cout << "\n";
    printSpace(margin);
    setColor(13);
    cout << "Tong so Page Fault: " << faultCount << "\n";
    setColor(7);
}

void printSimulation(vector<Step> rs, string name, int f) {
    int margin = 25;
    cout << "\n";
    printLine(90, margin);
    setColor(11);
    centerText(name, 140);
    cout << "\n";
    setColor(7);
    printLine(90, margin);
    cout << "\n";

    printSpace(margin);
    cout << setw(8) << "Page ";
    for (int j = 0; j < rs.size(); j++) cout << "+----";
    cout << "+\n";
    printSpace(margin);
    cout << "        ";
    for (Step s : rs) { cout << "|" << setw(4) << s.currentPage; }
    cout << "|\n";
    printSpace(margin);
    cout << "        ";
    for (int j = 0; j < rs.size(); j++) cout << "+----";
    cout << "+\n\n";

    for (int i = 0; i < f; i++) {
        printSpace(margin);
        cout << "Frame " << i + 1 << " ";
        for (int j = 0; j < rs.size(); j++) cout << "+----";
        cout << "+\n";
        printSpace(margin);
        cout << "        ";
        for (Step s : rs) {
            cout << "|";
            if (s.frame[i] == -1) cout << setw(4) << "-";
            else cout << setw(4) << s.frame[i];
        }
        cout << "|\n";
        printSpace(margin);
        cout << "        ";
        for (int j = 0; j < rs.size(); j++) cout << "+----";
        cout << "+\n";
    }
    cout << "\n";
    printSpace(margin);
    cout << setw(8) << "Fault ";
    for (int j = 0; j < rs.size(); j++) cout << "+----";
    cout << "+\n";
    printSpace(margin);
    cout << "        ";
    for (Step s : rs) {
        cout << "|";
        if (s.isFault) { setColor(12); cout << setw(4) << "F"; setColor(7); }
        else { setColor(10); cout << setw(4) << "-"; setColor(7); }
    }
    cout << "|\n";
    printSpace(margin);
    cout << "        ";
    for (int j = 0; j < rs.size(); j++) cout << "+----";
    cout << "+\n\n";
}

void compareAlgorithms(vector<Step> fifoResult, vector<Step> lruResult, vector<Step> optResult, vector<Step> clockResult) {
    int margin = 52;
    cout << "\n";
    printLine(35, margin);
    setColor(11);
    centerText("SO SANH PAGE FAULT", 140);
    setColor(7);
    cout << "\n";
    printLine(35, margin);
    cout << "\n";
    printSpace(50);
    cout << "+----------------------+------------+\n";
    printSpace(50);
    setColor(14);
    cout << "|" << setw(22) << "THUAT TOAN" << "|" << setw(12) << "FAULT" << "|\n";
    setColor(7);
    printSpace(50);
    cout << "+----------------------+------------+\n";
    printSpace(50);
    cout << "|" << setw(22) << "FIFO" << "|" << setw(12) << countFault(fifoResult) << "|\n";
    printSpace(50);
    cout << "|" << setw(22) << "LRU" << "|" << setw(12) << countFault(lruResult) << "|\n";
    printSpace(50);
    cout << "|" << setw(22) << "OPT" << "|" << setw(12) << countFault(optResult) << "|\n";
    printSpace(50);
    cout << "|" << setw(22) << "CLOCK" << "|" << setw(12) << countFault(clockResult) << "|\n";
    printSpace(50);
    cout << "+----------------------+------------+\n";
}

void pageReplacementMenu() {
   
    system("mode con cols=140 lines=45");

    int choice = 0;
    char key;

    while (true) {
        system("cls");

        int n;
        setColor(14);
        cout << "Nhap so luong trang: ";
        setColor(7);
        cin >> n;

        vector<int> pages(n);
        setColor(11);
        cout << "Nhap chuoi tham chieu:\n";
        setColor(7);
        for (int i = 0; i < n; i++) {
            cin >> pages[i];
        }

        int f;
        setColor(10);
        cout << "Nhap so frame: ";
        setColor(7);
        cin >> f;

        while (true) {
            system("cls");
            showMenu(choice);
            key = _getch();

            if (key == 224 || key == 0) {
                key = _getch();
            }
            if (key == 72) { 
                choice = (choice - 1 + 6) % 6;
            }
            else if (key == 80) { 
                choice = (choice + 1) % 6;
            }
            else if (key == 13) { 
                break;
            }
        }

        system("cls");
        
        switch (choice) {
        case 0: {
            vector<Step> rs = FIFO(pages, f);
            printSimulation(rs, "FIFO ALGORITHM", f);

            cout << "\n";
            setColor(14);
            printLine(40, 50);
            centerText("BANG CHI TIET", 140);
            cout << "\n";
            printLine(40, 50);
            setColor(7);
            cout << "\n";

            printResult(rs);
            break;
        }
        case 1: {
            vector<Step> rs = LRU(pages, f);
            printSimulation(rs, "LRU ALGORITHM", f);

            cout << "\n";
            setColor(14);
            printLine(40, 50);
            centerText("BANG CHI TIET", 140);
            cout << "\n";
            printLine(40, 50);
            setColor(7);
            cout << "\n";

            printResult(rs);
            break;
        }
        case 2: {
            vector<Step> rs = OPT(pages, f);
            printSimulation(rs, "OPT ALGORITHM", f);

            cout << "\n";
            setColor(14);
            printLine(40, 50);
            centerText("BANG CHI TIET", 140);
            cout << "\n";
            printLine(40, 50);
            setColor(7);
            cout << "\n";

            printResult(rs);
            break;
        }
        case 3: {
            vector<Step> rs = CLOCK(pages, f);
            printSimulation(rs, "CLOCK ALGORITHM", f);

            cout << "\n";
            setColor(14);
            printLine(40, 50);
            centerText("BANG CHI TIET", 140);
            cout << "\n";
            printLine(40, 50);
            setColor(7);
            cout << "\n";

            printResult(rs);
            break;
        }
        case 4: {
            vector<Step> fR = FIFO(pages, f);
            vector<Step> lR = LRU(pages, f);
            vector<Step> oR = OPT(pages, f);
            vector<Step> cR = CLOCK(pages, f);

            printSimulation(fR, "FIFO", f);
            cout << "\n";
            setColor(14);
            printLine(40, 50);
            centerText("BANG CHI TIET FIFO", 140);
            cout << "\n";
            printLine(40, 50);
            setColor(7);
            cout << "\n";
            printResult(fR);

            printSimulation(lR, "LRU", f);
            cout << "\n";
            setColor(14);
            printLine(40, 50);
            centerText("BANG CHI TIET LRU", 140);
            cout << "\n";
            printLine(40, 50);
            setColor(7);
            cout << "\n";
            printResult(lR);

            printSimulation(oR, "OPT", f);
            cout << "\n";
            setColor(14);
            printLine(40, 50);
            centerText("BANG CHI TIET OPT", 140);
            cout << "\n";
            printLine(40, 50);
            setColor(7);
            cout << "\n";
            printResult(oR);

            printSimulation(cR, "CLOCK", f);
            cout << "\n";
            setColor(14);
            printLine(40, 50);
            centerText("BANG CHI TIET CLOCK", 140);
            cout << "\n";
            printLine(40, 50);
            setColor(7);
            cout << "\n";
            printResult(cR);

            compareAlgorithms(fR, lR, oR, cR);
            break;
        }
        case 5: {
            setColor(12);
            cout << "\nThoat module Page Replacement\n";
            setColor(7);
            
            return;
        }
        }

        cout << "\n";
        system("pause");

        char again;
        cout << "\nNhap testcase moi? (y/n): ";
        cin >> again;

        if (again == 'y' || again == 'Y') {
            choice = 0;
            continue; 
        }
        else {
            break;
        }
    }
}