#pragma once
#include "sport.h"

// 快速排序
void rankingsQuickSortBoy(int left, int right, int i);
void rankingsQuickSortGirl(int left, int right, int i);

// 堆排序
void Down(int parent, int size, int evt, bool isBoy);
void rankingsHeapSort(int evt, bool isBoy);

void doSort();   // 统一排序入口（可切换快排/堆排）

void getMark1_boy(int k, int r);
void getMark2_boy(int k, int r);
void getMark1_girl(int k, int r);
void getMark2_girl(int k, int r);

void publishResults();
void calcTotalMark();
void showPersonMark();
