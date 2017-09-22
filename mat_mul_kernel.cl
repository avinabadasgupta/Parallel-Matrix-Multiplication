#define BLOCK_SIZE 32
__kernel void mat_mul(const int size,__global const float* a,__global const float* b,__global float* c,__local float* As,__local float*	Bs)																		
{																		
	int bx=get_group_id(0);															
	int by=get_group_id(1);															
	int tx=get_local_id(0);															
	int ty=get_local_id(1);															
 	int aBegin=size*BLOCK_SIZE*by;															
 	int aEnd=aBegin+size-1;															
 	int aStep=BLOCK_SIZE;																
 	int bBegin=BLOCK_SIZE*bx;															
 	int bStep=BLOCK_SIZE*size;															
	float Csub=0.0;																
	int i,j;																
 	for(i=aBegin,j=bBegin;i<=aEnd;i+=aStep,j+=bStep) 											
    	{																	
 		As[ty*BLOCK_SIZE+tx]=a[i+size*ty+tx];													
        	Bs[ty*BLOCK_SIZE+tx]=b[j+size*ty+tx];													
 		barrier(CLK_LOCAL_MEM_FENCE);													
 		for(int k=0;k<BLOCK_SIZE;k++)														
        	Csub+=As[ty*BLOCK_SIZE+k]*Bs[k*BLOCK_SIZE+tx];											
 		barrier(CLK_LOCAL_MEM_FENCE);													
	}																	
		int d=size*BLOCK_SIZE*by+BLOCK_SIZE*bx;                                                     					
		c[d+size*ty+tx]=Csub;															
}			
