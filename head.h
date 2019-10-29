//Philip Cho, Sebastian Pokrywa
#ifndef HEAD_H
#define HEAD_H

#include <vector>
using namespace std;
struct page{
    int pageNumber;
    int validBit;
    unsigned long time;
};

void fifoAlgo(int program, vector<vector<int> >& pageMem, int pageLocation, int demand, vector<vector<page> >& pageTable, unsigned long clockTick);

void lruAlgo(int program, vector<vector<int> >& pageMem, int pageLocation, int demand, vector<vector<page> >& pageTable, unsigned long clockTick);

void clockAlgo(int program, vector<vector<int> >& pageMem, int pageLocation, int demand, vector<vector<page> >& pageTable, unsigned long clockTick, vector<vector<int> >& rbits);
#endif