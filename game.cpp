#include "game.h"
#include <Windows.h>
#include <iostream>
#include <map>

extern int score;
static std::map<int *, int> rand_map;


bool rand_add_test(Mat<int> &data)
{
	bool res = false;
	int *data_ = const_cast<int *>(data.getData());
	for (int i = 0; i < 16; i++)
	{
		if (data_[i] == 0)
		{
			res = true;
			break;
		}
	}
	return res;
}

bool rand_add(Mat<int> &data, void(*createThread_toBig)(int))
{
	int *data_ = const_cast<int *>(data.getData());
	for (int i = 0; i < 16; i++)
	{
		if (data_[i] == 0)
		{
			rand_map.insert(std::make_pair(data_ + i, i));
		}
	}
	if (rand_map.size() == 0)
	{
		return false;
	}

	int r = (rand() % rand_map.size());
	//printf("size = %d r = %d\n",rand_v.size(),r);
	int v = (rand() % 5) == 0 ? 4 : 2;
	
	auto it = rand_map.begin();
	int i = 0;
	for (; it != rand_map.end(); it++,i++)
	{
		if (i == r)
		{
			*(it->first) = v;
			createThread_toBig(it->second);
			break;
		}
	}


	rand_map.clear();
	return true;
}

void swap(int *a, int *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

const int&& get_score(const int&& s)
{
	score += s;
	return std::forward<const int&&>(s);
}

#define DRAW_SLEEP \
		if(change){draw(); Sleep(10);}


		

bool up(Mat<int> &data,void(*draw)(), void(*createThread_toBig_toSmall)(int))
{
	bool change = false;
	bool change2 = false;
	int *d = const_cast<int *>(data.getData());
	int t = 0;


	for (int y = 0; y < 2; y++)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (d[i + j * 4] == 0)
				{
					if (d[i + (j + 1) * 4] != 0)
					{
						change = true;
						t = 4 - j;
						for (int k = 0; k < t; k++)
						{
							if (k == t - 1)
							{
								d[i + (j + k) * 4] = 0;
							}
							else {
								swap(&d[i + (j + k) * 4], &d[i + (j + k + 1) * 4]);
							}
						}
					}
				}
			}
			DRAW_SLEEP
		}
	}

	for (int i = 0; i < 4; i++)
	{
		bool ONCE = false;
		for (int j = 0; j < 3; j++)
		{
			if (d[i + j * 4] != 0)
			{
				if (d[i + (j + 1) * 4] != 0)
				{
					if (d[i + (j + 1) * 4] == d[i + j * 4])
					{
						d[i + j * 4] = get_score(d[i + j * 4] * 2);
						d[i + (j + 1) * 4] = 0;
						if (ONCE)
						{
							createThread_toBig_toSmall(i + (j - 1) * 4);
						}
						else {
							createThread_toBig_toSmall(i + j * 4);
						}
						change2 = true;
						ONCE = true;
					}
				}
			}
		}
	}
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (d[i + j * 4] == 0)
			{
				if (d[i + (j + 1) * 4] != 0)
				{
					t = 4 - j;
					for (int k = 0; k < t; k++)
					{
						if (k == t - 1)
						{
							d[i + (j + k) * 4] = 0;
						}
						else {
							swap(&d[i + (j + k) * 4], &d[i + (j + k + 1) * 4]);
						}
					}
				}
			}
		}
		DRAW_SLEEP
	}
	

	if (change || change2)
	{
		return true;
	}
	else {
		return false;
	}
}
bool down(Mat<int> &data,void(*draw)(), void(*createThread_toBig_toSmall)(int))
{
	bool change = false;
	bool change2 = false;
	int *d = const_cast<int *>(data.getData());
	int t = 0;

	for (int y = 0; y < 2; y++)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 3; j > 0; j--)
			{
				if (d[i + j * 4] == 0)
				{
					if (d[i + (j - 1) * 4] != 0)
					{
						change = true;
						t = j + 1;
						for (int k = 0; k < t; k++)
						{
							if (k == t - 1)
							{
								d[i + (j - k) * 4] = 0;
							}
							else {
								swap(&d[i + (j - k) * 4], &d[i + (j - k - 1) * 4]);
							}
						}
					}
				}
			}
			DRAW_SLEEP
		}
	}


	for (int i = 0; i < 4; i++)
	{
		bool ONCE = false;
		for (int j = 3; j > 0; j--)
		{
			if (d[i + j * 4] != 0)
			{
				if (d[i + (j - 1) * 4] != 0)
				{
					if (d[i + (j - 1) * 4] == d[i + j * 4])
					{
						d[i + j * 4] = get_score(d[i + j * 4] * 2);
						d[i + (j - 1) * 4] = 0;
						if (ONCE)
						{
							createThread_toBig_toSmall(i + (j + 1) * 4);
						}
						else {
							createThread_toBig_toSmall(i + j * 4);
						}
						change2 = true;
						ONCE = true;
					}
				}
			}
		}
	}
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 3; j > 0; j--)
		{
			if (d[i + j * 4] == 0)
			{
				if (d[i + (j - 1) * 4] != 0)
				{
					t = j + 1;
					for (int k = 0; k < t; k++)
					{
						if (k == t - 1)
						{
							d[i + (j - k) * 4] = 0;
						}
						else {
							swap(&d[i + (j - k) * 4], &d[i + (j - k - 1) * 4]);
						}
					}		
				}
			}	
		}
		DRAW_SLEEP
	}
	
	if (change || change2)
	{
		return true;
	}
	else {
		return false;
	}
}
bool left(Mat<int> &data,void(*draw)(), void(*createThread_toBig_toSmall)(int))
{
	bool change = false;
	bool change2 = false;
	int *d = const_cast<int *>(data.getData());
	int t = 0;
	for (int y = 0; y < 2; y++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 3; i++)
			{
				if (d[i + j * 4] == 0)
				{
					if (d[(i + 1) + j * 4] != 0)
					{
						change = true;
						t = 4 - i;
						for (int k = 0; k < t; k++)
						{
							if (k == t - 1)
							{
								d[(i + k) + j * 4] = 0;
							}
							else {
								swap(&d[(i + k) + j * 4], &d[(i + k + 1) + j * 4]);
							}
						}			
					}
				}
			}
			DRAW_SLEEP
		}
	}
	for (int j = 0; j < 4; j++)
	{
		bool ONCE = false;
		for (int i = 0; i < 3; i++)
		{
			if (d[i + j * 4] != 0)
			{
				if (d[(i + 1) + j * 4] != 0)
				{
					if (d[(i + 1) + j * 4] == d[i + j * 4])
					{
						d[i + j * 4] = get_score(d[i + j * 4] * 2);
						d[(i + 1) + j * 4] = 0;
						if (ONCE)
						{
							createThread_toBig_toSmall((i - 1) + j * 4);
						}
						else {
							createThread_toBig_toSmall(i + j * 4);
						}
						change2 = true;
						ONCE = true;
					}
				}
			}
		}
	}
	
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			if (d[i + j * 4] == 0)
			{
				if (d[(i + 1) + j * 4] != 0)
				{
					t = 4 - i;
					for (int k = 0; k < t; k++)
					{
						if (k == t - 1)
						{
							d[(i + k) + j * 4] = 0;
						}
						else {
							swap(&d[(i + k) + j * 4], &d[(i + k + 1) + j * 4]);
						}
					}
				}
			}
		}
		DRAW_SLEEP
	}
	
	
	if (change || change2)
	{
		return true;
	}
	else {
		return false;
	}
}
bool right(Mat<int> &data,void(*draw)(), void(*createThread_toBig_toSmall)(int))
{
	bool change = false;
	bool change2 = false;
	int *d = const_cast<int *>(data.getData());
	int t = 0;
	for (int y = 0; y < 2; y++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int i = 3; i > 0; i--)
			{
				if (d[i + j * 4] == 0)
				{
					if (d[(i - 1) + j * 4] != 0)
					{
						change = true;
						t = i + 1;
						for (int k = 0; k < t; k++)
						{
							if (k == t - 1)
							{
								d[(i - k) + j * 4] = 0;
							}
							else {
								swap(&d[(i - k) + j * 4], &d[(i - k - 1) + j * 4]);
							}
						}			
					}
				}
			}
			DRAW_SLEEP
		}
	}

	for (int j = 0; j < 4; j++)
	{
		bool ONCE = false;
		for (int i = 3; i > 0; i--)
		{
			if (d[i + j * 4] != 0)
			{
				if (d[(i - 1) + j * 4] != 0)
				{
					if (d[(i - 1) + j * 4] == d[i + j * 4])
					{
						d[i + j * 4] = get_score(d[i + j * 4] * 2);
						d[(i - 1) + j * 4] = 0;
						if (ONCE)
						{
							createThread_toBig_toSmall((i + 1) + j * 4);
						}
						else {
							createThread_toBig_toSmall(i + j * 4);
						}
						change2 = true;
						ONCE = true;
					}
				}
			}
		}
	}

	
	for (int j = 0; j < 4; j++)
	{
		for (int i = 3; i > 0; i--)
		{
			if (d[i + j * 4] == 0)
			{
				if (d[(i - 1) + j * 4] != 0)
				{
					t = i + 1;
					for (int k = 0; k < t; k++)
					{
						if (k == t - 1)
						{
							d[(i - k) + j * 4] = 0;
						}
						else {
							swap(&d[(i - k) + j * 4], &d[(i - k - 1) + j * 4]);
						}
					}
				}
			}
		}
		DRAW_SLEEP
	}
	
	if (change || change2)
	{
		return true;
	}
	else {
		return false;
	}
}
