SIMOBJS = jobQueue.o jobQueueArray.o jobType.o printerListType.o printerType.o testSimulationProg.o
SIMHDRS = simulation.h queueAsArray.h linkedQueue.h queueADT.h 

sim: $(SIMOBJS)
	g++ -Wall -g *.o -lm -o sim
$(SIMOBJS): $(SIMHDRS) makefile
testSimulationProg.o: testSimulationProg.cpp 
	g++ -Wall -c -g testSimulationProg.cpp

jobQueue.o: jobQueue.cpp simulation.h
	g++ -Wall -c -g jobQueue.cpp 

jobQueueArray.o: jobQueueArray.cpp simulation.h
	g++ -Wall -c -g jobQueueArray.cpp

jobType.o: jobType.cpp simulation.h
	g++ -Wall -c -g jobType.cpp

printerListType.o: printerListType.cpp simulation.h
	g++ -Wall -c -g printerListType.cpp

printerType.o: printerType.cpp simulation.h
	g++ -Wall -c -g printerType.cpp

clean:
	rm *.o sim 