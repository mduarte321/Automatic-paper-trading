#include "StockCollection.h"
#include "Plot.h"

StockCollection::StockCollection() {
  inputFile = "";
}

StockCollection::~StockCollection() {
}

void StockCollection::Load(string _inputFile) {
  inputFile = _inputFile;
  
  string line;
  ifstream infile;
  infile.open(inputFile.c_str());
  while(infile) {
    getline(infile, line);

    if(line.length()==0) break;

    size_t stSym = line.rfind(" ");
    string symbolDesc = line.substr(0, stSym);
    line.erase(0, stSym + 1);
    string symbol = line;

    string dataDir = "/Users/margaritaduarte/Downloads/stocks-forDist/data/";
    string fileToLoad = dataDir + symbol + ".csv";
    
    cout << "symbol = " << symbol << " desc: " << symbolDesc << " file to load: " << fileToLoad << endl;

    Stock stock;
    stock.Load(symbol, symbolDesc, fileToLoad);
    allStocks.push_back(stock);
  }
}

bool StockCollection::HasSymbol(string symbol) {
  cout << "HasSymbol. searching for " << symbol << " num stocks in collection: " << allStocks.size() << endl;
  for(vector<Stock>::iterator sit=allStocks.begin(); sit!= allStocks.end(); sit++) {
    cout << "symbol: " << (*sit).GetSymbol() << " desc: " << (*sit).GetDescription() << endl;
    if( (*sit).GetSymbol() == symbol ) {
      cout << "symbol **found**: " << symbol << " desc: " << (*sit).GetDescription() << endl;
      return true;
    }
  }

  return false;
}

void StockCollection::Plot(string symbol) {
  for(vector<Stock>::iterator sit=allStocks.begin(); sit!= allStocks.end(); sit++) {
    //cout << "symbol: " << (*sit).GetSymbol() << " desc: " << (*sit).GetDescription() << endl;
    if( (*sit).GetSymbol() == symbol ) {
      cout << "Plot --> symbol **found**: " << symbol << " desc: " << (*sit).GetDescription() << endl;
      //return true;

      Stock& s = (*sit);
      /*
      vector<double> plotData;
      s.PrepareData(plotData);

      MyPlot(s.GetSymbol(), plotData);
      */
      
      vector<double> plotData;
      vector<double> ma5data;
      vector<double> ma10data;
      vector<double> ma20data;
      vector<double> ma40data;
      vector<double> rsi14data;
      vector<double> scoredata;

      s.PrepareData(plotData, ma5data, ma10data, ma20data, ma40data, rsi14data, scoredata);

      MyPlot(s.GetSymbol(), plotData, ma5data, ma10data, ma20data, ma40data, rsi14data, scoredata);
    }
  }
}


// sending data through python
void StockCollection::PythonPlot(string symbol) {
  for(vector<Stock>::iterator sit=allStocks.begin(); sit!= allStocks.end(); sit++) {
    //cout << "symbol: " << (*sit).GetSymbol() << " desc: " << (*sit).GetDescription() << endl;
    if( (*sit).GetSymbol() == symbol ) {
      cout << "Plot --> symbol **found**: " << symbol << " desc: " << (*sit).GetDescription() << endl;
      //return true;

      Stock& s = (*sit);

      s.WritePythonPlotFile("data.csv");

      MyPythonPlot();
    }
  }
}


