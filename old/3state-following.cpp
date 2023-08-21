#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <utility>
#include <bitset>
#include <cmath>	
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <climits>
#include <stdlib.h>
#include <chrono>
#include <thread>

//3 states of random, chasing, and zigzag each randomly selected in blocks of some size

using namespace std;
#define LL long long

bool invis = false;

float slope(int x1, int y1, int x2, int y2)
{
	return float((y2 - y1)) / (x2 - x1);
}

bool inRed(int curX, int curY)
{
	if (curX >= 1792 && curX <= 2304)
	{
		if (curY >= 1792 && curY <= 2304)
		{
			return true;
		}
	}
	return false;
}

std::pair<int, int> bresenham1step(int x1, int y1, int x2, int y2) // for slopes <= 1
{

	int o45 = 0, o8 = 0;

	if (x2 - x1 < 0 && y2 - y1>0) {
		x2 += 2 * (x1 - x2);
		o45 = 1;

	}
	else if (x2 - x1 < 0 && y2 - y1 < 0) {
		x2 += 2 * (x1 - x2);
		y2 += 2 * (y1 - y2);
		o45 = 1;
		o8 = 1;
	}
	else if (x2 - x1 > 0 && y2 - y1 < 0) {
		y2 += 2 * (y1 - y2);
		o8 = 1;
	}

	std::pair<int, int> temp = { x1,y1 };

	int m_new = 2 * (y2 - y1);
	int slope_error_new = m_new - (x2 - x1);
	for (int x = x1, y = y1, i = 0; x <= x2 && i < 2; x++, i++)
	{
		//std::cout << "(" << x - o45*(2*(x-x1)) << "," << y - o8*(2*(y-y1)) << ")\n";
		//myfile << std::to_string(x - o45*(2*(x-x1))) + ", " + std::to_string(y - o8*(2*(y-y1))) + "\n";
		temp = { x - o45 * (2 * (x - x1)),y - o8 * (2 * (y - y1)) };

		// Add slope to increment angle formed
		slope_error_new += m_new;

		// Slope error reached limit, time to
		// increment y and update slope error.
		if (slope_error_new >= 0)
		{
			y++;
			slope_error_new -= 2 * (x2 - x1);
		}

	}
	return temp;
}

std::pair<int, int> bresenham2step(int x1, int y1, int x2, int y2) //for reflected slopes of > 1
{


	int o36 = 0, o7 = 0;

	if (x2 - x1 < 0 && y2 - y1>0) {
		x2 += 2 * (x1 - x2);
		o36 = 1;
	}
	else if (x2 - x1 < 0 && y2 - y1 < 0) {
		x2 += 2 * (x1 - x2);
		y2 += 2 * (y1 - y2);
		o36 = 1;
		o7 = 1;
	}
	else if (x2 - x1 > 0 && y2 - y1 < 0) {
		y2 += 2 * (y1 - y2);
		o7 = 1;
	}

	std::pair<int, int> temp = { x1,y1 };

	int m_new = 2 * (x2 - x1);
	int slope_error_new = m_new - (y2 - y1);
	for (int x = x1, y = y1, i = 0; y <= y2 && i < 2; y++, i++)
	{
		//std::cout << "(" << x - o36*(2*(x-x1)) << "," << y - o7*(2*(y-y1)) << ")\n";
		//myfile << std::to_string(x - o36*(2*(x-x1))) + ", " + std::to_string(y - o7*(2*(y-y1))) + "\n";
		temp = { x - o36 * (2 * (x - x1)), y - o7 * (2 * (y - y1)) };
		// Add slope to increment angle formed
		slope_error_new += m_new;

		// Slope error reached limit, time to
		// increment y and update slope error.
		if (slope_error_new >= 0)
		{
			x++;
			slope_error_new -= 2 * (y2 - y1);
		}

	}
	return temp;
}

std::pair<int, int> bresenhamstep(int x1, int y1, int x2, int y2) {
	if (x1 == x2) {
		int dir = (y2 - y1) / abs(y2 - y1);
		return { x1,y1 + dir };
	}
	else {
		float m = slope(x1, y1, x2, y2);

		if (abs(m) <= 1) {
			return bresenham1step(x1, y1, x2, y2);
		}
		else {
			return bresenham2step(x1, y1, x2, y2);
		}
	}
}

std::pair<int, int> randMove(int x, int y) // note: x is positive towards right, y is positive downwards
{

	int gen = rand() % 10 + 1;
	if (gen == 1)
	{
		// northwest
		x--; y--;
	}
	if (gen == 2)
	{
		// north
		y--;
	}
	if (gen == 3) {
		// northeast
		x++;  y--;
	}
	if (gen == 4)
	{
		//west
		x--;
	}
	if (gen == 5)
	{
		//stay still
	}
	if (gen == 6)
	{
		// east
		x++;
	}
	if (gen == 7)
	{
		// southwest
		x--; y++;
	}
	if (gen == 8)
	{
		//south
		y++;
	}
	if (gen == 9)
	{
		// southeast
		x++; y++;
	}
	if (gen == 10)
	{
		// invisible
		// invis = true;
	}

	if (x < 0 || x > 4096 || y < 0 || y > 4096)
		return randMove(x, y);
	else
		return { x,y };
}

pair<int, int> zigzag(int curX, int curY)
{
	// zigzag
	int i = 15 + rand() % 10;

	// randomly choose a cardinal direction to zig zag in
	int direction = rand() % 4 + 1;
	pair<int, int> move1, move2, move3 = { 0,0 };
	if (direction == 1)
	{
		// north
		move1 = { -1,1 };
		move2 = { 1,1 };
	}
	if (direction == 2)
	{
		// south
		move1 = { -1,-1 };
		move2 = { 1,-1 };
	}
	if (direction == 3)
	{
		// east
		move1 = { 1,-1 };
		move2 = { 1,1 };
	}
	if (direction == 4)
	{
		// west
		move1 = { -1,-1 };
		move2 = { -1,1 };
	}
	bool failure = false;

	
	int j = 0;
	
	while (j < i)
	{
		int segmentLength = i / 5 + rand() % 3;
		j += segmentLength;
		if (j > i)
			segmentLength -= j - i;

		int randMoveType = rand() % 2 + 1;
		pair<int, int> curMove;
		if (randMoveType == 1)
			curMove = move1;
		else if (randMoveType == 2)
			curMove = move2;
		else if (randMoveType == 3)
			curMove = move3;

		for (int k = 0; k < segmentLength; k++)
		{
			int x = curX; int y = curY;

			x += curMove.first; y += curMove.second;

			if (x < 0 || x > 4096 || y < 0 || y > 4096)
			{
				failure = true;
				break;
			}
			else
			{
				curX = x; curY = y;
				cout << curX << ", " << curY << "\n";

				if (curX == 2047 && curY == 2047)
					return { curX,curY };
			}
		}

		if (failure)
			break;
	}
	return { curX,curY };
}

int main(void)
{
	freopen("out.csv", "w", stdout); // opens file to write to
	// allows us to use std output to write to the file
	int curX = 3000, curY = 3000;

	while (make_pair(curX, curY) != make_pair(2047, 2047))
	{

		pair<int, int> temp;

		// randomly choose one of three states: chasing, randomwalk, and zig-zag thing
		int choice = rand() % 3 + 1;


		if (choice == 1 || choice == 2)
		{
			for (int i = 0; i < 15 + rand() % 10; i++)
			{
				if (choice == 1)
				{
					/*
					//chase/correction
					int r = rand() % 10;
					if (r == 0)
					{
						// random walk
						temp = randMove(curX, curY);
					}
					else
					*/
						temp = bresenhamstep(curX, curY, 2047, 2047);
				}
				else if (choice == 2)
				{
					// random walk
					temp = randMove(curX, curY);
				}

				curX = temp.first; curY = temp.second;
				//write time and coords to a file
				cout << curX << ", " << curY << "\n";

				if (curX == 2047 && curY == 2047)
					break;
			}
		}
		else if (choice == 3)
		{
			auto temp = zigzag(curX, curY);
			curX = temp.first; curY = temp.second;
		}

		if (curX == 2047 && curY == 2047) // or some other condition
			break;
	}
	return 0;
}
