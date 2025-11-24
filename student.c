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

/* ------------------ INTERNAL INPUT HELPER UTILITIES ------------------ */

/**
 * trim - delete leading and trailing whitespaces
 *
 * @s: pointer to string to trim
 * Return: void
 */
static void trim(char *s)
{
    char *end;

    // trim leading spaces by advancing pointer to first non-space character
    while (isspace((unsigned char)*s))
        s++;

    // if string consists of all spaces, return only null terminator
    if (*s == 0)
    {
        *s = '\0';
        return;
    }

    // trim trailing spaces by moving pointer backward to last non-space character
    end = s + strlen(s) - 1;

    while (end > s && isspace((unsigned char)*end))
        end--;

    *(end + 1) = '\0';
}

/**
 * read_line - read a full line into buffer [after prompting user]
 *
 * @prompt: prompt string to display before reading input
 * @buf: buffer to store the input line
 * @size: size of the buffer
 *
 * Return: int - 1 on success, 0 on EOF/error.
 */
static int read_line(const char *prompt, char *buf, size_t size)
{
    // display prompt if provided
    if (prompt)
        printf("%s", prompt);

    // read line from stdin into buffer
    if (fgets(buf, (int)size, stdin) == NULL)
        return 0;

    // remove newline character if present
    buf[strcspn(buf, "\n")] = '\0';

    // trim leading/trailing whitespace
    trim(buf);

    return 1;
}

/**
 * read_int_range - read an integer with range checking
 *
 * @prompt: prompt string to display before reading input
 * @min: minimum acceptable integer value
 * @max: maximum acceptable integer value
 *
 * Return: int(val) - valid integer within range
 */
static int read_int_range(const char *prompt, int min, int max)
{
    char buf[128];
    char *endptr;
    long val;

    // loop until we get a valid integer within specified range
    while (1)
    {
        // read a line of input with prompt
        if (!read_line(prompt, buf, sizeof(buf)))
            return min; // fallback to min val on EOF/error

        // check for empty input
        if (buf[0] == '\0')
        {
            printf("Input cannot be empty.\n");
            continue;
        }

        // convert buffer string to long integer
        val = strtol(buf, &endptr, 10);
        errno = 0;

        // check for conversion errors and re-prompt if invalid
        if (endptr == buf)
        {
            printf("Invalid integer. Please try again.\n");
            continue;
        }

        // check for out-of-range errors and re-enter loop if invalid
        if (errno == ERANGE || val < INT_MIN || val > INT_MAX)
        {
            printf("Integer out of range.\n");
            continue;
        }

        // check if value is within specified min-max range
        if (val < min || val > max)
        {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }

        // return valid integer value within range
        return (int)val;
    }
}

/**
 * read_float_range - read a float with range checking
 *
 * @prompt: prompt string to display before reading input
 * @min: minimum acceptable float value
 * @max: maximum acceptable float value
 *
 * Return: float(val) - valid float within range
 */
static float read_float_range(const char *prompt, float min, float max)
{
    char buf[128];
    char *endptr;
    float val;

    // loop until we get a valid float within specified range
    while (1)
    {
        // read a line of input with prompt
        if (!read_line(prompt, buf, sizeof(buf)))
            return min; // EOF fallback

        // check for empty input and re-prompt
        if (buf[0] == '\0')
        {
            printf("Input cannot be empty.\n");
            continue;
        }

        // convert buffer string to float
        val = strtof(buf, &endptr);
        errno = 0;

        // check for conversion errors and re-prompt if invalid
        if (endptr == buf)
        {
            printf("Invalid number. Please try again.\n");
            continue;
        }

        // check for out-of-range errors and re-enter loop if invalid
        if (errno == ERANGE)
        {
            printf("Number out of range.\n");
            continue;
        }

        // check if value is within specified min-max range
        if (val < min || val > max)
        {
            printf("Value must be between %.2f and %.2f.\n", min, max);
            continue;
        }

        // return valid float value within range
        return val;
    }
}

/**
 * read_name - read a non-empty name into dest (size bytes)
 *
 * @prompt: prompt string to display before reading input
 * @dest: destination buffer to store the name
 * @size: size of the destination buffer
 *
 * Return: void
 */
static void read_name(const char *prompt, char *dest, size_t size)
{
    char buf[256];

    // loop until we get a valid non-empty name
    while (1)
    {
        // read a line of input with prompt
        if (!read_line(prompt, buf, sizeof(buf)))
        {
            dest[0] = '\0'; // return empty string on EOF/error
            return;
        }

        // check for empty input and re-prompt
        if (buf[0] == '\0')
        {
            printf("Name cannot be empty.\n");
            continue;
        }

        // validate: allow alphabetic & special characters, and spaces only (letters >= 1)
        int has_letter = 0;
        int valid = 1;

        // iterate through each character in input buffer
        for (size_t i = 0; buf[i] != '\0'; ++i)
        {
            // check if character is alphabetic or a space or hyphen
            if (isalpha((unsigned char)buf[i]))
                has_letter = 1;
            else if (buf[i] == ' ' || buf[i] == '-')
                continue;
            else
            {
                valid = 0;
                break;
            }
        }

        // if name is invalid or does not contain at least one letter, re-prompt for valid input
        if (!valid || !has_letter)
        {
            printf("Name must contain only letters and spaces, and include at least one letter.\n");
            continue;
        }

        // copy name in buffer into destination string up to size-1
        strncpy(dest, buf, size - 1);
        dest[size - 1] = '\0';

        return;
    }
}

/**
 * find_student_index - locate student index by roll number
 *
 * @sys: pointer to student record system
 * @roll: roll number to search for
 *
 * Return: index of matching student or -1 if not found
 */
static int find_student_index(const StudentSystem *sys, int roll)
{
    // iterate through student records to find matching roll number
    for (int i = 0; i < sys->count; i++)
    {
        // check if current student's roll number matches the target roll number
        if (sys->students[i].roll_number == roll)
        {
            // return index of matching student
            return i;
        }
    }

    // return -1 if no matching student found
    return -1;
}
