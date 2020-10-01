#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <iostream>
#include <semaphore.h>

using namespace std;

int threads = 10;
int flag=0;
long n=10;
sem_t count_sem; 
sem_t barrier_sem; 
int counter=0;
void* Thread(void * rank);

int main(int argc, char* argv[]) {
  long thread;
  pthread_t* thread_handles;
  sem_init(&barrier_sem, 0, 0); 
  sem_init(&count_sem, 0, 1); 
    
  thread_handles = new pthread_t[threads];
//  malloc(threads*sizeof(pthread_t));
  
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
  
  return 0;
}

void* Thread(void* rank){
  long my_rank = (long) rank;
  cout<<"hice algo #"<<my_rank<<endl;
  
  
  sem_wait(&count_sem);
  if (counter == threads - 1) {
    counter = 0;
    sem_post(&count_sem);
    for (int j = 0; j < threads-1; j++)
      sem_post(&barrier_sem);
  } 
  else {
    counter++;
    cout<<"espero  #"<<my_rank<<endl;
    sem_post(&count_sem);
    sem_wait(&barrier_sem);
  }


  
  
  cout<<"fin #"<<my_rank<<endl;

  return NULL;
}