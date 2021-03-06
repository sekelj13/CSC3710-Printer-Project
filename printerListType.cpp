#include <iostream>
#include <string>
#include <cstdlib>

#include "simulation.h"
#include "queueAsArray.h"

using namespace std;

/* ======================== printerListType ======================== */

printerListType::printerListType(int numPrinters,int pr[], int down,double probOfFailure)
{
    //in simulation.h, printers is delcared as printers[numOfPrinters]. This should
    //  take care of allocation.
    numOfPrinters=numPrinters;
    printers = new printerType[numPrinters];
    for (int i=0;i < numPrinters;i++) {
        printers[i].setPrintRate(pr[i]);
        printers[i].setDownTime(down);
        printers[i].setProbOfFailure(probOfFailure);
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
                                     jobType cJob,ofstream& outfile)
{
    int time;
    
    time = cJob.getPrintTime();
    
    outfile << "Printer " << printerID+1 << " gets job " << cJob.getJobNumber() << endl;
    
    printers[printerID].setCurrentJob(cJob);
    printers[printerID].setBusy();
    printers[printerID].setPrintTime(time,outfile);
}

void printerListType::updatePrinters(ofstream& outfile)
{
    int i;
    bool fail;
    for (i = 0; i < numOfPrinters; i++)
        if (!printers[i].isFree())
        {
            printers[i].decreasePrintTime(outfile);
            fail=printers[i].checkFail(outfile); //@TODO: Fix checkFail for parameters(too few)
            if (printers[i].getRemainingPrintTime() <= 0)
            {
                if(fail){
                    outfile << "Printer "<<(i+1)<<" failed. Time till fixed: "
                    << printers[i].getFixTime() << endl;
                    printers[i].refillPrinter();
                } else {
                    outfile << "From printer number  " << (i + 1)
                    << " job number "
                    << printers[i].getCurrentJobNumber()
                    << " departed."
                    << endl;
                    printers[i].setFree();
                }
            }
        }
}
