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
    failure = false;
    maxPages = 100;
}

printerType::printerType(int maxpg,double prob) {
    status = "free";
    printTime = 0;
    failure = false;
    maxPages = maxpg;
    if (0 >= prob || 1 <= prob) {
        cerr << "ERROR: Invalid probability of failure" << endl
           << "probOfFailure set to .05" << endl;
        probOfFailure = .05;
    } else
        probOfFailure = prob;
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

bool printerType::checkFail() {
    if ((int check = rand() %1000 + 1) <= (probOfFailure * 1000)) {//check is a random integer between 1 and 1000
                                                                  //if check <= probOfFailure*1000, a failure occurs
        failure = true;
    } else
        failure = false;
    return failure;
}

int getPaperLeft() {
    return paperLeft;
}

void refillPrinter() {
    paperLeft = maxPaper;
}

void setProbOfFailure(int prob {
    probOfFailure = prob;
}

double getProbOfFailure {
    return probOfFailure;
}
