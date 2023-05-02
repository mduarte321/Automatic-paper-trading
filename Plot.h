#include <fstream>
#include <vector>
#include <map>
#include <limits>
#include <cmath>
#include <cstdio>
#include <iostream>
using namespace std;

/*
// Warn about use of deprecated functions.
#define GNUPLOT_DEPRECATE_WARN
#include "gnuplot-iostream.h"
*/

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

// http://stackoverflow.com/a/1658429
#ifdef _WIN32
    #include <windows.h>
    inline void mysleep(unsigned millis) {
        ::Sleep(millis);
    }
#else
    #include <unistd.h>
    inline void mysleep(unsigned millis) {
        ::usleep(millis * 1000);
    }
#endif

/*
void pause_if_needed() {
#ifdef _WIN32
    // For Windows, prompt for a keystroke before the Gnuplot object goes out of scope so that
    // the gnuplot window doesn't get closed.
    std::cout << "Press enter to exit." << std::endl;
    std::cin.get();
#endif
}
*/

// Tell MSVC to not warn about using fopen.
// http://stackoverflow.com/a/4805353/1048959
#if defined(_MSC_VER) && _MSC_VER >= 1400
#pragma warning(disable:4996)
#endif


void MyPlot(string symbol, vector<double>& plotData);
void MyPlot(string symbol, vector<double>& plotData, vector<double>& ma5Data, vector<double>& ma10Data, vector<double>& ma20Data, vector<double>& ma40Data, vector<double>& rsi14Data, vector<double>& scoreData);

void MyPythonPlot();
