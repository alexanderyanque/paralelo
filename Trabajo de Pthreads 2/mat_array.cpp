#include <iostream>
#include <pthread.h>
#include <random>
#include <time.h>
using namespace std;
int m =  8;
int n =  8000000;
int limites = 4;
 
int **A;
int *x;
int *y;

int threads = 1;


void* funcion(void* rank){
 long my_rank = (long) rank;
 int i, j;
 int local_m = m/threads;
 int my_first_row = my_rank*local_m;
 int my_last_row = (my_rank+1)*local_m - 1;
  for (int i = my_first_row; i <= my_last_row; i++) {
  y[i] = 0.0;
  for (j = 0; j < n; j++)
    y[i] += A[i][j]*x[j];
 }

 return NULL;
}

int main() {
  long thread;
  srand(time(NULL));
  pthread_t* thread_handles;
  
    
  thread_handles = new pthread_t[threads];

  A = new int*[m];
  for (int i = 0; i < m; ++i)
    A[i] = new int[n];

  x=new int[n];
  y=new int[m];
  for(long i=0;i<m;++i){
    for(long j=0;j<n;++j){
      A[i][j]=rand()%limites;
    }
  }
  for(long i=0;i<n;++i){
    x[i]=rand()%limites;
  }
  
  clock_t start = clock();
  
  for (thread = 0; thread < threads; thread++){
      pthread_create(&thread_handles[thread], NULL, 
      funcion, (void*) thread);
      
  }

  for (thread = 0; thread < threads; thread++)
    pthread_join(thread_handles[thread], NULL);

  clock_t end = clock();
  float seconds = (float)(end - start) / CLOCKS_PER_SEC;
  delete[] thread_handles;
  cout<<seconds<<endl;

  for (int i = 0; i < m; ++i)
    delete [] A[i];
  
  delete[] A;
  
  delete[] x;
  
  delete[] y;
  return 0;
}