#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
* CITITOR
**/

typedef struct {
    char nume[50];
    char adresa[50];
} Cititor;


/**
* CARTE
**/

typedef struct node {
    char titlu[50];
    char autor[50];
    Cititor cititor;
    int termen;

    struct node* next;
} Carte;



void Carte_vidare(Carte* head) {
    for (Carte* i = head; i != NULL;) {
        Carte* next = i->next;
        free(i);
        i = next;
    }
}

void Carte_imprumut_nou(
    Carte** head,
    char* titlu_carte,
    char* autor_carte,
    char* nume_cititor,
    char* adresa_cititor,
    int termen_returnare
) {

    Carte* crt = (Carte*)malloc(sizeof(Carte));
    if (crt == NULL) {
        fprintf(stderr, "Memorie insuficienta!\n");
        return;
    }

    strcpy(crt->titlu, titlu_carte);
    strcpy(crt->autor, autor_carte);
    crt->termen = termen_returnare;

    strcpy(crt->cititor.nume, nume_cititor);
    strcpy(crt->cititor.adresa, adresa_cititor);

    crt->next = *head == NULL ? NULL : *head;
    *head = crt;
}

int Carte_comp(const void* a, const void* b) {
    Carte* c1 = (Carte*)a;
    Carte* c2 = (Carte*)b;
    if (c1->termen == c2->termen) {
        return 0;
    }
    return c1->termen < c2->termen ? 1 : -1;
}

void Carte_actualizare(Carte* head) {
    Carte** overdue = NULL;
    int len = 0;

    for (Carte* i = head; i != NULL; i = i->next) {
        i->termen--;
        if (i->termen <= 0) {
            overdue = (Carte**)realloc(overdue, (len + 1) * sizeof(Carte*));
            overdue[len++] = i;
        }
    }

    qsort(overdue, len, sizeof(Carte*), Carte_comp);
    for (int i = 0; i < len; i++) {
        printf("%s %d\n", overdue[i]->cititor.nume, overdue[i]->termen);
    }

    free(overdue);
}

void Carte_returnare(Carte** head, char* nume_cititor) {
    if (*head == NULL)
        return;

    Carte* temp = *head, * prev;
    if (temp != NULL && !strcmp(temp->cititor.nume, nume_cititor)) {
        *head = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && strcmp(temp->cititor.nume, nume_cititor))
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;
    prev->next = temp->next;
    free(temp);
}

void Carte_afisare(Carte* head) {
    for (Carte* temp = head; temp != NULL; temp = temp->next) {
        printf("%s %s %s %s %d\n",
            temp->titlu,
            temp->autor,
            temp->cititor.nume,
            temp->cititor.adresa,
            temp->termen);
    }
}

/**
* MAIN
**/

int main() {

    Carte* biblioteca = NULL;
    Carte_imprumut_nou(&biblioteca, "Kriss Gardner Bibliografie", "Kriss", "Diana Kovacs", "Timisoara la La", 1000);
    Carte_imprumut_nou(&biblioteca, "Kriss Gardner Bibliografie", "Kriss", "Andrei Stoica", "Timisoara la gara", 0);
    Carte_afisare(biblioteca);
    Carte_actualizare(biblioteca);

    Carte_returnare(&biblioteca, "Diana Kovacs");
    Carte_afisare(biblioteca);

    Carte_vidare(biblioteca);

    return 0;
}