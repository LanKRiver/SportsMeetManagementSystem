#include "signup.h"
#include <iomanip>
#include <limits>

void collegePrint() {
    cout << "1.法学院  2.商学院  3.马克思主义学院  4.长江新闻与传播学院  5.数学与计算机学院  6.文学院" << endl;
}

void printCollege(int k) {
    switch (k) {
    case 1: cout << "法学院"; break;
    case 2: cout << "商学院"; break;
    case 3: cout << "马克思主义学院"; break;
    case 4: cout << "长江新闻与传播学院"; break;
    case 5: cout << "数学与计算机学院"; break;
    case 6: cout << "文学院"; break;
    default: cout << "未知学院"; break;
    }
}

void group(int studentIndex, int p) {
    int flag = sports[studentIndex].event[p];
    int sex = sports[studentIndex].sex;

    if (sex == 1) {
        csportsboy[studentIndex].cname = sports[studentIndex].name;
        csportsboy[studentIndex].ccollege = sports[studentIndex].college;
        s_boy[flag].push_back(csportsboy[studentIndex]);
        sports[studentIndex].position[p] = (int)s_boy[flag].size() - 1;
    }
    else {
        csportsgirl[studentIndex].cname = sports[studentIndex].name;
        csportsgirl[studentIndex].ccollege = sports[studentIndex].college;
        s_girl[flag].push_back(csportsgirl[studentIndex]);
        sports[studentIndex].position[p] = (int)s_girl[flag].size() - 1;
    }
}

void signUpSystem() {
    int count;
    cout << "请输入本次需要报名的人数：";
    while (!(cin >> count) || count <= 0 || count > 20000)
    {
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "非数字字符，请重新输入数字：";
        }
        else
            cout << "人数不合理，请重新输入：";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    

    while (count--) {
        cout << "\n--- 第 " << number + 1 << " 位选手 ---" << endl;
        cout << "姓名："; cin >> sports[number].name;

        cout << "性别（1男 2女）：";
        while (!(cin >> sports[number].sex) || (sports[number].sex != 1 && sports[number].sex != 2)) {
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "非数字字符，请重新输入数字：";
            }
            else
                cout << "数量必须为 1 或 2，请重新输入";
        }

        collegePrint();
        cout << "学院：";
        while (!(cin >> sports[number].college) || sports[number].college < 1 || sports[number].college > 6) {
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "非数字字符，请重新输入数字：";
            }
            else
                cout << "请输入数字 1 ~ 5：";
        }

        cout << "\n项目：\n1.100 2.200 3.400 4.800 5.1500 6.5000 7.接力 8.跳高 9.跳远 10.铅球" << endl;

        cout << "请输入要报名项目的数量( 1 ~ 3 ）：";
        while (!(cin >> sports[number].count) || sports[number].count <= 0 || sports[number].count > 3)
        {
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "请输入有效的数字：";
            }
            else {
                cout << "数量必须在 1 ~ 3 之间，请重新输入：";
            }
        }

        cout << "每次填写一个项目数字：" << endl;
        for (int i = 0; i < sports[number].count; i++)
        {
            bool validInput = false;
            while (!validInput)
            {
                cout << "请输入：";

                if (!(cin >> sports[number].event[i])) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "输入无效，请重新输入( 1 ~ 10)：";
                    continue;
                }

                if (sports[number].event[i] <= 0 || sports[number].event[i] > 10)
                {
                    cout << "无效输入，请重新输入( 1 ~ 10)：";
                    continue;
                }

                bool isDuplicate = false;
                for (int j = 0; j < i; j++)
                {
                    if (sports[number].event[i] == sports[number].event[j])
                    {
                        isDuplicate = true;
                        break;
                    }
                }

                if (isDuplicate)
                {
                    cout << "输入重复，请输入其他项目:";
                    continue;
                }
                validInput = true;
            }
            group(number, i);
        }
        number++;
        cout << "报名成功！" << endl;
    }
}