#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <string>
#include <vector>
#include <cstdint>

/** 
* @file network.h
* @brief Определения классов для управления сетевым взаимодействием.
* @details Этот файл содержит определения классов для управления сетевыми подключениями и передачей данных.
* @date 20.11.2024
* @version 1.0
* @authors Косов Р. С.
* @copyright ИБСТ ПГУ
*/

/** 
* @brief Класс для управления сетевым подключением и взаимодействием.
*/
class NetworkManager
{
public:
    /**
    * @brief Конструктор класса NetworkManager.
    * @param address Адрес сервера.
    * @param port Порт сервера.
    */
    NetworkManager(const std::string &address, uint16_t port);

    /**
    * @brief Метод для получения адреса сервера.
    * @return Адрес сервера.
    */
    std::string &getAddress();

    /**
    * @brief Метод для получения порта сервера.
    * @return Порт сервера.
    */
    uint16_t &getPort();

    /**
    * @brief Метод для установления сетевого подключения.
    * @throw NetworkError Если не удалось создать сокет или установить соединение.
    */
    void conn();

    /**
    * @brief Метод для аутентификации пользователя.
    * @param username Имя пользователя.
    * @param password Пароль.
    * @throw AuthError Если не удалось отправить сообщение об аутентификации или аутентификация не удалась.
    */
    void auth(const std::string &username, const std::string &password);

    /**
    * @brief Метод для передачи данных и получения результата.
    * @param data Данные для обработки.
    * @return Результаты обработки данных.
    * @throw NetworkError Если не удалось отправить или получить данные.
    */
    std::vector<uint32_t> calc(const std::vector<std::vector<uint32_t>> &data);

    /**
    * @brief Метод для закрытия сетевого подключения.
    */
    void close();

private:
    int socket; ///< Сокет подключения.
    std::string address; ///< Адрес сервера.
    uint16_t port; ///< Порт сервера.
};

#endif // NETWORK_MANAGER_H
