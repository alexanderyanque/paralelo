#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <iostream>
#include <semaphore.h>

using namespace std;

int threads = 6;
int flag=0;
long n=10;
pthread_mutex_t mutex;
pthread_cond_t cond_var;

int counter=0;
void* Thread(void * rank);

int main(int argc, char* argv[]) {
  long thread;
  pthread_t* thread_handles;
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond_var,NULL);
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
  pthread_cond_destroy(&cond_var);
  pthread_mutex_destroy(&mutex); 
  printf("%f segundos",seconds);
  
  return 0;
}

void* Thread(void* rank){
  long my_rank = (long) rank;
  cout<<"hice algo #"<<my_rank<<endl;
  
  

  pthread_mutex_lock(&mutex);
  counter++;
  if (counter == threads) {
    counter = 0;
    pthread_cond_broadcast(&cond_var);
  }else{
    cout<<"Espero #"<<my_rank<<endl;
    while (pthread_cond_wait(&cond_var, &mutex) != 0);

  }
  pthread_mutex_unlock(&mutex);

  
  
  cout<<"fin #"<<my_rank<<endl;

  return NULL;
}