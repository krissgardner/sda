#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "filme.txt"
#define SORTED_BY_NAME "nume_filme.txt"
#define SORTED_BY_CATEGORY "categorii_filme.txt"
#define SORTED_BY_AWARD "premii_filme.txt"
#define LINEMAX 100 // checked
#define TITLEMAX 70
#define CATEGORYMAX 20

#define DEV

#ifndef DEV
#define READLIMIT
#else
#define READLIMIT if (data.len >= 10) break;
#endif

// GLOBALS
const char* const hasAward[] = { "Nu", "Da" };

/***************************
* Helpers
**/

void removeEndl(char* str) {
    str[strcspn(str, "\r\n")] = '\0';
}

/***************************
* Data types and methods
**/

// Made categories static to save memory
typedef struct {
    char** array;
    int len;
    int* sortedIndex;
} Categories;

Categories categs; // Global category manager

void Categories_free() {
    for (int i = 0; i < categs.len; i++) {
        free(categs.array[i]);
    }
    free(categs.array);
    free(categs.sortedIndex);
}

int Categories_comp(const void* a, const void* b) {
    return strcmp(categs.array[*(int*)a], categs.array[*(int*)b]);
}

void Categories_makeSortedMap() {
    for (int i = 0; i < categs.len; i++) {
        categs.sortedIndex[i] = i;
    }
    qsort(categs.sortedIndex, categs.len, sizeof(int), Categories_comp);
    int map[categs.len];
    for (int i = 0; i < categs.len; i++) {
        map[categs.sortedIndex[i]] = i;
    }
    for (int i = 0; i < categs.len; i++) {
        categs.sortedIndex[i] = map[i];
    }
}

/**
* Movie
**/

typedef struct {
    short year;
    short duration;
    char* title;
    char category;
    char hasAwards;
} Movie;

void Movie_swap(Movie** a, Movie** b) {
    Movie* temp = *a;
    *a = *b;
    *b = temp;
}

void Movie_print(Movie* movie, FILE* fout) {
    fprintf(fout, "%-70s %-6hd %-6hd %-16s %-3s\n",
        movie->title,
        movie->year,
        movie->duration,
        categs.array[movie->category],
        hasAward[movie->hasAwards]
    );
}

// Modified Quicksort for Movie by category
void Movie_partition(Movie** movies, int low, int high, int* i, int* j) {
    // Handles two elements
    if (high - low <= 1) {
        if (categs.sortedIndex[movies[high]->category] < categs.sortedIndex[movies[low]->category]) {
            Movie_swap(movies + high, movies + low);
        }
        *i = low;
        *j = high;
        return;
    }

    int mid = low;
    int pivot = movies[high]->category;

    while (mid <= high) {
        if (categs.sortedIndex[movies[mid]->category] < categs.sortedIndex[pivot]) {
            Movie_swap(movies + low++, movies + mid++);
        }
        else if (categs.sortedIndex[movies[mid]->category] > categs.sortedIndex[pivot]) {
            Movie_swap(movies + mid, movies + high--);
        }
        else {
            mid++;
        }
    }

    // Update i and j
    *i = low - 1;
    *j = mid;
}

void Movie_quicksort(Movie** movies, int low, int high) {
    // Handles one or no elements
    if (low >= high) {
        return;
    }

    int i, j;
    Movie_partition(movies, low, high, &i, &j);

    // Recur two halves
    Movie_quicksort(movies, low, i);
    Movie_quicksort(movies, j, high);
}

/**
* Data
**/

typedef struct {
    Movie** movies;
    int len;
} Data;

void Data_free(Data data) {
    for (int i = 0; i < data.len; i++) {
        //free(data.movies[i]->category);
        free(data.movies[i]->title);
        free(data.movies[i]);
    }
    free(data.movies);
    Categories_free();
}

void Data_print(Data data, FILE* fout) {
    for (int i = 0; i < data.len; i++) {
        Movie_print(data.movies[i], fout);
    }
}


// HEAPSORT for Data
// Note: maybe make comp function a param
void Data_heapify(Data data, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < data.len && strcmp(data.movies[l]->title, data.movies[largest]->title) < 0) {
        largest = l;
    }

    if (r < data.len && strcmp(data.movies[r]->title, data.movies[largest]->title) < 0) {
        largest = r;
    }

    if (largest != i) {
        Movie_swap(&data.movies[i], &data.movies[largest]);
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
        Movie_swap(data.movies + 0, data.movies + data.len);

        // call max heapify on the reduced heap
        Data_heapify(data, 0);
    }
}

// Countsort for Data
void Data_countsort(Data data) {
    // Output array 
    Movie* output[data.len];
    int count[2] = { 0 };

    // Store count of occurrences in count[] 
    for (int i = 0; i < data.len; i++) {
        count[data.movies[i]->hasAwards]++;
    }

    count[1] += count[0];

    // Build the output array 
    for (int i = data.len - 1; i >= 0; i--) {
        output[count[data.movies[i]->hasAwards] - 1] = data.movies[i];
        count[data.movies[i]->hasAwards]--;
    }

    // Copy output to initial array
    for (int i = 0; i < data.len; i++) {
        data.movies[i] = output[data.len - i - 1];
    }
}


/***************************
* Main
**/

int main() {

    /**
    * FILE
    **/

    // Variable to hold current file name
    char* filename;
    // Variables that hold in/out file refs
    FILE* fin, * fout;

    // Get file name
    filename = FILENAME;
    fin = fopen(filename, "r");
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

    categs.array = NULL;
    categs.len = 0;

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
                // strcpy(crt->category, token);
                // crt->category = (char*)realloc(crt->category, (strlen(crt->category) + 1) * sizeof(char));
                int found = 0;
                for (int i = 0; i < categs.len; i++) {
                    if (!strcmp(token, categs.array[i])) {
                        found = 1;
                        // Store index
                        crt->category = i;
                        break;
                    }
                }
                if (!found) {
                    int memoryFail = 0;

                    // Add new category
                    char** temp = (char**)realloc(categs.array, (categs.len + 1) * sizeof(char*));
                    if (temp == NULL) {
                        // Flag error
                        memoryFail = 1;
                    }
                    if (!memoryFail) {// Skip if it has failed
                        categs.array = temp;
                        categs.array[categs.len] = (char*)malloc((strlen(token) + 1) * sizeof(char));
                        if (categs.array[categs.len] == NULL) {
                            // Flag error
                            memoryFail = 1;
                        }
                    }
                    // Handle free
                    if (memoryFail) {
                        free(crt->title);
                        fprintf(stderr, "Failed malloc\n");
                        free(data.movies[data.len]);
                        Data_free(data);
                        fclose(fin);
                        exit(EXIT_FAILURE);
                    }
                    strcpy(categs.array[categs.len], token);
                    categs.len++;

                    // Assign new index
                    crt->category = categs.len - 1;
                }
                break;
            }
            case 4: {
                crt->hasAwards = !strcmp(token, hasAward[0]) ? 0 : 1;
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

    // Create sorted category array
    categs.sortedIndex = (int*)malloc(categs.len * sizeof(int));
    if (categs.sortedIndex == NULL) {
        fprintf(stderr, "Failed malloc\n");
        Data_free(data);
        exit(EXIT_FAILURE);
    }

    // Map array
    Categories_makeSortedMap();


    /***************************
    * Logic
    */

    // Task 1

    // Open out file for sorted titles
    filename = SORTED_BY_NAME;
    fout = fopen(filename, "w");
    if (fout == NULL) {
        fprintf(stderr, "Could no open file %s\n", filename);
        Data_free(data);
        exit(EXIT_FAILURE);
    }

    // Sort and print
    Data_heapSort(data);
    Data_print(data, fout);

    // Close the file
    fclose(fout);

    // Task 2

    // Open out file for sorted categories
    filename = SORTED_BY_CATEGORY;
    fout = fopen(filename, "w");
    if (fout == NULL) {
        fprintf(stderr, "Could no open file %s\n", filename);
        Data_free(data);
        exit(EXIT_FAILURE);
    }

    // Sort movies by category and print data
    Movie_quicksort(data.movies, 0, data.len - 1);
    Data_print(data, fout);

    // Close the file
    fclose(fout);

    // Task 3

    // Open out file for sorted categories
    filename = SORTED_BY_AWARD;
    fout = fopen(filename, "w");
    if (fout == NULL) {
        fprintf(stderr, "Could no open file %s\n", filename);
        Data_free(data);
        exit(EXIT_FAILURE);
    }

    // Sort movies by award and print data
    Data_countsort(data);
    Data_print(data, fout);

    // Close the file
    fclose(fout);


    /***************************
    * Clean
    */

    Data_free(data);
    return 0;
}