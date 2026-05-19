#include "database.h"
#include <iostream>
#include <string>

/**
 * @file student.cpp
 * @brief Реализация режима студента
 */

/**
 * @brief Главная функция режима студента
 * 
 * Запрашивает логин/пароль, проверяет их через Database::verifyStudent()
 * и выводит таблицу с оценками из таблицы grades.
 */

void studentMode() {
    Database db;
    std::string login, password;
    
    std::cout << "Режим студента" << std::endl;
    std::cout << "Введите логин: ";
    std::cin >> login;
    std::cout << "Введите пароль: ";
    std::cin >> password;
    
    if (!db.verifyStudent(login, password)) {
        std::cout << "Неверный логин или пароль" << std::endl;
        return;
    }
    
    int studentId = db.getStudentId(login);
    std::string studentName = db.getStudentName(studentId);
    
    std::cout << "Студент: " << studentName << std::endl;
    
    try {
        pqxx::connection* conn = db.getConnection();
        pqxx::work txn(*conn);
        std::string sql = "SELECT subject, grade, date FROM grades WHERE student_id = " + std::to_string(studentId);
        pqxx::result r = txn.exec(sql);
        txn.commit();
        
        if (r.size() == 0) {
            std::cout << "Оценок нет" << std::endl;
        } else {
            std::cout << "Предмет | Оценка | Дата" << std::endl;
            for (int i = 0; i < r.size(); i++) {
                std::cout << r[i][0].as<std::string>() << " | " 
                          << r[i][1].as<int>() << " | " 
                          << r[i][2].as<std::string>() << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }
}