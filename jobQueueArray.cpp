#include <iostream>
#include <string>
#include <cstdlib>

#include "simulation.h"
#include "queueAsArray.h"

using namespace std;

/* ======================== jobQueueArray ======================== */

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
        dummyJob.setJobInfo(-1,-1,-1, -1);
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
        dummyJob.setJobInfo(-1,-1,-1, -1);
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