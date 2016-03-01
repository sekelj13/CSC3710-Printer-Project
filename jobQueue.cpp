#include <iostream>
#include <string>
#include <cstdlib>

#include "simulation.h"
#include "queueAsArray.h"

using namespace std;

/* ======================== jobQueue ======================== */

jobQueue::jobQueue()
{
    numJobsCreated=0;
}

/*
 *
 * Creating dummy job with -1 in order to figure out where queue ends & begins
 * inserting dummy job @ -1, when -1 hit again, do not process job
 * Now queue full & ready
 *
 */
void jobQueue::updateWaitingQueue()
{
    
    job.setWaitingTime(-1);
    int wTime = 0;
    
    //Adds @ tail 
    addQueue(job);

    /*
     * Takes job out of queue, adds time to it, re-inserts it
     * until hit's -1
     */
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

int jobQueue::getWaitTime()
{
    return waitTime;
}
