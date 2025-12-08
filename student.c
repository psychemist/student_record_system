#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

#define INITIAL_CAPACITY 10

/* ------------------ STUDENT RECORD SYSTEM STARTER FUNCTIONS ------------------ */

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
    char buf[8];
    char *endptr;

    // loop until we get a valid integer choice in range [0..10]
    while (1)
    {
        // print menu to display
        printf("\n===== STUDENT RECORD SYSTEM MENU =====\n");
        printf("1.  Add Student\n");
        printf("2.  Modify Student\n");
        printf("3.  Remove Student\n");
        printf("4.  Search Student\n");
        printf("5.  Display All Students\n");
        printf("6.  Sort Students\n");
        printf("7.  Calculate Average Marks\n");
        printf("8.  Verify Student Marks\n");
        printf("9.  Save to File\n");
        printf("10. Load from File\n");
        printf("0.  Exit\n");
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

/* ------------------ STUDENT RECORD SYSTEM MEMORY MANAGEMENT ------------------ */

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

/* ------------------ INTERNAL INPUT HELPER UTILITIES ------------------ */

/**
 * trim - delete leading and trailing whitespaces
 *
 * @s: pointer to string to trim
 * Return: void
 */
static void trim(char *s)
{
    char *start = s;
    char *end;

    // find first non-whitespace character
    while (*start && isspace((unsigned char)*start))
        start++;

    // if the string is all whitespaces, clear it
    if (*start == '\0')
    {
        *s = '\0';
        return;
    }

    // find last non-whitespace character
    end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end))
        end--;

    // terminate sring after the last non-space character
    *(end + 1) = '\0';

    // shift trimmed substring to the beginning if needed
    if (start != s)
        memmove(s, start, (size_t)(end - start + 2));
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
    char buf[64];
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
        if (errno == ERANGE)
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
    char buf[64];
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
 * Return: int - index of matching student or -1 if not found
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

/* ------------------ STUDENT RECORD SYSTEM CORE FEATURES ------------------ */

/**
 * add_student - add new student to record system
 *
 * @sys: pointer to student record system variable
 * Return: void
 */
void add_student(StudentSystem *sys)
{
    // increase system capacity if student records array is full
    resize_system(sys);

    // declare new student
    Student student;

    // read name into student struct using helper function
    read_name("Enter student name: ", student.name, sizeof(student.name));

    // read roll number and ensure uniqueness
    while (1)
    {
        // read roll number using helper function
        student.roll_number = read_int_range("Enter roll number: ", 0, INT_MAX);

        // check for uniqueness of roll number
        if (find_student_index(sys, student.roll_number) != -1)
        {
            // if roll number already exists, prompt for a unique one
            printf("Roll number %d already exists. Please enter a unique roll number.\n",
                   student.roll_number);
            continue;
        }
        break;
    }

    // read marks and store in student struct
    student.marks = read_float_range("Enter marks: ", 0.0f, 100.0f);

    // verify if student passed or failed based on marks entered
    verify_marks(student.marks);

    // add new student to record system and print success message
    sys->students[sys->count++] = student;
    printf("\nStudent added successfully.\n");
}

/**
 * modify_student - modify existing student in record system by roll number
 *
 * @sys: pointer to student record system variable
 * Return: void
 */
void modify_student(StudentSystem *sys)
{
    // prompt user for roll number to modify
    int roll;
    roll = read_int_range("Enter roll number to modify: ", 0, INT_MAX);

    // iterate through student records to find matching roll number
    for (int i = 0; i < sys->count; i++)
    {
        // if roll number matches, prompt for new details
        if (sys->students[i].roll_number == roll)
        {
            printf("Modifying student: %s\n", sys->students[i].name);

            // read new roll number and ensure uniqueness
            int new_roll;
            while (1)
            {
                // read new roll number using helper function
                new_roll = read_int_range("Enter new roll number (re-enter current to keep): ", 0, INT_MAX);

                // verify that new roll number is unique
                if (new_roll != sys->students[i].roll_number &&
                    find_student_index(sys, new_roll) != -1)
                {
                    // if not unique, prompt for different roll number
                    printf("Roll number %d already exists. Please choose a different roll number.\n",
                           new_roll);
                    continue;
                }
                break;
            }

            // update student record with new roll number
            sys->students[i].roll_number = new_roll;

            // read new name using helper function
            read_name("Enter new name: ", sys->students[i].name, sizeof(sys->students[i].name));

            // read new marks and verify pass/fail status
            sys->students[i].marks = read_float_range("Enter new marks: ", 0.0f, 100.0f);
            verify_marks(sys->students[i].marks);

            // print success message and return early
            printf("\nStudent record updated.\n");
            return;
        }
    }

    // if no matching roll number found, print error message
    printf("Student with roll number %d not found.\n", roll);
}

/**
 * remove_student - remove student from record system by roll number
 *
 * @sys: pointer to student record system variable
 * Return: void
 */
void remove_student(StudentSystem *sys)
{
    int roll, index = -1;

    // prompt user for roll number to remove
    roll = read_int_range("Enter roll number to remove: ", 0, INT_MAX);

    // search for student with matching roll number
    for (int i = 0; i < sys->count; i++)
    {
        // if roll number matches, store index and break out of loop
        if (sys->students[i].roll_number == roll)
        {
            index = i;
            break;
        }
    }

    // if found, reorder student records array to remove student
    if (index != -1)
    {
        // iterate over records and shift elements left to fill gap
        for (int i = index; i < sys->count - 1; i++)
        {
            sys->students[i] = sys->students[i + 1];
        }
        // decrease student count and print success message
        sys->count--;
        printf("\nStudent removed successfully.\n");
    }
    // else print not found message
    else
    {
        printf("Student with roll number %d not found.\n", roll);
    }
}

/* ------------------ STUDENT RECORD SYSTEM ADDITIONAL FEATURES ------------------ */

/**
 * search_student - search for student by roll number and display details
 *
 * @sys: pointer to student record system variable
 * Return: void
 */
void search_student(const StudentSystem *sys)
{
    // prompt user for roll number to search
    int roll;
    roll = read_int_range("Enter roll number to search: ", 0, INT_MAX);

    // iterate through student records to find matching roll number
    for (int i = 0; i < sys->count; i++)
    {
        // if roll number matches, display student details and return early
        if (sys->students[i].roll_number == roll)
        {
            printf("\nStudent Found:\n");
            printf("Name: %s\n", sys->students[i].name);
            printf("Roll No: %d\n", sys->students[i].roll_number);
            printf("Marks: %.2f\n", sys->students[i].marks);
            printf("Status: %s\n", (sys->students[i].marks >= 40.0f) ? "PASS" : "FAIL");
            return;
        }
    }

    // if no matching roll number found, print not found message
    printf("Student with roll number %d not found.\n", roll);
}

/**
 * display_students - display all student records in tabular format
 *
 * @sys: pointer to student record system variable
 * Return: void
 */
void display_students(const StudentSystem *sys)
{
    // if no records, print 404 message and return
    if (sys->count == 0)
    {
        printf("No records found.\n");
        return;
    }

    // print table header
    printf("\n====================== STUDENT RECORDS =======================\n");
    printf("| %-5s | %-20s | %-7s | %-8s | %-5s |\n", "Count", "Name", "Roll No", "Marks", "Status");
    printf("|------------------------------------------------------------|\n");

    // iterate through student records and print each student in formatted row
    for (int i = 0; i < sys->count; i++)
    {
        printf("| %-5d | %-20s | %-7d | %-8.2f | %-5s  |\n", i + 1,
               sys->students[i].name, sys->students[i].roll_number, sys->students[i].marks,
               (sys->students[i].marks >= 40.0f) ? "PASS" : "FAIL");
    }
    printf("|____________________________________________________________|\n");
}

/**
 * compare_asc - comparison function for ascending order sort by marks
 *
 * @a: pointer to first student
 * @b: pointer to second student
 * Return: int - comparison result
 */
int compare_asc(const void *a, const void *b)
{
    // cast void pointers to Student pointers
    Student *s1 = (Student *)a;
    Student *s2 = (Student *)b;

    // compare marks for ascending order and return result to qsort function
    if (s1->marks > s2->marks)
        return 1;
    if (s1->marks < s2->marks)
        return -1;

    // return equality
    return 0;
}

/**
 * compare_desc - comparison function for descending order sort by marks
 *
 * @a: pointer to first student
 * @b: pointer to second student
 * Return: int - comparison result
 */
int compare_desc(const void *a, const void *b)
{
    // cast void pointers to Student pointers
    Student *s1 = (Student *)a;
    Student *s2 = (Student *)b;

    // compare marks for descending order and return result to qsort function
    if (s1->marks < s2->marks)
        return 1;
    if (s1->marks > s2->marks)
        return -1;

    // marks are equal
    return 0;
}

/**
 * sort_students - sort student records by marks in ascending/descending order
 *
 * @sys: pointer to student record system variable
 * Return: void
 */
void sort_students(StudentSystem *sys)
{
    // prompt user for sort order choice and read into choice variable
    int choice;
    printf("Sort by marks:\n1. Ascending\n2. Descending\n");
    choice = read_int_range("Enter choice: ", 1, 2);

    // call qsort with appropriate comparison function based on user choice
    if (choice == 1)
    {
        // sort in ascending order
        qsort(sys->students, sys->count, sizeof(Student), compare_asc);
        printf("Sorted in ascending order.\n");
    }
    else if (choice == 2)
    {
        // sort in descending order
        qsort(sys->students, sys->count, sizeof(Student), compare_desc);
        printf("Sorted in descending order.\n");
    }
    else
    {
        // invalid choice entered
        printf("Invalid choice.\n");
    }

    // display sorted student records
    display_students(sys);
}

/**
 * calculate_average - calculate and display average marks of all students
 *
 * @sys: pointer to student record system variable
 * Return: void
 */
void calculate_average(const StudentSystem *sys)
{
    // if no students in record system, print message and return
    if (sys->count == 0)
    {
        printf("No students to calculate average.\n");
        return;
    }

    // calculate sum of marks and compute average
    float sum = 0;
    for (int i = 0; i < sys->count; i++)
    {
        sum += sys->students[i].marks;
    }

    // print average marks to two decimal places
    printf("\nAverage Marks: %.2f\n", sum / sys->count);
}

/**
 * verify_marks - verify student marks and determine pass/fail status
 *
 * @marks: float value of student marks to verify
 * Return: void
 */
void verify_marks(float marks)
{
    float val = marks;

    // if caller passed a negative value (called from main), prompt the user for marks
    if (val < 0.0f)
    {
        val = read_float_range("Enter marks (0-100): ", 0.0f, 100.0f);
    }

    // determine pass/fail status based on marks and print result
    if (val >= 40.0f)
    {
        printf("\nResult: PASSED\n");
    }
    else
    {
        printf("\nResult: FAILED\n");
    }
}

/* ------------------ STUDENT RECORD SYSTEM FILE OPERATIONS ------------------ */

/**
 * save_to_file - save student records to a file
 *
 * @sys: pointer to student record system variable
 * Return: void
 */
void save_to_file(const StudentSystem *sys)
{
    char filename[64];

    // prompt until we get a non-empty filename or EOF (cancel)
    while (1)
    {
        // read filename from user
        if (!read_line("Enter filename to save to: ", filename, sizeof(filename)))
        {
            // EOF or error: abort save file operation
            printf("No filename provided. Aborting save.\n");
            return;
        }

        // check for empty filename and re-prompt
        if (filename[0] == '\0')
        {
            printf("Filename cannot be empty. Press Ctrl+D to cancel or enter a filename.\n");
            continue;
        }
        break;
    }

    // open file pointer for writing
    FILE *fp = fopen(filename, "w");

    // check for file open errors and abort if any
    if (fp == NULL)
    {
        printf("Error opening file '%s' for writing.\n", filename);
        return;
    }

    // write student count to file
    fprintf(fp, "%d\n", sys->count);

    // iterate through student records and write each to file
    for (int i = 0; i < sys->count; i++)
    {
        fprintf(fp, "%s\n%d\n%f\n",
                sys->students[i].name, sys->students[i].roll_number, sys->students[i].marks);
    }

    // close file pointer after writing and print success message
    fclose(fp);
    printf("\nRecords saved to %s.\n", filename);
}

/**
 * load_from_file - load student records from a file
 *
 * @sys: pointer to student record system variable
 * Return: void
 */
void load_from_file(StudentSystem *sys)
{
    char filename[64];

    // prompt until we get a non-empty filename or EOF (cancel)
    while (1)
    {
        // read filename from user
        if (!read_line("Enter filename to load from: ", filename, sizeof(filename)))
        {
            // EOF or error: abort load file operation
            printf("No filename provided. Aborting load.\n");
            return;
        }

        // check for empty filename and re-prompt
        if (filename[0] == '\0')
        {
            printf("Filename cannot be empty. Press Ctrl+D to cancel or enter a filename.\n");
            continue;
        }
        break;
    }

    // open file pointer for reading
    FILE *fp = fopen(filename, "r");

    // check for file open errors and abort if any
    if (fp == NULL)
    {
        printf("File '%s' not found or error opening file.\n", filename);
        return;
    }

    // reset current student record system count
    sys->count = 0;

    // read student count from file (first word on first line is number of records)
    int count;
    if (fscanf(fp, "%d", &count) != 1)
    {
        // invalid file format or no suitable conversion; close file pointer and exit
        fclose(fp);
        return;
    }

    // consume newline(s) after count
    char buffer[8];
    fgets(buffer, sizeof(buffer), fp);

    // read each student record from file and add to system
    for (int i = 0; i < count; i++)
    {
        // ensure system has enough capacity for new students loaded from file
        resize_system(sys);
        Student student;

        // read student details from file
        if (fgets(student.name, sizeof(student.name), fp) == NULL)
            break;
        // remove newline character from name
        student.name[strcspn(student.name, "\n")] = 0;

        // read roll number and marks from file
        if (fscanf(fp, "%d", &student.roll_number) != 1)
            break;
        if (fscanf(fp, "%f", &student.marks) != 1)
            break;

        // consume newline after marks
        fgets(buffer, sizeof(buffer), fp);

        // store loaded student to record system at count index
        sys->students[sys->count++] = student;
    }

    // close file pointer after reading and print success message
    fclose(fp);
    printf("\nRecords loaded from %s.\n", filename);
}