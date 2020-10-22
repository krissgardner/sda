#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define FILENAME "COR_Descrescator.txt"
#define LINE_MAX 350


typedef struct {
    int code;
    char* title;
} Profession;

typedef struct {
    Profession* professions;
    int len;
} Data;

void data_filter(Data data, char* search) {
    int len = strlen(search);
    for (int i = 0; i < data.len; i++) {
        char* title = data.professions[i].title;
        if (!strncmp(title, search, len) && !isalpha(title[len])) {
            printf("%s\n", title);
        }
    }
}

void removeEndl(char* str) {
    str[strcspn(str, "\n")] = '\0';
}

int interpolationSearch(Profession* v, int length, int x) {
    int left = 0, right = (length - 1);
    while (left <= right && x <= v[left].code && x >= v[right].code) {
        if (left == right) {
            if (v[left].code == x) {
                return left;
            }
            return -1;
        }
        int pos = left + (((double)(right - left) / (v[left].code - v[right].code)) * (v[left].code - x));
        if (v[pos].code == x) {
            return pos;
        }
        else if (v[pos].code > x) {
            left = pos + 1;
        }
        else {
            right = pos - 1;
        }
    }
    return -1;
}


int main() {

    /**
    * FILE
    **/

    // Get file name
    const char filename[] = FILENAME;

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
    data.professions = NULL;
    Profession* temp = NULL;
    char* line;

    // Create line holder
    line = (char*)malloc(LINE_MAX * sizeof(char));
    if (line == NULL) {
        fprintf(stderr, "Failed malloc\n");
        fclose(fin);
        exit(EXIT_FAILURE);
    }

    // Get file contents
    fgets(line, LINE_MAX, fin); // Dump useless data
    while (fgets(line, LINE_MAX, fin)) {
        int crt;
        char* token;

        // Remove trailing \n if any
        removeEndl(line);

        // Get more memory
        temp = (Profession*)realloc(data.professions, (data.len + 1) * sizeof(Profession));
        if (temp == NULL) {
            for (int i = 0; i < data.len; i++) {
                free(data.professions[i].title);
            }
            free(data.professions);
            free(line);
            fclose(fin);
            fprintf(stderr, "Failed malloc\n");
            exit(EXIT_FAILURE);
        }
        data.professions = temp;
        data.len++;
        crt = data.len - 1;

        // Get code
        token = strtok(line, "\t");
        sscanf(token, "%d", &data.professions[crt].code);

        // Get title
        token = strtok(NULL, "\t");

        data.professions[crt].title = (char*)malloc((strlen(token) + 1) * sizeof(char));
        if (data.professions[crt].title == NULL) {
            for (int i = 0; i < data.len; i++) {
                free(data.professions[i].title);
            }
            free(data.professions);
            free(line);
            fclose(fin);
            fprintf(stderr, "Failed malloc\n");
            exit(EXIT_FAILURE);
        }
        strcpy(data.professions[crt].title, token);
    }

    // Close the file
    fclose(fin);


    /**
    * Logic
    */

    int result, searchCode;

    // Get code
    scanf("%d", &searchCode);
    getchar();

    // Search for code
    result = interpolationSearch(data.professions, data.len, searchCode);
    if (result != -1) {
        printf("%s\n", data.professions[result].title);
    }
    else {
        printf("COD INEXISTENT\n");
    }

    // Get word
    //fgets(line, LINE_MAX, stdin);
    scanf("%s", line);
    removeEndl(line);

    // Search for word and print profession
    data_filter(data, line);

    /**
    *  Clean
    **/

    for (int i = 0; i < data.len; i++) {
        free(data.professions[i].title);
    }
    free(data.professions);
    free(line);

    return 0;
}