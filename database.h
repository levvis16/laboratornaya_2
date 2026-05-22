#ifndef DATABASE_H
#define DATABASE_H

#include <pqxx/pqxx>
#include <iostream>
#include <string>
#include <fstream>

/** 
 * @file database.h
 * @brief Класс для работы с базой данных PostgreSQL
 */


class Database {
private:
    pqxx::connection* conn;
    std::string adminSecret;
    
public:
    Database() {
        std::string dbname = "distance_system";
        std::string user = "postgres";
        std::string password = "Htmlpagelev_2007";
        std::string host = "localhost";
        std::string conn_string = "host=" + host + " dbname=" + dbname + " user=" + user + " password=" + password;
        conn = new pqxx::connection(conn_string);
        if (conn->is_open()) {
            std::cout << "Подключено к БД" << std::endl;
        }
        
        std::ifstream envFile(".env");
        std::string line;
        while (getline(envFile, line)) {
            if (line.find("SECRET_NAME=") == 0) {
                adminSecret = line.substr(12);
            }
        }
        envFile.close();
    }
    
    ~Database() {
        delete conn;
    }
    /**
             * @brief Деструктор
             * 
             * Закрывает соединение с базой данных
             */
    
    pqxx::connection* getConnection() {
        return conn;
    }
    /**
     * @brief Получить указатель на соединение
     * @return pqxx::connection* 
     */
    
    
    bool checkAdmin(std::string secret)
    
    {
        return secret == adminSecret;
    }
    /**
     * @brief Проверка секретного ключа администратора
     * @param secret Секретный ключ, введенный пользователем
     * @return true если ключ совпадает с SECRET_NAME из .env, иначе false
     */
    bool verifyTeacher(std::string login, std::string password) {
        try {
            pqxx::work txn(*conn);
            std::string sql = "SELECT COUNT(*) FROM teachers WHERE login = '" + login + "' AND password = '" + password + "'";
            pqxx::result r = txn.exec(sql);
            txn.commit();
            return r[0][0].as<int>() > 0;
        } catch(...) {
            return false;
        }
    }
        /**
     * @brief Авторизация преподавателя
     * @param login Логин
     * @param password Пароль
     * @return true если такой преподаватель существует в БД
     */
    bool verifyStudent(std::string login, std::string password) {
        try {
            pqxx::work txn(*conn);
            std::string sql = "SELECT COUNT(*) FROM students WHERE login = '" + login + "' AND password = '" + password + "'";
            pqxx::result r = txn.exec(sql);
            txn.commit();
            return r[0][0].as<int>() > 0;
        } catch(...) {
            return false;
        }
    }
    
    int getStudentId(std::string login) {
        try {
            pqxx::work txn(*conn);
            std::string sql = "SELECT id FROM students WHERE login = '" + login + "'";
            pqxx::result r = txn.exec(sql);
            txn.commit();
            return r[0][0].as<int>();
        } catch(...) {
            return -1;
        }
    }
    
    std::string getStudentName(int id) {
        try {
            pqxx::work txn(*conn);
            std::string sql = "SELECT full_name FROM students WHERE id = " + std::to_string(id);
            pqxx::result r = txn.exec(sql);
            txn.commit();
            return r[0][0].as<std::string>();
        } catch(...) {
            return "";
        }
    }
    
    std::string getTeacherGroup(std::string login) {
        try {
            pqxx::work txn(*conn);
            std::string sql = "SELECT group_name FROM teachers WHERE login = '" + login + "'";
            pqxx::result r = txn.exec(sql);
            txn.commit();
            return r[0][0].as<std::string>();
        } catch(...) {
            return "";
        }
    }
    
    std::string getTeacherSubject(std::string login) {
        try {
            pqxx::work txn(*conn);
            std::string sql = "SELECT subject FROM teachers WHERE login = '" + login + "'";
            pqxx::result r = txn.exec(sql);
            txn.commit();
            return r[0][0].as<std::string>();
        } catch(...) {
            return "";
        }
    }
};

#endif

/**
 * @class Database
 * @brief Управляет подключением к БД и проверкой пользователей
 */