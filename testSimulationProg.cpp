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
void runSimulation(int numOfPrinters, int numJobs, int maxPages, int printRate[], int numTiers, map<string, int> tiers, double jpm, double costPerPage, int printCapacity, int downTime,ofstream &outfile);

/*
 *
 */
int poisson(double *cutoffs, double jpm);

double* poissonQuickFix(double *cutOffs, double jpm);

/*
 *
 */
void poissonJobs(int k, double *cutoffs, int *jobNum, jobQueueArray *jqArr, int sTime, int maxPages, int *totalPagesPrinted,ofstream &outfile);


/*
 *
 */
int factorial(int n);


/*  ============================ Main Starts ============================ */
int main(void)
{
    int numJobs = 100, numOfPrinters = 3, maxPages = 50, numTiers = 3,
        printCapacity = 300, downTime = 10, pr = 0;
    
    double costPerPage = .3,jobsPerMinute = 1;
    
    char boolPrintRate = 'x';
    
    char jobFrequency = 'a';

    ofstream outfile;
    string outfileName;
    cin >> outfileName;
    outfile.open(outfileName.c_str());
    //@TODO: Change cins to istream(read all data inputs / info from a file or from cmdline)

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
    //Run the simulation now that data has all been collected
    runSimulation(numOfPrinters,numJobs,maxPages,printRate,numTiers,tiers,jobsPerMinute,costPerPage,printCapacity,downTime,outfile);

    return 0;
}
//--------------------------- Main Ends -----------------------------------


/*
 * Functions Begin
 */

//Runs Simulation

void runSimulation(int numOfPrinters, int numJobs, int maxPages, int printRate[], int numTiers, map<string, int> tiers, double jpm, double costPerPage, int printCapacity, int downTime,ofstream &outfile)
{
    /*
     * sTime = Simluation Time
     * numOfPrinters = Total Number of Printers
     * transtime = Transaction Time
     *
     */

    char checkSeed;
    
    double *cutoffs;
    
    int seed = 0, sTime = 0, jobNum = 0, waitTime = 0;
    int totalPagesPrinted = 0; //total number of pages printed
    
    //Seed the program
    //@TODO: Maybe change type of seed from int to something
        //i.e., typecast or figure out what type of seeds were possible
    cin >> checkSeed;
    if(toupper(checkSeed) == 'Y'){
        cin >> seed;
        srand(seed);
    } else {
        srand(time(NULL));
    }
    
    int k=poisson(cutoffs,jpm);
    double cutOffs[k];          //could not figure out how to make this a working function, so it remains here.
    double poisson;
    double totalpoisson=0;
    for(int l=0;l<(k-1);l++){
        poisson = pow(jpm,l) * exp(-jpm)/factorial(l);
        totalpoisson += poisson;
        cutOffs[l] = totalpoisson;
        cout<<cutOffs[l]<<endl;
    }
    cutOffs[k]=1;
//    cutOffs=poissonQuickFix(cutoffs,jpm);
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
        outfile << endl << "At time unit "<< sTime << endl;

        //update printer list & decrements
        printerList.updatePrinters(outfile);
        
        //job queue array update
        jqArr.updateWaitingQueues();

        //create jobs while jobNum < numJobs
        if (jobNum < numJobs) {
            cout<<cutOffs[0]<<endl;
            poissonJobs(k,cutOffs,&jobNum,&jqArr,sTime,maxPages,&totalPagesPrinted,outfile);
            cout<<jobNum<<endl;
        }
        break;
        //if printer is free and queue nonempty, pair job with printer
        while (printerList.getFreePrinterID()!= -1 && !jqArr.isEmpty()){
            if (jqArr.checkNextJob().getWaitingTime() != -1 ) {
                jobType job;
                waitTime += job.getWaitingTime();
                printerList.setPrinterBusy(printerList.getFreePrinterID(), jqArr.getNextJob(),outfile);
            }
        }
    }
    
    //Output Statistics
    outfile    << endl << "Simulation Completed.\n"
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

int poisson(double *cutoffs, double jpm)
{
    double totalpoisson=0;
    int k = 0;
    double poisson;
    while (totalpoisson < .95){
        poisson = pow(jpm,k) * exp(-jpm)/factorial(k);
        totalpoisson += poisson;
        cutoffs[k] = totalpoisson;
        cout<<cutoffs[k]<<endl;
        k++;
    }
    cutoffs[k]=1;
    return k;
}

double* poissonQuickFix(double *cutOffs, double jpm){
    double totalpoisson=0;
    int k = 0;
    double poisson;
    while (totalpoisson < .95){
        poisson = pow(jpm,k) * exp(-jpm)/factorial(k);
        totalpoisson += poisson;
        cutOffs[k] = totalpoisson;
        cout<<cutOffs[k]<<endl;
        k++;
    }
    cutOffs[k]=1;
    return cutOffs;
}

void poissonJobs(int k, double *cutoffs, int *jobNum, jobQueueArray *jqArr, int sTime, int maxPages,int *totalPagesPrinted,ofstream &outfile)
{
    double prob=(double)rand()/RAND_MAX;
    int i=0;
    int j=0;
    jobType job;
    for (i=0; i <= k; i++) {
        cout << cutoffs[i] << endl;
        if (prob <= cutoffs[i]) {
            for (j=0; j < i; j++) {
                (*jobNum)++;
                job.setJobInfo(*jobNum,sTime,0,maxPages);
                outfile << "Job number " << job.getJobNumber() << "\nPages Created " << job.getNumPages() << endl;
                *totalPagesPrinted=(*totalPagesPrinted)+job.getNumPages();
                jqArr->sendJob(job);
            }
        }
    }
}

//Factorial function ripped directly off of cplusplus.com
int factorial(int n) {
    return (n == 1 || n == 0) ? 1 : factorial(n-1) * n;
}
