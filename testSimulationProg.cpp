#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <ctime>

#include "simulation.h"
#include "queueAsArray.h" 

using namespace std;

void runSimulation(int numOfPrinters, int numJobs, int maxPages,int printRate,int numTiers,int jpm,int cpp,int printCapacity,int downTime);

int main()
{
    int numJobs = 100, numOfPrinters = 3,printRate = 5, maxPages = 50, numTiers = 3, jpm = 1,cpp = .3,printCapacity = 300,downTime = 10;
    string jobFrequency = "aa";
    //get numJobs
    cout << "Enter Number of Jobs: " << endl;
    cin >> numJobs;
    //get numOfPrinters
    cout << "Specify the Number of Printers: " << endl;
    cin >> numOfPrinters;
    //do printers all print at same rate?
    cout << "Do all printers print at the same rate? (y/n) "; << endl;
    char ans;
    cin >> ans;
    if (toupper(ans) == "Y") {
        //get printRate
        cout << "Specify Print Rate: " << endl;
	//@TODO: I think we need a dynamically-allocated array for pr as well here.
    } else {
        //for each printer, get print rate
        for (int i = 0;i < numOfPrinters;i++) {
            cout << "Specify printer " << i+1 << "'s print rate: ";
	    cin >> 
            
        }
    }
    //get maxPages
    cout << "Input the Maximum Pages Able To Print: " << endl;
    cin >> maxPages;
    //get numTiers
    cout << "Input number of tiers for print jobs: ";
    cin >> numTiers;
    //@TODO: need to figure out displacement of tiers as well

    //Figure out the average number of jobs per minute
    while (toupper(jobFrequency.c_str() != "JM" and toupper(jobFrequency.c_str() != "MJ") {
        cout << "Will there be an average of multiple jobs per minute coming in?" << endl
             << "OR" << endl
             << "Will there be an average of one job every several minutes?" << endl
             << "Input 'JM' for jobs per minute or 'MJ' for minutes per job: ";
        cin >> jobFrequency;
    }
    jobFrequency = toupper(jobFrequency.c_str();
    if (jobFrequency == "JM") {
        cout << "Enter the average number of jobs per minute: ";
        cin >> jpm;
    } else {
        cout << "Enter the average number of minutes per job: ";
	cin >> jpm;
	jpm = 1/jpm;
    }
    //get cost per page in dollars
    cout << "Enter the approximate cost per page in dollars: ";
    cin >> cpp;
    //get printing capacity before requiring maintenance
    cout << "Enter number of pages the printers can print before requiring maintenance: ";
    cin >> printCapacity;
    //get amount of time printer is offline
    cout << "Enter amount of time printer is down for maintenance in minutes: ";
    cin >> downTime;
    runSimulation(numOfPrinters,numJobs,maxPages,printRate,numTiers,jpm,cpp,printCapacity,downTime);

    return 0;
}
//--------------------------- Main Ends -----------------------------------
/*
 * Functions Begin
 */

//Runs Simulation
void runSimulation(int numOfPrinters, int numJobs, int maxPages, int printRate,int numTiers)
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

    int jobNum = 0;
    //create printerList
    printerListType printerList(/*numOfPrinters,*/printRate);

    //Create a jobQueueArray Object to pass jobs
    jobQueueArray jqArr;

    //create a job to give to jobQueue whenever needed
    jobType job;
    
    int waitTime = 0;
    
    int printPages = 0;

    //while loop to continue until jobQueue empty and printerList empty as well
    while (printerList.getNumberOfBusyPrinters() != 0 || !jqArr.isEmpty()) {

        //increment sTime
        sTime++;
        cout << endl << "At time unit "<< sTime << endl;
        //update printer list & decrements
        printerList.updatePrinters(cout);
        
        //job queue array update
        jqArr.updateWaitingQueues();

        if (jobNum < numJobs) {
          jobNum++;
          job.setJobInfo(jobNum, clock, 0, maxPages);
          cout << "Job number " << job.getJobNumber() << "\nPages Created " << job.getNumPages() << endl;
          jqArr.sendJob(job);
        }
        
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
            << "Total Jobs: "  << jobNum << endl
            << "Average Wait Time: " << (float)waitTime/jobNum << endl;
    
}
