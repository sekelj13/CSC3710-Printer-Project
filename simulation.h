/*
 * Group: D.J. Bozentka, Johnathan Sekela, Matthew Carnaghi
 * Date: January 26, 2016
 * Class: 3710-01
 * Location: Zip File(Under: Sim Folder)
 * Statement: The program takes user input for simulation time, 
 *    number of printers, print time, and time b/w job
 *    arrival. Outputs businesses stats related to the amount
 *    of jobs dealing with total number of printers to help
 *    client understand how much printers should have on staff
 *    that is economically helpful. 
 *   
 *    Added new function(queueWaitTime) which takes waitTime
 *      by reference and returns amount of jobs still
 *      in queue & simulation & their total wait times. 
 *
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "queueAsArray.h"
#include "linkedQueue.h"

using namespace std; 
 

/* =========================== jobType =========================== */
class jobType
{
public:
    jobType(int cN = 0, int arrvTime = 0, int wTime = 0, 
                 int max = 30);
      //Constructor to initialize the instance variables
      //according to the parameters
      //If no value is specified in the object declaration, 
      //the default values are assigned.
      //Postcondition: jobNumber = cN;
      //               arrivalTime = arrvTime;
      //               waitingTime = wTime;
      //               printTime = tTime

    void setJobInfo(int jobN = 0, int inTime = 0, 
                         int wTime = 0, int max = 30);
      //Function to initialize the instance variables.
      //Instance variables are set according to the parameters.
      //Postcondition: jobNumber = jobN;
      //               arrivalTime = arrvTime;
      //               waitingTime = wTime;
      //               printTime = tTime;
      //               maxPages = max;

    int getWaitingTime() const;
      //Function to return the waiting time of a job.
      //Postcondition: The value of waitingTime is returned.

    void setWaitingTime(int time);
      //Function to set the waiting time of a job.
      //Postcondition: waitingTime = time;

    void incrementWaitingTime();
      //Function to increment the waiting time by one time unit.
      //Postcondition: waitingTime++;

    void incrementPrintTime();
      //Function to increment the print time by one time unit.
      //Postcondition: printTime++;

    int getArrivalTime() const;
      //Function to return the arrival time of a job.
      //Postcondition: The value of arrivalTime is returned.

    int getPrintTime() const;
      //Function to return the print time of a job.
      //Postcondition: The value of printTime is returned.

    int getJobNumber() const;
      //Function to return the job number.
      //Postcondition: The value of jobNumber is returned.

    void setTier();
      //sets tier according to value of pages
      //postcondition: tier is set to either 1,2, or 3

    int getTier();
      //returns tier member variable
      //postcondition: tier is returned

    void setPages(int val);
      //sets pages to val
      //postcondition: pages is set to val

    int getNumPages();
      //returns number of pages
      //postcondition: pages is returned
    
    int getMaxPages();
        //returns total number of max pages allowed
        //postcondition: max Total Pages is returned
    

private:
    int jobNumber;
    int arrivalTime;
    int waitingTime; 
    int printTime; //Time it takes to print
    int pages;     //pages in job
    int maxPages;  //Maximum pages allowed
    int tier;
};

/* =========================== jobQueue =========================== */
class jobQueue : public linkedQueueType<jobType> {
public:
    jobQueue();
    //Initalizies number of jobs created to 0
    //
    
    int getNumJobs();

    void updateWaitTime();

    void addJob(jobType job);

    jobType removeJob();

    jobType removeJob(int x);

    int getNumJobsCreated();
    
    int getJobTimes();

    void updateWaitingQueue();
      //taken from jobWaitingQueueType at the bottom
      //increments waiting time of every job in the queue

    int queueWaitTime(int& waitTime);
    
    int getWaitTime();

    
private:
    int numJobs; //jobs currently in queue
    int numJobsCreated; //total jobs placed in this queue
    int waitTime;  //total time of every job spent waiting
    jobType job;
};

    //************* jobQueueArray **************
class jobQueueArray {
public:
    //@TODO: Comment This
    //- Take Job, Add to Tier Queue Array
    //-
    
    /*
     * ---- Constructor
     * When an instance is created, pass the amount of tiers
     *    and their cutoffs. Sent via a hashmap
     */
    jobQueueArray(map<string, int> tiers);
    
    //Send job to Queue
    void sendJob(jobType job);
    
    //getNextJob in list of queues
    jobType getNextJob();
    
    //For each job queue in the array, update the waiting queue time
    void updateWaitingQueues();
    
    bool isEmpty();
    
    jobType checkNextJob();

    int getMaxJobs();

    //Returns pointer to array containing all jobQueue numJobsCreated vars
    map<string, int> getJobQArrNumJobsCreated();

    //Returns pointer to array containing all jobQueue waitTime vars
    map<string, int> getJobQArrWaitTimes();
    

private:
    //@TODO: Make the set or get const not the actual variable?
    int maxJobs;
    int jobCounter;
    vector<jobQueue> jobQArr;
    map<string, int> tiers;
    jobType job;
};
	//*************  printerType  ****************

static const ofstream NULL_STREAM;
class printerType
{
public:
    printerType();
      //Default constructor
      //Sets the values of the instance variables to their default
      //values.
      //Postcondition: currentJob is initialized by its
      //               default constructor; status = "free"; and
      //               the print time is initialized to 0.

    printerType(int maxpg,double prob,int down);
      //Sets the values of the instance variables to specified values
      //Postcondition: currentJob is initialized by its
      //               default constructor; status = "free"; print
      //               time is initialized to 0; maxPages is
      //               initialized; probOfFailure is initialized

    bool isFree() const;
      //Function to determine if the printer is free.
      //Postcondition: Returns true if the printer is free, 
      //               otherwise returns false.

    void setPrintRate (int pr = 1);
      //Sets printRate
      //postcondition: printRate is set

    int getPrintRate();
      //returns printRate
      //postcondition: printRate is returned

    void setBusy();
      //Function to set the status of the printer to busy.
      //Postcondition: status = "busy";

    void setFree();
      //Function to set the status of the printer to "free".
      //Postcondition: status = "free";

    void setPrintTime(int t=1, ofstream& outfile = NULL_STREAM);
      //Function to set the print time according to the 
      //parameter t.
      //Postcondition: printTime = t;

    int getRemainingPrintTime() const;
      //Function to return the remaining print time.
      //Postcondition: The value of printTime is returned.

    void decreasePrintTime(ofstream& outifle);
      //Function to decrease the printTime by 1 unit.
      //Postcondition: printTime--;

    void setCurrentJob(jobType cJob);
      //Function to set the info of the current job 
      //according to the parameter cJob.
      //Postcondition: currentJob = cJob;


    int getCurrentJobNumber() const;
      //Function to return the job number of the current
      //job.
      //Postcondition: The value of jobNumber of the 
      //               current job is returned.

    int getCurrentJobArrivalTime() const;
      //Function to return the arrival time of the current 
      //job.
      //Postcondition: The value of arrivalTime of the current 
      //               job is returned.

    int getCurrentJobWaitingTime() const;
      //Function to return the current waiting time of the 
      //current job.
      //Postcondition: The value of printTime is 
      //               returned.

    int getCurrentJobPrintTime() const;
      //Function to return the print time of the 
      //current job. 
      //Postcondition: The value of printTime of the 
      //               current job is returned.

    bool checkFail();
      //Function to check if printer fails. If no prob is given, %5 chance
      //to fail.
      //Postcondition: Bool failure will be true if there is a failure, else
      // failure will be false.

    int getPaperLeft();
      //Function that returns the paper left in the printer
      //Postcondition: Returns paper left before printer runs out

    void refillPrinter();
      //Function to refill the printer when it is out of paper/ink/whatever
      //Postcondition: Printer will have maxPaper amount of paper


    void setProbOfFailure(int prob);
      //Function to set probOfFailure
      //NOTE: assumes error checking has already occurred.
      //postcondition: probOfFailure equals prob

    double getProbOfFailure();

    void setFixTime(int time = 0);

    void setDownTime(int time = 10);

    int getFixTime();

private:
    jobType currentJob;
    string status;
    int pagesLeft;
    int printRate;
    int printTime;
    double probOfFailure; //Chance that printer fails
    bool failure; //True if printer broke, false if still running
    int costPerPage; //How much each page costs to print with current printer
    int paperLeft; //Paper left in the printer
    int maxPaper; //Maximum number of paper a printer can hold
    int fixTime;  //time left before printer is operable
    int downTime;  //when printer fails, fixTime is set to this
};


//*************  printerListType  ****************
class printerListType
{
public:
    printerListType(int numPrinters = 3, int pr[] = 0,int down = 10);
      //Constructor to initialize a list of printers
      //Postcondition: numOfPrinters = num
      //               A list of printers, specified by num, 
      //               is created and each printer is 
      //               initialized to "free". 

    ~printerListType();
      //Destructor
      //Postcondition: The list of printers is destroyed.

    int getFreePrinterID() const;
      //Function to search the list of printers. 
      //Postcondition: If a free printer is found, returns 
      //               its ID; otherwise, returns -1.

    int getNumberOfBusyPrinters() ;
      //Function to return the number of busy printers.
      //Postcondition: The number of busy printers is returned.
    
    int getNumberOfFreePrinters() ;
    //Returns number of Free Printers available

    void setPrinterBusy(int printerID, jobType cJob, int tTime,ofstream& outfile);
      //Function to set a printer as busy. 
      //Postcondition: The printer specified by serverID is set
      //               to "busy", to serve the job 
      //               specified by cJob, and the 
      //               print time is set according to the 
      //               parameter tTime.

    void setPrinterBusy(int printerID, jobType cJob);
      //Function to set a printer as busy.
      //Postcondition: The printer specified by serverID is set
      //               to "busy", to serve the job 
      //               specified by cJob. 

    void updatePrinters(ostream& outFile);
      //Function to update the status of a printer.
      //Postcondition: The print time of each busy 
      //               printer is decremented by one unit. If 
      //               the print time of a busy printer 
      //               is reduced to zero, the printer is set 
      //               to "free". Moreover, if the actual 
      //               parameter corresponding to outFile is 
      //               cout, a message indicating which job
      //               has been served is printed on the screen, 
      //               together with the job's departing 
      //               time. Otherwise, the output is sent to 
      //               a file specified by the user.
    
private:
    int numOfPrinters;
    printerType *printers;
};
#endif
