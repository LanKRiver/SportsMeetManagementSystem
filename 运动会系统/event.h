#pragma once
#include "sport.h"
#include <string>

bool isTrackEvent(int evt);
double timeStrToSeconds(std::string timeStr);
std::string secondsToTimeStr(double seconds);
void eventPrint();
void printEvent(int k);
void showEventInfo();
void showSchedule();
