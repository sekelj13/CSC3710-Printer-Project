#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <ctime>

#include "simulation.h"
#include "queueAsArray.h" 

using namespace std;

void setSimulationParameters(int& sTime, int& numOfPrinters,
                             int& transTime,
                             int& tBetweenCArrival);

void runSimulation();

//argv[1] = numJobs
//argv[2] = numOfPrinters
//argv[3] = maxPages
int main(argc,argv[])
{
    int numJobs = atoi(argv[1]), numOfPrinters = atoi(argv[2]), maxPages = atoi(argv[3]);
    runSimulation(numJobs,numOfPrinters,maxPages);

    return 0;
}
//--------------------------- Main Ends -----------------------------------
/*
 * Functions Begin
 *
 */

/*
//Sets Sim Params
void setSimulationParameters(int& numJobs, int& numOfPrinters, int maxPages)
{
    cout << "Enter the number of jobs to print: ";
    cin >> numJobs;
    cout << endl;

    cout << "Enter the number of printers: ";
    cin >> numOfPrinters;
    cout << endl;

    cout << "Enter the maximum number of pages a job may have: ";
    cin >> maxPages;
    cout << endl;
}
*/

//Prints simulation results
void printSimResults(int sTime,int numOfPrinters,int waitTime,int custNum) {

    int numQueuedJobs = jobQueue.queueWaitTime(waitTime);

    int transactingJobs = printerList.getNumberOfBusyPrinters();
    int finishedJobs = custNum-transactingCustomers-numQueuedCustomers;
    cout    << endl << "Simulation Completed.\n"
            << "Simulation time: " << sTime << endl
            << "Number of printers: " << numOfPrinters << endl
            << "transaction time: " << transTime << endl
            << "Time between job arrivals: " << tBetweenCArrival << endl
            << "Total Wait Time: "  << waitTime << endl
            << "Total Jobs: "  << custNum << endl
            << "Average Wait Time: " << (float)waitTime/custNum << endl
            << "Jobs still in queue: " << numQueuedCustomers << endl
            << "Jobs still in transaction: " << transactingCustomers << endl
            << "Jobs who finished receiving service: " << finishedCustomers << endl;
	    //display total waiting time and average waiting time
}

//Runs Simulation
void runSimulation(int numOfPrinters, int numJobs, int maxPages)
{
    /*
     * sTime = Simluation Time
     * numOfPrinters = Total Number of Printers
     * transtime = Transaction Time
     *
     */
    int sTime = 0;
 
    srand(time(NULL));
    int custNum = 0;

    printerListType serverList(numOfPrinters);

    //create jobQueueArray to hold EVERYTHING
    jobQueueArray jqArr[3];

    jobType customer;
    
    int waitTime = 0;

    // Need new random every clock tick
    // for loop to create new job every clock tick
    for (int clock = 1; clock <= numJobs; clock++){

        //increment sTime
	sTime++;
    	//update printer list & decrements
    	printerList.updatePrinters(cout);
        
        //job queue update
        jobQueue.updateWaitingQueue();

        //increment numcustomers and add customer
	//want equal possibility for each tier: t1 is 0-9, t2 is 10-19, t3 is 20-29.
        transTime = (rand() % maxPages) + 1;//+1 so minimum number of pages is now 1, not 0
            
        custNum++; //incremented job by 1
        //Create Job
        job.setJobInfo(custNum, clock, 0, transTime, maxPages);
        jobQueue.addQueue(customer);

        //if printer is free and queue nonempty, pair job with printer
        if (printerList.getFreePrinterID()!= -1 && !jobQueue.isEmptyQueue()){
            if (jobQueue.front().getJobNumber() != -1) {
                waitTime += job.getWaitingTime();
                printerList.setPrinterBusy(serverList.getFreeServerID(), job, transTime);
                jobQueue.deleteQueue();
            }
        }
        
    }

    //while loop to continue until jobQueue empty and printerList empty as well
    while (printerList.getNumberOfFreePrinters() != numOfPrinters && !jobQueue.isEmptyQueue()) {

        //increment sTime
	sTime++;
	
	//update printer list & decrement current job times
        printerList.updatePrinters(cout);

	//job queue update
	jobQueue.updaitWaitingQueue();
        
	//if printer is free and queue nonempty, pair job with printer
        if (printerList.getFreePrinterID()!= -1 && !jobQueue.isEmptyQueue()){
            if (jobQueue.front().getJobNumber() != -1) {
                waitTime += job.getWaitingTime();
                printerList.setPrinterBusy(serverList.getFreeServerID(), job, transTime);
                jobQueue.deleteQueue();
            }
	}

    }
    
    printSimResults(sTime,numOfPrinters,waitTime,custNum);
    
}
