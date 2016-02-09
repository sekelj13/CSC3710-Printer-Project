#include <iostream>
#include <string>
#include <cstdlib>
 
#include "simulation.h"
#include "queueAsArray.h"

using namespace std;
 

//*************** jobType ************
jobType::jobType(int jobN, int arrvTime,
                           int wTime, int max)
{
    setJobInfo(jobN, arrvTime, wTime, max);
    setTier();
}

void jobType::setJobInfo(int customerN, int arrvTime,
                         int wTime, int max,int random)
{
    
    
    jobNumber = customerN;
    arrivalTime = arrvTime;
    waitingTime = wTime;
    maxPages = max;

    pages = random % maxPages;
}

int jobType::getWaitingTime() const
{
    return waitingTime;
}

void jobType::incrementWaitingTime()
{
    waitingTime++;
}

void jobType::incrementPrintTime(){
    printTime++;
}

void jobType::setWaitingTime(int time)
{
    waitingTime = time;
}

int jobType::getArrivalTime() const
{
    return arrivalTime;
}

int jobType::getPrintTime() const
{
    return printTime;
}

int jobType::getJobNumber() const
{
    return jobNumber;
}

void jobType::setTier()
{
    if (pages < 11)
        tier = 1;
    else if (pages < 21)
        tier = 2;
    else
        tier = 3;
}

int jobType::getTier() {
    return tier;
}

void jobType::setPages(int val) {
    pages = val;
}

int jobType::getNumPages() {
    return pages;
}

//**************** jobQueue **********

jobQueue::jobQueue()
{
    numJobsCreated=0;
}

//Creating dummy job with -1 in order to figure out where queue ends & begins
//inserting dummy job @ -1, when -1 hit again, do not process customer
//Now queue full & ready
void jobQueue::updateWaitingQueue()
{
    
    job.setWaitingTime(-1);
    int wTime = 0;
	
	addQueue(job);
//Doesn't process -1 job
    while (wTime != -1)
    {
        job = front();
        deleteQueue();

        wTime = job.getWaitingTime();
        if (wTime == -1)
            break;
        job.incrementWaitingTime();
        addQueue(job);
    }
}

int jobQueue::queueWaitTime(int& waitTime)
{
    int numQueuedJobs = 0;
    while (!isEmptyQueue() && front().getJobNumber() != -1){
        numQueuedJobs++;
        waitTime += front().getWaitingTime();
        deleteQueue();
    }
    return numQueuedJobs;
}
    
int jobQueue::getNumJobs()
{
    return numJobs;
}

void jobQueue::updateWaitTime()
{
   numJobs=0;
   jobType job;
   job.setWaitingTime(-1);
   int wTime=0;

    while(wTime!=-1){
        job=front();
        deleteQueue();
        wTime=job.getWaitingTime();
        if(wTime!=-1){
            job.incrementWaitingTime();
            waitTime++;
            numJobs++;
            addQueue(job);
        }
     }
    //cycle through jobs using dummy job as placeholder
}

void jobQueue::addJob(jobType job)
{
    numJobsCreated++;
    addQueue(job);
}

jobType jobQueue::removeJob()
{
    jobType job=front();
    deleteQueue();
    return job;
}

int jobQueue::getNumJobsCreated()
{
    return numJobsCreated;
}


//**************** jobQueueArray *************

jobQueueArray::jobQueueArray()
{
    //Create Job Queue Array
    //@TODO: Change to more modular type
    /*
    if(job.getTier() == 1){
        jobQArr[0].addQueue(job);
    } else if (job.getTier() == 2) {
        jobQArr[1].addQueue(job);
    } else if (job.getTier() == 3) {
        jobQArr[2].addQueue(job);
    } else {
        cerr << "Tier Error in jobQueueArray";
    }*/
    
}


jobType jobQueueArray::getNextJob()
{
    if(!jobQArr[0].isEmptyQueue()){
        job = jobQArr[0].front();
        jobQArr[0].deleteQueue();
        return job;
    } else if(!jobQArr[1].isEmptyQueue()){
        job = jobQArr[1].front();
        jobQArr[1].deleteQueue();
        return job;
    } else if(!jobQArr[2].isEmptyQueue()){
        job = jobQArr[2].front();
        jobQArr[2].deleteQueue();
        return job;
    } else {
        jobType dummyJob;
        dummyJob.setJobInfo(-1,-1,-1, -1,-1);
        return dummyJob;
    }
    
}

jobType jobQueueArray::checkNextJob()
{
    if(!jobQArr[0].isEmptyQueue()){
        job = jobQArr[0].front();
        return job;
    } else if(!jobQArr[1].isEmptyQueue()){
        job = jobQArr[1].front();
        return job;
    } else if(!jobQArr[2].isEmptyQueue()){
        job = jobQArr[2].front();
        return job;
    } else {
        jobType dummyJob;
        dummyJob.setJobInfo(-1,-1,-1, -1,-1);
        return dummyJob;
    }
}

void jobQueueArray::sendJob(jobType job)
{
    if(job.getTier() == 1){
        jobQArr[0].addQueue(job);
    } else if (job.getTier() == 2) {
        jobQArr[1].addQueue(job);
    } else if (job.getTier() == 3) {
        jobQArr[2].addQueue(job);
    } else {
        cerr << "Tier Error in jobQueueArray";
    }
}

bool jobQueueArray::isEmpty()
{
    for(int i = 0; i < 3; i++){
        if(!jobQArr[i].isEmptyQueue()){
            return false;
        }
    }
    return true;
}

void jobQueueArray::updateWaitingQueues()
{
    for(int i = 0; i < 3; i++)
    {
        //jobQArr[i].
    }
}


//**************** printerType **********

printerType::printerType()
{
    status = "free";
    printTime = 0;
}

bool printerType::isFree() const
{
    return (status == "free");
}

void printerType::setBusy()
{
    status = "busy";
}

void printerType::setFree()
{
    status = "free";
}

void printerType::setPrintRate(int pr) {
    printRate = pr;
}

int printerType::getPrintRate() {
    return printRate;
}

void printerType::setPrintTime(int t)
{
    printTime = t;
}

void printerType::setPrintTime()
{
    int time;

    time = currentJob.getPrintTime();

    printTime = time;
}

void printerType::decreasePrintTime()
{
    printTime--;
}

int printerType::getRemainingPrintTime() const
{
    return printTime;
}

void printerType::setCurrentJob(jobType cCustomer)
{
    currentJob = cCustomer;
}


//these functions aren't needed
int printerType::getCurrentJobNumber() const
{
    
    return currentJob.getJobNumber();
}

int printerType::getCurrentJobArrivalTime() const
{
    return currentJob.getArrivalTime();
}

int printerType::getCurrentJobWaitingTime() const
{
    return currentJob.getWaitingTime();
}

int printerType::getCurrentJobPrintTime() const
{
    return currentJob.getPrintTime();
}



//************** printerListType ***********

printerListType::printerListType(int num, int pr)
{
    numOfPrinters = num;
cout << "swag\n";
    printers = new printerType[num];
cout << "Swag\n";

    for (int i=0;i < num;i++) {
        printers[i].setPrintRate(pr);
    }
}

printerListType::~printerListType()
{
    delete [] printers;
}

int printerListType::getFreePrinterID() const
{
    int printerID = -1;

    int i;

    for (i = 0; i < numOfPrinters; i++)
        if (printers[i].isFree())
        {
            printerID = i;
            break;
        }

    return printerID;
}

int printerListType::getNumberOfBusyPrinters() 
{
    int busyPrinters = 0;

    int i;

    for (i = 0; i < numOfPrinters; i++)
        if (!printers[i].isFree())
            busyPrinters++;

    return busyPrinters;
}

int printerListType::getNumberOfFreePrinters()
{
    int freePrinters = 0;
    
    int i;
    
    for (i = 0;i < numOfPrinters;i++) {
        if (printers[i].isFree()) {
            freePrinters++;
        }
    }
    
    return freePrinters;
}

/*void printerListType::setPrinterBusy(int printerID, jobType cJob, int tTime)
{
    printers[printerID].setBusy();
    printers[printerID].setPrintTime(tTime);
    printers[printerID].setCurrentJob(cJob);
}*/

void printerListType::setPrinterBusy(int printerID,
                                   jobType cJob)
{
    int time;

    time = cJob.getPrintTime();

    printers[printerID].setBusy();
    printers[printerID].setPrintTime(time);
    printers[printerID].setCurrentJob(cJob);
}

void printerListType::updatePrinters(ostream& outFile)
{
    int i;

    for (i = 0; i < numOfPrinters; i++)
        if (!printers[i].isFree())
        {
            printers[i].decreasePrintTime();

            if (printers[i].getRemainingPrintTime() == 0)
            {
                outFile << "From printer number  " << (i + 1) 
                        << " job number "
                        << printers[i].getCurrentJobNumber()
                        << "\n     departed at time unit "
                        << printers[i].
                              getCurrentJobArrivalTime()
                           + printers[i].
                              getCurrentJobWaitingTime() 
                           + printers[i].
                              getCurrentJobPrintTime()
                        << endl;
                printers[i].setFree();
            }
        }
}

//*************** waitQueue ************


waitingJobQueueType::waitingJobQueueType(int size)
                          :queueType<jobType>(size)
{
}

//Creating dummy job with -1 in order to figure out where queue ends & begins
//inserting dummy job @ -1, when -1 hit again, do not process customer
//Now queue full & ready
void waitingJobQueueType::updateWaitingQueue()
{
    jobType cust;

    cust.setWaitingTime(-1);  
    int wTime = 0;
	
	addQueue(cust);
//Doesn't process -1 job
    while (wTime != -1)
    {
        cust = front();
        deleteQueue();

        wTime = cust.getWaitingTime();
        if (wTime == -1)
            break;
        cust.incrementWaitingTime();
        addQueue(cust);
	}
}

int waitingJobQueueType::queueWaitTime(int& waitTime)
{
    int numQueuedJobs = 0;
    while (!isEmptyQueue() && front().getJobNumber() != -1){
        numQueuedJobs++;
        waitTime += front().getWaitingTime();
        deleteQueue();
    }
    return numQueuedJobs;
}
