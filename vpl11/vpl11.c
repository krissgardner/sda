#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int valid(char* build, char* must) {
    int result = 1;

    for (int i = 0; i < strlen(must); i++) {
        if (strchr(build, must[i]) == NULL) {
            result = 0;
            break;
        }
    }

    return result;
}

void generate(
    int n,
    char* alphabet,
    char* must,
    char* build,
    int* count) {

    if (strlen(build) == n) {
        if (valid(build, must)) {
            (*count)++;
        }

        return;
    }

    for (int i = 0; i < strlen(alphabet); i++) {
        int index = strlen(build);

        // Update
        build[index] = alphabet[i];

        // Call next stage
        generate(n, alphabet, must, build, count);

        // Reset changes
        build[index] = '\0';
    }

    return;
}

void kryptkat(char* pass, char* generate) {
    if (!strlen(pass)) {
        return;
    }

    int len = strlen(pass);
    int index = len / 2;

    generate[strlen(generate)] = pass[index];
    pass[index] = '\0';

    kryptkat(pass, generate);
    kryptkat(pass + index + 1, generate);
}

int main() {

    // Input data
    int n;
    char alphabet[100];
    char must[100];
    char pass[100];

    // Array where the generated passwords will be
    char build[100];
    int count = 0;

    // The encripted array
    char encripted[100];

    for (int i = 0; i < 100; i++) {
        build[i] = encripted[i] = '\0';
    }

    // Get data
    scanf("%d", &n);
    getchar();
    fgets(alphabet, 99, stdin);
    fgets(must, 99, stdin);
    fgets(pass, 99, stdin);
    alphabet[strcspn(alphabet, "\r\n")] = '\0';
    must[strcspn(must, "\r\n")] = '\0';
    pass[strcspn(pass, "\r\n")] = '\0';

    // Generate passwords
    generate(n, alphabet, must, build, &count);
    printf("%d\n", count);

    kryptkat(pass, encripted);
    printf("%s\n", encripted);
}