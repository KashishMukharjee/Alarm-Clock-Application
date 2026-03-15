#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <windows.h>

struct Alarm {
    int hour;
    int minute;
    int second;
    std::string message;
    time_t trigger_time;

    bool operator>(const Alarm& other) const {
        return trigger_time > other.trigger_time;
    }
};

time_t getFutureTriggerTime(int h, int m, int s) {
    std::time_t now = std::time(nullptr);
    std::tm tm = *std::localtime(&now);
    tm.tm_hour = h;
    tm.tm_min = m;
    tm.tm_sec = s;
    tm.tm_isdst = -1;
    std::time_t target = std::mktime(&tm);
    if (target <= now) {
        target += 24 * 3600;
    }
    return target;
}

void displayTime() {
    std::time_t now = std::time(nullptr);
    std::tm* tm = std::localtime(&now);
    std::cout << "\r" << std::setfill('0') << std::setw(2) << tm->tm_hour << ":"
              << std::setw(2) << tm->tm_min << ":" << std::setw(2) << tm->tm_sec << std::flush;
}

int main() {
    std::priority_queue<Alarm, std::vector<Alarm>, std::greater<Alarm>> alarms;
    char choice;

    std::cout << "Windows Alarm Clock using Priority Queue (DSA)\n";

    do {
        std::cout << "\n1. Add Alarm\n2. Show Alarms\n3. Start Clock\n0. Exit\nChoice: ";
        std::cin >> choice;

        if (choice == '1') {
            int h, m, s;
            std::string msg;
            std::cout << "Enter hour (0-23): ";
            std::cin >> h;
            std::cout << "Enter minute (0-59): ";
            std::cin >> m;
            std::cout << "Enter second (0-59): ";
            std::cin >> s;
            std::cout << "Message: ";
            std::cin.ignore();
            std::getline(std::cin, msg);

            Alarm alarm = {h, m, s, msg, getFutureTriggerTime(h, m, s)};
            alarms.push(alarm);
            std::cout << "Alarm added for " << std::setfill('0') << std::setw(2) << h << ":"
                      << std::setw(2) << m << ":" << std::setw(2) << s << "\n";
        } else if (choice == '2') {
            if (alarms.empty()) {
                std::cout << "No alarms set.\n";
            } else {
                auto temp = alarms;
                std::cout << "Alarms (earliest first):\n";
                while (!temp.empty()) {
                    Alarm a = temp.top(); temp.pop();
                    std::cout << std::setfill('0') << std::setw(2) << a.hour << ":"
                              << std::setw(2) << a.minute << ":" << std::setw(2) << a.second
                              << " - " << a.message << "\n";
                }
            }
        } else if (choice == '3') {
            break;
        }
    } while (choice != '0');

    if (alarms.empty()) {
        std::cout << "No alarms to monitor.\n";
        return 0;
    }

    std::cout << "Clock started. Press Ctrl+C to stop.\n";

    while (true) {
        displayTime();

        if (!alarms.empty()) {
            Alarm next = alarms.top();
            std::time_t now = std::time(nullptr);
            if (now >= next.trigger_time) {
                std::cout << "\n ALARM RINGING! " << next.message << "\n";
                Beep(800, 1000);
                alarms.pop();
            }
        }

        Sleep(1000);
    }

    return 0;
}
