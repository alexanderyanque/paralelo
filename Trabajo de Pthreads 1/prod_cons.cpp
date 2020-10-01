#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <iostream>
#include <semaphore.h>

using namespace std;

#define numero 14
int threads = numero;
int flag=0;
long n=10;
sem_t count_sem; 
sem_t barrier_sem; 
sem_t semaphores[numero];
int counter=0;
void* Send_msg(void * rank);
char* mensajes[numero];

int main(int argc, char* argv[]) {
  long thread;
  pthread_t* thread_handles;
  
    
  thread_handles = new pthread_t[threads];

  for(int i=0;i<threads;++i){
    mensajes[i]=NULL;
    sem_init(&semaphores[i],0,0);
  }
  for (thread = 0; thread < threads; thread++){
      pthread_create(&thread_handles[thread], NULL, 
      Send_msg, (void*) thread);
      
  }

  clock_t start = clock();
  
  for (thread = 0; thread < threads; thread++)
    pthread_join(thread_handles[thread], NULL);

  //printf("%f",sum);
  clock_t end = clock();
  float seconds = (float)(end - start) / CLOCKS_PER_SEC;
  delete[] thread_handles;
//  printf("%f segundos",seconds);
  
  return 0;
}



void* Send_msg(void* rank) {
  long my_rank = (long) rank;
  long dest = (my_rank + 1) % threads;
  char* my_msg=new char[200];
  sprintf(my_msg, "Hello to %ld from %ld", dest, my_rank);
  mensajes[dest] = my_msg;
  sem_post(&semaphores[dest]);


  sem_wait(&semaphores[my_rank]);
  printf("Thread %ld > %s\n", my_rank, mensajes[my_rank]);

 return NULL;
} 