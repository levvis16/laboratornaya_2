#include "database.h"
#include <iostream>
#include <string>

void prepodMode() {
    Database db;
    std::string login, password;
    
    std::cout << "Режим преподавателя" << std::endl;
    std::cout << "Введите логин: ";
    std::cin >> login;
    std::cout << "Введите пароль: ";
    std::cin >> password;
    
    if (!db.verifyTeacher(login, password)) {
        std::cout << "Неверный логин или пароль" << std::endl;
        return;
    }
    
    std::string groupName = db.getTeacherGroup(login);
    std::string subject = db.getTeacherSubject(login);
    
    std::cout << "Ваша группа: " << groupName << std::endl;
    std::cout << "Ваш предмет: " << subject << std::endl;
    
    try {
        pqxx::connection* conn = db.getConnection();
        
        pqxx::work txn(*conn);
        std::string sql = "SELECT id, full_name FROM students WHERE group_name = '" + groupName + "'";
        pqxx::result r = txn.exec(sql);
        txn.commit();
        
        if (r.size() == 0) {
            std::cout << "В группе нет студентов" << std::endl;
            return;
        }
        
        std::cout << "Студенты группы:" << std::endl;
        for (int i = 0; i < r.size(); i++) {
            std::cout << i + 1 << ". " << r[i][1].as<std::string>() << " (ID: " << r[i][0].as<int>() << ")" << std::endl;
        }
        
        int choice;
        std::cout << "Выберите студента (номер): ";
        std::cin >> choice;
        
        if (choice < 1 || choice > r.size()) {
            std::cout << "Неверный выбор" << std::endl;
            return;
        }
        
        int studentId = r[choice - 1][0].as<int>();
        std::string studentName = r[choice - 1][1].as<std::string>();
        
        int grade;
        std::cout << "Выберите оценку (2-5): ";
        std::cin >> grade;
        
        if (grade < 2 || grade > 5) {
            std::cout << "Неверная оценка" << std::endl;
            return;
        }
        
        pqxx::work txn2(*conn);
        std::string insertSql = "INSERT INTO grades (student_id, subject, grade) VALUES (" + 
                                std::to_string(studentId) + ", '" + subject + "', " + std::to_string(grade) + ")";
        txn2.exec(insertSql);
        txn2.commit();
        
        std::cout << "Оценка " << grade << " студенту " << studentName << " по предмету " << subject << " выставлена" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }
}