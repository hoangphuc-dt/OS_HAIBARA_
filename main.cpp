#include <iostream>
#include <vector>
#include <Windows.h>      // Thư viện để hỗ trợ tiếng Việt trên Console
#include "cpu_scheduler.h" // PHẢI CÓ để kết nối với module CPU

using namespace std;

int main() {
    // Thiết lập hiển thị tiếng Việt có dấu trên Console
    SetConsoleOutputCP(65001);

    int choice;
    while (true) {
        system("cls"); // Xóa màn hình cho sạch mỗi lần chọn lại
        cout << "===============================================\n";
        cout << "      DO AN HE DIEU HANH - NHOM HAIBARA        \n";
        cout << "===============================================\n";
        cout << "1. Dieu phoi CPU \n";
        cout << "2. Xu ly Deadlock \n";
        cout << "3. Thay the trang OPT \n";
        cout << "0. Thoat chuong trinh\n";
        cout << "===============================================\n";
        cout << "Lua chon cua ban: ";
        cin >> choice;

        if (choice == 0) break;

        switch (choice) {
        case 1: {
            // ĐOẠN NÀY LÀ LOGIC MAIN CŨ CỦA BẠN ĐƯỢC ĐƯA VÀO ĐÂY
            system("cls");
            cout << "--- TRINH MO PHONG DIEU PHOI CPU ---\n";
            cout << "  1. FCFS\n";
            cout << "  2. SJF (Non-Preemptive)\n";
            cout << "  3. SRTF (Preemptive)\n";
            cout << "  4. Priority (Non-Preemptive)\n";
            cout << "  5. Priority (Preemptive)\n";
            cout << "  6. Round Robin (RR)\n";
            cout << "Chon thuat toan: ";

            int cpuChoice;
            cin >> cpuChoice;

            bool hasPriority = (cpuChoice == 4 || cpuChoice == 5);

            // Gọi hàm nhập liệu từ file cpu_scheduler.cpp
            vector<Process> procs = readProcesses(hasPriority);

            if (!procs.empty()) {
                switch (cpuChoice) {
                case 1: runFCFS(procs); break;
                case 2: runSJF(procs); break;
                case 3: runSRTF(procs); break;
                case 4: runPriorityNP(procs); break;
                case 5: runPriorityP(procs); break;
                case 6: {
                    int q;
                    cout << "Time Quantum: "; cin >> q;
                    if (q > 0) runRoundRobin(procs, q);
                    break;
                }
                }
            }
            break;
        }
        case 2:
            cout << "\n[!] Module Deadlock dang duoc xay dung...\n";
            break;
        case 3:
            cout << "\n[!] Module Thay the trang OPT dang duoc xay dung...\n";
            break;
        default:
            cout << "\nLua chon khong hop le!\n";
        }

        cout << "\n";
        system("pause"); // Dừng màn hình để xem kết quả trước khi quay lại menu
    }

    return 0;
}