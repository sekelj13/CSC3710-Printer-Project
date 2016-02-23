#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <cstring>

#include "simulation.h"
#include "queueAsArray.h" 

using namespace std;

void runSimulation(int numOfPrinters, int numJobs, int maxPages,int printRate,int numTiers, int jobsPerMinute, double costPerPage,int printCapacity,int downTime);

int main()
{
    int numJobs = 100, numOfPrinters = 3, printRate = 5, maxPages = 50, numTiers = 3, jobsPerMinute = 1,
        printCapacity = 300, downTime = 10;
    
    //Changed to double, could be some fraction of a dollar
    double costPerPage = .3;
    
    char boolPrintRate = 'x';
    
    int jobFrequency = 0;
    
    //@TODO: Change cins to istream(read all data inputs / info from a file or from cmdline)
    //Get the number of jobs
    cout << "Enter Number of Jobs: " << endl;
    cin >> numJobs;
    
    //Get the number of printers
    cout << "Specify the Number of Printers: " << endl;
    cin >> numOfPrinters;
    
    //Printers print randomly or linearly
    cout << "Do all printers print at the same rate? (y/n) " << endl;
    cin >> boolPrintRate;
    if (toupper(boolPrintRate) == 'Y') {
        //Get Printer Rate
        cout << "Specify Print Rate: " << endl;
	//@TODO: I think we need a dynamically-allocated array for pr as well here.
    } else {
        //for each printer, get print rate
        for (int i = 0;i < numOfPrinters;i++) {
            cout << "Specify printer " << i+1 << "'s print rate: ";
            //@TODO: Fill in printer array
        }
    }
    
    //Get the maximum number of pages able to printer / job
    cout << "Input the Maximum Pages Able To Print: " << endl;
    cin >> maxPages;
    
    
    //Get the total number of tiers in the waiting list queue
    cout << "Enter the amount total amount of printer tiers: ";
    cin >> numTiers;
    //@TODO: need to figure out displacement of tiers as well


    /*
     * Check to see if their will be multiple jobs read in per minute
     * or
     * If their will be one job read in at a time
     *
     */
    //@TODO: Why was this a while loop, I consolidated it to one statement
    /*while (toupper(jobFrequency) != "JM" &&
           toupper(jobFrequency) != "MJ")
    {
        cout << "Will there be an average of multiple jobs per minute coming in?" << endl
             << "Or" << endl
             << "Will there be an average of one job every several minutes?" << endl
             << "Input 'JM' for jobs per minute or 'MJ' for minutes per job: ";
        cin >> jobFrequency;
    }*/
    
    
    cout << "If you will process one job at a time enter 1" << endl
         << "If you will process multiple jobs at a time enter 2" << endl;
    cin  >> jobFrequency;
    
    //@TODO: Add new Var for multiple jobs or a single job(need to differentiate b/w the two)
    if (jobFrequency == 2) {
        cout << "Enter the average number of jobs per minute: ";
        cin >> jobsPerMinute;
    
    } else {
        cout << "Enter the average number of minutes per job: ";
        cin >> jobsPerMinute;
        jobsPerMinute = 1 / jobsPerMinute;
    }
    
    //Get the printed page per dollar cost
    cout << "Enter the approximate cost per page in dollars: ";
    cin >> costPerPage;
    
    //Get max number pages printer can print before maintence is needed
    cout << "Enter number of pages the printers can print before requiring maintenance: ";
    cin >> printCapacity;
   
    //Get the amount of time a printer is down for maintence
    cout << "Enter amount of time printer is down for maintenance in minutes: ";
    cin >> downTime;
    
    
    //@TODO: Thought, could stash all data in a hash map(key => value pair) array and parse through it, would look cleaner
    //Run the simulation now that data has all been collected
    runSimulation(numOfPrinters,numJobs,maxPages,printRate,numTiers,jobsPerMinute,costPerPage,printCapacity,downTime);

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

    char checkSeed;
    
    int seed = 0, sTime = 0, jobNum = 0, waitTime = 0, printPages = 0;
    
    //Seed the program
    //@TODO: Maybe change type of seed from int to something
        //i.e., typecast or figure out what type of seeds were possible
    cout << "Type Y if you would like to manually seed your simulation: ";
    cin >> checkSeed;
    if(toupper(checkSeed) == 'Y'){
        cout << "Enter in your seed as an integer: ";
        cin >> seed;
        srand(seed);
    } else {
        srand(time(NULL));
    }
    
    
    //Create an instance of the printerList that will hold all the printers
    printerListType printerList(/*numOfPrinters,*/printRate);

    //Create jobQueueArray which will house all tieried 0->n-1 priority job queue's w/ tier information
    /*
     * @TODO: Create constructor which will tell how many tiers & explicit values for each of those tiers
     *          could possibly be in a hashmap, or read in via for/while loop and added dynamically in the
     *          constructor in jobQueueArrayClass
     */
    jobQueueArray jqArr;

    //Create an instance of a job
    jobType job;

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
