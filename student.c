#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

#define INITIAL_CAPACITY 10

/* ------------------ STUDENT RECORD SYSTEM CORE FUNCTIONS ------------------ */

/**
 * init_system - initialize student record system
 *
 * @sys: pointer to variable holding student record system
 * Return: void
 */
void init_system(StudentSystem *sys)
{
    // assign default variables to student record system
    sys->count = 0;
    sys->capacity = INITIAL_CAPACITY;

    // dynamically allocate array in memory for student records
    sys->students = (Student *)malloc(sys->capacity * sizeof(Student));

    // exit early if record system uninitialized properly
    if (sys->students == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
}

/**
 * free_system - free dynamically allocated memory before exiting program
 *
 * @sys: pointer to student record system variable
 * Return: void
 */
void free_system(StudentSystem *sys)
{
    // free dynamically allocated [heap] memory
    if (sys->students != NULL)
    {
        free(sys->students);
        sys->students = NULL;
    }

    // reset variables in [stack] memory
    sys->count = 0;
    sys->capacity = 0;
}

/**
 * resize_system - increase system capacity to accomodate more students
 *
 * @sys: pointer to student record system variable
 * Return: void
 */
void resize_system(StudentSystem *sys)
{
    // double student record system size if count exceeds current capacity
    if (sys->count >= sys->capacity)
    {
        sys->capacity *= 2;

        // reallocate more memory to dynamic array of student records
        sys->students = (Student *)realloc(sys->students, sys->capacity * sizeof(Student));

        // exit when a null pointer (empty system) is passed for reallocation
        if (sys->students == NULL)
        {
            printf("Memory reallocation failed!\n");
            exit(1);
        }
    }
}

/**
 * greet_user - prompt user for name and print welcome message to stdout
 *
 * Return: void
 */
void greet_user()
{
    // create static character array in memory to hold user name
    char username[50];

    printf("WELCOME TO MIVA OPEN UNIVERSITY COS 201 LAB PROJECT\n");
    printf("Please enter your name: ");

    // read and store characters up to length of username array from stdin
    if (fgets(username, sizeof(username), stdin) != NULL)
    {
        // remove newline at end of username string
        username[strcspn(username, "\n")] = '\0';
        // print welcome message formated to display username
        printf("\nHello, %s! Welcome to the Student Record System.\n", username);
    }
}

/**
 * get_menu_choice - prompt user to pick integer assigned to program function
 *
 * Return: int(choice) - user menu choice
 */
int get_menu_choice()
{
    int choice;
    char buf[64];
    char *endptr;

    // loop until we get a valid integer choice in range [0..10]
    while (1)
    {
        // print menu to display
        printf("\n===== STUDENT RECORD MENU =====\n");
        printf("1. Add Student\n");
        printf("2. Modify Student\n");
        printf("3. Remove Student\n");
        printf("4. Search Student\n");
        printf("5. Display All Students\n");
        printf("6. Sort Students\n");
        printf("7. Calculate Average Marks\n");
        printf("8. Verify Student Marks\n");
        printf("9. Save to File\n");
        printf("10. Load from File\n");
        printf("0. Exit\n");
        printf("\nEnter your choice: ");

        // read user input from stdin and save to buffer
        if (fgets(buf, sizeof(buf), stdin) == NULL)
        {
            // read or EOF error: exit program
            return 0;
        }

        // strip newline character at end of user input
        buf[strcspn(buf, "\n")] = '\0';

        // convert string to long integer; store invalid characters in endptr
        choice = (int)strtol(buf, &endptr, 10);

        // if buffer contains invalid characters, re-prompt for valid input from user
        if (endptr == buf)
        {
            printf("Invalid input. Please enter a number between 0 and 10.\n");
            continue;
        }

        // if choice is not within menu range, re-prompt for valid input
        if (choice < 0 || choice > 10)
        {
            printf("Choice must be between 0 and 10.\n");
            continue;
        }

        // return valid choice int to main function
        return choice;
    }
}
