#include "Stock.h"

Stock::Stock() {
  //int length = 0;
  //arrSize = 5;
  //data = new DayInfo[arrSize];
  inputFile = "";
}

Stock::~Stock() {
  //delete [] data;
}

void Stock::Load(string _symbolName, string _symbolDesc, string _inputFile) {
  symbolName = _symbolName;
  symbolDesc = _symbolDesc;
  inputFile = _inputFile;
  ifstream infile;
  infile.open(inputFile.c_str());
  while(infile) {
    DayInfo di;
    bool valid = di.Load(infile);
    if(!valid) break;
    else {
      cout << "Loaded d = " << di.GetDate() << " o = " << di.GetOpen() << " h = " << di.GetHigh() << " l = " << di.GetLow() << " c = " << di.GetClose() << " v = " << di.GetVolume() << endl;
      
      //Insert(di); //below a much simpler implementation
      data.push_back(di);
    }
  }//endwhile

  ComputeIndicatorsAndScore();
}

void Stock::ComputeIndicatorsAndScore() {
  indicatorStr.push_back("ma5");
  indicatorStr.push_back("ma10");
  indicatorStr.push_back("ma20");
  indicatorStr.push_back("ma40");
  for(int i=0; i<int(data.size()); i++) {
    bool isValid5;
    double stdDev;
    double ma5 = ComputeSimpleMovingAvg(i, 5, stdDev, isValid5);
    data[i].AddIndicator("ma5", ma5, isValid5);
    data[i].AddIndicator("bb5up", ma5+2*stdDev, isValid5);
    data[i].AddIndicator("bb5dn", ma5-2*stdDev, isValid5);
    bool isValid10;
    double ma10 = ComputeSimpleMovingAvg(i, 10, stdDev, isValid10);
    data[i].AddIndicator("ma10", ma10, isValid10);
    data[i].AddIndicator("bb10up", ma10+2*stdDev, isValid10);
    data[i].AddIndicator("bb10dn", ma10-2*stdDev, isValid10);
    bool isValid20;
    double ma20 = ComputeSimpleMovingAvg(i, 20, stdDev, isValid20);
    data[i].AddIndicator("ma20", ma20, isValid20);
    data[i].AddIndicator("bb20up", ma20+2*stdDev, isValid20);
    data[i].AddIndicator("bb20dn", ma20-2*stdDev, isValid20);
    bool isValid40;
    double ma40 = ComputeSimpleMovingAvg(i, 40, stdDev, isValid40);
    data[i].AddIndicator("ma40", ma40, isValid40);
    data[i].AddIndicator("bb40up", ma40+2*stdDev, isValid40);
    data[i].AddIndicator("bb40dn", ma40-2*stdDev, isValid40);
    bool isValidRSI14;
    double rsi14 = ComputeRSI(i,14, isValidRSI14);
    data[i].AddIndicator("rsi14", rsi14, isValidRSI14);
    bool isValidRSI28;
    double rsi28 = ComputeRSI(i,28, isValidRSI28);
    data[i].AddIndicator("rsi28", rsi28, isValidRSI28);
    
    bool isValidSc;
    double score = ComputeScore(i, 10, isValidSc);
    data[i].SetScore(score, isValidSc);
  }//endfor i
    

}

double Stock::ComputeSimpleMovingAvg(int _index, int _N, 
                                     double& stdDev, bool& isValid) {
  isValid = true;
  stdDev = 0;
  if( (_index - _N+1) < 0 ) {
    isValid = false;
    return -1;
    //return -10;
  }
  int count = 0;
  double sum = 0;
  for(int sI = max(0,_index - _N+1); sI<=_index; sI++) {
    sum += data[sI].GetClose();
    count++;
  }
  double avg = sum/count;

  sum = 0;
  for(int sI = max(0,_index - _N+1); sI<=_index; sI++) {
    sum += pow(data[sI].GetClose()-avg,2.0);
  }
  
  stdDev = sqrt( sum / count );
  
  return avg;
}

//compute a simple average of values in a vector
double averageValues(vector<double>& values) {
  double sum = 0;
  for(int i=0; i<(int)values.size(); i++) {
    sum += values[i];
  }//endfor i
  return sum/values.size();
}

double Stock::ComputeRSI(int _index, int _N, bool& isValid) {
  isValid = true;
  if( (_index - _N+1) < 0 ) {
    isValid = false;
    return -1;
    //return -10;
  }
  int count = 0;
  vector<double> gains;
  vector<double> losses;
  bool validGain = false;
  bool validLoss = false;
  for(int sI = max(1,_index - _N+1); sI<=_index; sI++) {
    double diff = data[sI].GetClose() - data[sI-1].GetClose();
    count++;
    if( diff > 0 ) { // gain
      gains.push_back( diff );
      losses.push_back( 0 );
      validGain = true;
    }
    else { // loss
      gains.push_back( 0 );
      losses.push_back( fabs(diff) );
      validLoss = true;
    }
  }//endfor sI

  double rsi = 0;
  if( validGain && validLoss ) { //avg gain&loss vector contain valid data
    double firstAvgGain = averageValues(gains);
    double firstAvgLoss = averageValues(losses);
    double computedGain = ((_N-1)*firstAvgGain + gains[ gains.size()-1 ])/_N; 
    double computedLoss = ((_N-1)*firstAvgLoss + losses[ losses.size()-1 ])/_N; 
    double RS = computedGain / computedLoss;
    rsi = 100 - 100/(1+RS);
  }
  else if( validGain ) { //no losses
    rsi = 100;
  }
  else if( validLoss ) { //no losses
    rsi = 0;
  }
  else {
    isValid = false;
  }

  return rsi;
}

double Stock::ComputeScore(int _index, int _N, bool& isValid) {
  isValid = true;
  if( (_index + _N) >= (int)data.size() ) {
    isValid = false;
    return 0;
    //return -10;
  }
  int count = 0;
  double score = 0;
  for(int sI = _index+1; sI<=_index+_N; sI++) {
    //day over start of period
    if( data[_index].GetClose() < data[sI].GetClose() ) score += 1.0;
    else score -= 1.0;
    count++;
    
    //day over previous day
    if( data[sI-1].GetClose() < data[sI].GetClose() ) score += 1.0;
    else score -= 1.0;
    count++;
  }
  return score/count;
}

void Stock::PrepareData(vector<double>& plotData) {
  plotData.clear();
  for(int i=0; i<int(data.size()); i++) {
    plotData.push_back( data[i].GetClose() );
  }
}
 
//prepares data for plotting 
void Stock::PrepareData(vector<double>& plotData, vector<double>& ma5Data, vector<double>& ma10Data, vector<double>& ma20Data, vector<double>& ma40Data, vector<double>& rsi14Data, vector<double>& scoreData) {
  plotData.clear();
  ma5Data.clear();
  ma10Data.clear();
  ma20Data.clear();
  ma40Data.clear();
  rsi14Data.clear();
  scoreData.clear();
  for(int i=0; i<int(data.size()); i++) {
    plotData.push_back( data[i].GetClose() );
 
    /* 
    vector<double>& indicators =  data[i].GetIndicators();
    if(indicators.size()>=1) ma5Data.push_back( indicators[0] );
    if(indicators.size()>=2) ma10Data.push_back( indicators[1] );
    if(indicators.size()>=3) ma20Data.push_back( indicators[2] );
    if(indicators.size()>=4) ma40Data.push_back( indicators[3] );
    */
    
    map< string, IndicatorData >& indicators =  data[i].GetIndicators(); 
    map< string, IndicatorData >::iterator it;
    it = indicators.find("ma5");
    if( it != indicators.end() ) {
      ma5Data.push_back( it->second.value );
    }
    it = indicators.find("ma10");
    if( it != indicators.end() ) {
      ma10Data.push_back( it->second.value );
    }
    it = indicators.find("ma20");
    if( it != indicators.end() ) {
      ma20Data.push_back( it->second.value );
    }
    it = indicators.find("ma40");
    if( it != indicators.end() ) {
      ma40Data.push_back( it->second.value );
    }
    it = indicators.find("rsi14");
    if( it != indicators.end() ) {
      rsi14Data.push_back( it->second.value );
    }

    scoreData.push_back( data[i].GetScore() );
  }
}

void Stock::WritePythonPlotFile(string filename) {
  ofstream outfile;
  outfile.open(filename.c_str());
  outfile << "Date,Open,High,Low,Close,Volume,ma5,ma10,ma20,ma40,rsi14,rsi28,bb5up,bb5dn,bb10up,bb10dn,bb20up,bb20dn,bb40up,bb40dn,score" << endl;
  for(int i=0; i<int(data.size()); i++) {
    outfile << data[i].GetDate() << ",";
    outfile << data[i].GetOpen() << ","; 
    outfile << data[i].GetHigh() << ","; 
    outfile << data[i].GetLow() << ","; 
    outfile << data[i].GetClose() << ","; 
    outfile << data[i].GetVolume() << ","; 
    
    map< string, IndicatorData >& indicators =  data[i].GetIndicators(); 
    map< string, IndicatorData >::iterator it;
    it = indicators.find("ma5");
    if( it != indicators.end() ) {
      outfile << it->second.value << ",";
    }
    else outfile << ",";
    it = indicators.find("ma10");
    if( it != indicators.end() ) {
      outfile << it->second.value << ",";
    }
    else outfile << ",";
    it = indicators.find("ma20");
    if( it != indicators.end() ) {
      outfile << it->second.value << ",";
    }
    else outfile << ",";
    it = indicators.find("ma40");
    if( it != indicators.end() ) {
      outfile << it->second.value << ",";
    }
    else outfile << ",";
    it = indicators.find("rsi14");
    if( it != indicators.end() ) {
      outfile << it->second.value << ",";
    }
    else outfile << ",";
    it = indicators.find("rsi28");
    if( it != indicators.end() ) {
      outfile << it->second.value << ",";
    }
    else outfile << ",";
    it = indicators.find("bb5up");
    if( it != indicators.end() ) {
      outfile << it->second.value << ",";
    }
    else outfile << ",";
    it = indicators.find("bb5dn");
    if( it != indicators.end() ) {
      outfile << it->second.value << ",";
    }
    else outfile << ",";
    it = indicators.find("bb10up");
    if( it != indicators.end() ) {
      outfile << it->second.value << ",";
    }
    else outfile << ",";
    it = indicators.find("bb10dn");
    if( it != indicators.end() ) {
      outfile << it->second.value << ",";
    }
    else outfile << ",";
    it = indicators.find("bb20up");
    if( it != indicators.end() ) {
      outfile << it->second.value << ",";
    }
    else outfile << ",";
    it = indicators.find("bb20dn");
    if( it != indicators.end() ) {
      outfile << it->second.value << ",";
    }
    else outfile << ",";
    it = indicators.find("bb40up");
    if( it != indicators.end() ) {
      outfile << it->second.value << ",";
    }
    else outfile << ",";
    it = indicators.find("bb40dn");
    if( it != indicators.end() ) {
      outfile << it->second.value << ",";
    }
    else outfile << ",";

    outfile << data[i].GetScore() << endl;

  }

  outfile.close();
}
 
//prepares data for being send into fann (actuall, making the file) 
void Stock::PrepareNNData(vector< vector<double> >& allInputs /*indicators*/,
                          vector< double >& allOutputs) {
  cout << "PrepareNNData stock = " << symbolName
       << " start input size = " << allInputs.size()
       << " start output size = " << allOutputs.size() << endl;
  for(int i=0; i<(int)data.size(); i++) {
    //vector<double>& indicators =  data[i].GetIndicators(); 
    //vector<bool>& indicatorValidity = data[i].GetIndicatorValidity();
    map< string, IndicatorData >& indicators =  data[i].GetIndicators(); 
    map< string, IndicatorData >::iterator it;
    bool allIndValid = true;
    bool scoreValid = data[i].GetScoreValid();
    //for(int j=0; j<(int)indicatorValidity.size(); j++) {
    //  if( ! indicatorValidity[j] ) {
    //	allIndValid = false;
    //  }
    //}//endfor j
    for(it=indicators.begin(); it!=indicators.end(); it++) {
      if( ! it->second.isValid )
	allIndValid = false;
    }//endfor it


    if( allIndValid && scoreValid ) {
      vector<double> inputs;
      data[i].SetNNInputs(inputs);
      /*
      inputs.push_back(data[i].GetOpen());  //1
      inputs.push_back(data[i].GetHigh());  //2
      inputs.push_back(data[i].GetLow());   //3
      inputs.push_back(data[i].GetClose()); //4
      inputs.push_back(data[i].GetVolume());//5
     
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
      */
      
      allInputs.push_back( inputs );
      //allInputs.push_back( indicators );
      allOutputs.push_back( data[i].GetScore() );
    }
  }//endfor i
  
  cout << "PrepareNNData stock = " << symbolName
       << " start input size = " << allInputs.size()
       << " start output size = " << allOutputs.size() << endl;

}
