#include<stdio.h>
#include<omp.h>
#include<stdlib.h>

static long num_steps = 1000000;
#define NUM_THREADS 4
double step;
//Nthreads = omp_get_num_threads();
int main()
{
    int nthreads;
    double pi = 0.0; // global  
    step = 1.0 / (double)num_steps;   
    double begintime = omp_get_wtime();
    omp_set_num_threads(NUM_THREADS);
    //double *ptr = malloc (NUM_THREADS*sizeof(double));
    #pragma omp parallel
    {
        int i,id,nthrds;
        double x,sum; // sum inside the region, each thread has their own copy
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        //int n = num_steps / (nthreads);
        if (id== 0) nthreads = nthrds;
        // num_steps/core == tot_num_steps/ num_cores    
        // writing another loop for the sum of the different core's result [and not doing an arithmatic sum_tot = sum_0 +...] 
        // since usually when we write parallel code we don't know the num_cores before hand!
    
        //for (int i=ID*n; i<(ID+1)*n; i++) {
         //   double x = (i+ 0.5)*step;
          //  ptr[ID] += 4.0/(1.0+x*x);
        //}
        for(i=id,sum=0.0;i<num_steps;i=i+nthreads){
            x= (i+0.5)*step;
            sum += 4.0/(1.0+x*x);
        }
        #pragma omp critical
             pi += sum*step; // this sum is on the stack and would go away after the parallel region! 
             // that is why we write the critical inside the parallel region so we still have all sum of all thread but written into pi a global value critically (one by one!)
            // not putting critical inside the for, it's basically seq cause they calc x super qucik and then wait in line to get into critical region!!
    }
printf("%f\n", pi);
   // printf("OK!\n");
   // for(int i=0; i<nthreads; i++ )
   // {
  //      ptr += ptr[i];
  //  }
 //   pi = step * ptr;
  

    double endtime =  omp_get_wtime();
    printf("work took %f seconds\n", endtime - begintime);
}
