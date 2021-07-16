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

double minDistance(pair<int,int> A, pair<int, int> B, pair<int, int> E) // gets minimum distance from point E to line segment AB
{

	// vector AB
	pair<double, double> AB;
	AB.first = B.first - A.first;
	AB.second = B.second - A.second;

	// vector BP
	pair<double, double> BE;
	BE.first = E.first - B.first;
	BE.second = E.second - B.second;

	// vector AP
	pair<double, double> AE;
	AE.first = E.first - A.first,
	AE.second = E.second - A.second;

	// Variables to store dot product
	double AB_BE, AB_AE;

	// Calculating the dot product
	AB_BE = (AB.first * BE.first + AB.second * BE.second);
	AB_AE = (AB.first * AE.first + AB.second * AE.second);

	// Minimum distance from
	// point E to the line segment
	double reqAns = 0;

	// Case 1
	if (AB_BE > 0) 
	{
		// Finding the magnitude
		double y = E.second - B.second;
		double x = E.first - B.first;
		reqAns = sqrt(x * x + y * y);
	}
	else if (AB_AE < 0) 
	{
		double y = E.second - A.second;
		double x = E.first - A.first;
		reqAns = sqrt(x * x + y * y);
	}
	else 
	{
		// Finding the perpendicular distance
		double x1 = AB.first;
		double y1 = AB.second;
		double x2 = AE.first;
		double y2 = AE.second;
		double mod = sqrt(x1 * x1 + y1 * y1);
		reqAns = abs(x1 * y2 - y1 * x2) / mod;
	}
	return reqAns;
}

/*
pair<int, int> fixBresenham(pair<int, int> b)
{
	pair<int, int> adjacentPoint1, adjacentPoint2;

	// get adj points

	double dist1 = minDistance({ 0,0 }, b, adjacentPoint1);
	double dist2 = minDistance({ 0,0 }, b, adjacentPoint2);
	if (dist1 <= dist2)
		return adjacentPoint1;
	else
		return adjacentPoint2;
}
*/

pair<int, int> randFollowMove(int curX, int curY, int goalX, int goalY)
{
	// 30% chance do bresenham
	// 5% for each of invis or 3 directions away
	// everything else 10%
	pair<int, int> t = bresenhamstep(curX, curY, 2047, 2047);
	pair<int, int> chaseMove = { t.first - curX, t.second - curY };
	//chaseMove = fixBresenham(chaseMove);

	// set up randomizer list
	map<pair<int, int>,bool> found;
	vector<pair<int, int>> randomizedList;
	randomizedList.push_back(chaseMove);
	randomizedList.push_back({ -1 * chaseMove.first, -1 * chaseMove.second }); // 5%
	found[chaseMove] = true; found[{ -1 * chaseMove.first, -1 * chaseMove.second }] = true;

	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
		{
			int totalDiff = abs(chaseMove.first - i) + abs(chaseMove.second - j);
			if (totalDiff == 3) // if chase was north, this would be SW/SE
			{
				randomizedList.push_back({ i,j });
				found[{i, j}] = true;
			}
		}
	
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
		{
			if (!found[{i, j}])
			{
				randomizedList.push_back({ i,j });
			}
		}
	

	pair<int, int> randMove = { curX, curY };

	do
	{
		// note: commented directions are assuming bresenham outputs (0,1) as move
		// randomizing
		int moveNum = rand() % 20 + 1;
		if (moveNum > 0 && moveNum <= 6)
		{
			// go with chase
			randMove = { curX + chaseMove.first, curY + chaseMove.second };
		}
		else if (moveNum == 7)
		{
			// invis
			invis = true;
		}
		else if (moveNum == 8)
		{
			// south
			randMove = { curX + randomizedList[1].first, curY + randomizedList[1].second };
		}
		else if (moveNum == 9)
		{
			// south west
			randMove = { curX + randomizedList[2].first, curY + randomizedList[2].second };
		}
		else if (moveNum == 10)
		{
			// south east * 9 and 10 may be swapped
			randMove = { curX + randomizedList[3].first, curY + randomizedList[3].second };
		}
	
		else if (moveNum > 10 && moveNum <= 12) // from here on out is all 10%
			randMove = { curX + randomizedList[4].first, curY + randomizedList[4].second };
		else if (moveNum > 12 && moveNum <= 14)
			randMove = { curX + randomizedList[5].first, curY + randomizedList[5].second };
		else if (moveNum > 14 && moveNum <= 16)
			randMove = { curX + randomizedList[6].first, curY + randomizedList[6].second };
		else if (moveNum > 16 && moveNum <= 18)
			randMove = { curX + randomizedList[7].first, curY + randomizedList[7].second };
		else if (moveNum > 18 && moveNum <= 20)
			randMove = { curX + randomizedList[8].first, curY + randomizedList[8].second };
		
	} while ((randMove.first < 0 || randMove.first > 4096)
		|| (randMove.second < 0 || randMove.second > 4096));

	return randMove;
}

int main(void)
{
	freopen("out.csv", "w", stdout); // opens file to write to
	// allows us to use std output to write to the file

	int curX = 2047, curY = 1790; // note: x is positive towards right, y is positive downwards

	for (int time = 0; time < 5000; time++)
	{
		//randomly move
		pair<int, int> temp = randFollowMove(curX, curY, 2047, 2047);
		curX = temp.first; curY = temp.second;

		//write time and coords to a file
		cout << curX << ", " << curY << "\n";
		
		/*
		if (invis)
		{
			cout << "   invisible";
			invis = false;
		}
		cout << endl;
		*/

		if (time == 5000 || (curX == 2047 && curY == 2047)) // or some other condition
			break;

		//std::this_thread::sleep_for(std::chrono::seconds(1)); // sleep for one second (commented out for speed's sake)
		if (time % 500 == 0)
		{
			int fivehundred = true;
		}
	}
	return 0;
}