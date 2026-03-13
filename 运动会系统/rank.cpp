#include "rank.h"
#include "event.h"
#include "signup.h"
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

// ===================== 排序核心修复：区分径赛/田赛 =====================
// 快速排序（径赛升序，田赛降序）
int getRandom(int left, int right) {
    return rand() % (right - left + 1) + left;
}

void rankingsQuickSortBoy(int left, int right, int i) {
    if (left >= right || s_boy[i].empty()) return;
    int mid = getRandom(left, right);
    double pos = s_boy[i][mid].cgrade;
    int l = left - 1, r = right + 1, j = left;

    bool isTrack = isTrackEvent(i);
    while (j < r) {
        if (isTrack) {
            // 径赛：时间越短越好（升序）
            if (s_boy[i][j].cgrade < pos) swap(s_boy[i][++l], s_boy[i][j++]);
            else if (s_boy[i][j].cgrade > pos) swap(s_boy[i][--r], s_boy[i][j]);
            else j++;
        }
        else {
            // 田赛：距离越大越好（降序）
            if (s_boy[i][j].cgrade > pos) swap(s_boy[i][++l], s_boy[i][j++]);
            else if (s_boy[i][j].cgrade < pos) swap(s_boy[i][--r], s_boy[i][j]);
            else j++;
        }
    }
    rankingsQuickSortBoy(left, l, i);
    rankingsQuickSortBoy(r, right, i);
}

void rankingsQuickSortGirl(int left, int right, int i) {
    if (left >= right || s_girl[i].empty()) return;
    int mid = getRandom(left, right);
    double pos = s_girl[i][mid].cgrade;
    int l = left - 1, r = right + 1, j = left;

    bool isTrack = isTrackEvent(i);
    while (j < r) {
        if (isTrack) {
            // 径赛：升序
            if (s_girl[i][j].cgrade < pos) swap(s_girl[i][++l], s_girl[i][j++]);
            else if (s_girl[i][j].cgrade > pos) swap(s_girl[i][--r], s_girl[i][j]);
            else j++;
        }
        else {
            // 田赛：降序
            if (s_girl[i][j].cgrade > pos) swap(s_girl[i][++l], s_girl[i][j++]);
            else if (s_girl[i][j].cgrade < pos) swap(s_girl[i][--r], s_girl[i][j]);
            else j++;
        }
    }
    rankingsQuickSortGirl(left, l, i);
    rankingsQuickSortGirl(r, right, i);
}

// 堆排序（同步修复径赛/田赛规则）
void Down(int parent, int size, int evt, bool isBoy) {
    int child = parent * 2;
    bool isTrack = isTrackEvent(evt);
    while (child <= size) {
        if (isBoy) {
            // 径赛：找更小值；田赛：找更大值
            if (child + 1 <= size) {
                if (isTrack) {
                    if (s_boy[evt][child].cgrade < s_boy[evt][child - 1].cgrade) child++;
                }
                else {
                    if (s_boy[evt][child].cgrade > s_boy[evt][child - 1].cgrade) child++;
                }
            }
            // 比较父节点和子节点
            if (isTrack) {
                if (s_boy[evt][parent - 1].cgrade <= s_boy[evt][child - 1].cgrade) break;
            }
            else {
                if (s_boy[evt][parent - 1].cgrade >= s_boy[evt][child - 1].cgrade) break;
            }
            swap(s_boy[evt][parent - 1], s_boy[evt][child - 1]);
        }
        else {
            if (child + 1 <= size) {
                if (isTrack) {
                    if (s_girl[evt][child].cgrade < s_girl[evt][child - 1].cgrade) child++;
                }
                else {
                    if (s_girl[evt][child].cgrade > s_girl[evt][child - 1].cgrade) child++;
                }
            }
            if (isTrack) {
                if (s_girl[evt][parent - 1].cgrade <= s_girl[evt][child - 1].cgrade) break;
            }
            else {
                if (s_girl[evt][parent - 1].cgrade >= s_girl[evt][child - 1].cgrade) break;
            }
            swap(s_girl[evt][parent - 1], s_girl[evt][child - 1]);
        }
        parent = child;
        child = parent * 2;
    }
}

void rankingsHeapSort(int evt, bool isBoy) {
    int n;
    if (isBoy) n = (int)s_boy[evt].size();
    else n = (int)s_girl[evt].size();

    if (n <= 1) return;

    for (int i = n / 2; i >= 1; i--)
        Down(i, n, evt, isBoy);

    for (int i = n; i > 1; i--) {
        if (isBoy) swap(s_boy[evt][0], s_boy[evt][i - 1]);
        else swap(s_girl[evt][0], s_girl[evt][i - 1]);
        Down(1, i - 1, evt, isBoy);
    }
}

// ===================== 处理同名次排名 =====================
// 男子组同名次处理
void setBoyRank(int evt) {
    if (s_boy[evt].empty()) return;
    int rank = 1;
    s_boy[evt][0].crank = rank;
    for (int i = 1; i < s_boy[evt].size(); i++) {
        // 成绩相等则名次相同
        if (abs(s_boy[evt][i].cgrade - s_boy[evt][i - 1].cgrade) < 1e-6) {
            s_boy[evt][i].crank = s_boy[evt][i - 1].crank;
        }
        else {
            rank = i + 1;
            s_boy[evt][i].crank = rank;
        }
    }
}

// 女子组同名次处理
void setGirlRank(int evt) {
    if (s_girl[evt].empty()) return;
    int rank = 1;
    s_girl[evt][0].crank = rank;
    for (int i = 1; i < s_girl[evt].size(); i++) {
        if (abs(s_girl[evt][i].cgrade - s_girl[evt][i - 1].cgrade) < 1e-6) {
            s_girl[evt][i].crank = s_girl[evt][i - 1].crank;
        }
        else {
            rank = i + 1;
            s_girl[evt][i].crank = rank;
        }
    }
}

// 统一排序入口
void doSort() {
    srand((unsigned int)time(0));

    // 快速排序（取消注释用堆排序）
    for (int i = 1; i <= 10; i++) {
        if (!s_boy[i].empty()) {
            rankingsQuickSortBoy(0, (int)s_boy[i].size() - 1, i);
            setBoyRank(i); // 排序后设置名次（支持并列）
        }
        if (!s_girl[i].empty()) {
            rankingsQuickSortGirl(0, (int)s_girl[i].size() - 1, i);
            setGirlRank(i); // 排序后设置名次（支持并列）
        }
    }

    // 堆排序（如需使用，注释上面快排，取消下面注释）
    //for (int i = 1; i <= 10; i++) {
    //    if (!s_boy[i].empty()) {
    //        rankingsHeapSort(i, true);
    //        setBoyRank(i);
    //    }
    //    if (!s_girl[i].empty()) {
    //        rankingsHeapSort(i, false);
    //        setGirlRank(i);
    //    }
    //}
}

// ===================== 加分规则 =====================
void getMark1_boy(int k, int r) {
    if (r == 1) s_boy[k][r - 1].cmark = 7;
    else if (r == 2) s_boy[k][r - 1].cmark = 5;
    else if (r == 3) s_boy[k][r - 1].cmark = 3;
    else if (r == 4) s_boy[k][r - 1].cmark = 2;
    else if (r == 5) s_boy[k][r - 1].cmark = 1;
}

void getMark2_boy(int k, int r) {
    if (r == 1) s_boy[k][r - 1].cmark = 5;
    else if (r == 2) s_boy[k][r - 1].cmark = 3;
    else if (r == 3) s_boy[k][r - 1].cmark = 2;
}

void getMark1_girl(int k, int r) {
    if (r == 1) s_girl[k][r - 1].cmark = 7;
    else if (r == 2) s_girl[k][r - 1].cmark = 5;
    else if (r == 3) s_girl[k][r - 1].cmark = 3;
    else if (r == 4) s_girl[k][r - 1].cmark = 2;
    else if (r == 5) s_girl[k][r - 1].cmark = 1;
}

void getMark2_girl(int k, int r) {
    if (r == 1) s_girl[k][r - 1].cmark = 5;
    else if (r == 2) s_girl[k][r - 1].cmark = 3;
    else if (r == 3) s_girl[k][r - 1].cmark = 2;
}

// ===================== 成绩发布（优化显示） =====================
void publishResults() {
    doSort();
    cout << "\n==================== 最终排名 ====================" << endl;

    for (int k = 1; k <= 10; k++) {
        cout << "\n--- "; printEvent(k); cout << " ---" << endl;

        cout << "\n【男子组】\n";
        if (s_boy[k].size() < 4)
            cout << "参赛人数不足4人，项目取消" << endl;
        else {
            int show = min(5, (int)s_boy[k].size());
            for (int m = 0; m < show; m++) {
                cout << "第" << s_boy[k][m].crank << "名 "  // 显示实际名次（支持并列）
                    << left << setw(8) << s_boy[k][m].cname << " ";
                // 径赛显示分秒毫秒，田赛显示米数
                if (isTrackEvent(k)) {
                    cout << secondsToTimeStr(s_boy[k][m].cgrade);
                }
                else {
                    cout << fixed << setprecision(3) << s_boy[k][m].cgrade << " 米";
                }
                cout << "  学院：";
                printCollege(s_boy[k][m].ccollege); cout << endl;
            }
            // 按实际名次加分
            for (int m = 0; m < show; m++) {
                if (s_boy[k].size() > 6) getMark1_boy(k, s_boy[k][m].crank);
                else getMark2_boy(k, s_boy[k][m].crank);
            }
        }

        cout << "\n【女子组】\n";
        if (s_girl[k].size() < 4)
            cout << "参赛人数不足4人，项目取消" << endl;
        else {
            int show = min(5, (int)s_girl[k].size());
            for (int m = 0; m < show; m++) {
                cout << "第" << s_girl[k][m].crank << "名 "  // 显示实际名次（支持并列）
                    << left << setw(8) << s_girl[k][m].cname << " ";
                if (isTrackEvent(k)) {
                    cout << secondsToTimeStr(s_girl[k][m].cgrade);
                }
                else {
                    cout << fixed << setprecision(3) << s_girl[k][m].cgrade << " 米";
                }
                cout << "  学院：";
                printCollege(s_girl[k][m].ccollege); cout << endl;
            }
            for (int m = 0; m < show; m++) {
                if (s_girl[k].size() > 6) getMark1_girl(k, s_girl[k][m].crank);
                else getMark2_girl(k, s_girl[k][m].crank);
            }
        }
    }
}

// ===================== 计算总分 =====================
void calcTotalMark() {
    for (int i = 0; i < number; i++) {
        sports[i].mark = 0;
        int sex = sports[i].sex;
        for (int k = 0; k < sports[i].count; k++) {
            int evt = sports[i].event[k];
            int pos = sports[i].position[k];
            if (sex == 1 && pos < s_boy[evt].size())
                sports[i].mark += s_boy[evt][pos].cmark;
            if (sex == 2 && pos < s_girl[evt].size())
                sports[i].mark += s_girl[evt][pos].cmark;
        }
    }
}

// ===================== 显示个人总分 =====================
void showPersonMark() {
    calcTotalMark();
    cout << "\n===== 个人总得分 =====" << endl;
    for (int i = 0; i < number; i++) {
        cout << left << setw(6) << sports[i].name
            << "(" << (sports[i].sex == 1 ? "男" : "女") << ") 得分："
            << sports[i].mark << endl;
    }
}