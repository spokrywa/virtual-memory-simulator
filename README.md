## Description

The goal of this assignment is to simulate a virtual memory management system. You will
experiment with different page replacement algorithms. Your program must accept the following
parameters at the command prompt in the order specified:

1. : plist (the name of the process list file)
2. : ptrace (the name of the process execution trace file)
3. : page size (# of memory locations for each page)
4. : FIFO, LRU, or Clock (type of page replacement algorithm)
  - FIFO: first-in first-out
  - LRU: least recently used
  - Clock: clock algorithm
5. : + or - (a flag to toggle a pre-paging feature)
  - +: turn pre-paging on
  - -: leave default demand paging

Two files, `plist` and `ptrace` are provided in the assignment description, and should be included in the parameters for running your code. An example command looks like this:

`VMsimulator plist ptrace 2 FIFO +`

Which is the compiled program `VMsimulator` simulating the processes in `ptrace` with a page size of 2, and a FIFO page replacement algorithm with pre-paging.

Your simulaton must:
1. Simulate a paging system
2. Implement the three different page replacement algorithms
3. Handle a variable page size specified by the user
4. Implement both demand and pre-paging
5. Record the number of page swaps that occured during a run

Each of these elements will be discussed in turn.

### Simulate a Paging System

In your simulation, consider a *memory location* to be an atomic unit, that is, the smallest possible unit we care to consider. Thus, in a system with a page size of 2, there are two memory locations on each page.

The program's main memory will hold **512** memory locations

Two files are supplies:
- plist
  - Contains a list of programs that will be loaded into main memory.
  - Each line has the format `(pid, total # of memory locations)` which specifies the total number of memory locations needed by each program
- ptrace
  - Contains a deterministic series of memory accesses which emulate a real system's memory usage.
  - Each line has the format `(pid, referenced memory location)`, which specifies the memory location requested by the program.

#### plist and Initial Loading

You must create page tables for every program in plist. Each page in each page table must have a name or number (which is *unique* with respect to all pages accross all page tables) so you can quickly determine whether it is present in main memory or not. The size of each page table is decided by the number of pages in the program. It is calculated by dividing the total number of memory locations of the program (found in `plist`) by the page size (from input parameters).

Thus, page tables might be represented by the following data structure:

| Page number | Valid bit         | Last time accessed |
|-------------|-------------------|--------------------|
| N1          | 0 (not in memory) | T1                 |
| N2          | 1 (in memory)     | T2                 |
| ...         | ...               | ...                |

Once you have the page tables, you will perform a default loading of memory before starting to read the pages as indicated in `ptrace`. That is, you will load an initial resident set of each program's page table into main memory. *The main memory is divided equally among all programs in plist*. Determine how many pages each program should get into its assigned partition of main memory and load those pages. These should be the first pages in the program. If the program doesn't have enough pages for its default load, leave its unused load blank. After initializing memory allocation, you update the page tables (i.e. set valid bit of corresponding pages in table to 1) according to the page assignment. *If it doesn't divide evenly, keep the leftover memory*.

#### ptrace

Finally, you will need to begin reading in `ptrace`. Each line of this file represents a memory location request within a program. You will need to translate this location into the unique page number that you stored in the page tables you made later, and determine if the requested page is in memory or not. If it is, simply continue with the next command in ptrace. If not, record that a page swap was made, and initiate a page replacement algorithm. **For each program, the pages to be replaced should be picked from those pages allocated to itself (which is caled local page allocation policy).**

![](paging_model.png?raw=true)

#### Implementation Details

- You can use `ifstream` from C++ to read from both plist and ptrace files, or any other method to parse the files in C and C++.
- Use a structure to record the pages in main memory. For every time a page is loaded into the memory, its unique page names will be put into this data structure.
- Each program in plist has an array as its page table. You can implement this in various ways such as a vector of arrays, or array of arrays.
- You can name the pages in any way you like, as long as every page is uniquely identified. It is easiest to use integers. For example, after parsing program 1 which has pages 1, ..., n, continue from n + 1 for the next program which has pages n+1, ..., m. Repeat for all programs.
- Translation of a memory location of a program to its unique page number consists of two steps:
  1. Look up the memory location by dividing the location by page size.
    - e.g. Program 0's 6th memory location in a page size = 4 system would be on page ceiling(6 / 4) = 2.
  2. Look up the absolute page number.
    - e.g. Go to program 0's page table and search for that page's unique identifier.

### Implementation of Page Replacement Algorithms

- First in, First Out (FIFO): The oldest page in memory will be the first to leave when a page swap is performed.
- Least Recently Used (LRU): The page with the oldest access time will be replaced in memory when a page swap is performed.
- Clock based policy: Use the simple version of this algorithm with only one use bit, which can be found in the text or slides.

#### Implementation Details

For each of these algorithms, `time()` and `clock()` are not sufficiently sensitive to timestamp memory accesses. Use an external library if you like, or simply keep a global counter which keeps track of memory accesses (as a relative measure of age). Ths counter may grow very large so make it `unsigned` and `long` to give it room to grow. It will not overflow with the files supply.

#### Page of Variable Sizes

This affects not only how many pages each program will take up, but also the "size" of main memory. For example, if the page size is 4 then main memory will have 128 available page spots. This simulation should be able to use page sizes that are powers of 2 up to a max of 32, and other sizes are out of scope.

#### Demand and pre-paging

Demand paging replaces 1 page with the requested page during a page fault

Pre-paging brings 2 pages into memory for every swap: the requested page and the next contiguous page which is not loaded in memory. Thus, if the next contiguous page in a table is already in memory, you must keep looking for the next page until you reach one that isn't in memory or until all pages have been checked. **You cannot bring back the page in memory which was just removed**.

#### Record Page Swaps

Anytime a memory location is read, it will be translated to a page number. If it is not found in main memory, a page swap ust be initiated. Record this in a counter during the run of the program. It will be used as a metric of each algorithm's performance. This makes sense: if a particular algorithm is using the disk less to swap pages into memory, the whole system will be running faster.

#### Experimentation

Once you have implemented all of this, please try running each algorithm (FIFO, LRU, Clock) with page sizes 1, 2, 3, 8, and 16, demand paging. Plot all three of these on a graph (x-axis page size, y-axis page swaps). Repeat for pre-paging. Write a 1-2 page report detailing your findings, including a discussion of algorithm complexity vs performance benefit. Compare demand paging and pre-paging results.

### Requirements

- The program must be written in C or C++ and run on a linux machine.
- All source code should be well documented.
- Programs that cannot be compiled by the CAs (or run when providing arguments in the order we expect) will result in an automatic grade of zero.

#### Grading
- Simulation of page system (25 pts)
  - Main memory must be correctly created in terms of page size
  - Page tables must be correctly created, with each page having a unique identifier
  - Must handle all page sizes of powers of two correctly
  - Must correctly load the default memory (consider case of program smaller than default load size)
- Page replacement algorithms (30 pts: 10 per algorithm)
- Implement pre-paging and demand paging (20 pts)
- Experimentation and analysis of results (20 pts)
  - Graph page faults as a function of page size accross all algorithms (5 pts)
  - Use page sizes 1, 2, 4, 8, 16, and represent all algorithms
  - Explain your results (15 pts):
  - Discuss what you expected to see vs. what you actually saw
  - If the results do not mirror your expectations / textbook claims, discuss why
  - Discuss the relative complexity of each algorithm
  - Discuss how the data might have changed if a completely random memory access trace had been supplied
 - Documentation (code comments) (5 pts)

#### Submission

- Submit a zip file of your assignment to canvas.
- Make sure your progress is logged on Github

#### Hints and Help

- Start early and tackle the problem in units (i.e. separate into parsing into data structures, general program tracing, specific page replacement algorithms)
- Check out the manual pages using Unix `man` for functions you might want to use.
- Come to the instructor or CAs for help.

