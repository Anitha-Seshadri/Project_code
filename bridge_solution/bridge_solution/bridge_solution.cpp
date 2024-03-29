
// bridge_solution.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <stdio.h>
#include <vector>
#include <exception>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/yaml.h>

#include <algorithm>
#include <iostream>

using namespace std;




float calcLeastTimeTakenForEachBridge(vector<float>& a)
{
	size_t sz = a.size();

	if (sz == 0) {
		return 0;
	}
	/*If there is only one hiker, minimum time taken to cross a bridge will be the time taken 
	  by the lone hiker */
	if (sz == 1) {
		return a[sz - 1];
	}

	/*If there are only two hikers, minimum time taken to cross a bridge will be the time taken
	  by the slowest hiker */
	if (sz == 2) {
		//return the max of 2 times taken 
		//because the slowest hiker will take longest time.
		return a[0] > a[1] ? a[0] : a[1];
	}

	float least = a[0];
	float sec_least = a[1];
	float total_time = 0;
	size_t arrlen = sz - 1;

	/*********************************************************************************************************************************************************************
	Algorithm used:
	1. Vector is sorted in ascending order
	2. least and sec_least, represent the hikers taking the least amount of time to cross the bridge. i.e First and second element in the sorted array.
	3. In every odd numbered iteration, least and sec_least cross the bridge and least comes back with the torch. Hence time taken to come back is kept minimal.
	4. In every even numbered iteration, last 2 hikers with highest times cross the bridge together. And sec_least returns back with the torch. Keeping return time minimal.
	5. This is continued till only least and sec_least hikers are remaining and they cross over the bridge at sec_least's time.
	6. Adding all these times together produces the least amount of time taken by all hikers to cross the bridge.	
	**********************************************************************************************************************************************************************/
	for (int i = 1; i < sz; ++i)
	{
		if (arrlen > 2)
		{
			if (i % 2 != 0)
			{
				total_time += sec_least;
				total_time += least;
			}
			else {
				total_time += a[arrlen];
				total_time += sec_least;
				arrlen -= 2;
			}

		}
		else {
			if (arrlen == 1) {

				total_time += sec_least;
			}
			else {

				/***********************************************************
				When 3 fastest hikers remain in the end:
				1. least and third_least travel together and least comes back: a[2] + a[0]
				2. least and sec_least travel together : a[1]
				The above 2 steps is equal to a[0] + a[1] + a[2]
				************************************************************/
				total_time += a[0];
				total_time += a[1];
				total_time += a[2];
				break;
			}
		}
	}

	
	return total_time;

}

float populateDataAndCalcTotalTimeTaken(std::string filename)
{
	float overall_time = 0;
	float individual_time = 0;
	vector<float> _hikerSpeedVector;
	vector<float> _hikerTimeVector;

	YAML::Node config = YAML::LoadFile(filename);
	
	//Access the yml file and for each bridge, add hikers to the _hikerSpeedVector
	cout << "Number of entries: " << config["bridges"].size() << endl;
	for (YAML::const_iterator it = config["bridges"].begin(); it != config["bridges"].end(); ++it)
	{
			float bridge_length = it->first.as<float>();

			for (size_t j = 0; j < it->second.size(); ++j)
			{
				float hiker_speed = it->second[j].as<float>();
				_hikerSpeedVector.push_back(hiker_speed);
			}
			
			/* Once we have the hikerspeeds for all the hikers who have to cross the bridge 
			   Populate the _hikerTimeVector with time taken by each hiker 
			*/
			for (size_t k = 0; k < _hikerSpeedVector.size(); ++k)
			{
				_hikerTimeVector.push_back(bridge_length / _hikerSpeedVector[k]);
			}
			//Sort _hikerTimeVector in ascending order 		
			std::sort(_hikerTimeVector.begin(), _hikerTimeVector.end());

			individual_time = calcLeastTimeTakenForEachBridge(_hikerTimeVector);
			
			overall_time += individual_time;

			cout << "Minimum time taken to cross the bridge of length: " << bridge_length << " is: " << individual_time << " mins"<<endl;
			_hikerTimeVector.clear();
	}

	return overall_time;
}

int main()
{

	std::string yml_filename = "../test/input.yml";
	float total = 0;
	try {
		total = populateDataAndCalcTotalTimeTaken(yml_filename);
	} catch (std::exception &e) {
		cout << "Failed to load yml file\n";
		return 0;
	}
	cout << "Minimum total time taken to cross all bridges: " << total <<" mins"<< endl;

}


