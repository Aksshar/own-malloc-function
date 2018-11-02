#include<stdio.h>
#include<stdlib.h>

#define max 25000

char memory[max];


struct Node {
  int free;
  int size;
  struct Node * next;
  struct Node * prev;
  char *ref;

};

struct Node * root;

void create_root()
{
  root = (struct Node *)memory;
  root->free = 1;
  root->size  = max - sizeof(struct Node);
  root->next = NULL;
  root->prev = NULL;
  root->ref = memory + sizeof(struct Node);
}

void separate(struct Node * x, unsigned int size)
{
  if(x->size - size <= sizeof(struct Node)) return;

  struct Node * new = ((void *)(x + 1) + size);

  new -> next = x->next;
  new -> prev =x;
  x->next = new;

  new->free = 1;
  new->size = x->size - size - sizeof(struct Node);
  x->size = size;
  new->ref = (void *)(new+1);

  if(x->next)
  {
    x -> next -> prev = new;
  }
}

void * MyMalloc(unsigned int size)
{
  if(root == NULL) create_root();

  struct Node * node = root;
  while(node!=NULL)
  {
    if(node->free == 1 && node -> size >= size )
    {
      if(node->size > size)
      {
        separate(node , size);
      }
      node->free = 0;
      return node->ref;
    }
    node = node->next;
  }
  return NULL;

}

void Merge(struct Node * node)
{
  node -> size = (node -> size) + (node -> next -> size) + sizeof(struct Node);
  node -> next = node->next->next;
  if(node -> next)
  {
    node -> next -> prev = node;
  }
}


void MyFree(void * ref)
{
  struct Node *node = root;
  while(node != NULL)
  {
    if(node -> ref == ref)
    {
      node->free = 1;
      if(node->next && node->next->free)
      {
        Merge(node);
      }
      if(node->prev && node->prev->free)
      {
        Merge(node->prev);
      }

      return;
    }
    node = node -> next;
  }
}
