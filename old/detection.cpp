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
using namespace std;
#define LL long long

double calcBoundary(double firstMean, double firstStdDev, double secMean, double secStdDev)
{
	double boundary = firstMean + firstStdDev * (secMean-firstMean)/(firstStdDev+secStdDev);
	return boundary;
}

double findEntropy(int timesteps, vector<pair<int, int>> boatpositions) {
	int dx, dy;
	int p[9] = {}; //0-8 each index represents a move (not accounting for invis yet)

	//loop through the position array to create probability array
	for (int i = 1; i < timesteps; i++) 
	{
		dx = boatpositions[i].first - boatpositions[i - 1].first;
		dy = boatpositions[i].second - boatpositions[i - 1].second;

		if (dx == 1) 
		{
			if (dy == 1) 
			{
				p[2] = p[2] + 1;
			}
			else if (dy == -1) 
			{
				p[4]++;
			}
			else 
			{
				p[3]++;
			}
		}
		else if (dx == -1) 
		{
			if (dy == 1) 
				p[8] ++;

			else if (dy == -1) 
				p[6]++;

			else 
				p[7]++;
		}
		else if (dx == 0) 
		{
			if (dy == 1) 
			{
				p[1] ++;
			}
			else if (dy == -1) 
			{
				p[5]++;
			}
			else 
			{
				p[0]++;
			}
		}

		//std::cout << dx << ',' << dy<< std::endl;
	}

	double H = 0; double P;
	for (int j = 0; j <= 8; j++) 
	{
		P = p[j] / double(timesteps);
		if (P == 0) 
			continue;
		
		//std::cout<< "probability: " << P << std::endl;
		H += P * log2(1 / P);
	}
	return H;

}

float findEntropyBuffer(int buffersize, int timesteps, vector<pair<int, int>> boatpositions) {
	float H; float sum = 0; int counter = 0;
	vector<pair<int, int>> slicedPos;

	for (int t = 0; t + buffersize < timesteps; t++) 
	{ 
		//iterate over timesteps in boatpositions
		for (int i = 0; i < buffersize; i++) 
		{
			//iterate of history buffer
			slicedPos.push_back(boatpositions[t + i]);
		}

		H = findEntropy(buffersize, slicedPos);
		//cout << H << " " << t << endl;
		sum += H;
		counter++;

		if (slicedPos[buffersize - 1].first == -1) 
		{
			break;
		}
	}
	return sum / counter;
}


int main(void)
{
	// stats.csv must be a two-column, three row csv, 
	// with means in first col and std dev in second col 
	// AND IN ORDER FROM 1st TO 3rd

    int buffersize = 128;
    int timesteps = 5000;


	fstream thresholdfile;
	thresholdfile.open("thresholds.csv");

    string line,bound1,bound2,bound3;
    vector<float> thresholds;
	while (thresholdfile.good()){
		getline(thresholdfile,line,' ');
        
        stringstream s_stream(line);
        while(s_stream.good()){
            string substr;
            getline(s_stream,substr,',');
            thresholds.push_back(stof(substr));

        }
	}
    //cout<<thresholds[0]<<','<<thresholds[1]<<','<<thresholds[2];
	float firstBound = thresholds[1];float secondBound = thresholds[2];

// importing trajectories
    ifstream traj("trajectories.csv");
    if (!traj.is_open()) cout<< "ERROR: File is open." << '\n';

    vector<pair<int,int>> positions;
    string activity;
    getline(traj,activity,',');
    while (traj.good()){
        string x,y;
        for (int t = 0; t < timesteps; t++){
            getline(traj,x,',');
            getline(traj,y,',');
            positions.push_back({stoi(x),stoi(y)});
        }
    }



/*
	// read in a csv file of coords
	FILE *f2 = freopen("trajectory.csv", "r", stdin);
	string line; 
	int timesteps = 0;
	vector<pair<int, int>> boatpositions;
	while (getline(cin,line))
	{
		timesteps++;
		vector<string> result;
		stringstream s_stream(line); //create string stream from the string
		while (s_stream.good()) {
			string substr;
			getline(s_stream, substr, ','); //get first string delimited by comma
			result.push_back(substr);
		}

		pair<int, int> temp;
		temp.first = stoi(result[0]);
		temp.second = stoi(result[1].substr(1, result[1].length() - 1));
		boatpositions.push_back(temp);
	}
	fclose(f2);
*/

	// get entropy
	double e = 0;
	e = findEntropyBuffer(128,timesteps,boatpositions);

	
	if (e < firstBoundary)
	{
		cout << "chasing";
	}
	else if (e >= firstBoundary && e < secBoundary)
	{
		cout << "following";
	}
	else if (e >= secBoundary)
	{
		cout << "randomwalk";
	}

	return 0;
}
