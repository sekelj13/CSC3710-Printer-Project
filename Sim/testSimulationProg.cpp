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
 
int main()
{
    runSimulation();

    return 0;
}
//--------------------------- Main Ends -----------------------------------
/*
 * Functions Begin
 *
 */

//Sets Sim Params
void setSimulationParameters(int& numJobs, int& numOfPrinters)
{
    cout << "Enter the number of jobs to print: ";
    cin >> numJobs;

    cout << "Enter the number of printers: ";
    cin >> numOfPrinters;
    cout << endl;
}

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
void runSimulation()
{
    /*
     * sTime = Simluation Time
     * numOfPrinters = Total Number of Printers
     * transtime = Transaction Time
     * tBetweenCArrival = Time b/w job arrival
     *
     */
    int numOfPrinters, transTime, tBetweenCArrival = 0;
 
    setSimulationParameters(numJobs, numOfPrinters);
    
    srand(time(NULL));
    int random = 0;
    int custNum = 0;

    printerListType serverList(numOfPrinters);
    
    //Job Queue
    waitingJobQueueType jobQueue;

    jobType customer;
    
    int waitTime = 0;

    // Need new random every clock tick
    for (int clock = 1; clock <= sTime; clock++){

    	//update printer list & decrements
    	printerList.updatePrinters(cout);
        
        //job queue update
        jobQueue.updateWaitingQueue();

        //if job arrives, increment numcustomers and add customer
        random = rand() % tBetweenCArrival;
        if (!random) { //New Job Arrived if 0
            
            custNum++; //incremented job by 1
            //Create Job
            job.setJobInfo(custNum, clock, 0, transTime);
            jobQueue.addQueue(customer);

        }
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
