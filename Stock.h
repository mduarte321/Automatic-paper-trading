#ifndef _STOCK_H_
#define _STOCK_H_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>
using namespace std;

#include "DayInfo.h"

class Stock {
public:

  Stock();
  ~Stock();

  //void IncreaseSize();
  //void Insert(DayInfo& _di);
  void Load(string _symbolName, string _symbolDesc, string _inputFile); 
  string GetSymbol() { return symbolName; }
  string GetDescription() { return symbolDesc; }

  void ComputeIndicatorsAndScore();
  double ComputeSimpleMovingAvg(int _index, int _N, double& stdDev, bool& isValid);
  double ComputeRSI(int _index, int _N, bool& isValid);
  double ComputeScore(int _index, int _N, bool& isValid);

  void PrepareData(vector<double>& plotData);
  void WritePythonPlotFile(string filename);
  void PrepareData(vector<double>& plotData, vector<double>& ma5Data, vector<double>& ma10Data, vector<double>& ma20Data, vector<double>& ma40Data, vector<double>& rsi14Data, vector<double>& scoreData);
  void PrepareNNData(vector< vector<double> >& allInputs /*indicators*/,
                     vector< double >& allOutputs);

  int GetNumDayInfo() { return data.size(); }
  DayInfo& GetDayInfo(int index) { return data[index]; }
    
    //
private:
  string symbolName;
  string symbolDesc;
  string inputFile;

  //int length;
  //int arrSize;
  //DayInfo* data;
  vector<DayInfo> data;

  vector<string> indicatorStr;
};

#endif
