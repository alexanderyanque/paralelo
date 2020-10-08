#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

const int MAX = 1000;

int threads = 2;
sem_t* sems;

void Usage(char* prog_name);
void *Tokenize(void* rank); 

int main(int argc, char* argv[]) {
   long        thread;
   pthread_t* thread_handles; 

   
   thread_handles = (pthread_t*) new pthread_t[threads];
   sems = (sem_t*) new sem_t[threads];
   sem_init(&sems[0], 0, 1);
   for (thread = 1; thread < threads; thread++)
      sem_init(&sems[thread], 0, 0);

    std::cout<<"String : "<<std::endl;
   for (thread = 0; thread < threads; thread++)
      pthread_create(&thread_handles[thread], (pthread_attr_t*) NULL,
          Tokenize, (void*) thread);

   for (thread = 0; thread < threads; thread++) {
      pthread_join(thread_handles[thread], NULL);
   }

   for (thread=0; thread < threads; thread++)
      sem_destroy(&sems[thread]);

   delete[] sems;
   delete[] thread_handles;
   return 0;
}



void *Tokenize(void* rank) {
   long my_rank = (long) rank;
   int count;
   int next = (my_rank + 1) % threads;
   char *fg_rv;
   char my_line[MAX];
   char *my_string;
   char *saveptr;

   sem_wait(&sems[my_rank]);  
   fg_rv = fgets(my_line, MAX, stdin);
   sem_post(&sems[next]);  
   while (fg_rv != NULL) {
      printf("Thread %ld > my line = %s", my_rank, my_line);

      count = 0; 
      my_string = strtok_r(my_line, " \t", &saveptr);
      while ( my_string != NULL ) {
         count++;
         printf("Thread %ld > string %d = %s\n", my_rank, count, my_string);
         my_string = strtok_r(NULL, " \t\n", &saveptr);
      } 

      sem_wait(&sems[my_rank]); 
      fg_rv = fgets(my_line, MAX, stdin);
      sem_post(&sems[next]);  
   }

   return NULL;
}

