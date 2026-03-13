#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

const int N = 20000;
const int MAX_EVENT = 10;

extern int number;

// 判断是否为径赛（1-7是径赛，8-10是田赛）
bool isTrackEvent(int evt);
// 分秒毫秒转总秒数（如 1:20.500 → 80.5）
double timeStrToSeconds(string timeStr);
// 总秒数转分秒毫秒字符串（如 80.5 → 1:20.500）
string secondsToTimeStr(double seconds);

struct sport {
    string name;
    int sex;
    int college;
    int event[3];
    int count;
    int position[3];
    double grade[3];  // 径赛存总秒数，田赛存米数
    int rank;
    int mark;

    sport()
        : name(" "), sex(0), college(0), event{ 0, 0, 0 }, count(0),
        position{ 0, 0, 0 }, grade{ 0.0, 0.0, 0.0 }, rank(0), mark(0)
    {
    }
};

struct sportCopyBoy {
    string cname;
    string csex;
    int ccollege;
    int cevent;
    double cgrade;
    int crank;       // 存储名次
    int cmark;

    sportCopyBoy() : cname(" "), csex("男"), ccollege(0), cevent(0), cgrade(0.0), crank(0), cmark(0) {}
};

struct sportCopyGirl {
    string cname;
    string csex;
    int ccollege;
    int cevent;
    double cgrade;
    int crank;       // 存储名次
    int cmark;

    sportCopyGirl() : cname(" "), csex("女"), ccollege(0), cevent(0), cgrade(0.0), crank(0), cmark(0) {}
};

extern sport sports[N];
extern sportCopyBoy csportsboy[N];
extern sportCopyGirl csportsgirl[N];
extern vector<sportCopyBoy> s_boy[15];
extern vector<sportCopyGirl> s_girl[15];