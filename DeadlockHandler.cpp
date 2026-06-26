#include "DeadlockHandler.h"


void centerTextBanker(string s, int width) {
    int left = (width - (int)s.size()) / 2;
    if (left > 0) {
        for (int i = 0; i < left; i++) cout << " ";
    }
    cout << s;
}

void showBanker(int choice) {
    vector<string> menu = {
        "                [1] Safety                ",
        "                [2] Request               ",
        "                [3] Deadlock Detection    ",
        "                [0] Exit                  "
    };
    vector<int> width = { 140, 140, 140, 140 };

    setColor(11); 
    cout << "\n";
    centerTextBanker("================================================================================", 140);
    cout << "\n";
    centerTextBanker("BANKER'S ALGORITHM", 140);
    cout << "\n";
    centerTextBanker("================================================================================", 140);
    cout << "\n\n";

    for (int i = 0; i < (int)menu.size(); i++) {
        if (i == choice) setColor(14); 
        else setColor(10); 
        centerTextBanker(menu[i], width[i]);
        cout << "\n\n";
    }

    setColor(11);
    centerTextBanker("================================================================================", 140);
    cout << "\n\n";
    setColor(13); 
    centerTextBanker("      Use UP / DOWN arrow keys and ENTER to select", 140);
    cout << "\n";
    setColor(15); 
}

void clearInput() {
    cin.clear();
    cin.ignore(1000, '\n');
}

void inputNM(int& n, int& m) {
    do {
        setColor(YELLOW);
        cout << "\nNhap so tien trinh: ";
        setColor(WHITE);
        if (!(cin >> n)) {
            setColor(RED); cout << "Du lieu nhap phai la so nguyen!\n"; setColor(WHITE);
            clearInput(); n = -1; continue;
        }

        if (n <= 0) {
            setColor(RED); cout << "So tien trinh phai lon hon 0\n"; setColor(WHITE);
            clearInput();
        }
        else if (n > MAX_PROCESS) {
            setColor(RED); cout << "So tien trinh vuot qua gioi han cho phep (" << MAX_PROCESS << ")\n"; setColor(WHITE);
            clearInput();
        }
    } while (n <= 0 || n > MAX_PROCESS);

    do {
        setColor(YELLOW);
        cout << "Nhap so tai nguyen: ";
        setColor(WHITE);
        if (!(cin >> m)) {
            setColor(RED); cout << "Du lieu nhap phai la so nguyen!\n"; setColor(WHITE);
            clearInput(); m = -1; continue;
        }

        if (m <= 0) {
            setColor(RED); cout << "So tai nguyen phai lon hon 0\n"; setColor(WHITE);
            clearInput();
        }
        else if (m > MAX_RESOURCE) {
            setColor(RED); cout << "So tai nguyen vuot qua gioi han cho phep (" << MAX_RESOURCE << ")\n"; setColor(WHITE);
            clearInput();
        }
    } while (m <= 0 || m > MAX_RESOURCE);
}

void inputAllocation(vector<vector<int>>& allocation, int n, int m) {
    bool error;
    do {
        error = false;
        setColor(YELLOW);
        cout << "\nNhap ma tran Allocation:\n";
        setColor(WHITE);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (!(cin >> allocation[i][j]) || allocation[i][j] < 0) {
                    error = true;
                }
            }
        }

        if (error) {
            setColor(RED);
            cout << "\nAllocation khong hop le (khong duoc chua chu hoac so am). Nhap lai toan bo Allocation!\n";
            setColor(WHITE);
            clearInput();
        }
    } while (error);
}

void inputMax(vector<vector<int>>& maxMatrix, vector<vector<int>>& allocation, int n, int m) {
    bool error;
    do {
        error = false;
        setColor(YELLOW);
        cout << "\nNhap ma tran Max:\n";
        setColor(WHITE);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (!(cin >> maxMatrix[i][j]) || maxMatrix[i][j] < 0 || maxMatrix[i][j] < allocation[i][j]) {
                    error = true;
                }
            }
        }

        if (error) {
            setColor(RED);
            cout << "\nMax khong hop le (khong duoc am va phai >= Allocation). Nhap lai toan bo Max!\n";
            setColor(WHITE);
            clearInput();
        }
    } while (error);
}

void inputInstance(vector<int>& instance, vector<vector<int>>& allocation, int n, int m) {
    bool error;
    do {
        error = false;
        setColor(YELLOW);
        cout << "\nNhap vector Instance:\n";
        setColor(WHITE);

        for (int j = 0; j < m; j++) {
            if (!(cin >> instance[j]) || instance[j] < 0) {
                error = true;
            }

            int sumAllocation = 0;
            for (int i = 0; i < n; i++) {
                sumAllocation += allocation[i][j];
            }

            if (instance[j] < sumAllocation) {
                error = true;
            }
        }

        if (error) {
            setColor(RED);
            cout << "\nInstance khong hop le (khong duoc chua chu, khong duoc am va phai >= tong Allocation hien tai)!\n";
            cout << "Nhap lai Instance!\n";
            setColor(WHITE);
            clearInput();
        }
    } while (error);
}

void inputRequest(vector<vector<int>>& request, int n, int m) {
    bool error;
    do {
        error = false;
        setColor(YELLOW);
        cout << "\nNhap ma tran Request:\n";
        setColor(WHITE);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (!(cin >> request[i][j]) || request[i][j] < 0)
                    error = true;
            }
        }

        if (error) {
            setColor(RED);
            cout << "Request khong hop le (khong am, khong chu). Nhap lai Request!\n";
            setColor(WHITE);
            clearInput();
        }
    } while (error);
}


void printSafetyTable(
    int n, int m,
    vector<vector<int>> allocation,
    vector<vector<int>> maxMatrix,
    vector<vector<int>> need,
    vector<int> available,
    int highlightProcess)
{
    const int BLOCK = 18;
    cout << "\n===================== BANG DU LIEU =====================\n\n";
    setColor(CYAN);
    cout << left << setw(10) << "Process";
    cout << left << setw(BLOCK) << "Allocation";
    cout << left << setw(BLOCK) << "Max";
    cout << left << setw(BLOCK) << "Available";
    cout << left << setw(BLOCK) << "Need";
    cout << endl;
    setColor(WHITE);
    cout << setw(10) << "";
    for (int k = 0; k < 4; k++) {
        for (int j = 0; j < m; j++) {
            setColor(PINK);
            cout << (char)('A' + j) << " ";
            setColor(WHITE);
        }
        cout << setw(BLOCK - m * 2) << "";
    }
    cout << endl;
    for (int i = 0; i < n; i++) {
        setColor(CYAN);
        cout << left << setw(10) << ("P" + to_string(i));

        if (i == highlightProcess)
            setColor(GREEN);
        else
            setColor(WHITE);

        for (int j = 0; j < m; j++) cout << allocation[i][j] << " ";
        cout << setw(BLOCK - m * 2) << "";
        for (int j = 0; j < m; j++) cout << maxMatrix[i][j] << " ";
        cout << setw(BLOCK - m * 2) << "";
        if (i == 0) {
            for (int j = 0; j < m; j++) cout << available[j] << " ";
        }
        else {
            for (int j = 0; j < m; j++) cout << "  ";
        }
        cout << setw(BLOCK - m * 2) << "";
        for (int j = 0; j < m; j++) cout << need[i][j] << " ";
        cout << endl;

        setColor(WHITE);
    }
    cout << endl;
}

void printDetectionTable(
    int n, int m,
    vector<vector<int>> allocation,
    vector<vector<int>> request,
    vector<int> available)
{
    const int BLOCK = 18;
    cout << "\n===================== BANG DU LIEU =====================\n\n";
    setColor(CYAN);
    cout << left << setw(10) << "Process";
    cout << left << setw(BLOCK) << "Allocation";
    cout << left << setw(BLOCK) << "Request";
    cout << left << setw(BLOCK) << "Available";
    cout << endl;
    setColor(WHITE);
    cout << setw(10) << "";
    for (int k = 0; k < 3; k++) {
        for (int j = 0; j < m; j++) {
            setColor(PINK);
            cout << (char)('A' + j) << " ";
            setColor(WHITE);
        }
        cout << setw(BLOCK - m * 2) << "";
    }
    cout << endl;
    for (int i = 0; i < n; i++) {
        setColor(CYAN);
        cout << left << setw(10) << ("P" + to_string(i));
        setColor(WHITE);
        for (int j = 0; j < m; j++) cout << allocation[i][j] << " ";
        cout << setw(BLOCK - m * 2) << "";
        for (int j = 0; j < m; j++) cout << request[i][j] << " ";
        cout << setw(BLOCK - m * 2) << "";
        if (i == 0) {
            for (int j = 0; j < m; j++) cout << available[j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

bool isSafety(
    int n, int m,
    vector<vector<int>> allocation,
    vector<vector<int>> need,
    vector<int> available,
    vector<int>& safeSequence)
{
    vector<int> work = available;
    vector<bool> finish(n, false);
    int count = 0;
    while (count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                bool possible = true;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) { possible = false; break; }
                }
                if (possible) {
                    for (int j = 0; j < m; j++) work[j] += allocation[i][j];
                    safeSequence.push_back(i);
                    finish[i] = true;
                    found = true;
                    count++;
                }
            }
        }
        if (!found) return false;
    }
    return true;
}


void safetyAlgorithm() {
    int n, m;
    inputNM(n, m);

    vector<vector<int>> allocation(n, vector<int>(m));
    vector<vector<int>> maxMatrix(n, vector<int>(m));
    vector<vector<int>> need(n, vector<int>(m));
    vector<int> available(m);
    vector<int> instance(m);

    inputAllocation(allocation, n, m);
    inputMax(maxMatrix, allocation, n, m);
    inputInstance(instance, allocation, n, m);

    
    for (int j = 0; j < m; j++) {
        int sumAllocation = 0;
        for (int i = 0; i < n; i++) {
            sumAllocation += allocation[i][j];
        }
        available[j] = instance[j] - sumAllocation;
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = maxMatrix[i][j] - allocation[i][j];

    printSafetyTable(n, m, allocation, maxMatrix, need, available);

    vector<int> safeSequence;
    if (isSafety(n, m, allocation, need, available, safeSequence)) {
        setColor(GREEN); cout << "He thong an toan\nChuoi an toan: "; setColor(WHITE);
        for (size_t i = 0; i < safeSequence.size(); i++) {
            cout << "P" << safeSequence[i];
            if (i != safeSequence.size() - 1) cout << " -> ";
        }
        cout << endl;
    }
    else {
        setColor(RED); cout << "He thong khong an toan\n"; setColor(WHITE);
    }
}


void requestAlgorithm() {
    int n, m;
    inputNM(n, m);

    vector<vector<int>> allocation(n, vector<int>(m));
    vector<vector<int>> maxMatrix(n, vector<int>(m));
    vector<vector<int>> need(n, vector<int>(m));
    vector<int> available(m);
    vector<int> instance(m);

    inputAllocation(allocation, n, m);
    inputMax(maxMatrix, allocation, n, m);
    inputInstance(instance, allocation, n, m);

    
    for (int j = 0; j < m; j++) {
        int sumAllocation = 0;
        for (int i = 0; i < n; i++) {
            sumAllocation += allocation[i][j];
        }
        available[j] = instance[j] - sumAllocation;
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = maxMatrix[i][j] - allocation[i][j];

    printSafetyTable(n, m, allocation, maxMatrix, need, available);

    int p;
    do {
        setColor(YELLOW); cout << "\nNhap tien trinh yeu cau (Pi): "; setColor(WHITE);
        if (!(cin >> p)) {
            setColor(RED); cout << "Vui long nhap so nguyen!\n"; setColor(WHITE);
            clearInput(); p = -1; continue;
        }
        if (p < 0 || p >= n) {
            setColor(RED); cout << "Tien trinh khong ton tai! Vui long nhap tu 0 den " << n - 1 << endl; setColor(WHITE);
        }
    } while (p < 0 || p >= n);

    vector<int> request(m);
    bool error;
    do {
        error = false;
        setColor(YELLOW); cout << "\nNhap vector Request cho P" << p << ":\n"; setColor(WHITE);
        for (int j = 0; j < m; j++) {
            if (!(cin >> request[j]) || request[j] < 0) error = true;
        }
        if (error) {
            setColor(RED); cout << "Request khong hop le (khong am, khong chu). Nhap lai Request!\n"; setColor(WHITE);
            clearInput();
        }
    } while (error);

    for (int j = 0; j < m; j++) {
        if (request[j] > need[p][j]) {
            setColor(RED); cout << "\nVuot qua yeu cau toi da (Request > Need)\n"; setColor(WHITE);
            return;
        }
    }
    for (int j = 0; j < m; j++) {
        if (request[j] > available[j]) {
            setColor(RED); cout << "\nTai nguyen khong du de cap phat (Request > Available)\n"; setColor(WHITE);
            return;
        }
    }

    for (int j = 0; j < m; j++) {
        available[j] -= request[j];
        allocation[p][j] += request[j];
        need[p][j] -= request[j];
    }
    cout << "\n===== SAU KHI CAP PHAT TAM =====\n";
    printSafetyTable(n, m, allocation, maxMatrix, need, available, p);

    vector<int> safeSequence;
    if (isSafety(n, m, allocation, need, available, safeSequence)) {
        setColor(GREEN); cout << "He thong an toan\nRequest duoc chap nhan\n"; setColor(WHITE);
    }
    else {
        for (int j = 0; j < m; j++) {
            available[j] += request[j];
            allocation[p][j] -= request[j];
            need[p][j] += request[j];
        }
        setColor(RED); cout << "He thong khong an toan\nRequest khong duoc chap nhan, khoi phuc trang thai cu\n"; setColor(WHITE);
    }
}


void deadlockDetection() {
    int n, m;
    inputNM(n, m);

    vector<vector<int>> allocation(n, vector<int>(m));
    vector<vector<int>> request(n, vector<int>(m));
    vector<int> available(m);
    vector<int> instance(m);

    inputAllocation(allocation, n, m);
    inputRequest(request, n, m);
    inputInstance(instance, allocation, n, m);

    
    for (int j = 0; j < m; j++) {
        int sumAllocation = 0;
        for (int i = 0; i < n; i++) {
            sumAllocation += allocation[i][j];
        }
        available[j] = instance[j] - sumAllocation;
    }

    printDetectionTable(n, m, allocation, request, available);
    vector<int> work = available;
    vector<bool> finish(n, false);

    for (int i = 0; i < n; i++) {
        bool zero = true;
        for (int j = 0; j < m; j++) {
            if (allocation[i][j] != 0) { zero = false; break; }
        }
        if (zero) finish[i] = true;
    }

    bool found = true;
    while (found) {
        found = false;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                bool possible = true;
                for (int j = 0; j < m; j++) {
                    if (request[i][j] > work[j]) { possible = false; break; }
                }
                if (possible) {
                    for (int j = 0; j < m; j++) work[j] += allocation[i][j];
                    finish[i] = true;
                    found = true;
                }
            }
        }
    }

    vector<int> deadlockProcesses;
    for (int i = 0; i < n; i++) { if (!finish[i]) deadlockProcesses.push_back(i); }

    if (deadlockProcesses.empty()) {
        setColor(GREEN); cout << "He thong khong bi deadlock\n"; setColor(WHITE);
    }
    else {
        setColor(RED); cout << "He thong bi deadlock\nDanh sach tien trinh bi deadlock: "; setColor(WHITE);
        for (size_t i = 0; i < deadlockProcesses.size(); i++) {
            cout << "P" << deadlockProcesses[i];
            if (i != deadlockProcesses.size() - 1) cout << ", ";
        }
        cout << endl;
    }
}


void showBankerMenu() {
    int choice = 0; char key;
    while (true) {
        for (;;) {
            system("cls");
            showBanker(choice);

            key = _getch();
            if (key == 224 || key == 0) key = _getch();
            if (key == 72) choice = (choice - 1 + 4) % 4; 
            else if (key == 80) choice = (choice + 1) % 4; 
            else if (key == 13) break; 
        }
        system("cls");

        if (choice == 3) {
            setColor(12);
            cout << "Thoat chuong trinh\n";
            setColor(15);
            break;
        }

        switch (choice) {
        case 0: safetyAlgorithm(); break;
        case 1: requestAlgorithm(); break;
        case 2: deadlockDetection(); break;
        }

        cout << "\n";
        setColor(10);
        system("pause");
        setColor(7);
    }
}