#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <ctime>

#include "simulation.h"
#include "queueAsArray.h" 

using namespace std;

void runSimulation(int numOfPrinters, int numJobs, int maxPages,int printRate);

//argv[1] = numJobs
//argv[2] = numOfPrinters
//argv[3] = maxPages
//@TODO: Can re-add, but wasn't working last night.
int main()
{
    //int numJobs = atoi(argv[1]), numOfPrinters = atoi(argv[2]), maxPages = atoi(argv[3]);

	//CHANGE THIS BACK WHEN DONE TESTING
    int numJobs = 100, numOfPrinters = 3,printRate = 5, maxPages = 30;
    cout << "Enter Number of Jobs: " << endl;
//    cin >> numJobs;
    cout << "Specify the Number of Printers: " << endl;
//    cin >> numOfPrinters;
    cout << "Specify Print Speed: " << endl;
//    cin >> printRate;
    cout << "Input the Maximum Pages Able To Print: " << endl;
//    cin >> maxPages;
    runSimulation(numJobs,numOfPrinters,maxPages,printRate);

    return 0;
}
//--------------------------- Main Ends -----------------------------------
/*
 * Functions Begin
 */

//Runs Simulation
void runSimulation(int numOfPrinters, int numJobs, int maxPages, int printRate)
{
    /*
     * sTime = Simluation Time
     * numOfPrinters = Total Number of Printers
     * transtime = Transaction Time
     *
     */

    //seed random
    cout << "Give a seed: ";
    int seed;
    cin >> seed;
    srand(seed);

    int sTime = 0;

    int custNum = 0;
    //create printerList
    printerListType printerList(numOfPrinters,printRate);

    //Create a jobQueueArray Object to pass jobs
    jobQueueArray jqArr;

    //create a job to give to jobQueue whenever needed
    jobType job;
    
    int waitTime = 0;
    
    int printPages = 0;

    // Need new random every clock tick
    // for loop to create new job every clock tick
    for (int clock = 1; clock <= numJobs; clock++) {

        //increment sTime
        sTime++;
    	//update printer list & decrements
    	printerList.updatePrinters(cout);
        
        //job queue array update
        jqArr.updateWaitingQueues();

        //increment numcustomers and add customer
        //want equal possibility for each tier: t1 is 0-9, t2 is 10-19, t3 is 20-29.
        
        custNum++; //incremented job by 1
        //Create Job -- job number and arrival time will be the same here
        job.setJobInfo(clock, clock, 0, maxPages, rand());
        jqArr.sendJob(job);

        //if printer is free and queue nonempty, pair job with printer
        if (printerList.getFreePrinterID()!= -1 && !jqArr.isEmpty()){
            if (jqArr.checkNextJob().getWaitingTime() != -1 ) {
                waitTime += job.getWaitingTime();
                printerList.setPrinterBusy(printerList.getFreePrinterID(), jqArr.getNextJob());
            }
        }
        
    }

    //while loop to continue until jobQueue empty and printerList empty as well
    while (printerList.getNumberOfFreePrinters() != numOfPrinters && !jqArr.isEmpty()) {

        //increment sTime
        sTime++;
        //update printer list & decrements
        printerList.updatePrinters(cout);
        
        //job queue array update
        jqArr.updateWaitingQueues();
        
        //if printer is free and queue nonempty, pair job with printer
        if (printerList.getFreePrinterID()!= -1 && !jqArr.isEmpty()){
            if (jqArr.checkNextJob().getWaitingTime() != -1 ) {
                jobType job;
                waitTime += job.getWaitingTime();
                printerList.setPrinterBusy(printerList.getFreePrinterID(), jqArr.getNextJob());
            }
        }
    }
    
    cout    << endl << "Simulation Completed.\n"
            << "Simulation time: " << sTime << endl
            << "Number of printers: " << numOfPrinters << endl
            << "Time between job arrivals: " << 1 << endl
            << "Total Wait Time between all jobs: "  << waitTime << endl
            << "Total Jobs: "  << custNum << endl
            << "Average Wait Time: " << (float)waitTime/custNum << endl;
    
}
