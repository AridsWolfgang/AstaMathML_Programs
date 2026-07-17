/*
 * ===================================================================
 *  CUSTOM MATH LIBRARY  —  Interactive Demo
 * ===================================================================
 *
 *  Every function here is implemented from first principles:
 *  Taylor series, Newton's method, continued fractions, etc.
 *
 *  We do NOT use <math.h> for our implementations. We only use
 *  it for comparison (to show how close we get to the real thing).
 * ===================================================================
 */

#include <stdio.h>
#include <ctype.h>

void run_constants_module(void);
void run_trig_module(void);
void run_exp_log_module(void);
void run_roots_module(void);
void run_linalg_module(void);
void run_stats_module(void);
void run_special_module(void);

void clear_input(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

static void show_menu(void)
{
    printf("\n");
    printf("  ========================================\n");
    printf("         CUSTOM MATH LIBRARY  (C)         \n");
    printf("  ========================================\n");
    printf("  Math from first principles — no cheating!\n");
    printf("  ----------------------------------------\n");
    printf("  1) Constants (Pi, E, Phi)\n");
    printf("  2) Trigonometry (sin, cos, tan)\n");
    printf("  3) Exponentials & Logs\n");
    printf("  4) Roots & Powers\n");
    printf("  5) Linear Algebra\n");
    printf("  6) Statistics\n");
    printf("  7) Special Functions\n");
    printf("  Q) Quit\n");
    printf("  ----------------------------------------\n");
    printf("  Choice: ");
}

int main(void)
{
    char choice;

    do {
        show_menu();
        choice = (char)getchar();
        clear_input();
        choice = (char)tolower((unsigned char)choice);

        switch (choice) {
            case '1': run_constants_module();  break;
            case '2': run_trig_module();       break;
            case '3': run_exp_log_module();    break;
            case '4': run_roots_module();      break;
            case '5': run_linalg_module();     break;
            case '6': run_stats_module();      break;
            case '7': run_special_module();    break;
            case 'q':
                printf("  Goodbye! Keep calculating.\n");
                break;
            default:
                printf("  Invalid choice.\n");
                break;
        }
    } while (choice != 'q');

    return 0;
}
