#include<stdio.h>
#include<stdlib.h>
#include<CL/cl.h>
#include<time.h>
#define MAX_SOURCE_SIZE (0x100000)
/*double drand(double low, double high)
{
    return ((double)rand() * (high - low)) / (double)RAND_MAX + low;
}*/
int main()
{
	int size,i,j;
	float var=0.0;	
	printf("enter size\n");
	scanf("%d",&size);
	clock_t tic=clock();			
	//Allocate memory for arrays		
	float *adata=(float *)malloc(sizeof(float)*size*size);
	float *bdata=(float *)malloc(sizeof(float)*size*size);
	float *cdata=(float *)malloc(sizeof(float)*size*size);
	FILE *fp; /* file pointer */ 
	//float mgone[48]; 
	/*fp=fopen("testerA","r");	
while(!feof(fp))
{   
    fscanf(fp,"%f",&adata[i]);
    while ((fgetc(fp) != '\n') && (!feof(fp))) { /* do nothing */ //}
   // printf("%f\n",adata[i]);     
    //++i;
//}
	FILE *myfile;
	myfile=fopen("testa1.txt","r");	
	if(myfile!=NULL)
		{	
			for(i=0;i<size;i++)
			{
				for(j=0;j<size;j++)
				{		
				//double x = drand(1,10);		
				fscanf(myfile,"%f",&var);
				//double y = drand(1,10);
				//bdata[i*size+j]=y;
				adata[i*size+j]=var;				
				}
			}
		}
	fclose(myfile);
	myfile=fopen("testb1.txt","r");	
	if(myfile!=NULL)
		{	
			for(i=0;i<size;i++)
			{
				for(j=0;j<size;j++)
				{		
				//double x = drand(1,10);		
				fscanf(myfile,"%f",&var);
				//double y = drand(1,10);
				//bdata[i*size+j]=y;
				bdata[i*size+j]=var;				
				}
			}
		}
	fclose(myfile);
	/*for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			printf("%f\n",adata[i*size+j]);
		}
	}
	int e;
	scanf("%d",&e);
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			printf("%f\n",bdata[i*size+j]);
		}
	}*/
	//Open and Read the Kernel File		
	//FILE *fp;
	char *source_str;
	size_t source_size;
	fp = fopen("mat_mul_kernel.cl", "r");
	if (!fp) {
	fprintf(stderr, "Failed to load kernel.\n");
	exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose( fp );
	
	//Get the Platform ID		
	cl_platform_id platform_id;
	cl_uint num_of_platforms;
	if(clGetPlatformIDs(1,&platform_id,&num_of_platforms)!=CL_SUCCESS)
	{	
		printf("Unable to get Platform\n");
		return 1;
	}
	//Get Device ID		
	cl_device_id device_id;
	cl_uint num_of_devices;
	if(clGetDeviceIDs(platform_id,CL_DEVICE_TYPE_CPU,1,&device_id,&num_of_devices)!=CL_SUCCESS)
	{
		printf("Unable to get device\n");
		return 1;
	}
	//Create Context	
	cl_context context;
	cl_context_properties properties[3];
	cl_int err;
	properties[0]=CL_CONTEXT_PLATFORM;
	properties[1]=(cl_context_properties)platform_id;
	properties[2]=0;
	context=clCreateContext(properties,1,&device_id,NULL,NULL,&err);
	//Create Command Queue		
	cl_command_queue command_queue;
	command_queue=clCreateCommandQueue(context,device_id,0,&err);
	//Create and Build Program	
	cl_program program = clCreateProgramWithSource(context, 1,(const char **)&source_str, (const size_t *)&source_size, &err);
	if(clBuildProgram(program,0,NULL,NULL,NULL,NULL)!=CL_SUCCESS)
	{
		printf("unable to build program\n");
		return 1;
	}
	//Create Kernel Object		
	cl_kernel kernel;
	kernel=clCreateKernel(program,"mat_mul",&err);
	//Create Memory buffer		
	cl_mem a,b,c;
	a=clCreateBuffer(context,CL_MEM_READ_ONLY,sizeof(float)*size*size,NULL,&err);
	b=clCreateBuffer(context,CL_MEM_READ_ONLY,sizeof(float)*size*size,NULL,&err);	
	c=clCreateBuffer(context,CL_MEM_WRITE_ONLY,sizeof(float)*size*size,NULL,&err);
	//Write Data on Memory Buffer	
	clEnqueueWriteBuffer(command_queue,a,CL_TRUE,0,sizeof(float)*size*size,adata,0,NULL,NULL);
	clEnqueueWriteBuffer(command_queue,b,CL_TRUE,0,sizeof(float)*size*size,bdata,0,NULL,NULL);	
	//Set Kernel Arguments		
	clSetKernelArg(kernel,0,sizeof(int),&size);		
	clSetKernelArg(kernel,1,sizeof(cl_mem),&a);
	clSetKernelArg(kernel,2,sizeof(cl_mem),&b);
	clSetKernelArg(kernel,3,sizeof(cl_mem),&c);
	clSetKernelArg(kernel,4,sizeof(float)*size,NULL);
	clSetKernelArg(kernel,5,sizeof(float)*size,NULL);	
	//Set Global Size	
	size_t globalsize[2];
	globalsize[0]=size;	
	globalsize[1]=size;
	//Set Local Size	
	size_t localsize[2];
	localsize[0]=32;
	localsize[1]=32;
	//Launch Kernel Function		
	clEnqueueNDRangeKernel(command_queue,kernel,2,NULL,globalsize,localsize,0,NULL,NULL);
	clFinish(command_queue);	
	//Read Data from Memory Buffer		
	clEnqueueReadBuffer(command_queue,c,CL_TRUE,0,sizeof(float)*size*size,cdata,0,NULL,NULL);
		
	
	FILE *ptr_file;
	ptr_file =fopen("output", "w");
	if (!ptr_file)
	return 1;
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		fprintf(ptr_file," %f\n ",cdata[i*size+j]);
		printf("\n");
			
	}			
	fclose(ptr_file);
	//Print the Output	
	/*for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		printf(" %f ",cdata[i*size+j]);
		printf("\n");
			
	}*/	
	//Release Memory	
	clReleaseMemObject(a);
	clReleaseMemObject(c);	
	clReleaseMemObject(b);	
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);
	free(adata);
	free(bdata);
	free(cdata);	
	clock_t toc=clock();	
	printf("elasped time = %f\n",(toc-tic)/(double)(CLOCKS_PER_SEC));		
	return 0;
}	
