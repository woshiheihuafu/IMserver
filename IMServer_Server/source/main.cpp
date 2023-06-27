#include<iostream>
using namespace std;
#include<CKernel.h>
#include<config.h>
int main()
{

	CKernel kernel;
	if (kernel.startServer())
	{
		while (1)
		{
			sleep(5000);
			printf("server is running\n");
		}
	}



	system("pause");
	return 0;
}
