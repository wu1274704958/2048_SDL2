#pragma once
#include "Mat.h"
#include <iostream>
#include <vector>

bool rand_add_test(Mat<int> &data);
bool rand_add(Mat<int> &data, void(*createThread_toBig)(int));
void swap(int *a, int *b);
const int&& get_score(const int&& s);
bool up(	Mat<int> &data,void(*draw)()	, void(*createThread_toBig_toSmall)(int));
bool down(	Mat<int> &data,void(*draw)()	, void(*createThread_toBig_toSmall)(int));
bool left(	Mat<int> &data,void(*draw)()	, void(*createThread_toBig_toSmall)(int));
bool right(	Mat<int> &data,void(*draw)()	, void(*createThread_toBig_toSmall)(int));

