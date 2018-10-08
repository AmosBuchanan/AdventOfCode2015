/**

   --- Day 10: Elves Look, Elves Say ---

   Today, the Elves are playing a game called look-and-say. They take turns
      making sequences by reading aloud the previous sequence and using that
      reading as the next sequence. For example, 211 is read as "one two, two
      ones", which becomes 1221 (1 2, 2 1s).

   Look-and-say sequences are generated iteratively, using the previous value as
      input for the next step. For each step, take the previous value, and
      replace each run of digits (like 111) with the number of digits (3)
      followed by the digit itself (1).

   For example:

    1 becomes 11 (1 copy of digit 1).
    11 becomes 21 (2 copies of digit 1).
    21 becomes 1211 (one 2 followed by one 1).
    1211 becomes 111221 (one 1, one 2, and two 1s).
    111221 becomes 312211 (three 1s, two 2s, and one 1).

   Starting with the digits in your puzzle input, apply this process 40 times.
      What is the length of the result?


 **/

#include <stdio.h>
#include <string>
#include <iostream>
#include <tuple>

using namespace std;

struct SequenceData
{
  uint64_t Length;
  string ResultSequence;
};

SequenceData
GetSequenceString(const string Sequence)
{
  uint32_t StrLen = Sequence.size();
  string ResultSequence = "";
  for(int CurCharIndex = 0; CurCharIndex < StrLen; ++CurCharIndex)
  {
    uint8_t CurChar = Sequence[CurCharIndex];
    uint32_t SequenceLength = 1;
    while(CurCharIndex < (StrLen - 1) && CurChar == Sequence[CurCharIndex + 1])
    {
      SequenceLength++;
      CurCharIndex++;
    }
    ResultSequence.append(to_string(SequenceLength));
    ResultSequence.push_back(CurChar);
  }
  SequenceData Result = {ResultSequence.size(), ResultSequence};

  return Result;
}

SequenceData
DoIterations(const string Sequence,
             int NumIterations,
             bool isTesting)
{
  SequenceData Result{0, Sequence};
  for(int i = 0; i < NumIterations; i++)
  {
    Result = GetSequenceString(Result.ResultSequence);
    if(isTesting)
    {
      cout << Result.ResultSequence << endl;
    }
  }

  return Result;
}

int
main(int argc,
     char *argv[])
{
  int errcode = 0;
  printf("Day10\n");

  string StartSequence;
  uint32_t NumIterations;
  bool isTest = false;
  if(argc == 3)
  {
    StartSequence = string{argv[1]};
    NumIterations = stoul(argv[2]);
  }
  else
  {
    StartSequence = "1";
    NumIterations = 5;
    isTest = true;
  }

  cout << "Start: " << StartSequence << " Iterations: " << NumIterations << " Testing: " << isTest << endl;

  auto [SequenceLength, ResultSequence]  = DoIterations(StartSequence, NumIterations, isTest);

  if(isTest)
  {
    cout << "Final Sequence: " << ResultSequence << " ";
  }

  cout << "Length: " << SequenceLength << endl;

  return errcode;
}
