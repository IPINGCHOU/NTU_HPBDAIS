#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

///#define NRA 62                 /* number of rows in matrix A */
///#define NCA 15                 /* number of columns in matrix A */
///#define NCB 7                  /* number of columns in matrix B */
///

int main (int argc, char *argv[]) 
{
int number_of_thread, times_of_matrix;
char *a_ = argv[1];
number_of_thread = atoi(a_);

char *b_ = argv[2];
times_of_matrix = atoi(b_);


int NRA = 62;
int NCA = 15;
int NCB = 7;

NRA = times_of_matrix * NRA;
NCA *= times_of_matrix;
NCB *= times_of_matrix;

printf("NRA %d, NCA %d, NCB %d\n", NRA, NCA, NCB);
printf("number of thread:%d , times of matrix:%d\n",number_of_thread, times_of_matrix);


int	tid, nthreads, i, j, k, chunk;
double	a[NRA][NCA],           /* matrix A to be multiplied */
	b[NCA][NCB],           /* matrix B to be multiplied */
	c[NRA][NCB];           /* result matrix C */

chunk = 10;                    /* set loop iteration chunk size */

clock_t start=0; 


start = clock();
printf("start:%ld\n",start);
/*** Spawn a parallel region explicitly scoping all variables ***/
#pragma omp parallel shared(a,b,c,nthreads,chunk) private(tid,i,j,k) num_threads(number_of_thread)
  {
  tid = omp_get_thread_num();
  // printf("tid:%d\n",tid);
  if (tid == 0)
    {
    nthreads = omp_get_num_threads();
    
    // printf("Starting matrix multiple example with %d threads\n",nthreads);
    // printf("Initializing matrices...\n");
    }
  /*** Initialize matrices ***/
  #pragma omp for schedule (static, chunk) 
  for (i=0; i<NRA; i++)
    for (j=0; j<NCA; j++)
      a[i][j]= i+j;
  #pragma omp for schedule (static, chunk)
  for (i=0; i<NCA; i++)
    for (j=0; j<NCB; j++)
      b[i][j]= i*j;
  #pragma omp for schedule (static, chunk)
  for (i=0; i<NRA; i++)
    for (j=0; j<NCB; j++)
      c[i][j]= 0;

  /*** Do matrix multiply sharing iterations on outer loop ***/
  /*** Display who does which iterations for demonstration purposes ***/
  // printf("Thread %d starting matrix multiply...\n",tid);


  #pragma omp for schedule (static, chunk)
  for (i=0; i<NRA; i++)    
    {
    // printf("Thread=%d did row=%d\n",tid,i);
    for(j=0; j<NCB; j++)       
      for (k=0; k<NCA; k++)
        c[i][j] += a[i][k] * b[k][j];
    }
  }   /*** End of parallel region ***/

 clock_t end;
 end = clock();
 printf("end:%ld \n",end);

 clock_t t = end - start;
 double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds    
 printf("fun() took %f seconds to execute \n", time_taken); 

/*** Print results ***/
printf("******************************************************\n");
/*
printf("Result Matrix:\n");
for (i=0; i<NRA; i++)
  {
  for (j=0; j<NCB; j++) 
    printf("%6.2f   ", c[i][j]);
  printf("\n"); 
  }
printf("******************************************************\n");
*/
printf ("Done.\n");

}
