/*

   --- Day 9: All in a Single Night ---

   Every year, Santa manages to deliver all of his presents in a single night.

   This year, however, he has some new locations to visit; his elves have provided him the distances between every pair of locations. He can start and end at any two (different) locations he wants, but he must visit each location exactly once. What is the shortest distance he can travel to achieve this?

   For example, given the following distances:

   London to Dublin = 464
   London to Belfast = 518
   Dublin to Belfast = 141

   The possible routes are therefore:

   Dublin -> London -> Belfast = 982
   London -> Dublin -> Belfast = 605
   London -> Belfast -> Dublin = 659
   Dublin -> Belfast -> London = 659
   Belfast -> Dublin -> London = 605
   Belfast -> London -> Dublin = 982

   The shortest of these is London -> Dublin -> Belfast = 605, and so the answer is 605 in this example.

   What is the distance of the shortest route?

 */

#include "day9.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <regex>

using namespace std;



uint32_t
CalculateDistances(vector<string> Cities,
                   unordered_map<string, unordered_map<string, uint32_t> > Distances)
{
  uint32_t CurDistance = 0;
  for(auto it = Cities.begin(); it != (Cities.end() - 1); ++it)
  {
    //cout << *it << " ";
    CurDistance += Distances[*it][*(it + 1)];
  }
  //cout << Cities.back() << " : " << CurDistance <<  endl;

  //this_thread::sleep_for(chrono::milliseconds(50));

  return CurDistance;
}

int
main(int argc,
     char *argv[])
{
  int errcode = 0;

  string filename;
  if(argc == 2)
  {
    filename =  string(argv[1]);
  }
  else
  {
    filename = string("files/day9_test.txt");
  }
  cout << "Parsing file: '" << filename << "'." << endl;

  unordered_map< string, unordered_map<string, uint32_t> > Distances;

  auto filestream = ifstream(filename);
  if(!filestream.is_open())
  {
    cout << "Failed to open file." << endl;
    errcode = 1;
  }
  else
  {
    string FileLine;
    smatch m;
    regex regex("(.*) to (.*) = (.*)");
    while(getline(filestream, FileLine))
    {
      regex_match(FileLine, m, regex);
      Distances[m[1]][m[2]] = Distances[m[2]][m[1]] = (uint32_t) stoul(m[3]);
    }
  }


  auto Cities = vector<string>(Distances.size());
  transform(Distances.begin(), Distances.end(), Cities.begin(), [](auto p) {return p.first;});

  {
    uint32_t MinDistance = UINT32_MAX;
    uint32_t MaxDistance = 0;

    auto startTime = chrono::high_resolution_clock::now();
    do
    {
      uint32_t CurDistance = CalculateDistances(Cities, Distances);
      if(CurDistance < MinDistance)
      {
        MinDistance = CurDistance;
      }
      if(CurDistance > MaxDistance)
      {
        MaxDistance = CurDistance;
      }
    } while (next_permutation(Cities.begin(), Cities.end()));

    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = endTime - startTime;
    auto dt_single = diff.count();
    cout << "No Threading Time: " << dt_single << endl;
    cout << "Min: " << MinDistance << endl;
    cout << "Max: " << MaxDistance << endl;
  }

  {

    vector<ThreadPool::TaskFuture<uint32_t> > threads;
    auto startTime = chrono::high_resolution_clock::now();
    do
    {
      threads.push_back(submitJob(CalculateDistances, Cities, Distances));
    } while (next_permutation(Cities.begin(), Cities.end()));

    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = endTime - startTime;
    auto dt_threads = diff.count();
    cout << "Setup Time: " << dt_threads << endl;

    uint32_t MinDistance = UINT32_MAX;
    uint32_t MaxDistance = 0;

    startTime = chrono::high_resolution_clock::now();
    for(auto& item: threads)
    {
      uint32_t CurDistance = item.get();
      if(CurDistance < MinDistance)
      {
        MinDistance = CurDistance;
      }
      if(CurDistance > MaxDistance)
      {
        MaxDistance = CurDistance;
      }

    }
    endTime = chrono::high_resolution_clock::now();
    diff = endTime - startTime;
    dt_threads = diff.count();
    cout << "Threading Time: " << dt_threads << endl;
    cout << "Min: " << MinDistance << endl;
    cout << "Max: " << MaxDistance << endl;
  }


  cout << "Done." << endl;
  return errcode;
}
