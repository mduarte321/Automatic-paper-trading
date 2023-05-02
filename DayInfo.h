#ifndef _DAYINFO_H_
#define _DAYINFO_H_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

struct IndicatorData {
  double value;
  bool isValid;
};

class DayInfo {
public:

  DayInfo();
 
  string GetDate() { return date; } 
  double GetOpen() { return open; }
  double GetHigh() { return high; }
  double GetLow() { return low; }
  double GetClose() { return close; }
  double GetVolume() { return volume; }

  bool Load(ifstream& infile);
  void AddIndicator(string name, double v, bool vI);
  void SetScore(double sc, bool vS);
  double GetScore() { return score; }
  bool GetScoreValid() { return scoreValid; }
  void SetNNScore(double sc, bool vS);
  double GetNNScore() { return nnScore; }
  void SetNNInputs(vector<double>& inputs);

  //vector<double>& GetIndicators() { return indicators; }
  //vector<bool>& GetIndicatorValidity() { return validInicators; }

  map< string, IndicatorData >& GetIndicators() { return indicatorsMap; }
    double GetIndicator(string name);
private:
  string date;
  double open, high, low, close, adjClose, volume;

  double score;    //computed score
  bool scoreValid;
  double nnScore;    //computed score
  bool nnScoreValid;
  //vector<double> indicators;
  //vector<bool> validInicators;

  //perhaps for indicators it would be better to have something like
  //a map to store a string value and the corresponding indicator data (value, isvalid)
  //that way it's easier to change indicators in and out. can still iterate through them
  map< string, IndicatorData > indicatorsMap;
    
    //
};

#endif
