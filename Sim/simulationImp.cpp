#include <iostream>
#include <string>
#include <cstdlib>
 
#include "Simulation.h"
#include "queueAsArray.h"

using namespace std;
 

//*************** customerType ************

void customerType::setCustomerInfo(int customerN, int arrvTime, 
                                   int wTime, int tTime)
{
    customerNumber = customerN;
    arrivalTime = arrvTime;
    waitingTime = wTime;
    transactionTime = tTime;
}

customerType::customerType(int customerN, int arrvTime, 
                           int wTime, int tTime)
{
    setCustomerInfo(customerN, arrvTime, wTime, tTime);
}


int customerType::getWaitingTime() const
{
    return waitingTime;
}

void customerType::incrementWaitingTime()
{
    waitingTime++;
}

void customerType::setWaitingTime(int time)
{
    //cout << "See Programming Exercise 17 at the end of this chapter." << endl;
    waitingTime = time;
}

int customerType::getArrivalTime() const
{
    //cout << "See Programming Exercise 17 at the end of this chapter." << endl;
    return arrivalTime;
}

int customerType::getTransactionTime() const
{
    //cout << "See Programming Exercise 17 at the end of this chapter." << endl;
    return transactionTime;
}

int customerType::getCustomerNumber() const
{
    //cout << "See Programming Exercise 17 at the end of this chapter." << endl;
    return customerNumber;
}


//**************** serverType **********

serverType::serverType()
{
    status = "free";
    transactionTime = 0;
}

bool serverType::isFree() const
{
    return (status == "free");
}

void serverType::setBusy()
{
    status = "busy";
}

void serverType::setFree()
{
    status = "free";
}

void serverType::setTransactionTime(int t)
{
    transactionTime = t;
}

void serverType::setTransactionTime()
{
    int time;

    time = currentCustomer.getTransactionTime();

    transactionTime = time;
}

void serverType::decreaseTransactionTime()
{
    transactionTime--;
}

int serverType::getRemainingTransactionTime() const
{
    //cout << "See Programming Exercise 17 at the end of this chapter." << endl;
    //TODO: Double Check
    return transactionTime;
}

void serverType::setCurrentCustomer(customerType cCustomer)
{
    //cout << "See Programming Exercise 17 at the end of this chapter." << endl;
    currentCustomer = cCustomer;
}

int serverType::getCurrentCustomerNumber() const
{
    //cout << "See Programming Exercise 17 at the end of this chapter." << endl;
    return currentCustomer.getCustomerNumber();
}

int serverType::getCurrentCustomerArrivalTime() const
{
    //cout << "See Programming Exercise 17 at the end of this chapter." << endl;
    return currentCustomer.getArrivalTime();
}

int serverType::getCurrentCustomerWaitingTime() const
{
    //cout << "See Programming Exercise 17 at the end of this chapter." << endl;
    return currentCustomer.getWaitingTime();
}

int serverType::getCurrentCustomerTransactionTime() const
{
    //cout << "See Programming Exercise 17 at the end of this chapter." << endl;
    return currentCustomer.getTransactionTime();
}


//************** serverListType ***********

serverListType::serverListType(int num)
{
    numOfServers = num;
    servers = new serverType[num];
}

serverListType::~serverListType()
{
    delete [] servers;
}

int serverListType::getFreeServerID() const
{
    int serverID = -1;

    int i;

    for (i = 0; i < numOfServers; i++)
        if (servers[i].isFree())
        {
            serverID = i;
            break;
        }

    return serverID;
}

int serverListType::getNumberOfBusyServers() const
{
    int busyServers = 0;

    int i;

    for (i = 0; i < numOfServers; i++)
        if (!servers[i].isFree())
            busyServers++;

    return busyServers;
}

void serverListType::setServerBusy(int serverID, 
                                   customerType cCustomer, 
                                   int tTime)
{
    servers[serverID].setBusy();
    servers[serverID].setTransactionTime(tTime);
    servers[serverID].setCurrentCustomer(cCustomer);
}

void serverListType::setServerBusy(int serverID, 
                                   customerType cCustomer)
{
    int time;

    time = cCustomer.getTransactionTime();

    servers[serverID].setBusy();
    servers[serverID].setTransactionTime(time);
    servers[serverID].setCurrentCustomer(cCustomer);
}

void serverListType::updateServers(ostream& outFile)
{
    int i;

    for (i = 0; i < numOfServers; i++)
        if (!servers[i].isFree())
        {
            servers[i].decreaseTransactionTime();

            if (servers[i].getRemainingTransactionTime() == 0)
            {
                outFile << "From server number  " << (i + 1) 
                        << " customer number "
                        << servers[i].getCurrentCustomerNumber()
                        << "\n     departed at time unit "
                        << servers[i].
                              getCurrentCustomerArrivalTime()
                           + servers[i].
                              getCurrentCustomerWaitingTime() 
                           + servers[i].
                              getCurrentCustomerTransactionTime()
                        << endl;
                servers[i].setFree();
            }
        }
}

//*************** waitQueue ************


waitingCustomerQueueType::waitingCustomerQueueType(int size)
                          :queueType<customerType>(size)
{
}

//Creating dummy customer with -1 in order to figure out where queue ends & begins
//inserting dummy customer @ -1, when -1 hit again, do not process customer
//Now queue full & ready
void waitingCustomerQueueType::updateWaitingQueue()
{
    customerType cust;

    cust.setWaitingTime(-1);  
    int wTime = 0;
	
	addQueue(cust);
//Doesn't process -1 customer
    while (wTime != -1)
    {
        cust = front();
        deleteQueue();

        wTime = cust.getWaitingTime();
        if (wTime == -1)
            break;
        cust.incrementWaitingTime();
        addQueue(cust);
	}
}

int waitingCustomerQueueType::queueWaitTime(int& waitTime)
{
    int numQueuedCustomers = 0;
    while (!isEmptyQueue() && front().getCustomerNumber() != -1){
        numQueuedCustomers++;
        waitTime += front().getWaitingTime();
        deleteQueue();
    }
    return numQueuedCustomers;
}
