#ifndef STUDENT_H
#define STUDENT_H

// structure to store student's information
typedef struct
{
    char name[100];
    int roll_number;
    float marks;
} Student;

// structure to store student record system
typedef struct
{
    Student *students;
    int count;
    int capacity;
} StudentSystem;

// initialization and memory management
void init_system(StudentSystem *sys);
void free_system(StudentSystem *sys);

// core functionality
void add_student(StudentSystem *sys);
void modify_student(StudentSystem *sys);
void remove_student(StudentSystem *sys);

// search and display
void search_student(const StudentSystem *sys);
void display_students(const StudentSystem *sys);

// sorting and calculations
void sort_students(StudentSystem *sys);
void calculate_average(const StudentSystem *sys);
void verify_marks(float marks);

// file operations
void save_to_file(const StudentSystem *sys);
void load_from_file(StudentSystem *sys);

// utilities
void greet_user();
int get_menu_choice();

#endif // STUDENT_H
