#include "database.h"
#include <iostream>
#include <string>

/**
 * @file admin.cpp
 * @brief Реализация режима администратора
 */

void adminMode() {
    Database db;
    
    std::cout << "Режим администратора" << std::endl;
    
    int choice;
    while (true) {
        std::cout << "\n--- Администратор ---"<< std::endl;
        std::cout << "1. Добавить студента" << std::endl;
        std::cout << "2. Добавить преподавателя" << std::endl;
        std::cout << "3. Просмотреть всех студентов" << std::endl;
        std::cout << "4. Просмотреть всех преподавателей" << std::endl;
        std::cout << "5. Выйти" << std::endl;
        std::cout << "Выбор: ";
        std::cin >> choice;
        
        if (choice == 5) break;
        
        try {
            pqxx::connection* conn = db.getConnection();
            
            if (choice == 1) {
                std::string name, group, login, password;
                std::cout << "ФИО: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                std::cout << "Группа: ";
                std::getline(std::cin, group);
                std::cout << "Логин: ";
                std::getline(std::cin, login);
                std::cout << "Пароль: ";
                std::getline(std::cin, password);
                
                pqxx::work txn(*conn);
                std::string sql = "INSERT INTO students (full_name, group_name, login, password) VALUES ('" + 
                                  name + "', '" + group + "', '" + login + "', '" + password + "')";
                txn.exec(sql);
                txn.commit();
                std::cout << "Студент добавлен" << std::endl;
            }
            else if (choice == 2) {
                std::string name, subject, group, login, password;
                std::cout << "ФИО: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                std::cout << "Предмет: ";
                std::getline(std::cin, subject);
                std::cout << "Группа: ";
                std::getline(std::cin, group);
                std::cout << "Логин: ";
                std::getline(std::cin, login);
                std::cout << "Пароль: ";
                std::getline(std::cin, password);
                
                pqxx::work txn(*conn);
                std::string sql = "INSERT INTO teachers (full_name, subject, group_name, login, password) VALUES ('" + 
                                  name + "', '" + subject + "', '" + group + "', '" + login + "', '" + password + "')";
                txn.exec(sql);
                txn.commit();
                std::cout << "Преподаватель добавлен" << std::endl;
            }
            else if (choice == 3) {
                pqxx::work txn(*conn);
                pqxx::result r = txn.exec("SELECT id, full_name, group_name FROM students ORDER BY id");
                txn.commit();
                
                if (r.size() == 0) {
                    std::cout << "Студентов нет" << std::endl;
                } else {
                    std::cout << "ID | ФИО | Группа" << std::endl;
                    for (int i = 0; i < r.size(); i++) {
                        std::cout << r[i][0].as<int>() << " | " 
                                  << r[i][1].as<std::string>() << " | " 
                                  << r[i][2].as<std::string>() << std::endl;
                    }
                }
            }
            else if (choice == 4) {
                pqxx::work txn(*conn);
                pqxx::result r = txn.exec("SELECT id, full_name, subject, group_name FROM teachers ORDER BY id");
                txn.commit();
                
                if (r.size() == 0) {
                    std::cout << "Преподавателей нет" << std::endl;
                      }
                else {
                  std::cout << "ID | ФИО | Предмет | Группа" << std::endl;
                    for (int i = 0; i < r.size(); i++) {
                        std::cout << r[i][0].as<int>() << " | " 
                                  << r[i][1].as<std::string>() << " | " 
                                  << r[i][2].as<std::string>() << " | "
                                  << r[i][3].as<std::string>() << std::endl;
                    }
                }
            }
                else {
                std::cout << "Неверный выбор" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
        }
    }
}

/**
 * @brief Главная функция режима администратора
 * 
 * Проверяет секретный ключ через Database::checkAdmin(),
 * затем в цикле показывает меню: добавление студента,
 * добавление преподавателя, просмотр списков.
 */