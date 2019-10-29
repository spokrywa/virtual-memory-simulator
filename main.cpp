//Philip Cho, Sebastian Pokrywa
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>  
#include <string>
#include <math.h>
#include <vector>
#include "head.h"

using namespace std;

int main(int argc, char *argv[]){
        if(argc != 6){
            cerr << "Not the correct number of arguments" << endl;
            return 1;
        }

        string algo = argv[4];
        string flag = argv[5];
        int demand;
        if(flag.compare("+") == 0){
            demand = 1;
        }
        else if(flag.compare("-") == 0){
            demand = 0;
        }
        else{
            cerr << "Error: Flag is not a + or -" << endl;
            return 1;
        }
        int size = atoi(argv[3]);

        //checking page size
        if(size == 0 || size > 32 || ceil(log2(size)) != floor(log2(size))){
            cerr << "Invalid page size, must be a power of 2 and less than 32" << endl;
            return 1;
        }

        ifstream list;
        ifstream trace;

        list.open(argv[1]);
        trace.open(argv[2]);

        //vector of vectors of pages
        vector<vector<page> > pageTables(10);
        int a, b;
        int pageNameCount = 1;

        //initializing page table from plist
        //a is program number, b is number of memory locations
        while(list >> a >> b){
            int sizePageTable = (b+size - 1)/size; //ceiling integers without converting to float
            pageTables[a] = vector<page>(sizePageTable);
            //need to insert pages into page tables here
            for(int i = 0; i < sizePageTable; i++){
                page newPage;
                newPage.pageNumber = pageNameCount++;
                newPage.validBit = 0;
                newPage.time = 0;
                pageTables[a][i] = newPage;
            }
        }

        //initial memory setup
        vector<vector<int> > pageMemory(10, vector<int>(512/size/10)); //leftover memory is kept

        //loading initial pages into memory
        int counter = 0;
        for(auto& progmem:pageMemory){
            int pagenum = 0;
            for(auto& page:progmem){
                page = pageTables[counter][pagenum].pageNumber; //load page into memory
                pageTables[counter][pagenum++].validBit = 1; //set bit so we know page is in memory
            }
            counter++;
        }

        //memory offset for pages
        vector<int> memoffset;
        for(int i = 0; i < 10; i++){
            memoffset.push_back(pageTables[i][0].pageNumber);
        }
        vector<vector<int> > clockrbit(10, vector<int>(512/size/10, 0));
        //reading trace and searching for pages
        //a program number, b memory location
        unsigned long swapcount = 0;
        unsigned long clockTick = 0;
        while(trace >> a >> b){
            clockTick++; //increment timer
            int pageloc; //location of the requested page in the page table
            if(a == 0){
                pageloc = (b + size - 1) / size;
            }else{
                pageloc = (b + size - 1) / size + memoffset[a];
            }

            int check = 0;
            //find the requested page in page tables and check if its not in memory
            for(int i = 0; i < pageTables[a].size(); i++){
                if(pageTables[a][i].pageNumber == pageloc && pageTables[a][i].validBit == 0){
                    swapcount++;
                    check = 1;
                    if(algo.compare("FIFO") == 0){
                       fifoAlgo(a, pageMemory, pageloc, demand, pageTables, clockTick);
                    }
                    else if(algo.compare("LRU") == 0){
                       lruAlgo(a, pageMemory, pageloc, demand, pageTables, clockTick);
                    }        
                    else if(algo.compare("Clock") == 0){
                        clockAlgo(a, pageMemory, pageloc, demand, pageTables, clockTick, clockrbit);
                    }
                    else{
                        cerr << "Provided algorithm is not valid" << endl;
                        return 1;
                    }
                }else if(pageTables[a][i].pageNumber == pageloc && pageTables[a][i].validBit == 1){
                    pageTables[a][i].time = clockTick;
                }
            }
               
            if(check == 0){
                if(algo.compare("Clock") == 0){
                    for(int j = 0; j < pageMemory[a].size(); j++){
                        if(pageMemory[a][j] == pageloc){
                            clockrbit[a][j] = 1;
                            break;
                        }
                    }
                }
            }
        }

        cout << "Page swaps: " << swapcount << endl;
        cout << "Time spent: " << clockTick << endl;


        list.close();
        trace.close();
        return 0;
}