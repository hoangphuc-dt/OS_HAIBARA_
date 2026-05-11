#include "PageReplacement.h"
#include "DeadlockHandler.h"

int main() {
    system("mode con cols=140 lines=45");
    int mainChoice = 0;

    char key;

    while (true) {
        while (true) {
            system("cls");
            setColor(GREEN);
            cout << "\n========== THAY HUY DEP TRAI ==========\n";


            for (int i = 0; i < 3; i++) {
                if (i == mainChoice) {
                    setColor(RED);

                }
                else {
                    setColor(WHITE); // Màu cho các dòng khác

                }

                switch (i) {
                case 0: cout << "1. Page Replacement \n"; break;
                case 1: cout << "2. Banker's Algorithm \n"; break;
                case 2: cout << "0. Exit \n"; break;
                }
            }

            setColor(GREEN);
            cout << "===========================================\n";
            setColor(YELLOW);
            cout << "Dung mui ten LEN/XUONG va ENTER de chon\n";
            setColor(WHITE);

            // Bắt sự kiện phím
            key = _getch();


            if (key == 224 || key == 0)
            {
                key = _getch();
            }


            if (key == 72)
            {
                mainChoice = (mainChoice - 1 + 3) % 3;
            }


            else if (key == 80)
            {
                mainChoice = (mainChoice + 1) % 3;
            }

            else if (key == 13)
            {
                break;
            }
        }

        system("cls");
        if (mainChoice == 0) {
            // Chèn logic main của Page Replacement vào đây
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
        else if (mainChoice == 1) {
            // Chèn logic main của Deadlock vào đây
            int choice = 0; char key;
            while (true) {
                while (true) {
                    system("cls"); setColor(GREEN); cout << "\n==========    Banker's Algorithm  ==========\n";
                    for (int i = 0; i < 4; i++) {
                        if (i == choice) setColor(RED); else setColor(WHITE);
                        if (i == 0) cout << "1. Safety\n"; else if (i == 1) cout << "2. Request\n";
                        else if (i == 2) cout << "3. Deadlock Detection\n"; else cout << "4. Exit\n";
                    }
                    setColor(GREEN);
                    cout << "===========================================\n";

                    setColor(YELLOW);
                    cout << "Dung mui ten LEN/XUONG va ENTER de chon";
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
        else if (mainChoice == 2) break;
    }
    return 0;
}