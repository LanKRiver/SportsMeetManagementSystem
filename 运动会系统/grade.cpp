#include "grade.h"
#include "event.h"
#include "schedule.h"
#include <limits>
#include <string>
#include <sstream>
#include <iomanip>

void get_grade(int evt, int i, int k) {
    int sex = sports[i].sex;
    unsigned int loc = sports[i].position[k];
    if (sex == 1 && loc < s_boy[evt].size())
        s_boy[evt][loc].cgrade = sports[i].grade[k];
    else if (sex == 2 && loc < s_girl[evt].size())
        s_girl[evt][loc].cgrade = sports[i].grade[k];
}

void rankingInputSystem() {
    cout << "\n===== 成绩录入系统 =====" << endl;
    // 先检查是否生成秩序册
    if (schedule.empty()) {
        cout << "请先生成秩序册（菜单3），再录入成绩！" << endl;
        return;
    }

    for (int i = 0; i < number; i++) {
        cout << "\n选手：" << sports[i].name << "(" << (sports[i].sex == 1 ? "男" : "女") << ")" << endl;
        // 检查运动员时间冲突
        if (checkAthleteConflict(i)) {
            cout << "该选手项目时间冲突！请调整报名后再录入成绩！" << endl;
            continue;
        }

        for (int k = 0; k < sports[i].count; k++) {
            int evt = sports[i].event[k];
            printEvent(evt);

            if (isTrackEvent(evt)) {
                // 径赛：分秒毫秒录入
                cout << "（格式：分:秒.毫秒 或 秒.毫秒，如 1:20.500 或 12.450）：";
                string timeStr;
                while (true) {
                    cin >> timeStr;
                    try {
                        double totalSec = timeStrToSeconds(timeStr);
                        if (totalSec <= 0) {
                            cout << "成绩需大于0，请重新输入：";
                            continue;
                        }
                        sports[i].grade[k] = totalSec;
                        break;
                    }
                    catch (...) {
                        cout << "格式错误！请按 分:秒.毫秒 格式输入（如 12.500）：";
                    }
                }
            }
            else {
                // 田赛：米数录入
                cout << "（单位：米，如 3.55）：";
                while (!(cin >> sports[i].grade[k]) || sports[i].grade[k] < 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "成绩需大于0，请重新输入：";
                }
            }
            get_grade(evt, i, k);
        }
    }
}