__kernel void mat_mul(const int size,__global const float* a,__global const float* b,__global float* c)						
{																		
	int i = get_global_id(0);														
	int j = get_global_id(1);														
	int k;																	
	float tmp=0.0;																
	for (k = 0; k < size; k++)													
	tmp = tmp + a[k + size*j] * b[k*size + i];												
	c[i + j*size] = tmp;														
}																																			
