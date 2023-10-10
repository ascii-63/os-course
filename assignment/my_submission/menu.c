#include <stdio.h>

int main(int argc, const char* argv[]) {
    int i, choice;

    if (argc <= 1) {
        fprintf(stderr, "Please give some options\n");
        return -1;
    }

    do {
        printf("0. Exit\n");

        for (i = 1; i < argc; i++)
            printf("%d. %s\n", i, argv[i]);
        
        printf("Your choice: ");
        scanf("%d", &choice);
    } while (choice < 0 || choice > argc);

    return choice;
}