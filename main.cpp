#include "PageReplacement.h"
#include "DeadlockHandler.h"
#include "CPUScheduler.h"

void centerTextMain(string s, int width) {
    int left = (width - s.size()) / 2;
    for (int i = 0; i < left; i++)
        cout << " ";
    cout << s;
}

void showMainMenu(int choice) {
    vector<string> menu = {
        "[1] CPU Scheduling      ",
        "[2] Page Replacement    ",
        "[3] Banker's Algorithm  ",
        "[0] Exit                "
    };
    vector<int> width = { 140, 140, 140, 140 };

    setColor(10);
    cout << "\n";
    centerTextMain("================================================================================", 140);
    cout << "\n";
    centerTextMain("OS HAIBARA", 140);
    cout << "\n";
    centerTextMain("================================================================================", 140);
    cout << "\n\n";

    for (int i = 0; i < menu.size(); i++) {
        if (i == choice) setColor(12);
        else setColor(15);
        centerTextMain(menu[i], width[i]);
        cout << "\n\n";
    }

    setColor(10);
    centerTextMain("================================================================================", 140);
    cout << "\n\n";
    setColor(14);
    centerTextMain("Use UP / DOWN arrow keys and ENTER to select", 140);
    cout << "\n";
    setColor(7);
}

void showBankerMenu(int choice) {
    // Chèn thêm khoảng trắng để các mục dài bằng nhau (24 ký tự), giúp menu thẳng hàng dọc
    vector<string> menu = {
        "                [1] Safety                ",
        "                [2] Request               ",
        "                [3] Deadlock Detection    ",
        "                [0] Exit                  "
    };
    vector<int> width = { 140, 140, 140, 140 };

    setColor(11);
    cout << "\n";
    centerTextMain("================================================================================", 140);
    cout << "\n";
    centerTextMain("BANKER'S ALGORITHM", 140);
    cout << "\n";
    centerTextMain("================================================================================", 140);
    cout << "\n\n";

    for (int i = 0; i < menu.size(); i++) {
        if (i == choice) setColor(14);
        else setColor(10);
        centerTextMain(menu[i], width[i]);
        cout << "\n\n";
    }

    setColor(11);
    centerTextMain("================================================================================", 140);
    cout << "\n\n";
    setColor(13);
    centerTextMain("     Use UP / DOWN arrow keys and ENTER to select", 140);
    cout << "\n";
    setColor(7);
}

int main() {
    system("mode con cols=140 lines=45");
    int mainChoice = 0;

    char key;

    while (true) {
        while (true) {
            system("cls");

            showMainMenu(mainChoice);

            key = _getch();

            if (key == 224 || key == 0)
            {
                key = _getch();
            }

            if (key == 72)
            {
                mainChoice = (mainChoice - 1 + 4) % 4;
            }
            else if (key == 80)
            {
                mainChoice = (mainChoice + 1) % 4;
            }
            else if (key == 13)
            {
                break;
            }
        }

        system("cls");

        if (mainChoice == 0) {
            cpuSchedulingMenu();
        }
        else if (mainChoice == 1) {
            int n; setColor(14); cout << "Nhap so luong trang: "; setColor(7); cin >> n;
            vector<int> pages(n); setColor(11); cout << "Nhap chuoi tham chieu:\n"; setColor(7);
            for (int i = 0; i < n; i++) cin >> pages[i];
            int f; setColor(10); cout << "Nhap so frame: "; setColor(7); cin >> f;
            int choice = 0; char key;
            while (true) {
                while (true) {
                    system("cls"); showMenu(choice); key = _getch();
                    if (key == 224 || key == 0) key = _getch();
                    if (key == 72) choice = (choice - 1 + 6) % 6;
                    else if (key == 80) choice = (choice + 1) % 6;
                    else if (key == 13) break;
                }
                system("cls");
                if (choice == 5) break; // Quay lai menu chinh
                switch (choice) {
                case 0: { vector<Step> rs = FIFO(pages, f); printSimulation(rs, "FIFO ALGORITHM", f); printResult(rs); break; }
                case 1: { vector<Step> rs = LRU(pages, f); printSimulation(rs, "LRU ALGORITHM", f); printResult(rs); break; }
                case 2: { vector<Step> rs = OPT(pages, f); printSimulation(rs, "OPT ALGORITHM", f); printResult(rs); break; }
                case 3: { vector<Step> rs = CLOCK(pages, f); printSimulation(rs, "CLOCK ALGORITHM", f); printResult(rs); break; }
                case 4: {
                    vector<Step> fR = FIFO(pages, f); vector<Step> lR = LRU(pages, f);
                    vector<Step> oR = OPT(pages, f); vector<Step> cR = CLOCK(pages, f);
                    printSimulation(fR, "FIFO", f); printResult(fR);
                    printSimulation(lR, "LRU", f); printResult(lR);
                    printSimulation(oR, "OPT", f); printResult(oR);
                    printSimulation(cR, "CLOCK", f); printResult(cR);
                    compareAlgorithms(fR, lR, oR, cR); break;
                }
                }
                system("pause");
            }
        }
        else if (mainChoice == 2) {
            int choice = 0; char key;
            while (true) {
                while (true) {
                    system("cls");

                    // Gọi hàm giao diện Banker mới thay vì in thủ công
                    showBankerMenu(choice);

                    key = _getch();
                    if (key == 224 || key == 0) key = _getch();
                    if (key == 72) choice = (choice - 1 + 4) % 4;
                    else if (key == 80) choice = (choice + 1) % 4;
                    else if (key == 13) break;
                }
                system("cls");
                if (choice == 3) break;
                switch (choice) {
                case 0: safetyAlgorithm(); break;
                case 1: requestAlgorithm(); break;
                case 2: deadlockDetection(); break;
                }
                system("pause");
            }
        }
        else if (mainChoice == 3) {
            break;
        }
    }
    return 0;
}