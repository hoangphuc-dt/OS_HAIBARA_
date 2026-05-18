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
        "[2] Banker's Algorithm  ",
        "[3] Page Replacement    ",
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

            showBankerMenu();
        }
        else if (mainChoice == 2) {
            pageReplacementMenu();
        }
        else if (mainChoice == 3) {
            break;
        }
    }
    return 0;
}