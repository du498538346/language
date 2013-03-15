#include <stdio.h>
#include "correctTest.h"
#include "effiTest.h"
#include "DBcorrect.h"

int main()
{
	printf("here testing fifo\n");
	printf("=================program starting=================\n");
	// correct test
	//WrRdRightNow    test;
	//test.run();

	//efficient test

	//write 100 million  recore  one thread
	//EffTest.runsingle_w();
	//EffTest.runsingle_r();
	//EffTest.runRdWr();
	//EffTest.run();
	DBcorrectTest  test;
	test.runCreateDB();

	printf("=================program exiting=================\n");
	return 0;
}
