#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <iostream>
#include <semaphore.h>
#include <random>

using namespace std;

int ops=5000;
int threads=8;  
struct node* cabeza=NULL;
pthread_mutex_t list_mutex;
pthread_mutex_t head_p_mutex;
pthread_rwlock_t rwlock;

struct node {
  int data;
  struct node* next;
  pthread_mutex_t mutex;
  node(){
    next=NULL;
    data=-1;
    pthread_mutex_init(&mutex, NULL);
  }
};

int Member(int value, struct node* head_p) {
  struct node* curr_p = head_p;

  while (curr_p != NULL && curr_p->data < value)
  curr_p = curr_p->next;

  if (curr_p == NULL || curr_p->data > value) {
    return 0;
  }
    else {
    return 1;
  }
}
int Member2(int value,struct node* head_p) {
  struct node* curr_p;
  pthread_mutex_lock(&head_p_mutex);
  curr_p = head_p;
  while (curr_p != NULL && curr_p->data < value) {
    if (curr_p->next != NULL)
      pthread_mutex_lock(&(curr_p->next->mutex));
    if (curr_p == head_p)
      pthread_mutex_unlock(&head_p_mutex);
    pthread_mutex_unlock(&(curr_p->mutex));
    curr_p = curr_p->next;
  }
  if (curr_p == NULL || curr_p->data > value) {
    if (curr_p == head_p)
      pthread_mutex_unlock(&head_p_mutex);
    if (curr_p != NULL)
      pthread_mutex_unlock(&(curr_p->mutex));
    return 0;
  }
  else {
    if (curr_p == head_p)
    pthread_mutex_unlock(&head_p_mutex);
    pthread_mutex_unlock(&(curr_p->mutex));
    return 1;
  }
} 

int Insert(int value, struct node** head_p) {
  struct node* curr_p = *head_p;
  struct node* pred_p = NULL;
  struct node* temp_p;

  while (curr_p != NULL && curr_p->data < value) {
    pred_p = curr_p;
    curr_p = curr_p->next;
  }

  if (curr_p == NULL || curr_p->data > value) {
    temp_p = new node;
    temp_p->data = value;
    temp_p->next = curr_p;
    if (pred_p == NULL) 
      *head_p = temp_p;
    else
      pred_p->next = temp_p;
    return 1;
    }
  else { 
    return 0;
  }
} 

int Delete(int value, struct node** head_p) {
  struct node* curr_p = *head_p;
  struct node* pred_p = NULL;

  while (curr_p != NULL && curr_p->data < value) {
  pred_p = curr_p;
  curr_p = curr_p->next;
  }

  if (curr_p != NULL && curr_p->data == value) {
    if (pred_p == NULL) { 
      *head_p = curr_p->next;
      delete curr_p;
    } 
    else {
      pred_p->next = curr_p->next;
      delete curr_p;
    }
    return 1;
  }
  else {
    return 0;
  }
}
void Free_list(struct node** head_pp) {
   struct node* curr;
   struct node* sig;

   if (head_pp==NULL) return;
   curr = *head_pp; 
   sig = curr->next;
   while (sig != NULL) {
      delete(curr);
      curr = sig;
      sig = curr->next;
   }
   delete(curr);
   *head_pp = NULL;
} 
void Print(struct node* head) {
   struct node* curr;
    int temp=0;
   curr = head;
   while (curr != NULL) {
//      cout<< curr->data<<" ";
      curr = curr->next;
    temp++;
   }
   cout<<temp<<endl;
  
}

int def=0;

void* Accion1(void* rank) {
  cout<<"mutex por lista"<<endl;
  for(int i=0;i<ops/threads;++i){
    int temp=rand()%10000;
    int acc=rand()%10000;
    long my_rank = (long) rank;
    pthread_mutex_lock(&list_mutex);
    
    if(acc<=1000)
      Insert(temp,&cabeza);
    else if(acc<=2000)
      Delete(temp,&cabeza);
    else
      Member(temp,cabeza);
    if(acc<=2000)def++;
    pthread_mutex_unlock(&list_mutex);
  }
  return NULL;
} 

void* Accion2(void* rank) {
  cout<<"mutex por nodo"<<endl;
  for(int i=0;i<ops/threads;++i){
    int temp=rand()%10000;
    int acc=rand()%10000;
    long my_rank = (long) rank;
    
    if(acc<=1000){
      Insert(temp,&cabeza);
    }
    else if(acc<=2000){
     Delete(temp,&cabeza);
    }
    else
      Member2(temp,cabeza);
    if(acc<=2000)def++;
  }
  return NULL;
} 

void* Accion3(void* rank) {
  //cout<<"rw_lock"<<endl;
  for(int i=0;i<ops/threads;++i){
    int temp=rand()%10000;
    int acc=rand()%10000;
    long my_rank = (long) rank;
    
    if(acc<=1000){
      pthread_rwlock_wrlock(&rwlock);
      Insert(temp,&cabeza);
      pthread_rwlock_unlock(&rwlock);
    }
    else if(acc<=2000){
      pthread_rwlock_wrlock(&rwlock);
      Delete(temp,&cabeza);
      pthread_rwlock_unlock(&rwlock);}
    else{
      pthread_rwlock_rdlock(&rwlock);
      Member(temp,cabeza);
      pthread_rwlock_unlock(&rwlock);
    }
    if(acc<=2000)def++;
  }
  return NULL;
} 
int main() {
  srand(time(NULL));
  long thread;
  pthread_t* thread_handles;
  pthread_mutex_init(&list_mutex, NULL);
  pthread_rwlock_init(&rwlock,NULL);
  for(int i=0;i<1000;++i){
    Insert(rand()%2000,&cabeza);
  }
//  Print(cabeza);
  thread_handles = new pthread_t[threads];
  for (thread = 0; thread < threads; thread++){
      pthread_create(&thread_handles[thread], NULL, 
      Accion3, (void*) thread);
      
  }

  clock_t start = clock();
  
  for (thread = 0; thread < threads; thread++)
    pthread_join(thread_handles[thread], NULL);

  //Print(cabeza);
  clock_t end = clock();
  float seconds = (float)(end - start) / CLOCKS_PER_SEC;
  cout<<"tiempo = "<<seconds<<" segundos"<<endl;
  delete[] thread_handles;
  Free_list(&cabeza);
  pthread_mutex_destroy(&list_mutex); 
  pthread_rwlock_destroy(&rwlock);
 //cout<<def<<endl;
  return 0;
}