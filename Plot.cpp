#include "Plot.h"

void MyPlot(string symbol, vector<double>& plotData) {
  /*
  cout << "MyPlot -> **" << symbol << "**" << endl;
  Gnuplot gp;
  gp << "plot '-' with lines\n";
  gp.send1d(plotData);
  */
}

void MyPlot(string symbol, vector<double>& plotData, vector<double>& ma5Data, vector<double>& ma10Data, vector<double>& ma20Data, vector<double>& ma40Data, vector<double>& rsi14Data, vector<double>& scoreData) {
  /*
  cout << "MyPlot -> **" << symbol << "**" << endl;

  cout << "plotData size: " << plotData.size() << endl;
  cout << "ma5Data size: " << ma5Data.size() << endl;
  cout << "ma10Data size: " << ma10Data.size() << endl;
  cout << "ma20Data size: " << ma20Data.size() << endl;
  cout << "ma40Data size: " << ma40Data.size() << endl;
  cout << "rsi14Data size: " << rsi14Data.size() << endl;
  cout << "scoreData size: " << scoreData.size() << endl;

  Gnuplot gp;
  //gp << "plot '-' with lines\n";
  //gp << "plot '-' with lines\n";
  //gp << "plot '-' with lines\n";
  //gp << "set style fill solid";
  gp << "plot '-' with lines, '-' with lines, '-' with lines, '-' with lines, '-' with lines, '-' with lines,  '-' with boxes\n";
  gp.send1d(plotData);
  gp.send1d(ma5Data);
  gp.send1d(ma10Data);
  gp.send1d(ma20Data);
  gp.send1d(ma40Data);
  gp.send1d(rsi14Data);
  gp.send1d(scoreData);
  */
}

void MyPythonPlot() {
  string command = "python3 /Users/margaritaduarte/Downloads/stocks-forDist/testPlot/custom_plot2.py";
  cout << "cmd: " << command << endl;
  system(command.c_str());
  cout << "--done with command." << endl;
}
