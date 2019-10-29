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

void clockAlgo(int program, vector<vector<int> >& pageMem, int pageLocation, int demand, vector<vector<page> >& pageTable, unsigned long clockTick, vector<vector<int> >& rbits){

    //demand paging
    int removedPage;
    int firstInsertIndex;

    while(1){
        //search until the r bit is 0
        if(rbits[program][0] == 0){
            for(int j = 0; j < pageTable[program].size(); j++){
                if(pageTable[program][j].pageNumber == pageLocation){
                    pageTable[program][j].validBit = 1;
                    pageTable[program][j].time = clockTick; //set time accessed as current clockTick  
                    firstInsertIndex = j;
                    int firstPageNumber = pageMem[program][0];   
                    for(int i = 0; i < pageTable[program].size(); i++){
                        if(pageTable[program][i].pageNumber == firstPageNumber){
                            pageTable[program][i].validBit = 0;
                            pageTable[program][i].time = clockTick; //set time accessed as current clockTick
                            break;
                        } 
                    }
                    //remove current page, and push back a new page with an r bit of 0 at the end.
                    rbits[program].erase(rbits[program].begin());
                    rbits[program].push_back(1);
                    removedPage = pageMem[program][0];
                    pageMem[program].erase(pageMem[program].begin());
                    pageMem[program].push_back(pageLocation);
                    
                    break;
                }
            }
            break;
        }
        //sets new rbit for current mem location to 0, emulates clock by pushing back first element to the end in both pagemem and rbits so they stay synced
        else{
            int currPage = pageMem[program][0];
            pageMem[program].erase(pageMem[program].begin());
            pageMem[program].push_back(currPage);
            rbits[program].erase(rbits[program].begin());
            rbits[program].push_back(0);
        }
    }
            //prepaging occurs
    if(demand == 1){
        int nextPage;
        while(1){
            //search until rbits is 0
            if(rbits[program][0] == 0){
                for(int j = firstInsertIndex + 1; j < pageTable[program].size(); j++){
                    pageLocation++;
                    if(pageTable[program][j].validBit == 0 && pageTable[program][j].pageNumber != removedPage){
                    nextPage = pageTable[program][j].pageNumber;
                    pageTable[program][j].validBit = 1;
                    pageTable[program][j].time = clockTick;
                        int secondPageNumber = pageMem[program][0];   
                        for(int i = 0; i < pageTable[program].size(); i++){
                            if(pageTable[program][i].pageNumber == secondPageNumber){
                                pageTable[program][i].validBit = 0;
                                pageTable[program][i].time = clockTick; //set time accessed as current clockTick
                                break;
                            } 
                        }
                        //remove memory location from main memory and push back the new memory location, as well as a new rbit
                        rbits[program].erase(rbits[program].begin());
                        rbits[program].push_back(1);
                        pageMem[program].erase(pageMem[program].begin());
                        pageMem[program].push_back(pageLocation);   
                        break;
                    }
                }
                break;
            }

            //sets new rbit for current mem location to 0, emulates clock by pushing back first element to the end in both pagemem and rbits so they stay synced
            else{
                int currPage = pageMem[program][0];
                pageMem[program].erase(pageMem[program].begin());
                pageMem[program].push_back(currPage);
                rbits[program].erase(rbits[program].begin());
                rbits[program].push_back(0);
            }
        }
    }
}
