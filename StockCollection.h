#ifndef _STOCKCOLLECTION_H_
#define _STOCKCOLLECTION_H_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

#include "Stock.h"

class StockCollection {
public:

  StockCollection();
  ~StockCollection();

  void Load(string _inputFile); 
  bool HasSymbol(string symbol);
  void Plot(string symbol);
  void PythonPlot(string symbol);

  vector<Stock>& GetAllStocks() { return allStocks; }

private:
  string inputFile;

  vector<Stock> allStocks;
};

#endif
