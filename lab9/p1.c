#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CATEGORY_COUNT 5

typedef struct node {
    int id;
    int order;
    char* message;

    struct node* next;
} Message;

typedef struct {
    int order;
    Message* head;
    Message* oldest;
} Category;

typedef struct {
    Category categories[CATEGORY_COUNT];
    int length;
} Manager;

int main() {
    Manager manager;
    manager.length = 0;


}