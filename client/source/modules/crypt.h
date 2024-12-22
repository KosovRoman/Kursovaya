#ifndef CRYPT_MANAGER_H
#define CRYPT_MANAGER_H

#include <string>

/** 
* @file crypt.h
* @brief Определения классов для криптографических операций.
* @details Этот файл содержит определения классов для генерации соли и вычисления хеша данных.
* @date 20.11.2024
* @version 1.0
* @authors Косов Р. С.
* @copyright ИБСТ ПГУ
*/

/** 
* @brief Класс для выполнения криптографических операций.
*/
class CryptManager
{
public:
    /**
    * @brief Статический метод для генерации соли.
    * @return Сгенерированная соль.
    */
    static std::string get_salt();

    /**
    * @brief Статический метод для вычисления хеша.
    * @param salt Соль, используемая для хеширования.
    * @param data Данные, которые нужно захешировать.
    * @return Вычисленный хеш.
    */
    static std::string get_hash(const std::string &salt, const std::string &data);
};

#endif // CRYPT_MANAGER_H
