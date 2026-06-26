#include "PageReplacement.h"


bool frameDaDay(vector<int> frame) {
    for (int x : frame) {
        if (x == -1)
            return false;
    }
    return true;
}


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
    int left = (width - (int)s.size()) / 2;
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

    setColor(CYAN);
    cout << "\n";
    centerText("================================================================================", 140);
    cout << "\n";
    centerText("PAGE REPLACEMENT SIMULATOR", 140);
    cout << "\n";
    centerText("================================================================================", 140);
    cout << "\n\n";

    for (int i = 0; i < (int)menu.size(); i++) {
        if (i == choice)
            setColor(YELLOW);
        else
            setColor(GREEN);
        centerText(menu[i], width[i]);
        cout << "\n\n";
    }

    setColor(CYAN);
    centerText("================================================================================", 140);
    cout << "\n\n";
    setColor(PINK);
    centerText("DUNG PHIM MUI TEN LEN/XUONG VA ENTER DE CHON", 140);
    cout << "\n";
    setColor(WHITE);
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
            if (x == page) {
                found = true;
                break;
            }
        }

        if (found) {
            step.isFault = false;
        }
        else {
            bool day = frameDaDay(frame);
            if (day) {
                step.isFault = true;
                step.replacedPage = frame[pointer];
                frame[pointer] = page;
                pointer = (pointer + 1) % f;
            }
            else {
                step.isFault = false;
                while (frame[pointer] != -1)
                    pointer = (pointer + 1) % f;

                frame[pointer] = page;
                pointer = (pointer + 1) % f;
            }
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

        if (found) {
            step.isFault = false;
        }
        else {
            bool day = frameDaDay(frame);
            if (day)
                step.isFault = true;
            else
                step.isFault = false;

            int pos = -1;
            for (int i = 0; i < f; i++) {
                if (frame[i] == -1) {
                    pos = i;
                    break;
                }
            }

            if (pos == -1) {
                int minTime = lastUsed[0];
                pos = 0;
                for (int i = 1; i < f; i++) {
                    if (lastUsed[i] < minTime) {
                        minTime = lastUsed[i];
                        pos = i;
                    }
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
    for (int i = 0; i < (int)pages.size(); i++) {
        int page = pages[i];
        Step step;
        step.currentPage = page;
        step.replacedPage = -1;
        bool found = false;
        for (int x : frame) {
            if (x == page) {
                found = true;
                break;
            }
        }

        if (found) {
            step.isFault = false;
        }
        else {
            bool day = frameDaDay(frame);
            if (day)
                step.isFault = true;
            else
                step.isFault = false;

            int pos = -1;
            for (int j = 0; j < f; j++) {
                if (frame[j] == -1) {
                    pos = j;
                    break;
                }
            }

            if (pos == -1) {
                int farthest = -1;
                pos = 0;

                for (int j = 0; j < f; j++) {
                    int nextUse = 1e9;
                    for (int k = i + 1; k < (int)pages.size(); k++) {
                        if (frame[j] == pages[k]) {
                            nextUse = k;
                            break;
                        }
                    }

                    if (nextUse > farthest) {
                        farthest = nextUse;
                        pos = j;
                    }
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

        if (found) {
            step.isFault = false;
        }
        else {
            bool day = frameDaDay(frame);
            if (day) {
                step.isFault = true;
                while (true) {
                    if (ref[pointer] == 0) {
                        step.replacedPage = frame[pointer];
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
            else {
                step.isFault = false;
                while (frame[pointer] != -1)
                    pointer = (pointer + 1) % f;
                frame[pointer] = page;
                ref[pointer] = 1;
                pointer = (pointer + 1) % f;
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
    for (Step s : rs) {
        if (s.isFault) cnt++;
    }
    return cnt;
}


void printResult(vector<Step> rs) {
    int faultCount = 0;
    int margin = 25;
    printLine(89, margin);
    printSpace(margin);
    setColor(CYAN);
    cout << "|" << setw(10) << "PAGE" << "|" << setw(18) << "STATUS" << "|" << setw(25) << "FRAME" << "|" << setw(15) << "REPLACED" << "|" << setw(15) << "USE BIT" << "|";
    setColor(WHITE);
    cout << "\n";

    printLine(89, margin);
    for (Step s : rs) {
        printSpace(margin);
        cout << "|" << setw(10) << s.currentPage << "|";
        if (s.isFault) {
            setColor(RED);
            cout << setw(18) << "PAGE FAULT";
            faultCount++;
        }
        else {
            cout << setw(18) << "";
        }

        setColor(WHITE);
        cout << "|";
        string frameText = "";
        for (int x : s.frame) {
            if (x == -1)
                frameText += "- ";
            else
                frameText += to_string(x) + " ";
        }

        cout << setw(25) << frameText << "|";

        if (s.replacedPage == -1)
            cout << setw(15) << "-";
        else
            cout << setw(15) << s.replacedPage;

        cout << "|";

        string bitText = "";
        if (s.refBits.empty()) {
            bitText = "-";
        }
        else {
            for (int b : s.refBits)
                bitText += to_string(b) + " ";
        }

        cout << setw(15) << bitText << "|";
        setColor(WHITE);
        cout << "\n";
    }

    printLine(89, margin);
    cout << "\n";
    printSpace(margin);
    setColor(PINK);
    cout << "Tong so Page Fault: " << faultCount << "\n";
    setColor(WHITE);
}


void printSimulation(vector<Step> rs, string name, int f) {
    int margin = 25;
    cout << "\n";
    printLine(90, margin);
    setColor(CYAN);
    centerText(name, 140);
    cout << "\n";
    setColor(WHITE);
    printLine(90, margin);
    cout << "\n";

    printSpace(margin);
    cout << setw(8) << "Page ";
    for (size_t j = 0; j < rs.size(); j++)
        cout << "+----";
    cout << "+\n";

    printSpace(margin);
    cout << "        ";
    for (Step s : rs) {
        cout << "|" << setw(4) << s.currentPage;
    }
    cout << "|\n";

    printSpace(margin);
    cout << "        ";
    for (size_t j = 0; j < rs.size(); j++)
        cout << "+----";
    cout << "+\n\n";

    for (int i = 0; i < f; i++) {
        printSpace(margin);
        cout << "Frame " << i + 1 << " ";
        for (size_t j = 0; j < rs.size(); j++)
            cout << "+----";
        cout << "+\n";

        printSpace(margin);
        cout << "        ";
        for (Step s : rs) {
            cout << "|";
            if (s.frame[i] == -1)
                cout << setw(4) << "-";
            else
                cout << setw(4) << s.frame[i];
        }
        cout << "|\n";

        printSpace(margin);
        cout << "        ";
        for (size_t j = 0; j < rs.size(); j++)
            cout << "+----";
        cout << "+\n";
    }

    cout << "\n";
    printSpace(margin);
    cout << setw(8) << "Fault ";
    for (size_t j = 0; j < rs.size(); j++)
        cout << "+----";
    cout << "+\n";

    printSpace(margin);
    cout << "        ";
    for (Step s : rs) {
        cout << "|";
        if (s.isFault) {
            setColor(RED);
            cout << setw(4) << "F";
            setColor(WHITE);
        }
        else {
            cout << setw(4) << "";
        }
    }
    cout << "|\n";

    printSpace(margin);
    cout << "        ";
    for (size_t j = 0; j < rs.size(); j++)
        cout << "+----";
    cout << "+\n\n";
}


void compareAlgorithms(vector<Step> fifoResult, vector<Step> lruResult, vector<Step> optResult, vector<Step> clockResult) {
    int margin = 52;
    cout << "\n";
    printLine(35, margin);
    setColor(CYAN);
    centerText("SO SANH PAGE FAULT", 140);
    setColor(WHITE);
    cout << "\n";
    printLine(35, margin);
    cout << "\n";
    printSpace(50);
    cout << "+----------------------+------------+\n";
    printSpace(50);
    setColor(YELLOW);
    cout << "|" << setw(22) << "THUAT TOAN" << "|" << setw(12) << "FAULT" << "|\n";
    setColor(WHITE);
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


void printInputInfo(vector<int> pages, int f) {
    setColor(CYAN);
    printLine(60, 40);
    centerText("THONG TIN DAU VAO", 140);
    cout << "\n";
    printLine(60, 40);
    setColor(WHITE);
    cout << "\n";
    printSpace(35);
    cout << "So frame: " << f << "\n\n";
    printSpace(35);
    cout << "Chuoi tham chieu: ";
    for (int x : pages)
        cout << x << " ";
    cout << "\n\n";
}

void runAlgorithmInternal(int choice, vector<int>& pages, int f) {
    switch (choice) {
    case 0: {
        vector<Step> rs = FIFO(pages, f);
        printSimulation(rs, "FIFO ALGORITHM", f);
        cout << "\n";
        setColor(YELLOW);
        printLine(40, 50);
        centerText("BANG CHI TIET", 140);
        cout << "\n";
        printLine(40, 50);
        setColor(WHITE);
        cout << "\n";
        printResult(rs);
        break;
    }
    case 1: {
        vector<Step> rs = LRU(pages, f);
        printInputInfo(pages, f);
        printSimulation(rs, "LRU ALGORITHM", f);
        cout << "\n";
        setColor(YELLOW);
        printLine(40, 50);
        centerText("BANG CHI TIET", 140);
        cout << "\n";
        printLine(40, 50);
        setColor(WHITE);
        cout << "\n";
        printResult(rs);
        break;
    }
    case 2: {
        vector<Step> rs = OPT(pages, f);
        printSimulation(rs, "OPT ALGORITHM", f);
        printInputInfo(pages, f);
        cout << "\n";
        setColor(YELLOW);
        printLine(40, 50);
        centerText("BANG CHI TIET", 140);
        cout << "\n";
        printLine(40, 50);
        setColor(WHITE);
        cout << "\n";
        printResult(rs);
        break;
    }
    case 3: {
        vector<Step> rs = CLOCK(pages, f);
        printInputInfo(pages, f);
        printSimulation(rs, "CLOCK ALGORITHM", f);
        cout << "\n";
        setColor(YELLOW);
        printLine(40, 50);
        centerText("BANG CHI TIET", 140);
        cout << "\n";
        printLine(40, 50);
        setColor(WHITE);
        cout << "\n";
        printResult(rs);
        break;
    }
    case 4: {
        vector<Step> fifoResult = FIFO(pages, f);
        vector<Step> lruResult = LRU(pages, f);
        vector<Step> optResult = OPT(pages, f);
        vector<Step> clockResult = CLOCK(pages, f);

        printSimulation(fifoResult, "FIFO ALGORITHM", f);
        cout << "\n";
        setColor(YELLOW);
        printLine(40, 50);
        centerText("BANG CHI TIET FIFO", 140);
        cout << "\n";
        printLine(40, 50);
        setColor(WHITE);
        cout << "\n";
        printResult(fifoResult);

        printSimulation(lruResult, "LRU ALGORITHM", f);
        cout << "\n";
        setColor(YELLOW);
        printLine(40, 50);
        centerText("BANG CHI TIET LRU", 140);
        cout << "\n";
        printLine(40, 50);
        setColor(WHITE);
        cout << "\n";
        printResult(lruResult);

        printSimulation(optResult, "OPT ALGORITHM", f);
        cout << "\n";
        setColor(YELLOW);
        printLine(40, 50);
        centerText("BANG CHI TIET OPT", 140);
        cout << "\n";
        printLine(40, 50);
        setColor(WHITE);
        cout << "\n";
        printResult(optResult);

        printSimulation(clockResult, "CLOCK ALGORITHM", f);
        cout << "\n";
        setColor(YELLOW);
        printLine(40, 50);
        centerText("BANG CHI TIET CLOCK", 140);
        cout << "\n";
        printLine(40, 50);
        setColor(WHITE);
        cout << "\n";
        printResult(clockResult);

        compareAlgorithms(fifoResult, lruResult, optResult, clockResult);
        break;
    }
    }
}


void pageReplacementMenu() {
    int choice = 0;
    char key;
    bool newTest;
    bool changeAlgorithm;

    int n;
    vector<int> pages;
    int f;

INPUT_TESTCASE:
    system("cls");

    setColor(YELLOW);
    cout << "Nhap so luong trang: ";
    setColor(WHITE);
    cin >> n;

    pages.assign(n, 0);

    setColor(CYAN);
    cout << "Nhap chuoi tham chieu:\n";
    setColor(WHITE);

    for (int i = 0; i < n; i++)
        cin >> pages[i];

    setColor(GREEN);
    cout << "Nhap so frame: ";
    setColor(WHITE);
    cin >> f;

CHOOSE_ALGORITHM:
    while (true) {
        system("cls");
        showMenu(choice);

        key = _getch();

        if (key == 224 || key == 0)
            key = _getch();

        if (key == 72) 
            choice = (choice - 1 + 6) % 6;
        else if (key == 80) 
            choice = (choice + 1) % 6;
        else if (key == 13)
            break;
    }

    system("cls");

    
    if (choice == 5) {
        return;
    }

    runAlgorithmInternal(choice, pages, f);

    newTest = false;
    changeAlgorithm = false;

    while (true) {
        int ch;
        cout << "\n";
        setColor(CYAN);
        cout << "[1] Xem lai input\n";
        setColor(YELLOW);
        cout << "[2] Chon thuat toan khac voi cung input\n";
        setColor(GREEN);
        cout << "[3] Nhap testcase moi\n";
        setColor(RED);
        cout << "[0] Quay lai Menu chinh\n";
        setColor(WHITE);
        cout << "\nLua chon: ";
        cin >> ch;

        if (ch == 1) {
            system("cls");
            printInputInfo(pages, f);
            system("pause");
            system("cls");
        }
        else if (ch == 2) {
            changeAlgorithm = true;
            break;
        }
        else if (ch == 3) {
            newTest = true;
            break;
        }
        else if (ch == 0) {
            return; 
        }
    }

    if (newTest)
        goto INPUT_TESTCASE;
    if (changeAlgorithm)
        goto CHOOSE_ALGORITHM;
}