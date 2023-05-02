#include "DayInfo.h"

DayInfo::DayInfo() {
  date = "";
  open = 0; high = 0; low = 0; close = 0; adjClose = 0; volume = 0;
  score = -1000;
  scoreValid = false;
  nnScore = 0;
  nnScoreValid = false;
}

bool DayInfo::Load(ifstream& infile) {
  string line;
  //int count = 0;
  getline(infile, line);

  if(line.length()==0) return false;

  size_t stDate = line.find(",");
  date = line.substr(0, stDate);
  line.erase(0, stDate + 1);

  size_t stOpen = line.find(",");
  open = stod(line.substr(0, stOpen));
  line.erase(0, stOpen + 1);

  size_t stHigh = line.find(",");
  high = stod(line.substr(0, stHigh));
  line.erase(0, stHigh + 1);

  size_t stLow = line.find(",");
  low = stod(line.substr(0, stLow));
  line.erase(0, stLow + 1);

  size_t stClose = line.find(",");
  close = stod(line.substr(0, stClose));
  line.erase(0, stClose + 1);

  size_t stVolume = line.find(",");
  volume = stod(line.substr(0, stVolume));
  line.erase(0, stVolume);

  return true;
}
  
void DayInfo::AddIndicator(string name, double v, bool vI) {
  //indicators.push_back(v);
  //validInicators.push_back(vI);
  IndicatorData id;
  id.value = v;
  id.isValid = vI;
  indicatorsMap[name] = id;
}

// +/////////////////////////////////////////////////


  
// Function to return the maximum profit
int maxProfit(int price[], int start, int end)
{
    // Initialise the profit
        int profit = 0;
    // The day at which the stock must be bought
    for (int i = start; i < end; i++) {
  
        // The day at which the stock must be sold
        for (int j = i + 1; j <= end; j++) {
  
            // If buying the stock at ith day and
            // selling it at jth day is profitable
            if (price[j] > price[i]) {
  
                // Update the current profit
                int curr_profit
                    = price[j] - price[i]
                      + maxProfit(price, start, i - 1)
                      + maxProfit(price, j + 1, end);
  
                // Update the maximum profit so far
                profit = max(profit, curr_profit);
            }
        }
    }
    return profit;
}




// +//////////////////////////////////////////////////



double DayInfo::GetIndicator(string name) {
    return indicatorsMap[name].value;
}

void DayInfo::SetScore(double _sc, bool _vS) {
  score = _sc;
  scoreValid = _vS;
}

void DayInfo::SetNNScore(double _sc, bool _vS) {
  nnScore = _sc;
  nnScoreValid = _vS;
}
  
void DayInfo::SetNNInputs(vector<double>& inputs) {
  inputs.clear();
  inputs.push_back(GetOpen());  //1
  inputs.push_back(GetHigh());  //2
  inputs.push_back(GetLow());   //3
  inputs.push_back(GetClose()); //4
  inputs.push_back(GetVolume());//5

  
  map< string, IndicatorData >& indicators = indicatorsMap;
  map< string, IndicatorData >::iterator it;
  //trying to add rsi14 
  it = indicators.find("rsi14");
  if( it != indicators.end() ) {
    inputs.push_back( it->second.value );//6
  }
  //trying to add rsi28 
  it = indicators.find("rsi28");
  if( it != indicators.end() ) {
    inputs.push_back( it->second.value );//7
  }
  it = indicators.find("ma5");
  if( it != indicators.end() ) {
    inputs.push_back( it->second.value );//8
  }
  it = indicators.find("bb5up");
  if( it != indicators.end() ) {
    inputs.push_back( it->second.value );//9
  }
  it = indicators.find("bb5dn");
  if( it != indicators.end() ) {
    inputs.push_back( it->second.value );//10
  }
  it = indicators.find("ma10");
  if( it != indicators.end() ) {
    inputs.push_back( it->second.value );//11
  }
  it = indicators.find("bb10up");
  if( it != indicators.end() ) {
    inputs.push_back( it->second.value );//12
  }
  it = indicators.find("bb10dn");
  if( it != indicators.end() ) {
    inputs.push_back( it->second.value );//13
  }
  it = indicators.find("ma20");
  if( it != indicators.end() ) {
    inputs.push_back( it->second.value );//14
  }
  it = indicators.find("bb20up");
  if( it != indicators.end() ) {
    inputs.push_back( it->second.value );//15
  }
  it = indicators.find("bb20dn");
  if( it != indicators.end() ) {
    inputs.push_back( it->second.value );//16
  }
  it = indicators.find("ma40");
  if( it != indicators.end() ) {
    inputs.push_back( it->second.value );//17
  }
  it = indicators.find("bb40up");
  if( it != indicators.end() ) {
    inputs.push_back( it->second.value );//18
  }
  it = indicators.find("bb40dn");
  if( it != indicators.end() ) {
    inputs.push_back( it->second.value );//19
  }
}
