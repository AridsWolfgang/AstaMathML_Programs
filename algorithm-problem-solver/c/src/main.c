/*
 * ===================================================================
 *  ALGORITHM & DATA STRUCTURES SOLVER  (C Language Edition)
 * ===================================================================
 *
 *  Why C? Because C gives you:
 *    - No hidden memory allocation (every malloc has a free)
 *    - No operator overloading or templates — you see every detail
 *    - Direct pointer arithmetic and manual control
 *    - The language that Unix, Linux, Python, and most embedded
 *      systems are built on
 *
 *  Each algorithm is implemented with:
 *    - Real-world use cases explained in comments
 *    - ASCII illustrations of how it works
 *    - Step-by-step trace output
 *    - Explicit memory management (malloc/free)
 * ===================================================================
 */

#include <stdio.h>
#include <ctype.h>

/* Function declarations for each module */
void run_sorting_module(void);
void run_searching_module(void);
void run_recursion_module(void);
void run_dp_module(void);
void run_greedy_module(void);
void run_graph_module(void);
void run_datastructs_module(void);

/* Clears stdin buffer */
void clear_input(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

static void show_main_menu(void)
{
    printf("\n");
    printf("  ========================================\n");
    printf("     ALGORITHM & DATA STRUCTURES SOLVER   \n");
    printf("                  (C Edition)             \n");
    printf("  ========================================\n");
    printf("  Learn by running, reading, and tracing!\n");
    printf("  ----------------------------------------\n");
    printf("  1) Sorting Algorithms\n");
    printf("  2) Searching Algorithms\n");
    printf("  3) Recursion\n");
    printf("  4) Dynamic Programming\n");
    printf("  5) Greedy Algorithms\n");
    printf("  6) Graph Algorithms\n");
    printf("  7) Data Structures\n");
    printf("  Q) Quit\n");
    printf("  ----------------------------------------\n");
    printf("  Choice: ");
}

int main(void)
{
    char choice;

    do {
        show_main_menu();
        choice = (char)getchar();
        clear_input();
        choice = (char)tolower((unsigned char)choice);

        switch (choice) {
            case '1': run_sorting_module();     break;
            case '2': run_searching_module();    break;
            case '3': run_recursion_module();    break;
            case '4': run_dp_module();           break;
            case '5': run_greedy_module();       break;
            case '6': run_graph_module();        break;
            case '7': run_datastructs_module();  break;
            case 'q':
                printf("  Goodbye! Keep learning.\n");
                break;
            default:
                printf("  Invalid choice. Try again.\n");
                break;
        }
    } while (choice != 'q');

    return 0;
}
