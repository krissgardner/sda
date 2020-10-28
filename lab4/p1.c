#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "filme.txt"
#define FILENAME_OUT "nume_filme.txt"
#define LINEMAX 100 // checked
#define TITLEMAX 70
#define CATEGORYMAX 20

#ifndef DEV
#define READLIMIT
#else
#define READLIMIT if (data.len >= 10) break;
#endif

/**
* Helpers
**/

void removeEndl(char* str) {
    str[strcspn(str, "\r\n")] = '\0';
}

/**
* Data types and methods
**/

// Note: maybe make categories static to save memory

// Movie
typedef struct {
    short year;
    short duration;
    char* title;
    char* category;
    char hasAwards;
} Movie;

void Movie_print(Movie* movie) {
    printf("%-60s %-6hd %-6hd %-20s %-6hhd\n",
        movie->title,
        movie->year,
        movie->duration,
        movie->category,
        movie->hasAwards
    );
}


// Data
typedef struct {
    Movie** movies;
    int len;
} Data;

void Data_swap(Movie** a, Movie** b) {
    Movie* temp = *a;
    *a = *b;
    *b = temp;
}

void Data_free(Data data) {
    for (int i = 0; i < data.len; i++) {
        free(data.movies[i]->category);
        free(data.movies[i]->title);
        free(data.movies[i]);
    }
    free(data.movies);
}

void Data_print(Data data) {
    for (int i = 0; i < data.len; i++) {
        Movie_print(data.movies[i]);
    }
}

// Used for the first task of p1
void Data_printTitles(Data data, FILE* fout) {
    for (int i = data.len - 1; i >= 0; i--) {
        fprintf(fout, "%s\n", data.movies[i]->title);
    }
}

/**
* HEAPSORT for Data
**/

// Note: maybe make comp function a param
void Data_heapify(Data data, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < data.len && strcmp(data.movies[l]->title, data.movies[largest]->title) > 0) {
        largest = l;
    }

    if (r < data.len && strcmp(data.movies[r]->title, data.movies[largest]->title) > 0) {
        largest = r;
    }

    if (largest != i) {
        Data_swap(&data.movies[i], &data.movies[largest]);
        Data_heapify(data, largest);
    }
}

void Data_heapSort(Data data) {
    // Build heap (rearrange array) 
    for (int i = data.len / 2 - 1; i >= 0; i--) {
        Data_heapify(data, i);
    }

    while (data.len != 1) {
        // Move largest item to the end of the heap
        data.len--;
        Data_swap(data.movies + 0, data.movies + data.len);

        // call max heapify on the reduced heap
        Data_heapify(data, 0);
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
    data.movies = NULL;
    Movie** temp = NULL;

    // Create line holder
    char* line = (char*)malloc(LINEMAX * sizeof(char));
    if (line == NULL) {
        fprintf(stderr, "Failed malloc\n");
        fclose(fin);
        exit(EXIT_FAILURE);
    }

    // Get file contents
    fgets(line, LINEMAX, fin); // dump useless data
    while (fgets(line, LINEMAX, fin)) {

        READLIMIT

            // Remove trailing format characters if any
            removeEndl(line);

        // Get more memory for next movie
        temp = (Movie**)realloc(data.movies, (data.len + 1) * sizeof(Movie*));
        if (temp == NULL) {
            fprintf(stderr, "Failed realloc\n");
            Data_free(data);
            fclose(fin);
            exit(EXIT_FAILURE);
        }
        data.movies = temp;

        // Get more memory for movie index
        data.movies[data.len] = (Movie*)malloc(sizeof(Movie));
        if (data.movies[data.len] == NULL) {
            fprintf(stderr, "Failed malloc\n");
            Data_free(data);
            fclose(fin);
            exit(EXIT_FAILURE);
        }

        // Current movie
        Movie* crt = data.movies[data.len];

        // Get memory for movie title
        crt->title = (char*)malloc(TITLEMAX * sizeof(char));
        if (crt->title == NULL) {
            fprintf(stderr, "Failed malloc\n");
            free(data.movies[data.len]);
            Data_free(data);
            fclose(fin);
            exit(EXIT_FAILURE);
        }

        // Get memory for movie category
        crt->category = (char*)malloc(CATEGORYMAX * sizeof(char));
        if (crt->category == NULL) {
            free(crt->title);
            fprintf(stderr, "Failed malloc\n");
            free(data.movies[data.len]);
            Data_free(data);
            fclose(fin);
            exit(EXIT_FAILURE);
        }

        for (char* token = strtok(line, "\t"), i = 0; token != NULL; token = strtok(NULL, "\t"), i++) {
            //printf("%s\n", token);
            switch (i) {
            case 0: {
                crt->year = atoi(token);
                break;
            }
            case 1: {
                crt->duration = atoi(token);
                break;
            }
            case 2: {
                // No need to handle realloc failure because there will always be less memory
                strcpy(crt->title, token);
                crt->title = (char*)realloc(crt->title, (strlen(crt->title) + 1) * sizeof(char));
                break;
            }
            case 3: {
                strcpy(crt->category, token);
                crt->category = (char*)realloc(crt->category, (strlen(crt->category) + 1) * sizeof(char));
                break;
            }
            case 4: {
                crt->hasAwards = atoi(token);
                break;
            }
            }
        }

        // Update movie count
        data.len++;
    }// End while

    // Close the file and free unused memory
    fclose(fin);
    free(line);

    /**
    * Logic
    */

    // Open out file for sorted titles
    char* filenameOut = FILENAME_OUT;
    FILE* fout = fopen(filenameOut, "w");
    if (fout == NULL) {
        fprintf(stderr, "Could no open file %s\n", filenameOut);
        Data_free(data);
        fclose(fout);
        exit(EXIT_FAILURE);
    }

    // Sort and print
    Data_heapSort(data);
    Data_printTitles(data, fout);

    // Close the file
    fclose(fout);




    /**
    * Clean
    */

    Data_free(data);
    return 0;
}