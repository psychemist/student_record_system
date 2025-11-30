#include <stdio.h>
#include "student.h"

int main()
{
    // create and initialize student record system
    StudentSystem sys;
    init_system(&sys);

    // welcome user (prompt for name and print welcome message)
    greet_user();

    // main menu loop
    int choice;
    do
    {
        // get user menu choice
        choice = get_menu_choice();

        // handle user menu choice
        switch (choice)
        {
        case 1:
            add_student(&sys);
            break;
        case 2:
            modify_student(&sys);
            break;
        case 3:
            remove_student(&sys);
            break;
        case 4:
            search_student(&sys);
            break;
        case 5:
            display_students(&sys);
            break;
        case 6:
            sort_students(&sys);
            break;
        case 7:
            calculate_average(&sys);
            break;
        case 8:
            verify_marks(-1.0);
            break;
        case 9:
            save_to_file(&sys);
            break;
        case 10:
            load_from_file(&sys);
            break;
        case 0:
            printf("Exiting program... Goodbye!\n");
            break;
        default:
            printf("Invalid choice! Please try again.\n");
            break;
        }
    } while (choice != 0);

    // free allocated memory before exiting
    free_system(&sys);

    // program end
    return 0;
}