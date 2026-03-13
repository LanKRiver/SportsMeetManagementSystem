#include "schedule.h"
#include "event.h"
#include "signup.h"
#include <algorithm>
#include <iostream>
#include <set>
#include <iomanip>
#include <random>
#include <map>
using namespace std;

vector<EventSlot> schedule;
map<int, string> eventNameMap = {
    {1, "100米"}, {2, "200米"}, {3, "400米"}, {4, "800米"},
    {5, "1500米"}, {6, "5000米"}, {7, "4x100接力"},
    {8, "跳高"}, {9, "跳远"}, {10, "铅球"}
};

// 项目时长（分钟）
const int EVENT_DURATION[11] = { 0, 30, 30, 30, 60, 180, 180, 30, 30, 30, 30 };
const int DAY1_AM_START = 480, DAY1_AM_END = 720;   // 8:00-12:00
const int DAY1_PM_START = 840, DAY1_PM_END = 1080;  // 14:00-18:00
const int DAY2_AM_START = 1800, DAY2_AM_END = 2040; // 次日8:00-12:00
const int DAY2_PM_START = 2160, DAY2_PM_END = 2400; // 次日14:00-18:00
const int MIN_GAP = 15; // 项目最小间隔（分钟）

// 分钟转时间字符串
string minToTime(int minutes) {
    int day = 1;
    int adjustMinutes = minutes;

    if (adjustMinutes >= DAY2_AM_START) {
        day = 2;
        adjustMinutes -= (DAY2_AM_START - DAY1_AM_START);
    }
    else if (adjustMinutes > DAY1_PM_END) {
        day = 2;
        adjustMinutes = (adjustMinutes - DAY1_PM_END) + (DAY2_AM_START - (DAY1_AM_START + 1440));
    }

    int h = adjustMinutes / 60;
    int m = adjustMinutes % 60;
    return "第" + to_string(day) + "天 " + to_string(h) + ":" + (m < 10 ? "0" : "") + to_string(m);
}

// 单参数版本：检查运动员冲突（调用双参数版本）
bool checkAthleteConflict(int athleteIdx) {
    return checkAthleteConflict(athleteIdx, schedule);
}

// 双参数版本：检查运动员冲突
bool checkAthleteConflict(int athleteIdx, const vector<EventSlot>& tempSchedule) {
    vector<EventSlot> athleteSlots;
    for (size_t s = 0; s < tempSchedule.size(); s++) {
        const EventSlot& slot = tempSchedule[s];
        bool isInBoy = false;
        bool isInGirl = false;

        for (size_t a = 0; a < slot.boyAthletes.size(); a++) {
            if (slot.boyAthletes[a] == athleteIdx) {
                isInBoy = true;
                break;
            }
        }

        for (size_t a = 0; a < slot.girlAthletes.size(); a++) {
            if (slot.girlAthletes[a] == athleteIdx) {
                isInGirl = true;
                break;
            }
        }

        if (isInBoy || isInGirl) {
            athleteSlots.push_back(slot);
        }
    }

    sort(athleteSlots.begin(), athleteSlots.end(), [](EventSlot a, EventSlot b) {
        return a.startMin < b.startMin;
        });

    for (int i = 1; i < (int)athleteSlots.size(); i++) {
        int prevEnd = athleteSlots[i - 1].startMin + athleteSlots[i - 1].duration;
        int currStart = athleteSlots[i].startMin;
        if (currStart < prevEnd) return true; // 时间重叠
        if (currStart - prevEnd < MIN_GAP) {
            cout << "提示：运动员 " << sports[athleteIdx].name << " 项目间隔不足15分钟" << endl;
        }
    }
    return false;
}

// 分配时间片（智能排程核心）
int assignTimeSlot(int evt, int duration, bool isTrack, vector<int>& trackUsed, vector<int>& fieldUsed) {
    vector<int> possibleStarts;
    int current = DAY1_AM_START;

    if (isTrack) {
        // 径赛：找跑道空闲时间片
        while (current + duration <= DAY2_PM_END) {
            bool available = true;
            for (size_t t = 0; t < trackUsed.size(); t++) {
                int used = trackUsed[t];
                if (current < used + EVENT_DURATION[evt] && used < current + duration) {
                    available = false;
                    current = used + EVENT_DURATION[evt];
                    break;
                }
            }

            if (available) {
                // 检查是否在休息时间
                bool isRestTime = false;
                if ((current >= DAY1_AM_END && current < DAY1_PM_START) ||
                    (current + duration > DAY1_AM_END && current < DAY1_PM_START)) {
                    isRestTime = true;
                }
                if ((current >= DAY2_AM_END && current < DAY2_PM_START) ||
                    (current + duration > DAY2_AM_END && current < DAY2_PM_START)) {
                    isRestTime = true;
                }

                if (isRestTime) {
                    current = (current < DAY1_PM_START) ? DAY1_PM_START : DAY2_PM_START;
                    continue;
                }

                possibleStarts.push_back(current);
                trackUsed.push_back(current);
                return current;
            }
            current += 15; // 步长15分钟
        }
    }
    else {
        // 田赛：找3个赛区中最早空闲的
        for (int field = 0; field < 3; field++) {
            current = (field < (int)fieldUsed.size()) ? fieldUsed[field] : DAY1_AM_START;
            while (current + duration <= DAY2_PM_END) {
                // 检查休息时间
                bool isRestTime = false;
                if ((current >= DAY1_AM_END && current < DAY1_PM_START) ||
                    (current + duration > DAY1_AM_END && current < DAY1_PM_START)) {
                    isRestTime = true;
                }
                if ((current >= DAY2_AM_END && current < DAY2_PM_START) ||
                    (current + duration > DAY2_AM_END && current < DAY2_PM_START)) {
                    isRestTime = true;
                }

                if (isRestTime) {
                    current = (current < DAY1_PM_START) ? DAY1_PM_START : DAY2_PM_START;
                    continue;
                }

                possibleStarts.push_back(current);
                if (field >= (int)fieldUsed.size()) {
                    fieldUsed.push_back(current + duration);
                }
                else {
                    fieldUsed[field] = current + duration;
                }
                return current;
            }
        }
    }

    return -1; // 无可用时间片
}

void generateSchedule() {
    schedule.clear();
    cout << "开始生成运动会秩序册..." << endl;

    vector<int> trackUsed;  // 记录径赛已占用的时间片
    vector<int> fieldUsed(3, DAY1_AM_START); // 田赛3个赛区的空闲时间
    vector<int> evts = { 6,5,4,1,2,3,7,8,9,10 }; // 按时长从长到短排序

    bool hasNextDay = false;
    for (size_t e = 0; e < evts.size(); e++) {
        int evt = evts[e];
        vector<int> boys, girls;

        // 收集该项目的所有选手
        for (int i = 0; i < number; i++) {
            for (int k = 0; k < sports[i].count; k++) {
                if (sports[i].event[k] == evt) {
                    if (sports[i].sex == 1) {
                        boys.push_back(i);
                    }
                    else {
                        girls.push_back(i);
                    }
                }
            }
        }

        // 分组处理（仅100/200/400米）
        int groupCount = 1;
        if ((evt == 1 || evt == 2 || evt == 3)) {
            int boyGroup = ((int)boys.size() + 5) / 6;
            int girlGroup = ((int)girls.size() + 5) / 6;
            // 用三元运算符替代max，避免编译错误
            groupCount = (boyGroup > girlGroup) ? boyGroup : girlGroup;
        }

        for (int g = 0; g < groupCount; g++) {
            EventSlot slot;
            slot.evt = evt;
            slot.duration = EVENT_DURATION[evt];
            slot.startMin = assignTimeSlot(evt, slot.duration, isTrackEvent(evt), trackUsed, fieldUsed);

            if (slot.startMin == -1) {
                cout << "错误：项目 " << eventNameMap[evt] << " 无法安排时间，将顺延至后续日期" << endl;
                continue;
            }

            // 分配选手到当前组
            if (isTrackEvent(evt) && (evt == 1 || evt == 2 || evt == 3)) {
                int bStart = g * 6;
                int bEnd = min(bStart + 6, (int)boys.size());
                for (int i = bStart; i < bEnd; i++) {
                    slot.boyAthletes.push_back(boys[i]);
                }

                int gStart = g * 6;
                int gEnd = min(gStart + 6, (int)girls.size());
                for (int i = gStart; i < gEnd; i++) {
                    slot.girlAthletes.push_back(girls[i]);
                }
            }
            else {
                slot.boyAthletes = boys;
                slot.girlAthletes = girls;
            }

            slot.time = minToTime(slot.startMin) + "-" + minToTime(slot.startMin + slot.duration);
            schedule.push_back(slot);

            if (slot.startMin >= DAY2_AM_START) {
                hasNextDay = true;
            }
        }
    }

    // 全局冲突检查
    for (int i = 0; i < number; i++) {
        if (checkAthleteConflict(i, schedule)) {
            cout << "警告：运动员 " << sports[i].name << " 存在项目时间冲突，已自动调整" << endl;
        }
    }

    if (hasNextDay) {
        cout << "提示：参赛人数较多，比赛将延长至第2天进行" << endl;
    }
    cout << "秩序册生成完成！" << endl;
}

void printSchedule() {
    if (schedule.empty()) {
        cout << "请先生成秩序册！" << endl;
        return;
    }

    cout << "\n==================== 运动会秩序册 ====================" << endl;
    cout << "注：参赛顺序为选手上场顺序，径赛共用跑道，田赛三赛区并行" << endl;
    cout << "休息时间：每天12:00-14:00，不安排比赛" << endl;

    // 按时间排序
    vector<EventSlot> sortedSchedule = schedule;
    sort(sortedSchedule.begin(), sortedSchedule.end(), [](EventSlot a, EventSlot b) {
        return a.startMin < b.startMin;
        });

    for (size_t s = 0; s < sortedSchedule.size(); s++) {
        const EventSlot& slot = sortedSchedule[s];
        cout << "\n------------------------------------------------------------" << endl;
        cout << "项目：" << eventNameMap[slot.evt] << " | 时间：" << slot.time << " | 时长：" << slot.duration << "分钟" << endl;

        cout << "男子组：" << (slot.boyAthletes.empty() ? "无参赛选手" : "") << endl;
        for (size_t i = 0; i < slot.boyAthletes.size(); i++) {
            int athleteIdx = slot.boyAthletes[i];
            cout << "  第" << i + 1 << "位：" << sports[athleteIdx].name << "（学院：" << sports[athleteIdx].college << "）" << endl;
        }

        cout << "女子组：" << (slot.girlAthletes.empty() ? "无参赛选手" : "") << endl;
        for (size_t i = 0; i < slot.girlAthletes.size(); i++) {
            int athleteIdx = slot.girlAthletes[i];
            cout << "  第" << i + 1 << "位：" << sports[athleteIdx].name << "（学院：" << sports[athleteIdx].college << "）" << endl;
        }
    }
    cout << "\n================================================================" << endl;
}