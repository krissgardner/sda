#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "Studenti1.txt"
#define STUDENT_COUNT 5000
#define NAME_MAX 35
#define OP_MAX 15
#define TRUE 1
#define FALSE 0


/**
* Helpers
**/

void removeEndl(char* str) {
    str[strcspn(str, "\r\n")] = '\0';
}

void int_swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int String_comp_asc(char* str1, char* str2) {
    return strcmp(str1, str2);
}

int String_comp_desc(char* str1, char* str2) {
    return strcmp(str2, str1);
}


/**
* Data types and methods
**/

// Stats
typedef struct {
    unsigned long int comparisons;
    unsigned long int attribs;
} Stats;


// Student
typedef struct {
    char* name;
    char options[4][3];
} Student;

void Student_insertionSort(Student* student) {
    char temp[3];
    for (int i = 1; i < 4; i++) {
        strcpy(temp, student->options[i]);
        int j = i - 1;
        for (; j >= 0 && strcmp(student->options[j], temp) > 0; j--) {
            strcpy(student->options[j + 1], student->options[j]);
        }
        strcpy(student->options[j + 1], temp);
    }
}

void Student_print(Student* student) {
    printf("%s\n%s %s %s %s\n",
        student->name,
        student->options[0],
        student->options[1],
        student->options[2],
        student->options[3]
    );
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

void Data_free(Data data, int studentCount, int nameCount) {
    for (int i = 0; i < nameCount; i++) {
        free(data.students[i]->name);
    }
    for (int i = 0; i < studentCount; i++) {
        free(data.students[i]);
    }
    free(data.students);
}

Stats Data_cocktailSort(Data data, int (*String_comp)(char*, char*)) {
    Stats stats;
    stats.attribs = 0;
    stats.comparisons = 0;
    int swapped = TRUE;
    int start = 0;
    int end = data.len - 1;

    while (swapped) {
        swapped = FALSE;
        for (int i = start; i < end; ++i) {
            if (String_comp(data.students[i]->name, data.students[i + 1]->name) > 0) {
                Data_swap(data.students + i, data.students + i + 1);
                stats.attribs += 3;
                swapped = TRUE;
            }
            stats.comparisons++;
        }
        if (!swapped) {
            break;
        }
        swapped = FALSE;
        --end;
        for (int i = end - 1; i >= start; --i) {
            if (String_comp(data.students[i]->name, data.students[i + 1]->name) > 0) {
                Data_swap(data.students + i, data.students + i + 1);
                stats.attribs += 3;
                swapped = TRUE;
            }
            stats.comparisons++;
        }
        ++start;
    }

    return stats;
}

void Data_print(Data data) {
    for (int i = 0; i < data.len; i++) {
        Student_print(data.students[i]);
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
        int errors = 0;
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


        // Get options line
        if (fgets(line, OP_MAX, fin) == NULL) {// Free memory and exit
            free(crt->name);
            Data_free(data, STUDENT_COUNT, data.len);
            free(data.students);
            exit(EXIT_FAILURE);
            fprintf(stderr, "Failed to get name at index %d\n", data.len);
        }
        removeEndl(line);
        sscanf(line, "%s %s %s %s", crt->options[0], crt->options[1], crt->options[2], crt->options[3]);

        // Update student count
        data.len++;
    }// End while

    // Close the file and free unused memory
    fclose(fin);
    free(line);

    Data_print(data);


    /**
    * Logic
    */

    for (int i = 0; i < 2; i++) {
        // Get function
        int (*fn)(char*, char*) = i % 2 ? String_comp_asc : String_comp_desc;

        // Sort and get stats
        Stats result = Data_cocktailSort(data, fn);

        // Print resulting array
        Data_print(data);

        // Print stats
        printf("Comp: %lu\nAttrib: %lu\n", result.comparisons, result.attribs);
    }

    // Get data for a random student, since its not specified how to pick them
    Student randomStudent = *data.students[8];
    Student_insertionSort(&randomStudent);

    // Print both for comparison
    Student_print(data.students[8]);
    Student_print(&randomStudent);


    /**
    * Clean
    */

    Data_free(data, STUDENT_COUNT, STUDENT_COUNT);

    return 0;
}