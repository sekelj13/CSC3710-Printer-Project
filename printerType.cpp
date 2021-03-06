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
    maxPaper = 100;
}

printerType::printerType(int maxpg,double prob,int down)
{
    status = "free";
    printTime = 0;
    failure = false;
    maxPaper = maxpg;
    if (0 >= prob || 1 <= prob) {
        cerr << "ERROR: Invalid probability of failure" << endl
           << "probOfFailure set to .05" << endl;
        probOfFailure = .05;
    } else
        probOfFailure = prob;
    fixTime = down;
    downTime = down;
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

void printerType::setPrintTime(int t,ofstream& outfile)
{
    int time;
    
    time = currentJob.getNumPages();
    
    outfile << "In job " << currentJob.getJobNumber() << " Pages left to print: " << time << endl;
    
    printTime = time;
}

void printerType::decreasePrintTime(ofstream& outfile)
{
    printTime-=printRate;
    paperLeft-=printRate;
    outfile << "Print time left in job number " << currentJob.getJobNumber() << ": " << printTime << endl;
    if(time<0&&!failure)
        pagesLeft-=printTime; //Don't want to waste paper

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

bool printerType::checkFail(ofstream &outfile)
{
    if(!failure){
        double check = ((double)rand())/RAND_MAX;
        if (check <= probOfFailure) {
         failure = true;
         setFixTime(downTime);
         outfile << "PRINTER JAMMED.\n";
    } else if(paperLeft<=0){
         failure = true;
         setFixTime(downTime);
         outfile << "PRINTER RAN OUT OF PAPER.\n";
    } else
        failure = false;
    }return failure;
}

int printerType::getPaperLeft()
{
    return paperLeft;
}

void printerType::refillPrinter()
{
    if(fixTime<=0){
        paperLeft = maxPaper;
        failure=false;
    }else
        fixTime--;
}

void printerType::setProbOfFailure(int prob)
{
    probOfFailure = prob;
}

double printerType::getProbOfFailure()
{
    return probOfFailure;
}

void printerType::setFixTime(int time)
{
    fixTime = time;
}

int printerType::getFixTime()
{
    return fixTime;
}

void printerType::setDownTime(int time)
{
    downTime = time;
}
