#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
  struct Node* previous;
  struct Node* next;
  char* string;
} Node;

Node* create_node(const char* string) {
  Node* node = malloc(sizeof(Node));
  if (node == NULL) {
    fprintf(stderr, "Ran out of Memory!\n");
    exit(1);
  }
  
  node->string = strdup(string);
  if (node->string == NULL) {
    fprintf(stderr, "Ran out of Memory!\n");
    exit(1);
  }
  
  node->previous = NULL;
  node->next = NULL;
  return node;
}

Node* find_node(Node* list, const char* string) {
  while (list != NULL) {
    if (strcmp(string, list->string) == 0) {
      return list;
    }
    list = list->next;
  }
  return NULL; // If not found
}

void insert_node(Node** list, Node* previous, const char* string) {
  Node* node = create_node(string);
  
  if (previous == NULL) {
    node->next = *list; // Easy insert at front
    if (*list != NULL) {
      (*list)->previous = node;
    }
    *list = node;
  } else {
    node->next = previous->next;
    node->previous = previous;
    if (previous->next != NULL) {
      previous->next->previous = node;
    }
    previous->next = node;
    }
  }
void remove_node(Node** list, Node* node) {
  if (node->previous != NULL) {
    node->previous->next = node->next;
  } else {
    *list = node-> next;
  }
  
  if (node->next != NULL) {
    node->next->previous = node->previous;
  }
  
    free(node->string);
    free(node);
  }
  
void clear_list(Node** list) {
  Node* node = *list;
  while (node != NULL) {
    Node* next = node->next;
    free(node->string);
    free(node);
    node = next;
  }
  *list = NULL; //Free Pointer
}
  
void print_list(Node* list) {
  int i = 0;
  while (list != NULL) {
    printf(" [%d] %s\n", i, list->string);
    list = list->next;
    ++i;
  }
}
  
int main(void) {
  Node* list = NULL;
  insert_node(&list, NULL, "red");
  insert_node(&list, NULL, "blue");
  print_list(list);

  
  remove_node(&list, find_node(list, "blue"));
  print_list(list);
  
  clear_list(&list);
  print_list(list); //Should be back to NULL
  
  return 0;
}