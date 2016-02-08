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
#include <fstream>
#include <string>
#include "queueAsArray.h"

using namespace std; 
 

  //**************** jobType  ****************
class jobType
{
public:
    jobType(int cN = 0, int arrvTime = 0, int wTime = 0, 
                 int max = 0);
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
      //Postcondition: jobNumber = customerN;
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

    int getPages();
      //returns number of pages
      //postcondition: pages is returned

private:
    int jobNumber;
    int arrivalTime;
    int waitingTime; 
    int printTime; //Time it takes to print
    int tier;
    int pages;     //pages in job
    int maxPages;  //Maximum pages allowed
};

	//*************  jobQueue  ****************
class jobQueue{
public:
    jobQueue();

    int getNumJobs();

    void addJob(jobType job);

    jobType removeJob();

    jobType removeJob(int x);

    int getNumJobsCreated();

private:
    int numJobsCreated;
}

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
      //               the print time is initialized to 0.

    bool isFree() const;
      //Function to determine if the printer is free.
      //Postcondition: Returns true if the printer is free, 
      //               otherwise returns false.

    void setPrintRate (int pr);
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

    void setPrintTime(int t);
      //Function to set the print time according to the 
      //parameter t.
      //Postcondition: printTime = t;

    void setPrintTime();
      //Function to set the print time according to 
      //the print time of the current job.
      //Postcondition: 
      //   printTime = currentJob.transactionTime;

    int getRemainingPrintTime() const;
      //Function to return the remaining print time.
      //Postcondition: The value of printTime is returned.

    void decreasePrintTime();
      //Function to decrease the printTime by 1 unit.
      //Postcondition: printTime--;

    void setCurrentJob(jobType cCustomer);
      //Function to set the info of the current job 
      //according to the parameter cJob.
      //Postcondition: currentJob = cCustomer;

/*
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
*/
private:
    jobType currentJob;
    string status;
    int pagesLeft;
    int printRate;
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


