#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "Studenti2.txt"
#define STUDENT_COUNT 5000
#define NAME_MAX 35
#define TRUE 1
#define FALSE 0


/**
* Helpers
**/

void removeEndl(char* str) {
    str[strcspn(str, "\r\n")] = '\0';
}


/**
* Data types and methods
**/

// Stats
typedef struct {
    unsigned long long int comparisons;
    unsigned long long int attribs;
} Stats;

void Stats_print(Stats* stats) {
    printf("Comparisons: %u\nAttribs: %u\n", stats->comparisons, stats->comparisons);
}

// Student
typedef struct {
    char* name;
    float avg;
} Student;

void Student_print(Student* student) {
    printf("%-35s %f", student->name, student->avg);
}

int Student_compAvgAsc(Student* s1, Student* s2) {
    return s1->avg > s2->avg;
}

int Student_compAvgDesc(Student* s1, Student* s2) {
    return s1->avg < s2->avg;
}

// Data
typedef struct {
    Student** students;
    int len;
} Data;

void Data_swap(Student** a, Student** b) {
    Student* temp = *a;
    *a = *b;
    *b = temp;
}

Stats Data_selSort(Data data, int (*comp)(Student*, Student*)) {
    Stats stats;
    stats.attribs = 0;
    stats.comparisons = 0;

    for (int i = 0; i < data.len - 1; i++) {
        int min = i;
        for (int j = i + 1; j < data.len; j++) {
            if (data.students[j] < data.students[min]) {
                min = j;
            }
            stats.comparisons++;
        }
        Data_swap(data.students + min, data.students + i);
        stats.attribs++;
    }

    return stats;
}

Stats Data_bubbleSort(Data data, int (*comp)(Student*, Student*)) {
    Stats stats;
    stats.attribs = stats.comparisons = 0;
    int swapped;
    do {
        swapped = 0;
        for (int i = 0; i < data.len - 1; i++) {
            if (data.students[i] > data.students[i + 1]) {
                Data_swap(data.students + i, data.students + i + 1);
                stats.attribs += 3;
                swapped = 1;
            }
            stats.comparisons++;
        }
    } while (swapped);

    return stats;
}

void Data_free(Data data, int studentCount, int nameCount) {
    for (int i = 0; i < nameCount; i++) {
        free(data.students[i]->name);
    }
    for (int i = 0; i < studentCount; i++) {
        free(data.students[i]);
    }
    free(data.students);
}


void Data_print(Data data) {
    for (int i = 0; i < data.len; i++) {
        Student_print(data.students[i]);
        printf("\n");
    }
}

/**
* Main
**/

int main() {

    /**
    * FILE
    **/

    // Get file name
    const char* filename = FILENAME;

    // Open file
    FILE* fin = fopen(filename, "r");
    if (fin == NULL) {
        fprintf(stderr, "Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }


    /**
    * Data management
    **/

    // Create data variables
    Data data;
    data.len = 0;
    data.students = (Student**)malloc(STUDENT_COUNT * sizeof(Student*));
    if (data.students == NULL) {
        fclose(fin);
        fprintf(stderr, "Failed malloc\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < STUDENT_COUNT; i++) {
        data.students[i] = (Student*)malloc(sizeof(Student));
        if (data.students[i] == NULL) {
            Data_free(data, i, 0);
            fclose(fin);
            fprintf(stderr, "Failed malloc\n");
            exit(EXIT_FAILURE);
        }
    }

    // Create line holder
    char* line = (char*)malloc(NAME_MAX * sizeof(char));
    if (line == NULL) {
        Data_free(data, STUDENT_COUNT, 0);
        fprintf(stderr, "Failed malloc\n");
        fclose(fin);
        free(data.students);
        exit(EXIT_FAILURE);
    }

    // Get file contents
    while (TRUE) {
        Student* crt = data.students[data.len];

        // Get name line
        if (fgets(line, NAME_MAX, fin) == NULL) {

            if (data.len == STUDENT_COUNT) {// Check for end of file
                break;
            }
            else {// Free memory and exit
                Data_free(data, STUDENT_COUNT, data.len);
                free(data.students);
                exit(EXIT_FAILURE);
                fprintf(stderr, "Failed to get name at index %d\n", data.len);
            }
        }
        removeEndl(line);
        crt->name = (char*)malloc((strlen(line) + 1) * sizeof(char));
        if (crt->name == NULL) {// Free memory and exit
            Data_free(data, STUDENT_COUNT, data.len);
            free(data.students);
            exit(EXIT_FAILURE);
            fprintf(stderr, "Failed to get name at index %d\n", data.len);
        }
        strcpy(crt->name, line);


        // Get avg line
        if (fgets(line, NAME_MAX, fin) == NULL) {// Free memory and exit
            free(crt->name);
            Data_free(data, STUDENT_COUNT, data.len);
            free(data.students);
            exit(EXIT_FAILURE);
            fprintf(stderr, "Failed to get name at index %d\n", data.len);
        }
        removeEndl(line);
        sscanf(line, " %f ", &crt->avg);

        // Update student count
        data.len++;
    }// End while

    // Close the file and free unused memory
    fclose(fin);
    free(line);


    /**
    * Logic
    */


    for (int i = 0; i < 4; i++) {
        printf("%s %s\n", i / 2 ? "BubbleSort" : "SelSort", i % 2 ? "Asc" : "Desc");
        Stats(*sorter)(Data, int (*)(Student*, Student*)) = i / 2 ? Data_bubbleSort : Data_selSort;
        int (*comp)(Student*, Student*) = i % 2 ? Student_compAvgAsc : Student_compAvgDesc;
        Stats result = sorter(data, comp);
        Stats_print(&result);
    }


    /**
    * Clean
    */

    Data_free(data, STUDENT_COUNT, STUDENT_COUNT);

    return 0;
}