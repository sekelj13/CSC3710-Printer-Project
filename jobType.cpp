#include <iostream>
#include <string>
#include <cstdlib>

#include "simulation.h"
#include "queueAsArray.h"

using namespace std;

/* ======================== jobType ======================== */

jobType::jobType(int jobN, int arrvTime,
                int wTime, int max)
{
    setJobInfo(jobN, arrvTime, wTime, max);
    setTier();
}

void jobType::setJobInfo(int jobN, int arrvTime,
                         int wTime, int max)
{
        
    jobNumber = jobN;
    arrivalTime = arrvTime;
    waitingTime = wTime;
    
    if(max==0){
        maxPages=30;
        
    }else
        maxPages = max;
    
    pages = (rand() % maxPages) + 1;
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

int jobType::getMaxPages() {
    return maxPages;
}
