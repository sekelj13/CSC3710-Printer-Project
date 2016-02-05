/*
 * Group: D.J. Bozentka, Johnathan Sekela, Matthew Carnaghi
 * Date: January 26, 2016
 * Class: 3710-01
 * Location: Zip File(Under: Sim Folder)
 * Statement: The program takes user input for simulation time, 
 *    number of printers, transaction time, and time b/w job
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
#include <fstream>
#include <string>
#include "queueAsArray.h"

using namespace std; 
 

  //**************** jobType  ****************
class jobType
{
public:
    jobType(int cN = 0, int arrvTime = 0, int wTime = 0, 
                 int tTime = 0);
      //Constructor to initialize the instance variables
      //according to the parameters
      //If no value is specified in the object declaration, 
      //the default values are assigned.
      //Postcondition: jobNumber = cN;
      //               arrivalTime = arrvTime;
      //               waitingTime = wTime;
      //               transactionTime = tTime

    void setJobInfo(int jobN = 0, int inTime = 0, 
                         int wTime = 0, int tTime = 0);
      //Function to initialize the instance variables.
      //Instance variables are set according to the parameters.
      //Postcondition: jobNumber = customerN;
      //               arrivalTime = arrvTime;
      //               waitingTime = wTime;
      //               transactionTime = tTime;

    int getWaitingTime() const;
      //Function to return the waiting time of a job.
      //Postcondition: The value of waitingTime is returned.

    void setWaitingTime(int time);
      //Function to set the waiting time of a job.
      //Postcondition: waitingTime = time;

    void incrementWaitingTime();
      //Function to increment the waiting time by one time unit.
      //Postcondition: waitingTime++;

    int getArrivalTime() const;
      //Function to return the arrival time of a job.
      //Postcondition: The value of arrivalTime is returned.

    int getTransactionTime() const;
      //Function to return the transaction time of a job.
      //Postcondition: The value of transactionTime is returned.

    int getJobNumber() const;
      //Function to return the job number.
      //Postcondition: The value of jobNumber is returned.

private:
    int jobNumber;
    int arrivalTime;
    int waitingTime; 
    int transactionTime;
};


	//*************  printerType  ****************
class printerType
{
public:
    printerType();
      //Default constructor
      //Sets the values of the instance variables to their default
      //values.
      //Postcondition: currentJob is initialized by its
      //               default constructor; status = "free"; and
      //               the transaction time is initialized to 0.

    bool isFree() const;
      //Function to determine if the printer is free.
      //Postcondition: Returns true if the printer is free, 
      //               otherwise returns false.

    void setBusy();
      //Function to set the status of the printer to busy.
      //Postcondition: status = "busy";

    void setFree();
      //Function to set the status of the printer to "free".
      //Postcondition: status = "free";

    void setTransactionTime(int t);
      //Function to set the transaction time according to the 
      //parameter t.
      //Postcondition: transactionTime = t;

    void setTransactionTime();
      //Function to set the transaction time according to 
      //the transaction time of the current job.
      //Postcondition: 
      //   transactionTime = currentJob.transactionTime;

    int getRemainingTransactionTime() const;
      //Function to return the remaining transaction time.
      //Postcondition: The value of transactionTime is returned.

    void decreaseTransactionTime();
      //Function to decrease the transactionTime by 1 unit.
      //Postcondition: transactionTime--;

    void setCurrentJob(jobType cCustomer);
      //Function to set the info of the current job 
      //according to the parameter cJob.
      //Postcondition: currentJob = cCustomer;

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
      //Postcondition: The value of transactionTime is 
      //               returned.

    int getCurrentJobTransactionTime() const;
      //Function to return the transaction time of the 
      //current job. 
      //Postcondition: The value of transactionTime of the 
      //               current job is returned.

private:
    jobType currentJob;
    string status;
    int transactionTime; 
};


//*************  printerListType  ****************
class printerListType
{
public:
    printerListType(int num = 1);
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

    int getNumberOfBusyPrinters() const;
      //Function to return the number of busy printers.
      //Postcondition: The number of busy printers is returned.

    void setPrinterBusy(int printerID, jobType cJob,
                       int tTime);
      //Function to set a printer as busy. 
      //Postcondition: The printer specified by serverID is set
      //               to "busy", to serve the job 
      //               specified by cJob, and the 
      //               transaction time is set according to the 
      //               parameter tTime.

    void setPrinterBusy(int printerID, jobType cJob);
      //Function to set a printer as busy.
      //Postcondition: The printer specified by serverID is set
      //               to "busy", to serve the job 
      //               specified by cJob. 

    void updatePrinters(ostream& outFile);
      //Function to update the status of a printer.
      //Postcondition: The transaction time of each busy 
      //               printer is decremented by one unit. If 
      //               the transaction time of a busy printer 
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


//**************** waitQueue  *************
//Deriving queueType, only need operationg of incremeting wait time
//for each job in the queue
class waitingJobQueueType: public queueType<jobType>
{
public:
    waitingJobQueueType(int size = 100);
      //Constructor
      //Postcondition: The queue is initialized according to 
      //               the parameter size. The value of size 
      //               is passed to the constructor of queueType.

    void updateWaitingQueue();
      //Function to increment the waiting time of each 
      //job in the queue by one time unit.
    
    int queueWaitTime(int& waitTime);
      //Function to find total waiting time of jobs still in queue
      //precondition: queue contains or does not contain elements
      //postcondition: waittime now has total waiting time of jobs in queue as well
    
};


