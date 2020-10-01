#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <iostream>

using namespace std;

int threads = 4;
int flag=0;
long n=10;
pthread_mutex_t barrier_mutex;
int counter=0;
void* Thread(void * rank);

int main(int argc, char* argv[]) {
  long thread;
  pthread_t* thread_handles;

  thread_handles = new pthread_t[threads];
//  malloc(threads*sizeof(pthread_t));
  pthread_mutex_init(&barrier_mutex, NULL);
  for (thread = 0; thread < threads; thread++){
      pthread_create(&thread_handles[thread], NULL, 
      Thread, (void*) thread);
      
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
  
  pthread_mutex_destroy(&barrier_mutex); 
  return 0;
}

void* Thread(void* rank){
  long my_rank = (long) rank;
  cout<<"hice algo #"<<my_rank<<endl;
  pthread_mutex_lock(&barrier_mutex);
  counter++;
  cout<<"espero  #"<<my_rank<<endl;
  pthread_mutex_unlock(&barrier_mutex);
  while (counter < threads);
  cout<<"fin #"<<my_rank<<endl;

  return NULL;
}