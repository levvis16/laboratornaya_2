#include <iostream>
#include <string>

/**
 * @file main.cpp
 * @brief Точка входа в программу "Система учета успеваемости"
 */

/**
 * @brief Главная функция
 * 
 * Выводит меню выбора режима и вызывает соответствующие функции:
 * - 1: studentMode()
 * - 2: prepodMode() 
 * - 3: adminMode()
 * - 4: Выход
 * 
 * @return 0 при успешном завершении
 */

void studentMode();
void prepodMode();
void adminMode();

int main() {
    int choice;
    
    while (true) {
        std::cout << "\n система учета успеваемости " << std::endl;
        std::cout << "1. режим студента" << std::endl;
        std::cout << "2. режим преподавателя" << std::endl;
        std::cout << "3. режим администратора" << std::endl;
        std::cout << "4. выход" << std::endl;
        std::cout << "выберите режим: ";
        std::cin >> choice;
        
        if (choice == 1) {
            studentMode();
        } else if (choice == 2) {
            prepodMode();
        } else if (choice == 3) {
            adminMode();
        } else if (choice == 4) {
            std::cout << "пока" << std::endl;
            break;
        } else {
            std::cout << "неверный выбор" << std::endl;
        }
    }
    
    return 0;
}