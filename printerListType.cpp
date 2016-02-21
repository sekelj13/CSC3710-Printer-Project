#include <iostream>
#include <string>
#include <cstdlib>

#include "simulation.h"
#include "queueAsArray.h"

using namespace std;

/* ======================== printerListType ======================== */

printerListType::printerListType(int numPrinters,int pr)
{
    //in simulation.h, printers is delcared as printers[numOfPrinters]. This should
    //  take care of allocation.
    numOfPrinters=numPrinters;
    printers = new printerType[numPrinters];
    for (int i=0;i < numPrinters;i++) {
        printers[i].setPrintRate(pr);
    }
    
}

printerListType::~printerListType()
{
    delete [] printers;
}

int printerListType::getFreePrinterID() const
{
    int printerID = -1;
    
    int i;
    
    for (i = 0; i < numOfPrinters; i++)
        if (printers[i].isFree())
        {
            printerID = i;
            break;
        }
    
    return printerID;
}

int printerListType::getNumberOfBusyPrinters()
{
    int busyPrinters = 0;
    
    int i;
    
    for (i = 0; i < numOfPrinters; i++)
        if (!printers[i].isFree())
            busyPrinters++;
    
    return busyPrinters;
}

int printerListType::getNumberOfFreePrinters()
{
    int freePrinters = 0;
    
    int i;
    
    for (i = 0;i < numOfPrinters;i++) {
        if (printers[i].isFree()) {
            freePrinters++;
        }
    }
    
    return freePrinters;
}

void printerListType::setPrinterBusy(int printerID,
                                     jobType cJob)
{
    int time;
    
    time = cJob.getPrintTime();
    
    cout << "Printer " << printerID+1 << " gets job " << cJob.getJobNumber() << endl;
    
    printers[printerID].setCurrentJob(cJob);
    printers[printerID].setBusy();
    printers[printerID].setPrintTime(time);
}

void printerListType::updatePrinters(ostream& outFile)
{
    int i;
    
    for (i = 0; i < numOfPrinters; i++)
        if (!printers[i].isFree())
        {
            printers[i].decreasePrintTime();
            
            if (printers[i].getRemainingPrintTime() <= 0)
            {
                outFile << "From printer number  " << (i + 1)
                << " job number "
                << printers[i].getCurrentJobNumber()
                << "\n     departed at time unit "
                << printers[i].
                getCurrentJobArrivalTime()
                + printers[i].
                getCurrentJobWaitingTime()
                + printers[i].
                getCurrentJobPrintTime()
                << endl;
                printers[i].setFree();
            }
        }
}