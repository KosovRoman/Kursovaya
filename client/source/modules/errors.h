#ifndef ERRORS_H
#define ERRORS_H

#include <exception>
#include <string>

/** 
* @file errors.h
* @brief Определения классов исключений для клиентской части.
* @details Этот файл содержит определения классов для обработки различных исключений, возникающих в клиентской части программы.
* @date 20.11.2024
* @version 1.0
* @authors Косов Р. С.
* @copyright ИБСТ ПГУ
*/

/** 
* @brief Базовый класс для клиентских ошибок.
*/
class BasicClientError : public std::exception
{
public:
    /**
    * @brief Конструктор BasicClientError.
    * @param name Имя исключения.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникла ошибка.
    */
    BasicClientError(const std::string &name, const std::string &message, const std::string &func);

    /**
    * @brief Метод для получения сообщения об ошибке.
    * @return Сообщение об ошибке.
    */
    const char *what() const noexcept override;

protected:
    std::string name;  ///< Имя исключения.
    std::string func;  ///< Имя функции, в которой возникла ошибка.
    mutable std::string message;  ///< Сообщение об ошибке.
};

/** 
* @brief Исключение для ошибок ввода-вывода.
*/
class IOError : public BasicClientError
{
public:
    /**
    * @brief Конструктор IOError.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникла ошибка.
    */
    IOError(const std::string &message, const std::string &func);
};

/** 
* @brief Исключение для ошибок декодирования аргументов.
*/
class ArgsDecodeError : public BasicClientError
{
public:
    /**
    * @brief Конструктор ArgsDecodeError.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникла ошибка.
    */
    ArgsDecodeError(const std::string &message, const std::string &func);
};

/** 
* @brief Исключение для ошибок декодирования данных.
*/
class DataDecodeError : public BasicClientError
{
public:
    /**
    * @brief Конструктор DataDecodeError.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникла ошибка.
    */
    DataDecodeError(const std::string &message, const std::string &func);
};

/** 
* @brief Исключение для ошибок аутентификации.
*/
class AuthError : public BasicClientError
{
public:
    /**
    * @brief Конструктор AuthError.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникла ошибка.
    */
    AuthError(const std::string &message, const std::string &func);
};

/** 
* @brief Исключение для сетевых ошибок.
*/
class NetworkError : public BasicClientError
{
public:
    /**
    * @brief Конструктор NetworkError.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникла ошибка.
    */
    NetworkError(const std::string &message, const std::string &func);
};

#endif // ERRORS_H
