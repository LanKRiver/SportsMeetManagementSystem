#include "fileIO.h"
#include "signup.h"
#include "event.h"
#include "grade.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

// 清空内存数据
void clearOldData() {
    number = 0;
    // 清空 sports 数组
    for (int i = 0; i < N; i++) {
        sports[i] = sport();
        csportsboy[i] = sportCopyBoy();
        csportsgirl[i] = sportCopyGirl();
    }
    // 清空分组容器
    for (int i = 1; i <= 10; i++) {
        s_boy[i].clear();
        s_girl[i].clear();
    }
}

// 保存所有数据
void saveAllData() {
    cout << "\n开始保存数据 - 总选手数：" << number << endl;
    if (number == 0) {
        cout << "无选手数据可保存！" << endl;
    }

    string filePath = "F:/VS2022CppCode/运动会系统/data.txt";
    ofstream fout(filePath, ios::out | ios::trunc);

    if (!fout) {
        cout << "保存失败：无法打开文件！路径：" << filePath << endl;
        cout << "   请检查路径是否存在，或是否有写入权限！" << endl;
        return;
    }
    fout << number << endl;

    for (int i = 0; i < number; i++) {
        cout << "   保存选手：" << sports[i].name << " | 成绩：" << sports[i].grade[0] << endl; // 日志
        fout << sports[i].name << " "
            << sports[i].sex << " "
            << sports[i].college << " "
            << sports[i].count << " ";
        for (int j = 0; j < sports[i].count; j++) {
            fout << sports[i].event[j] << " " << sports[i].grade[j] << " ";
        }
        fout << sports[i].mark << endl;
    }

    // 6. 写入男子分组数据
    fout << "BOY_GROUP" << endl;
    int boyCount = 0;
    for (int evt = 1; evt <= 10; evt++) {
        if (s_boy[evt].empty()) continue;
        boyCount += s_boy[evt].size();
        fout << evt << " " << s_boy[evt].size() << endl;
        for (auto& boy : s_boy[evt]) {
            fout << boy.cname << " " << boy.ccollege << " " << boy.cgrade << " " << boy.cmark << endl;
        }
    }
    cout << "   保存男子分组数据：" << boyCount << " 条" << endl;
    fout << "END_BOY" << endl;

    fout << "GIRL_GROUP" << endl;
    int girlCount = 0;
    for (int evt = 1; evt <= 10; evt++) {
        if (s_girl[evt].empty()) continue;
        girlCount += s_girl[evt].size();
        fout << evt << " " << s_girl[evt].size() << endl;
        for (auto& girl : s_girl[evt]) {
            fout << girl.cname << " " << girl.ccollege << " " << girl.cgrade << " " << girl.cmark << endl;
        }
    }
    cout << "   保存女子分组数据：" << girlCount << " 条" << endl;
    fout << "END_GIRL" << endl;

    fout.close();
    cout << "数据保存成功！文件路径：" << filePath << endl;
}

// 加载所有数据
void loadAllData() {
    ifstream fin("data.txt");
    if (!fin) {
        cout << "无历史数据，开始新系统！" << endl;
        return;
    }
    clearOldData();
    fin >> number;
    for (int i = 0; i < number; i++) {
        fin >> sports[i].name >> sports[i].sex >> sports[i].college >> sports[i].count;
        for (int j = 0; j < sports[i].count; j++) {
            fin >> sports[i].event[j] >> sports[i].grade[j];
        }
        fin >> sports[i].mark;
        for (int j = 0; j < sports[i].count; j++) {
            group(i, j);
        }
    }
    string flag;
    fin >> flag;
    if (flag == "BOY_GROUP") {
        while (true) {
            fin >> flag;
            if (flag == "END_BOY") break;
            int evt = stoi(flag);
            int size;
            fin >> size;
            for (int i = 0; i < size; i++) {
                string name;
                int college;
                double grade;
                int mark;
                fin >> name >> college >> grade >> mark;
                for (auto& boy : s_boy[evt]) {
                    if (boy.cname == name && boy.ccollege == college) {
                        boy.cgrade = grade;
                        boy.cmark = mark;
                        break;
                    }
                }
            }
        }
    }

    fin >> flag;
    if (flag == "GIRL_GROUP") {
        while (true) {
            fin >> flag;
            if (flag == "END_GIRL") break;
            int evt = stoi(flag);
            int size;
            fin >> size;
            for (int i = 0; i < size; i++) {
                string name;
                int college;
                double grade;
                int mark;
                fin >> name >> college >> grade >> mark;
                for (auto& girl : s_girl[evt]) {
                    if (girl.cname == name && girl.ccollege == college) {
                        girl.cgrade = grade;
                        girl.cmark = mark;
                        break;
                    }
                }
            }
        }
    }

    fin.close();
    cout << "历史数据（报名+成绩+加分）已完整加载！" << endl;
}

// 清空所有数据
void clearAllData() {
    char confirm;
    cout << "\n警告：此操作会清空所有运动会数据（报名/成绩/排名），且无法恢复！" << endl;
    cout << "请确认是否清空（输入 Y/y 确认，其他键取消）：";
    cin >> confirm;
    if (confirm != 'Y' && confirm != 'y') {
        cout << "已取消清空操作！" << endl;
        return;
    }
    clearOldData();
    ofstream fout("data.txt", ios::out | ios::trunc);
    if (fout) {
        fout.close();
        cout << "所有数据已清空！文件 data.txt 已重置，可用于新运动会。" << endl;
    }
    else {
        cout << "清空文件失败，但内存数据已清空！" << endl;
    }
}