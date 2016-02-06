#include <iostream>
#include <string>
#include <cstdlib>
 
#include "Simulation.h"
#include "queueAsArray.h"

using namespace std;
 

//*************** jobType ************

void jobType::setJobInfo(int customerN, int arrvTime, 
                                   int wTime, int tTime)
{
    jobNumber = customerN;
    arrivalTime = arrvTime;
    waitingTime = wTime;
    printTime = tTime;
}

jobType::jobType(int customerN, int arrvTime, 
                           int wTime, int tTime)
{
    setJobInfo(jobN, arrvTime, wTime, tTime);
}


int jobType::getWaitingTime() const
{
    return waitingTime;
}

void jobType::incrementWaitingTime()
{
    waitingTime++;
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

void setTier() {
    if (pages < 11)
        tier=1;
    else if (pages < 21)
        tier=2;
    else
        tier=3;
}

int getTier() {
    return tier;
}

void setPages(int val) {
    pages = val;
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

void setPrintRate(int pr) {
    printRate = pr;
}

int getPrintRate() {
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

/*
these functions aren't needed
int printerType::getCurrentJobNumber() const
{
    return currentJob.getCustomerNumber();
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

*/

//************** printerListType ***********

printerListType::printerListType(int num)
{
    numOfPrinters = num;
    printers = new printerType[num];
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

int printerListType::getNumberOfBusyPrinters() const
{
    int busyPrinters = 0;

    int i;

    for (i = 0; i < numOfPrinters; i++)
        if (!printers[i].isFree())
            busyPrinters++;

    return busyPrinters;
}

void printerListType::setPrinterBusy(int printerID, 
                                   jobType cJob, 
                                   int tTime)
{
    printers[printerID].setBusy();
    printers[printerID].setPrintTime(tTime);
    printers[printerID].setCurrentJob(cCustomer);
}

void printerListType::setPrinterBusy(int printerID, 
                                   jobType cJob)
{
    int time;

    time = cJob.getPrintTime();

    printers[printerID].setBusy();
    printers[printerID].setPrintTime(time);
    printers[printerID].setCurrentJob(cCustomer);
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


waitingJobQueueType::waitingCustomerQueueType(int size)
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
