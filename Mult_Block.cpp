#include<iostream>
#include <cstdio>
#include <cstdlib>
#include "papi.h"

const int N = 64;

int code_to_be_measured(const double * A, double * B, double * C, int blocksize);

int main(int argc, char **argv)
{
  // Matrix declaration : Modeled as 1D array
  // Declare as pointers and ask for memory to use the heap
  
  double *A = new double [N*N], *B = new double [N*N], *C = new double [N*N];
  
  // initialize matrices
  
  for (int ii =0; ii < N; ++ii) {
    for (int jj =0; jj < N; ++jj) {
      A[ii*N + jj] = ii + jj + 1;
      B[ii*N + jj] = ii + jj + 5;
      C[ii*N + jj] = 0.0;
    }
  }
  
  
  // PAPI vars
  
  float real_time, proc_time,mflops;
  long long flpops;
  float ireal_time, iproc_time, imflops;
  long long iflpops;
  int retval;
  
  // PERFOMANCE MEASURE
  // start PAPI counters
  
  if((retval=PAPI_flops(&ireal_time,&iproc_time,&iflpops,&imflops)) < PAPI_OK)
    {
      printf("Could not initialise PAPI_flops \n");
      printf("Your platform may not support floating point operation event.\n");
      printf("retval: %d\n", retval);
      exit(1);
    }

  for(int h = 1; h <= N; h *= 2)
    {
      code_to_be_measured(A, B, C, h);
      
      if((retval=PAPI_flops( &real_time, &proc_time, &flpops, &mflops))<PAPI_OK)
	{
	  printf("retval: %d\n", retval);
	  exit(1);
	}
      
      printf("Real_time: %f Proc_time: %f Total flpops: %lld MFLOPS: %f\n",
	      real_time, proc_time, flpops, mflops);
    }
  delete [] A;
  delete [] B;
  delete [] C;
  
  return 0;
}





int code_to_be_measured(const double * D, double * E, double * F, int blocksize)
{

  int i, j, k, kk, jj;
  double sum;
  int en = blocksize * (N/blocksize);

  for(i = 0; i < N; i++)
    {
      for(j = 0; j < N; j++)
	{
	  F[i*N + j] = 0.0;
	}
    }

  for(kk = 0; k < en; kk += blocksize)
    {
      for(jj = 0; jj < en; jj++)
	{
	  for(i = 0; i < N; i++)
	    {
	      for(j = jj; j < jj + blocksize; j++)
		{
		  sum = F[i*N + j];
		  for(k = kk; k < kk + blocksize; k++)
		    {
		      sum += D[i*N + k] * E[k*N + j];
		    }
		  F[i*N + j] = sum;
		}
	    }
	}
    }
  
  return 0;
}
