#include "effiTest.h"
#include <sched.h>

EffiTest  EffTest(1, "threadTest");

void* w_routine(void* ptr)
{
	timeObj   obj("write_thread");
	int  sObj = 0;

	//th_arg*   arg = (th_arg*)ptr;
	//if (arg->type == 1)
	//{
	//	sObj = sizeof(struct ftest);
	//}
	//else if (arg->type == 2)
	//{
	//	sObj = sizeof(struct stest);
	//}
	//else if (arg->type == 3)
	//{
		sObj = sizeof(struct ttest);
	//}
	//else if (arg->type == 4)
	//{
	//	sObj = sizeof(struct rtest);
	//}
	//else
	//{
	//	printf("+++++++++++++++++error type++++++++++++++\n");
	//}

	//GWFifo   fifo(filename, strlen(filename));
	th_arg* arg = (th_arg*)ptr;
	assert(arg->type == 3);
	for (int i = 0; i < 100000; )
	{
		if (!EffTest.fifo.set(arg->type, arg->st, sObj))
		{
			sched_yield();
			continue;
		}
		else
		{
			++i;
		}
	}

	return NULL;
}

void* r_routine(void* ptr)
{
	timeObj   obj("read_thread");
	int  size = sizeof(struct ttest);
	int  type = 0;


	th_arg* arg = (th_arg*)ptr;
	assert(arg->type == 3);

	ttest   var;
	bzero(&var, sizeof(struct ttest));
	for (int i = 0; i < recordN;)
	{
		//if (arg->type == 1)
		//{
		//	ftest   var;
		//	bzero(&var, sizeof(struct ftest));
		//	if (EffTest.fifo.get(&type, (void*)&var, &size))
		//	{
		//		assert(type == arg->type);
		//		assert(var == *((ftest*)arg->st));
		//	}
		//	else
		//	{
		//		printf("read time: %d\n", i);
		//		break;
		//	}
		//}
		//else if(arg->type == 2)
		//{
		//	stest   var;
		//	bzero(&var, sizeof(struct stest));
		//	if (EffTest.fifo.get(&type, (void*)&var , &size))
		//	{
		//		assert(type == arg->type);
		//		assert(var == *((stest*)arg->st));
		//	}
		//	else
		//	{
		//		printf("read time: %d\n", i);
		//		break;
		//	}
		//}
		//else if(arg->type == 3)
		//{
		if (EffTest.fifo.get(&type, (void*)&var , &size))
		{
			++i;
			assert(type == arg->type);
			assert(var == *((ttest*)arg->st));
		}
		else
		{
			printf("read time: %d\n", i);
			break;
			//sched_yield();
			//continue;
		}
		//}
		//else
		//{
		//	rtest   var;
		//	bzero(&var, sizeof(struct rtest));
		//	if (EffTest.fifo.get(&type, (void*)&var, &size))
		//	{
		//		assert(type == arg->type);
		//		assert(var == *((rtest*)arg->st));
		//	}
		//	else
		//	{
		//		printf("read time: %d\n", i);
		//		break;
		//	}
		//}
	}

	return NULL;
}

EffiTest::EffiTest(const int num, const char* filename): m_th_num(num), fifo(filename, strlen(filename))
{
	m_pthread_w = new pthread_t[m_th_num];
	m_pthread_r = new pthread_t[m_th_num];
}

EffiTest::~EffiTest()
{
	delete []m_pthread_w;
	delete []m_pthread_r;
}

void EffiTest::runsingle_w()
{
	//ftest
	ftest   var1;
	var1.a = 1;
	var1.b = 1;
	var1.b = 'a' + 1;

	th_arg  arg1 = {1, &var1};

	for (int i = 0; i < m_th_num; ++i)
	{
		if (0 != pthread_create(&m_pthread_w[i], NULL, w_routine, (void*)&arg1))
		{
			printf("create failed\n");
		}
	}

	for (int i = 0; i < m_th_num; ++i)
	{
		pthread_join(m_pthread_w[i], NULL);
	}
}

void EffiTest::runsingle_r()
{
	ftest   var1;
	var1.a = 1;
	var1.b = 1;
	var1.b = 'a' + 1;

	th_arg  arg1 = {1, &var1};

	for (int i = 0; i < m_th_num; ++i)
	{
		if (0 != pthread_create(&m_pthread_w[i], NULL, r_routine, (void*)&arg1))
		{
			printf("create failed\n");
		}
	}

	for (int i = 0; i < m_th_num; ++i)
	{
		pthread_join(m_pthread_w[i], NULL);
	}
}

void EffiTest::runRdWr()
{
	ttest   var3;
	var3.a =  3;
	var3.b =  1.02 + 3;
	var3.c =  8.96 + 3;
	th_arg  arg3 = {3, &var3};

	for (int i = 0; i < m_th_num; ++i)
	{
		if (0 != pthread_create(&m_pthread_w[i], NULL, w_routine, (void*)&arg3))
		{
			printf("create failed\n");
		}

		if (0 != pthread_create(&m_pthread_r[i], NULL, r_routine, (void*)&arg3))
		{
			printf("create failed\n");
		}
	}

	for (int i = 0; i < m_th_num; ++i)
	{
		pthread_join(m_pthread_w[i], NULL);
		pthread_join(m_pthread_r[i], NULL);
	}

}


void EffiTest::run()
{
	ttest   var3;
	var3.a =  3;
	var3.b =  1.02 + 3;
	var3.c =  8.96 + 3;
	th_arg  arg3 = {3, &var3};

	pthread_t  write[5];
	pthread_t  read[2];

	bzero(write, 5 * sizeof(pthread_t));
	bzero(read, 2 * sizeof(pthread_t));

	for (int i = 0; i < 5; ++i)
	{
		if (0 != pthread_create(&write[i], NULL, w_routine, (void*)&arg3))
		{
			printf("create failed\n");
		}
	}

	for (int i = 0; i < 2; ++i)
	{
		if (0 != pthread_create(&read[i], NULL, r_routine, (void*)&arg3))
		{
			printf("create failed\n");
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		pthread_join(write[i], NULL);
	}

	for (int i = 0; i < 2; ++i)
	{
		pthread_join(read[i], NULL);
	}
}

