#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <cstring>
#include <cmath>
#include <cctype>
#include <clocale>
#include <map> //Hash Map
#include <sstream>

#include "simulation.h"
#include "queueAsArray.h" 

using namespace std;

/*
 * Run the simulation function(main will populate the run simulation parameters)
 *
 */
void runSimulation(int numOfPrinters, int numJobs, int maxPages, int printRate[], int numTiers, map<string, int> tiers, int jpm, double costPerPage, int printCapacity, int downTime);

/*
 *
 */
int poisson(double *cutoffs, int jpm);

/*
 *
 */
void poissonJobs(int k, double *cutoffs, int *jobNum, jobType *job, int clock, int maxPages);


/*
 *
 */
int factorial(int n);


/*  ============================ Main Starts ============================ */
int main(void)
{
    int numJobs = 100, numOfPrinters = 3, maxPages = 50, numTiers = 3, jobsPerMinute = 1,
        printCapacity = 300, downTime = 10, pr = 0;
    
    double costPerPage = .3;
    
    char boolPrintRate = 'x';
    
    char jobFrequency = 'a';

    
    
    //@TODO: Change cins to istream(read all data inputs / info from a file or from cmdline)

    //Get the number of jobs
    cout << "Enter Number of Jobs: " << endl;
    cin >> numJobs;
    
    //Get the number of printers
    cout << "Specify the Number of Printers: " << endl;
    cin >> numOfPrinters;
    int printRate[numOfPrinters];

    
    //Printers print randomly or linearly
    //pr = print rate
    cout << "Do all printers print at the same rate? (y/n) " << endl;
    cin >> boolPrintRate;
    if (toupper(boolPrintRate) == 'Y') {
        //Get Printer Rate
        cout << "Specify Print Rate: " << endl;
	cin >> pr;
	for (int i=0;i < numOfPrinters;i++)
          printRate[i]=pr;
    } else {
        //for each printer, get print rate
        for (int i=0; i < numOfPrinters; i++) {
            cout << "Specify printer " << i+1 << "'s print rate: ";
            cin >> pr;
            printRate[i] = pr;
        }
    }
    
    //Get the maximum number of pages able to printer / job
    cout << "Input the Maximum Pages Able To Print: " << endl;
    cin >> maxPages;
    
    
    //Get the total number of tiers in the waiting list queue
    cout << "Enter the amount total amount of printer tiers: ";
    cin >> numTiers;
    
    /*
     * 
     * Used a hashmap to initalize tiers & cutoffs
     * send hashmap to jobQueueArray & fill each array that way
     *
     */
    //Get the cutoff point for each tier
    map<string, int> tiers;
    // map<string, int>::iterator tierIterator; //Iterate through the tiers
    int tier = 0;
    stringstream temp;

    for (int i= 0; i < numTiers; i++) {
        cout << "Enter the cutoff point for tier " << i+1 << ": ";
        cin >> tier;
        temp << "tier" << i;
        tiers[temp.str()] = tier;
        temp.clear();
    }
    
    //@TODO: Remove
    jobQueueArray newTier(tiers);
    
    //this while loop is a bit of error-checking to make sure jobFrequency gets one
    //of the 2 values it needs.
    //@TODO: Clean up cout statement a bit?
    while (toupper(jobFrequency) != 'J' &&
           toupper(jobFrequency) != 'M')
    {
        cout << "Will there be an average of multiple jobs per minute coming in?" << endl
             << "Or" << endl
             << "Will there be an average of one job every several minutes?" << endl
             << "Input 'J' for jobs per minute or 'M' for minutes per job: ";
        cin >> jobFrequency;
    }
    
    if (toupper(jobFrequency) == 'J') {
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
    
    
    //Run the simulation now that data has all been collected
    runSimulation(numOfPrinters,numJobs,maxPages,printRate,numTiers,tiers,jobsPerMinute,costPerPage,printCapacity,downTime);

    return 0;
}
//--------------------------- Main Ends -----------------------------------


/*
 * Functions Begin
 */

//Runs Simulation
void runSimulation(int numOfPrinters, int numJobs, int maxPages, int printRate[], int numTiers, map<string,int> tiers, int jpm, double costPerPage, int printCapacity, int downTime)
{
    /*
     * sTime = Simluation Time
     * numOfPrinters = Total Number of Printers
     * transtime = Transaction Time
     *
     */

    char checkSeed;
    
    
    int seed = 0, sTime = 0, jobNum = 0, waitTime = 0;
    int totalPagesPrinted = 0; //total number of pages printed
    
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
    printerListType printerList(numOfPrinters, printRate, downTime);

    //Create jobQueueArray which will house all tieried 0->n-1 priority job queue's w/ tier information
    /*
     * @TODO: Create constructor which will tell how many tiers & explicit values for each of those tiers
     *          could possibly be in a hashmap, or read in via for/while loop and added dynamically in the
     *          constructor in jobQueueArrayClass
     */
    
    jobQueueArray jqArr(tiers);

    //Create an instance of a job
    jobType job;

    //while loop to continue until jobQueue empty and printerList empty as well
    while (jobNum < numJobs || printerList.getNumberOfBusyPrinters() != 0 || !jqArr.isEmpty()) {

        //increment Simulation Time(sTime)
        sTime++;
        cout << endl << "At time unit "<< sTime << endl;

        //update printer list & decrements
        printerList.updatePrinters(cout);
        
        //job queue array update
        jqArr.updateWaitingQueues();

        //create jobs while jobNum < numJobs
        if (jobNum < numJobs) {
          jobNum++;
          job.setJobInfo(jobNum, sTime, 0, maxPages);
          cout << "Job number " << job.getJobNumber() << "\nPages Created " << job.getNumPages() << endl;
          totalPagesPrinted += job.getNumPages();
          jqArr.sendJob(job);
        }
        
        //if printer is free and queue nonempty, pair job with printer
        while (printerList.getFreePrinterID()!= -1 && !jqArr.isEmpty()){
            if (jqArr.checkNextJob().getWaitingTime() != -1 ) {
                jobType job;
                waitTime += job.getWaitingTime();
                printerList.setPrinterBusy(printerList.getFreePrinterID(), jqArr.getNextJob());
            }
        }
    }
    
    //Output Statistics
    cout    << endl << "Simulation Completed.\n"
            << "Simulation time: " << sTime << endl
            << "Number of printers: " << numOfPrinters << endl
            << "Total number of pages printed: " << totalPagesPrinted << endl
            << "Total Wait Time between all jobs: " << waitTime << endl << endl;
	    cout << "============ Tier-by-Tier Statistics ============" << endl;

    //@TODO: Need to assign these via the return functions implemented in jobQueueArray
    map<string, int> queueJobsCreated;
    map<string, int> queueWaitTimes;

    map<string, int>::iterator tierIterator;
    int i = 1;

    
    
    for(tierIterator = queueJobsCreated.begin(); tierIterator != queueJobsCreated.end(); tierIterator++){
        cout << "Tier " << i << "number of jobs: " << tierIterator->second << endl;
        cout << "Tier " << i << "number of pages printed: " << 1 << endl; //@TODO: Implement
        i++;
    }
    for(tierIterator = queueWaitTimes.begin(); tierIterator != queueWaitTimes.end(); tierIterator++){
        cout << "Tier " << i << " average job wait time: " << 1 << endl; //@TODO: Implement subdivison
    }
    
    cout    << "Total Jobs: "  << jobNum << endl
            << "Average Wait Time between all jobs: " << (float)waitTime/jobNum << endl;
}

int poisson(double *cutoffs, int jpm)
{
    double totalpoisson=0;
    int k = 0;
    double poisson;
    while (totalpoisson < .95){
        poisson = pow(jpm,k) * exp(-jpm)/factorial(k);
        totalpoisson += poisson;
        cutoffs[k] = totalpoisson;
        k++;
    }
    cutoffs[k]=1;
    return k;
}

void poissonJobs(int k, double *cutoffs, int *jobNum, jobType *job, int clock, int maxPages)
{
    double prob=(double)rand()/RAND_MAX;
    int i=0;
    int j=0;
    for (i=0; i <= k; i++) {
        if (prob <= cutoffs[i]) {
            for (j=0; j < i; j++) {
                (*jobNum)++;
                job->setJobInfo(*jobNum,clock,0,maxPages);
            }
        }
    }
}

//Factorial function ripped directly off of cplusplus.com
int factorial(int n) {
    return (n == 1 || n == 0) ? 1 : factorial(n-1) * n;
}
