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
    transactionTime = tTime;
}

jobType::customerType(int customerN, int arrvTime, 
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

int jobType::getTransactionTime() const
{
    return transactionTime;
}

int jobType::getJobNumber() const
{
    return jobNumber;
}


//**************** printerType **********

printerType::serverType()
{
    status = "free";
    transactionTime = 0;
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

void printerType::setTransactionTime(int t)
{
    transactionTime = t;
}

void printerType::setTransactionTime()
{
    int time;

    time = currentJob.getTransactionTime();

    transactionTime = time;
}

void printerType::decreaseTransactionTime()
{
    transactionTime--;
}

int printerType::getRemainingTransactionTime() const
{
    return transactionTime;
}

void printerType::setCurrentJob(jobType cCustomer)
{
    currentJob = cCustomer;
}

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

int printerType::getCurrentJobTransactionTime() const
{
    return currentJob.getTransactionTime();
}


//************** printerListType ***********

printerListType::serverListType(int num)
{
    numOfPrinters = num;
    printers = new serverType[num];
}

printerListType::~serverListType()
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

void printerListType::setPrinterBusy(int serverID, 
                                   jobType cJob, 
                                   int tTime)
{
    printers[serverID].setBusy();
    printers[serverID].setTransactionTime(tTime);
    printers[serverID].setCurrentJob(cCustomer);
}

void printerListType::setPrinterBusy(int serverID, 
                                   jobType cJob)
{
    int time;

    time = cJob.getTransactionTime();

    printers[serverID].setBusy();
    printers[serverID].setTransactionTime(time);
    printers[serverID].setCurrentJob(cCustomer);
}

void printerListType::updatePrinters(ostream& outFile)
{
    int i;

    for (i = 0; i < numOfPrinters; i++)
        if (!printers[i].isFree())
        {
            printers[i].decreaseTransactionTime();

            if (printers[i].getRemainingTransactionTime() == 0)
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
                              getCurrentJobTransactionTime()
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
