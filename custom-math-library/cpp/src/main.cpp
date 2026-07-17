#include <iostream>
#include <cctype>

void runConstantsModule();
void runTrigModule();
void runExpLogModule();
void runRootsModule();
void runLinalgModule();
void runStatsModule();
void runSpecialModule();

int main()
{
    char choice;
    do {
        std::cout << "\n"
                  << "  ========================================\n"
                  << "     CUSTOM MATH LIBRARY  (C++ Edition)\n"
                  << "  ========================================\n"
                  << "  1) Constants (Pi, E, Phi)\n"
                  << "  2) Trigonometry (sin, cos, tan)\n"
                  << "  3) Exponentials & Logs\n"
                  << "  4) Roots & Powers\n"
                  << "  5) Linear Algebra\n"
                  << "  6) Statistics\n"
                  << "  7) Special Functions\n"
                  << "  Q) Quit\n"
                  << "  Choice: ";
        std::cin >> choice;
        choice = std::tolower(choice);

        switch (choice) {
            case '1': runConstantsModule(); break;
            case '2': runTrigModule(); break;
            case '3': runExpLogModule(); break;
            case '4': runRootsModule(); break;
            case '5': runLinalgModule(); break;
            case '6': runStatsModule(); break;
            case '7': runSpecialModule(); break;
            case 'q': std::cout << "  Goodbye!\n"; break;
            default:  std::cout << "  Invalid.\n"; break;
        }
    } while (choice != 'q');
    return 0;
}
