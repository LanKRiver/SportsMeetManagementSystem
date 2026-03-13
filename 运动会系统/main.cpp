#include <iostream>
#include "sport.h"
#include "signup.h"
#include "event.h"
#include "grade.h"
#include "rank.h"
#include "fileIO.h"
#include "schedule.h"
using namespace std;

void showMenu() {
    cout << "\n========== 运动会管理系统 ==========" << endl;
    cout << "1. 选手报名" << endl;
    cout << "2. 比赛项目查询" << endl;
    cout << "3. 生成秩序册（时间+参赛顺序）" << endl;
    cout << "4. 查看秩序册" << endl;
    cout << "5. 成绩录入" << endl;
    cout << "6. 排名公布" << endl;
    cout << "7. 个人总分查询" << endl;
    cout << "8. 清空所有数据" << endl;
    cout << "9. 保存并退出" << endl;
    cout << "====================================" << endl;
    cout << "请选择功能：";
}

int main() {
    loadAllData();
    int op;
    while (true) {
        showMenu();
        cin >> op;
        switch (op) {
        case 1: signUpSystem(); break;
        case 2: showEventInfo(); break;
        case 3: generateSchedule(); break;
        case 4: printSchedule(); break;
        case 5: rankingInputSystem(); break;
        case 6: publishResults(); break;
        case 7: showPersonMark(); break;
        case 8: clearAllData(); break;
        case 9: saveAllData(); cout << "再见！" << endl; return 0;
        default: cout << "无效选项，请重新选择！" << endl;
        }
    }
}