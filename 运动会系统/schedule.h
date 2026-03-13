#pragma once
#include "sport.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

struct EventSlot {
    int evt;
    string time;
    int startMin;
    int duration;
    vector<int> boyAthletes;
    vector<int> girlAthletes;

    EventSlot() : evt(0), time(""), startMin(0), duration(0) {}
};

void generateSchedule();
void printSchedule();
bool checkAthleteConflict(int athleteIdx);
bool checkAthleteConflict(int athleteIdx, const vector<EventSlot>& tempSchedule);

extern vector<EventSlot> schedule;
extern map<int, string> eventNameMap;