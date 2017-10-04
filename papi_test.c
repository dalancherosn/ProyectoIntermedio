#include <stdio.h>
#include <stdlib.h>
#include <papi.h>

int your_code();

int main()
{
  float real_time, proc_time, mflops;
  long long flpops;
  float ireal_time, iproc_time, imflops;
  long long iflpops;
  int retval;
  
  if((retval = PAPI_flops(&ireal_time, &iproc_time, &iflpops, &imflops)) < PAPI_OK)
    {
      printf("Could not initialise PAPI_flops \n");
      printf("Your platform may not support floating point operation event.\n");
      printf("retval: %d\n", retval);
      exit(1);
    }
  
  your_code();

  if((retval = PAPI_flops(&real_time, &proc_time, &flpops, &mflops)) < PAPI_OK)
    {
      printf("retval: %d\n", retval);
      exit(1);
    }

  printf("Real_time: %f Proc_time: %f Total flpops: %11d MFLOPS: %f\n", real_time, proc_time, flpops, mflops);

  exit(0);
}

int your_code()
{
  int i;
  double tmp = 1.1;

  for(i = 1; i < 2000; ++i)
    {
      tmp = (tmp * 100)/i;
    }
  return 0;
  
}
