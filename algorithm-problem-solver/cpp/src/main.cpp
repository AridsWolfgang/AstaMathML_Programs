#include <iostream>
#include <string>
#include <cctype>

// Function declarations for each module
void runSortingModule();
void runSearchingModule();
void runRecursionModule();
void runDPModule();
void runGreedyModule();
void runGraphModule();
void runDataStructsModule();

void clearInput()
{
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}

char tolowerInput(char c)
{
    return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
}

void showMainMenu()
{
    std::cout << "\n";
    std::cout << "  ========================================\n";
    std::cout << "     ALGORITHM & DATA STRUCTURES SOLVER   \n";
    std::cout << "  ========================================\n";
    std::cout << "  Learn by running, reading, and tracing!\n";
    std::cout << "  ----------------------------------------\n";
    std::cout << "  1) Sorting Algorithms\n";
    std::cout << "  2) Searching Algorithms\n";
    std::cout << "  3) Recursion\n";
    std::cout << "  4) Dynamic Programming\n";
    std::cout << "  5) Greedy Algorithms\n";
    std::cout << "  6) Graph Algorithms\n";
    std::cout << "  7) Data Structures\n";
    std::cout << "  Q) Quit\n";
    std::cout << "  ----------------------------------------\n";
    std::cout << "  Enter your choice: ";
}

int main()
{
    char choice;

    do {
        showMainMenu();
        std::cin >> choice;
        choice = tolowerInput(choice);

        switch (choice) {
            case '1': runSortingModule();       break;
            case '2': runSearchingModule();     break;
            case '3': runRecursionModule();     break;
            case '4': runDPModule();            break;
            case '5': runGreedyModule();        break;
            case '6': runGraphModule();         break;
            case '7': runDataStructsModule();   break;
            case 'q':
                std::cout << "  Goodbye! Keep learning.\n";
                break;
            default:
                std::cout << "  Invalid choice. Try again.\n";
                break;
        }
    } while (choice != 'q');

    return 0;
}
