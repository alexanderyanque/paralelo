#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int threads = 32;
int flag=0;
long n=10000000;
float sum;
void *Hello(void* rank);
void *Thread_sumbusy(void* rank);
void* Thread_summutex(void* rank);
pthread_mutex_t mutex;

int main(int argc, char* argv[]) {
  long thread;
  pthread_t* thread_handles;

  thread_handles = new pthread_t[threads];
//  malloc(threads*sizeof(pthread_t));
  pthread_mutex_init(&mutex, NULL);
  for (thread = 0; thread < threads; thread++){
      //pthread_create(&thread_handles[thread], NULL, 
      //Thread_sumbusy, (void*) thread);
      pthread_create(&thread_handles[thread], NULL, 
      Thread_summutex, (void*) thread);

  }

  //  printf("hi from main\n");
  clock_t start = clock();
  /*Do something*/
  
  for (thread = 0; thread < threads; thread++)
    pthread_join(thread_handles[thread], NULL);

  //printf("%f",sum);
  clock_t end = clock();
  float seconds = (float)(end - start) / CLOCKS_PER_SEC;
  delete[] thread_handles;
  printf("%f segundos",seconds);
  
  pthread_mutex_destroy(&mutex); 
  return 0;
}

void* Hello(void* rank){
  long my_rank = (long) rank;
  printf("Hello from thread %ld of %d\n",my_rank,threads);
  return NULL;
}

void* Thread_sumbusy(void* rank) {
  long my_rank = (long) rank;
  double factor,mysum=0.0;
  long long i;
  long long my_n = n/threads;
  long long my_first_i = my_n*my_rank;
  long long my_last_i = my_first_i + my_n;
  if (my_first_i % 2 == 0)
    factor = 1.0;
  else
    factor = -1.0; 

  for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
    mysum += factor/(2*i+1);
    
  }
  while (flag != my_rank);
  sum+=mysum;
  flag = (flag+1) % threads;
   

  return NULL;
  } 
  
void* Thread_summutex(void* rank) {
  long my_rank = (long) rank;
  double factor,mysum=0.0;
  long long i;
  long long my_n = n/threads;
  long long my_first_i = my_n*my_rank;
  long long my_last_i = my_first_i + my_n;
  if (my_first_i % 2 == 0)
    factor = 1.0;
  else
    factor = -1.0; 

  for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
    mysum += factor/(2*i+1);
    
  }
  pthread_mutex_lock(&mutex);
  sum+=mysum;
  pthread_mutex_unlock(&mutex);
  flag = (flag+1) % threads;
   

  return NULL;
  } 