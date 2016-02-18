#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <ctime>

#include "simulation.h"
#include "queueAsArray.h" 

using namespace std;

void runSimulation(int numOfPrinters, int numJobs, int maxPages,int printRate,int numTiers,int jpm);

int main()
{
    int numJobs = 100, numOfPrinters = 3,printRate = 5, maxPages = 50, numTiers = 3, jpm = 1;
    string jobFrequency = "aa";
    cout << "Enter Number of Jobs: " << endl;
    cin >> numJobs;
    cout << "Specify the Number of Printers: " << endl;
    cin >> numOfPrinters;
    cout << "Specify Print Speed: " << endl;
    cin >> printRate;
    cout << "Input the Maximum Pages Able To Print: " << endl;
    cin >> maxPages;
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
    runSimulation(numOfPrinters,numJobs,maxPages,printRate,numTiers,jpm);

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

    //currently, all jobs are created and enqueued before any of them get printed.
    //for loop to create all new jobs
    for (int clock = 1; clock <= numJobs; clock++) {

/*
        sTime++;
        cout << endl << "At time unit "<< sTime << endl;
    	//update printer list & decrements
        printerList.updatePrinters(cout);
        
        //job queue array update
        jqArr.updateWaitingQueues();

        //increment numjobs and add job
        //want equal possibility for each tier: t1 is 0-9, t2 is 10-19, t3 is 20-29.
*/        
        jobNum++; //incremented job by 1
        //Create Job -- job number and arrival time will be the same here
        job.setJobInfo(jobNum, clock, 0, maxPages);
        cout << "Job number " << job.getJobNumber() << "\nPages Created "<< job.getNumPages() << endl;
        jqArr.sendJob(job);

/*
        //if printer is free and queue nonempty, pair job with printer
        if (printerList.getFreePrinterID()!= -1 && !jqArr.isEmpty()){
            if (jqArr.checkNextJob().getWaitingTime() != -1 ) {
                waitTime += job.getWaitingTime();
                printerList.setPrinterBusy(printerList.getFreePrinterID(), jqArr.getNextJob());
            }
        }
*/
        
    }
    cout << endl << "All jobs created and enqueued." << endl << endl;
    //while loop to continue until jobQueue empty and printerList empty as well
    while (printerList.getNumberOfBusyPrinters() != 0 || !jqArr.isEmpty()) {

        //increment sTime
        sTime++;
        cout << endl << "At time unit "<< sTime << endl;
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
            << "Total Jobs: "  << jobNum << endl
            << "Average Wait Time: " << (float)waitTime/jobNum << endl;
    
}
