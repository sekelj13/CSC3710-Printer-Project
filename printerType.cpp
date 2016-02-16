#include <iostream>
#include <string>
#include <cstdlib>

#include "simulation.h"
#include "queueAsArray.h"

using namespace std;

/* ======================== printerType ======================== */

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
    int time;
    
    time = currentJob.getNumPages();
    
    cout << "In job " << currentJob.getJobNumber() << " Pages left to print: " << time << endl;
    
    printTime = time;
}

void printerType::decreasePrintTime()
{
    printTime-=printRate;
    cout << "Print time left in job number " << currentJob.getJobNumber() << ": " << printTime << endl;
}

int printerType::getRemainingPrintTime() const
{
    return printTime;
}

void printerType::setCurrentJob(jobType cJob)
{
    currentJob = cJob;
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