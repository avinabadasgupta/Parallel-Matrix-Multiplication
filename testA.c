#include<stdio.h>
#include<stdlib.h>
double drand(double low, double high)
{
    return ((double)rand() * (high - low)) / (double)RAND_MAX + low;
}
int main()
{
	int i,j;
	float var;
	FILE *myfile;
	myfile=fopen("testa1.txt","w");	
	if(myfile!=NULL)
	{	
		for(i=0;i<2048;i++)
			
		{
			for(j=0;j<2048;j++)
			{			
				double x = drand(1,10);		
				var=x;	
				fprintf(myfile,"%f\n",var);
			}
		}
	}
	fclose(myfile);
	return 0;
}
