//Philip Cho, Sebastian Pokrywa
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


//uses first in first out in order to replace page in memory, program is what program it is, pageMem is the main meory, pageLocation is 
void fifoAlgo(int program, vector<vector<int> >& pageMem, int pageLocation, int demand, vector<vector<page> >& pageTable, unsigned long clockTick){
    int firstPageNumber = pageMem[program][0];
    int firstInsertIndex;
    //set requested page as in memory first, order doesnt really matter
    for(int i = 0; i < pageTable[program].size(); i++){
        if(pageTable[program][i].pageNumber == pageLocation){
            pageTable[program][i].validBit = 1;
            firstInsertIndex = i;
            pageTable[program][i].time = clockTick; //set time accessed as current clockTick
            break;
        }         }
    //find the first page in memory in the page table
    for(int i = 0; i < pageTable[program].size(); i++){
        if(pageTable[program][i].pageNumber == firstPageNumber){
            pageTable[program][i].validBit = 0;
            pageTable[program][i].time = clockTick; //set time accessed as current clockTick
            break;
        } 
    }

    //reallocate the pages
    int removedPage = pageMem[program][0];
    pageMem[program].erase(pageMem[program].begin());
    pageMem[program].push_back(pageLocation);
    //prepaging is used
    if(demand == 1){
        //finding next contiguous page
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
        //setting head of memory list to not in memory
        firstPageNumber = pageMem[program][0];
        for(int i = 0; i < pageTable[program].size(); i++){
            if(pageTable[program][i].pageNumber == firstPageNumber){
                pageTable[program][i].validBit = 0;
                pageTable[program][i].time = clockTick; //set time accessed as current clockTick
                break;
            }

        }               }
        //reallocate pages
        pageMem[program].erase(pageMem[program].begin());
        pageMem[program].push_back(pageLocation);
}