## Description

This was a team project that simulated multiple paging algorithms and compared their page swaps. 

## Build

No makefile is provided but to build the project run g++ with all the .cpp files as arguments.

## Instructions
To run the program execute it as such:  
`VMsimulator plist ptrace 2 FIFO +`  
The arguments are:  
`<page list> <page trace> <page size> <algorithm> <demand paging on/off>`  
Supported algorithms are: `LRU FIFO Clock`  
Pre-paging on is + and pre-paging off is -.
