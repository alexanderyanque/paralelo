#include <iostream>

using namespace std;

struct node {
  int data;
  struct node* next;
  node(){
    next=NULL;
    data=-1;
  }
  void print(){
    cout<<data<<" ";
    if(next!=NULL){
      next->print();
    }
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
void Print(struct node* head_p) {
   struct node* curr_p;

   printf("list = ");

   curr_p = head_p;
   while (curr_p != NULL) {
      printf("%d ", curr_p->data);
      curr_p = curr_p->next;
   }
   printf("\n");
}

int main() {
  struct node* cabeza=NULL;
  Insert(4,&cabeza);
  Insert(2,&cabeza);
  Insert(5,&cabeza);
  Insert(3,&cabeza);
  Insert(6,&cabeza);
  Print(cabeza);

}