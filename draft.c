#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100 // Maximum number of students allowed

// Structure to store student information
struct Student
{
    char name[50];
    int roll;
    float marks;
};

// Function prototypes
void addStudent(struct Student students[], int *count);
void displayStudents(struct Student students[], int count);
void modifyStudent(struct Student students[], int count);
void removeStudent(struct Student students[], int *count);

int main()
{
    char username[50];
    struct Student students[MAX_STUDENTS];
    int count = 0; // Number of students stored
    int choice;

    // Welcome Page
    printf("WELCOME TO MIVA OPEN UNIVERSITY COS 201 PROJECT\n");
    printf("Enter your name: ");
    scanf("%s", username);
    fgets(students[count].name, 50, stdin);
    username[strlen(username) - 1] = '\0';
    printf("Hello, %s! Welcome to the Student Record System.\n", username);

    while (1)
    {
        // Menu
        printf("\n===== STUDENT RECORD MENU =====\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Modify Student\n");
        printf("4. Remove Student\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addStudent(students, &count);
            break;

        case 2:
            displayStudents(students, count);
            break;

        case 3:
            modifyStudent(students, count);
            break;

        case 4:
            removeStudent(students, &count);
            break;

        case 5:
            printf("Exiting... Goodbye!\n");
            return 0;

        default:
            printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

// FUNCTION TO ADD STUDENT
void addStudent(struct Student students[], int *count)
{
    if (*count >= MAX_STUDENTS)
    {
        printf("Cannot add more students. Storage is full!\n");
        return;
    }

    printf("Enter Student Name: ");
    scanf("%s", students[*count].name);

    printf("Enter Roll Number: ");
    scanf("%d", &students[*count].roll);

    printf("Enter Marks: ");
    scanf("%f", &students[*count].marks);

    // Pass/Fail Logic
    if (students[*count].marks >= 40)
        printf("Status: PASS\n");
    else
        printf("Status: FAIL\n");

    (*count)++;
    printf("Student added successfully!\n");
}

// FUNCTION TO DISPLAY ALL STUDENTS
void displayStudents(struct Student students[], int count)
{
    if (count == 0)
    {
        printf("No student records available.\n");
        return;
    }

    printf("\n=== LIST OF STUDENTS ===\n");
    for (int i = 0; i < count; i++)
    {
        printf("Student %d:\n", i + 1);
        printf("Name: %s\n", students[i].name);
        printf("Roll Number: %d\n", students[i].roll);
        printf("Marks: %.2f\n", students[i].marks);
        printf("----------------------------\n");
    }
}

// FUNCTION TO MODIFY STUDENT DETAILS
void modifyStudent(struct Student students[], int count)
{
    int roll, found = 0;

    printf("Enter roll number of student to modify: ");
    scanf("%d", &roll);

    for (int i = 0; i < count; i++)
    {
        if (students[i].roll == roll)
        {
            found = 1;

            printf("Enter New Name: ");
            scanf("%s", students[i].name);

            printf("Enter New Marks: ");
            scanf("%f", &students[i].marks);

            printf("Record updated successfully!\n");
            return;
        }
    }

    if (!found)
        printf("Student with roll number %d not found!\n", roll);
}

// FUNCTION TO REMOVE STUDENT
void removeStudent(struct Student students[], int *count)
{
    int roll, index = -1;

    printf("Enter roll number to remove: ");
    scanf("%d", &roll);

    for (int i = 0; i < *count; i++)
    {
        if (students[i].roll == roll)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("Student not found!\n");
        return;
    }

    // Shift the remaining students left
    for (int i = index; i < *count - 1; i++)
    {
        students[i] = students[i + 1];
    }

    (*count)--;
    printf("Student removed successfully!\n");
}
