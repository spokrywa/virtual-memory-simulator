#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>  
#include <string>
#include <math.h>
#include <vector>
#include <limits.h>
#include "head.h"

using namespace std;

void lruAlgo(int program, vector<vector<int> >& pageMem, int pageLocation, int demand, vector<vector<page> >& pageTable, unsigned long clockTick){
     //setting request page in memory
    int firstInsertIndex;
    for(int i = 0; i < pageTable[program].size(); i++){
        if(pageTable[program][i].pageNumber == pageLocation){
            pageTable[program][i].validBit = 1;
            pageTable[program][i].time = clockTick; //set time accessed as current clockTick
            firstInsertIndex = i;
            break;
        }
    }

    //finding oldest page in memory
    unsigned long min = ULONG_MAX;
    int oldpage;
    int oldPageMemIndex;
    for(int i = 0; i < pageMem[program].size(); i++){
        int pageCheck = pageMem[program][i];
        //finding the page in the page table
        for(int j = 0; j < pageTable[program].size(); j++){
            if(pageTable[program][j].pageNumber == pageCheck){
                if(pageTable[program][j].time < min){ 
                    min = pageTable[program][j].time;
                    oldpage = pageMem[program][i];
                    oldPageMemIndex = i;
                }
                break;
            }
        }
        if (min == 0) //should only happen when pages are still in default initial state with time = 0
            break;
    }

    //setting removed page as out of memory
    for(int i = 0; i < pageTable[program].size(); i++){
        if(pageTable[program][i].pageNumber == oldpage){
            pageTable[program][i].validBit = 0;
            pageTable[program][i].time = clockTick; //set time accessed as current clockTick
            break;
        }
    }

    int removedPage =  pageMem[program][oldPageMemIndex];
    //reallocating pages in memory
    pageMem[program].erase(pageMem[program].begin() + oldPageMemIndex);
    pageMem[program].push_back(pageLocation);
    
    //if prepage is set do it again
    if(demand == 1){
        //finding oldest page in memory
        min = ULONG_MAX;
        for(int k = 0; k < pageMem[program].size(); k++){
            int pageCheck = pageMem[program][k];
            //finding the page in the page table
            for(int j = 0; j < pageTable[program].size(); j++){
                if(pageTable[program][j].pageNumber == pageCheck){
                    if(pageTable[program][j].time < min){ 
                        min = pageTable[program][j].time;
                        oldpage = pageMem[program][k];
                        oldPageMemIndex = k;
                    }
                    break;
                }
            }
            if (min == 0)
                break;
        }
        //finding next contiguous page to insert
        int nextPage;
        int i = firstInsertIndex+1;
        for(i; i < pageTable[program].size(); i++){
            pageLocation++;
            if(pageTable[program][i].validBit == 0 && pageTable[program][i].pageNumber != removedPage){ // making sure its not in memory and not recently removed
                nextPage = pageTable[program][i].pageNumber;
                pageTable[program][i].validBit = 1;
                pageTable[program][i].time = clockTick;
                break;
            }
        }   

        for(int i = 0; i < pageTable[program].size(); i++){
            if(pageTable[program][i].pageNumber == oldpage){
                pageTable[program][i].validBit = 0;
                pageTable[program][i].time = clockTick; //set time accessed as current clockTick
                break;
            }
        }
        //reallocating pages in memory
        pageMem[program].erase(pageMem[program].begin() + oldPageMemIndex);
        pageMem[program].push_back(pageLocation);

    }
}