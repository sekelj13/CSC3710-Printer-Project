#include <iostream>
#include <string>
#include <cstdlib>
#include <map> //Hash Map
#include <sstream>

#include "simulation.h"
#include "queueAsArray.h"

using namespace std;

/* ======================== jobQueueArray ======================== */

jobQueueArray::jobQueueArray(map<string, int> mainTiers)
{
    /*
     * Initalize the vector size(jobQArr)
     * @TODO: Forgetting something here....
     */
    //map<string, int>::iterator tierIterator;
    tiers = mainTiers;
    jobQArr.resize(tiers.size());
    
}

jobType jobQueueArray::getNextJob()
{
    for(int i = 0; i < jobQArr.size(); i++){
        if(!jobQArr[i].isEmptyQueue()){
            job = jobQArr[i].front();
            jobQArr[i].deleteQueue();
            return job;
        }
    }
    
    //If no jobs in the dynamic array, return dummy job(Shows empty Queue)
    jobType dummyJob;
    dummyJob.setJobInfo(-1,-1,-1,-1);
    return dummyJob;
}

jobType jobQueueArray::checkNextJob()
{
    for(int i = 0; i < jobQArr.size(); i++){
        if(!jobQArr[i].isEmptyQueue()){
            job = jobQArr[i].front();
            return job;
        }
    }
    
    //If no jobs in the dynamic array, return dummy job(Shows empty Queue)
    jobType dummyJob;
    dummyJob.setJobInfo(-1,-1,-1, -1);
    return dummyJob;
}

//@TODO: TEST THIS DAMN FUNCTION!
void jobQueueArray::sendJob(jobType job)
{
    map<string, int>::iterator tierIterator;
    int prev = 1;
    int i = 0;
    
    for(tierIterator = tiers.begin(); tierIterator != tiers.end(); tierIterator++){
        if(prev <= job.getTier() && job.getTier() < tierIterator->second){
            jobQArr[i].addQueue(job);
        } else if(prev <= job.getTier() && job.getTier() < job.getMaxPages()){
            jobQArr[i].addQueue(job);
        }
        prev = tierIterator->second;
        i++;
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
    //Increment waiting queue
    for(int i = 0; i < jobQArr.size(); i++)
    {
        jobQArr[i].updateWaitingQueue();
    }
}

int jobQueueArray::getMaxJobs()
{
    return maxJobs;
}

map<string, int> jobQueueArray::getJobQArrNumJobsCreated()
{
    map<string, int> numJobsCreated;
    stringstream curTier;
    
    for(int i=0; i < jobQArr.size(); i++){
        curTier << "tier" << i;
        numJobsCreated[curTier.str()] = jobQArr[i].getNumJobsCreated();
        curTier.clear();
    }
    
    /*
     * Return hashmap of the number of jobs created
     * if it is not empty
     * if it is empty, return -1 
     */
    if(!(numJobsCreated.empty())){
        return numJobsCreated;
    
    } else {
        numJobsCreated["tier0"] = -1;
        return numJobsCreated;
    }
}

map<string, int> jobQueueArray::getJobQArrWaitTimes()
{
    map<string, int> numWaitTimes;
    stringstream curTier;
    
    for(int i=0; i < jobQArr.size(); i++){
        curTier << "tier" << i;
        numWaitTimes[curTier.str()] = jobQArr[i].getWaitTime();
        curTier.clear();
    }
    
    /*
     * Return hashmap of the number of jobs created
     * if it is not empty
     * if it is empty, return -1
     */
    if(!(numWaitTimes.empty())){
        return numWaitTimes;
        
    } else {
        numWaitTimes["tier0"] = -1;
        return numWaitTimes;
    }
}
