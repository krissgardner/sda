#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * STUDENT
 */

typedef struct {
    char name[100];
    char gender;
    float average;
} Student;

/**
 * Array
 */

typedef struct {
    Student* arr;
    int length;
} Array;

int comp(const void* first, const void* second) {
    Student* a = (Student*)first;
    Student* b = (Student*)second;

    if ((*a).gender < (*b).gender) {
        return -1;
    }
    if ((*a).gender > (*b).gender) {
        return 1;
    }

    if ((*a).average < (*b).average) {
        return -1;
    }
    if ((*a).average > (*b).average) {
        return 1;
    }

    if (strcmp((*a).name, (*b).name) < 0) {
        return -1;
    }
    if (strcmp((*a).name, (*b).name) > 0) {
        return 1;
    }

    return 0;
}

void Data_insertionSort(Array data) {
    int i, j;
    Student key;
    for (i = data.length - 2; i >= 0; i--) {
        key = data.arr[i];
        j = i + 1;

        while (j < data.length && data.arr[j].average > key.average) {
            data.arr[j - 1] = data.arr[j];
            j++;
        }
        data.arr[j - 1] = key;
    }
}

int Data_find(Array data, char* str) {
    for (int i = 0; i < data.length; i++) {
        if (!strcmp(str, data.arr[i].name)) {
            return i;
        }
    }

    return -1;
}

void Data_print(Array data) {
    for (int i = 0; i < data.length; i++) {
        printf("%s %c %f\n", data.arr[i].name, data.arr[i].gender,
               data.arr[i].average);
    }
}

int main() {

    /**
     * GET DATA FROM STDIN
     */

    char line[150];
    Array data;
    scanf("%d", &data.length);
    getchar();

    // Get space
    data.arr = (Student*)malloc(data.length * sizeof(Student));
    if (data.arr == NULL) {
        fprintf(stderr, "Not enough memory\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < data.length; i++) {
        Student* crt = data.arr + i;
        fgets(line, 149, stdin);
        sscanf(line, "%f %c %[^\t\n]", &crt->average, &crt->gender, crt->name);
    }

    char name1[100], name2[100];
    fgets(name1, 99, stdin);
    name1[strcspn(name1, "\r\n")] = '\0';
    fgets(name2, 99, stdin);
    name2[strcspn(name2, "\r\n")] = '\0';

    /**
     * TASK 1
     */

    Data_insertionSort(data);

    // Data_print(data);

    printf("%d\n", Data_find(data, name1));

    /**
     * TASK 2
     */
    qsort(data.arr, data.length, sizeof(Student), comp);

    // Data_print(data);

    printf("%d\n", Data_find(data, name2));

    return 0;
}