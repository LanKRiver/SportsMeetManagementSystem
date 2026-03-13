#include "event.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// 判断是否为径赛（1-7是径赛，8-10是田赛）
bool isTrackEvent(int evt) {
    return evt >= 1 && evt <= 7;
}

// 分秒毫秒字符串转总秒数（格式：分:秒.毫秒 或 秒.毫秒）
double timeStrToSeconds(string timeStr) {
    int minutes = 0;
    double seconds = 0.0;
    size_t colonPos = timeStr.find(':');
    size_t dotPos = timeStr.find('.');

    if (colonPos != string::npos) {
        // 有分钟（如 1:20.500）
        minutes = stoi(timeStr.substr(0, colonPos));
        if (dotPos != string::npos) {
            // 有毫秒
            double secPart = stod(timeStr.substr(colonPos + 1, dotPos - colonPos - 1));
            double msPart = stod(timeStr.substr(dotPos)) / pow(10, timeStr.length() - dotPos - 1);
            seconds = minutes * 60 + secPart + msPart;
        }
        else {
            // 无毫秒（如 1:20）
            seconds = minutes * 60 + stod(timeStr.substr(colonPos + 1));
        }
    }
    else {
        // 无分钟（如 12.500 或 12）
        seconds = stod(timeStr);
    }
    return seconds;
}

string secondsToTimeStr(double seconds) {
    int minutes = (int)seconds / 60;
    double secRemain = fmod(seconds, 60);
    ostringstream oss;
    if (minutes > 0) {
        oss << minutes << ":" << setfill('0') << setw(2) << (int)secRemain << "."
            << setw(3) << (int)((secRemain - (int)secRemain) * 1000);
    }
    else {
        oss << setfill('0') << setw(2) << (int)secRemain << "."
            << setw(3) << (int)((secRemain - (int)secRemain) * 1000);
    }
    return oss.str();
}

void eventPrint() {
    cout << "径赛（时间越短越好）：1.100米 2.200米 3.400米 4.800米 5.1500米 6.5000米 7.4x100接力" << endl;
    cout << "田赛（距离/高度越大越好）：8.跳高 9.跳远 10.铅球" << endl;
}

void printEvent(int k) {
    switch (k) {
    case 1: cout << "100米（径赛）"; break;
    case 2: cout << "200米（径赛）"; break;
    case 3: cout << "400米（径赛）"; break;
    case 4: cout << "800米（径赛）"; break;
    case 5: cout << "1500米（径赛）"; break;
    case 6: cout << "5000米（径赛）"; break;
    case 7: cout << "4x100接力（径赛）"; break;
    case 8: cout << "跳高（田赛）"; break;
    case 9: cout << "跳远（田赛）"; break;
    case 10: cout << "铅球（田赛）"; break;
    default: break;
    }
}

void showEventInfo() {
    cout << "\n===== 比赛项目一览 =====" << endl;
    eventPrint();
}

void showSchedule() {
    cout << "\n===== 比赛时间安排 =====" << endl;
    cout << "100米、200米：第一天上午\n400米、800米：第一天下午\n1500米、5000米：第二天上午\n"
        << "接力、跳高、跳远、铅球：第二天下午" << endl;
}