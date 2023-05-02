#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#include "DayInfo.h"
#include "Stock.h"
#include "StockCollection.h"
#include "Learner.h"

int main() {

  StockCollection sc;
  //sc.Load("../russell_3000_2019_test.txt");
    sc.Load("/Users/margaritaduarte/Downloads/stocks-forDist/russell_3000_2019_test.txt");

  TrainNN(sc);

  string symbol;
  while( true ) {
    cout << "Enter a symbol to plot..." << endl;
    cin >> symbol;

    if( symbol == "exit" ) exit(-1);
    else if( sc.HasSymbol( symbol ) ) {
      //sc.Plot(symbol);
      sc.PythonPlot(symbol);
    }
    else {
      cout << "Invalid symbol..." << endl;
    }
  }
  


  return 0;
}
