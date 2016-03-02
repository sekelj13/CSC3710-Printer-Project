#include <fstream>
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
void runSimulation(int numOfPrinters, int numJobs, int maxPages, int printRate[], int numTiers, map<string, int> tiers, double jpm, double costPerPage, int printCapacity, int downTime,double probOfFailure,ofstream &outfile);

/*
 *
 */
int poisson(vector<double> &cutoffs, double jpm);

/*
 *
 */
void poissonJobs(int k, const vector<double> &cutoffs, int &jobNum, jobQueueArray &jqArr, int sTime, int maxPages,int &totalPagesPrinted,ofstream &outfile);


/*
 *
 */
int factorial(int n);


/*  ============================ Main Starts ============================ */
int main(void)
{
    int numJobs = 100, numOfPrinters = 3, maxPages = 50, numTiers = 3,
        printCapacity = 300, downTime = 10, pr = 0;
    
    double costPerPage = .3,jobsPerMinute = 1,probOfFailure = .05;
    
    char boolPrintRate = 'x';
    
    char jobFrequency = 'a';

    ofstream outfile;
    string outfileName;
    cin >> outfileName;
    outfile.open(outfileName.c_str());
    cout << "File name: " << outfileName << endl;

    //Get the number of jobs
    cin >> numJobs;
    
    //Get the number of printers
    cin >> numOfPrinters;
    int printRate[numOfPrinters];
    
    //Printers print randomly or linearly
    //pr = print rate
    cin >> boolPrintRate;
    if (toupper(boolPrintRate) == 'Y') {
        //Get Printer Rate
	cin >> pr;
	for (int i=0;i < numOfPrinters;i++)
          printRate[i]=pr;
    } else {
        //for each printer, get print rate
        for (int i=0; i < numOfPrinters; i++) {
            cin >> pr;
            printRate[i] = pr;
        }
    }
    
    //Get the maximum number of pages able to printer / job
    cin >> maxPages;
    
    //Get the total number of tiers in the waiting list queue
    cin >> numTiers;

    //Initalize & Setup Tiers
    map<string, int> tiers;
    int tier = 0;
    stringstream temp;
    
    for (int i= 0; i < numTiers; i++) {
        cin >> tier;
        temp << "tier" << i;
        tiers[temp.str()] = tier;
        temp.clear();
    }
    //@TODO: Remove
    //jobQueueArray newTier(tiers);
    

    //@TODO: Clean Up
    while (toupper(jobFrequency) != 'J' &&
           toupper(jobFrequency) != 'M')
    {
            // << "Or" << endl
            // << "Will there be an average of one job every several minutes?" << endl
            // << "Input 'J' for jobs per minute or 'M' for minutes per job: ";
        cin >> jobFrequency;
    }
    
    if (toupper(jobFrequency) == 'J') {
        cin >> jobsPerMinute;
    
    } else {
        cin >> jobsPerMinute;
        jobsPerMinute = 1 / jobsPerMinute;
    }
    
    //Get the printed page per dollar cost
    cin >> costPerPage;
    
    //Get max number pages printer can print before maintence is needed
    cin >> printCapacity;
    
    //Get the amount of time a printer is down for maintence
    cin >> downTime;

    //get probability of failure as double less than 1
    cin >> probOfFailure;
    
    //Run the simulation now that data has all been collected
    runSimulation(numOfPrinters,numJobs,maxPages,printRate,numTiers,tiers,jobsPerMinute,costPerPage,printCapacity,downTime,probOfFailure,outfile);

    return 0;
}
//--------------------------- Main Ends -----------------------------------


/*
 * Functions Begin
 */

//Runs Simulation

void runSimulation(int numOfPrinters, int numJobs, int maxPages, int printRate[], int numTiers, map<string, int> tiers, double jpm, double costPerPage, int printCapacity, int downTime, double probOfFailure, ofstream &outfile)
{
    /*
     * sTime = Simluation Time
     * numOfPrinters = Total Number of Printers
     * transtime = Transaction Time
    cutoffs.push_back(0);
     *
     */

    char checkSeed;
    int seed = 0, sTime = 0, jobNum = 0, waitTime = 0;
    int totalPagesPrinted = 0;
    
    //Set or Use time Seed
    cin >> checkSeed;
    if(toupper(checkSeed) == 'Y'){
        cin >> seed;
        srand(seed);
    } else {
        srand(time(NULL));
    }
    
    //Initalize cuttoffs with 0, otherwise cause seg fault because ! filled
    vector<double> cutoffs;
    
    int k=poisson(cutoffs,jpm);

    //Create an instance of the printerList that will hold all the printers
    printerListType printerList(numOfPrinters, printRate, downTime,probOfFailure);

    //Create jobQueueArray which will house all tieried 0->n-1 priority job queue's w/ tier information
    jobQueueArray jqArr(tiers);

    //Create an instance of a job
    jobType job;

    //while loop to continue until jobQueue empty and printerList empty as well
    while (jobNum < numJobs || printerList.getNumberOfBusyPrinters() != 0 || !jqArr.isEmpty()) {

        sTime++;
        outfile << endl << "At time unit "<< sTime << endl;

        //update printer list & decrements
        printerList.updatePrinters(outfile);
        
        jqArr.updateWaitingQueues();

        //if not enough jobs, have chance to make jobs
        if (jobNum < numJobs)
            poissonJobs(k,cutoffs,jobNum,jqArr,sTime,maxPages,totalPagesPrinted,outfile);

        //if printer is free and queue nonempty, pair job with printer
        while (printerList.getFreePrinterID()!= -1 && !jqArr.isEmpty()){
            if (jqArr.checkNextJob().getWaitingTime() != -1 ) {
                waitTime += jqArr.checkNextJob().getWaitingTime();
                printerList.setPrinterBusy(printerList.getFreePrinterID(), jqArr.getNextJob(),outfile);
            }
        }
    }

    
    //Output Statistics
    outfile << endl << "Simulation Completed.\n"
            << "Total number of jobs: " << jobNum << endl
            << "Simulation time: " << sTime << endl
            << "Number of printers: " << numOfPrinters << endl
            << "Total number of pages printed: " << totalPagesPrinted << endl
            << "Total cost of printing: " << totalPagesPrinted*costPerPage << endl
            << "Total Wait Time between all jobs: " << waitTime << endl << endl;
	    outfile << "============ Tier-by-Tier Statistics ============" << endl;

    //@TODO: Need to assign these via the return functions implemented in jobQueueArray
    map<string, int> queueJobsCreated;
    map<string, int> queueWaitTimes;

    map<string, int>::iterator tierIterator;
    int i = 1;

    
    
    for(tierIterator = queueJobsCreated.begin(); tierIterator != queueJobsCreated.end(); tierIterator++){
        outfile << "Tier " << i << "number of jobs: " << tierIterator->second << endl;
        outfile << "Tier " << i << "number of pages printed: " << 1 << endl; //@TODO: Implement
        i++;
    }
    for(tierIterator = queueWaitTimes.begin(); tierIterator != queueWaitTimes.end(); tierIterator++){
        outfile << "Tier " << i << " average job wait time: " << 1 << endl; //@TODO: Implement subdivison
    }
    
    outfile    << "Total Jobs: "  << jobNum << endl
            << "Average Wait Time between all jobs: " << (float)waitTime/jobNum << endl;
}

int poisson(vector<double> &cutoffs, double jpm)
{
    double totalpoisson=0;
    int k = 0;
    double poisson;
    
    while (totalpoisson < .95){
        poisson = pow(jpm,k) * exp(-jpm)/factorial(k);
        totalpoisson += poisson;
        cutoffs.push_back(totalpoisson);
        k++;
    }
    cutoffs.push_back(1);
    return k;
}

void poissonJobs(int k, const vector<double> &cutoffs, int &jobNum, jobQueueArray &jqArr, int sTime, int maxPages,int &totalPagesPrinted,ofstream &outfile)
{
    double prob=((double)rand())/RAND_MAX;
    int i=0;
    jobType job;
    //increment i until reach point in cutoffs where prob is less than cutoffs[i]
    while (prob > cutoffs[i] and i < k) {
        i++;
    }
    //the ith position is also the number of jobs we need to create, so we create i jobs
    for (int j = 0;j < i;j++) {
        jobNum++;
        job.setJobInfo(jobNum,sTime,0,maxPages);
        outfile << "Job number " << job.getJobNumber() << "\nPages Created " << job.getNumPages() << endl;
        totalPagesPrinted += job.getNumPages();
        jqArr.sendJob(job);
    }
}

//Factorial function ripped directly off of cplusplus.com
int factorial(int n) {
    return (n == 1 || n == 0) ? 1 : factorial(n-1) * n;
}
