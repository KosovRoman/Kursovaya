#ifndef IO_MANAGER_H
#define IO_MANAGER_H
#include <cstdint>
#include <string>
#include <vector>
#include <array>
#include "errors.h"

/** 
* @file io.h
* @brief Определения классов для управления вводом-выводом.
* @details Этот файл содержит определения классов для работы с файлами ввода-вывода и конфигурационными данными.
* @date 20.11.2024
* @version 1.0
* @authors Косов Р. С.
* @copyright ИБСТ ПГУ
*/

/** 
* @brief Класс для управления вводом и выводом данных.
*/
class IOManager {
public:
    /**
    * @brief Конструктор класса IOManager.
    * @param path_to_conf Путь к файлу конфигурации.
    * @param path_to_in Путь к входному файлу.
    * @param path_to_out Путь к выходному файлу.
    */
    IOManager(
        const std::string& path_to_conf, 
        const std::string& path_to_in, 
        const std::string& path_to_out
    );

    /**
    * @brief Метод для чтения конфигурационных данных.
    * @return Массив строк с конфигурационными данными.
    * @throw IOError Если не удалось открыть файл конфигурации.
    * @throw DataDecodeError Если отсутствуют логин или пароль.
    */
    std::array<std::string, 2> conf();

    /**
    * @brief Метод для чтения данных из файла.
    * @return Двумерный вектор с данными.
    * @throw IOError Если не удалось открыть входной файл для чтения.
    */
    std::vector<std::vector<uint32_t>> read();

    /**
    * @brief Метод для записи данных в файл.
    * @param data Вектор данных для записи.
    * @throw IOError Если не удалось открыть выходной файл для записи.
    */
    void write(const std::vector<uint32_t>& data);

private:
    std::string path_to_conf; ///< Путь к файлу конфигурации.
    std::string path_to_in; ///< Путь к входному файлу.
    std::string path_to_out; ///< Путь к выходному файлу.
};

#endif // IO_MANAGER_H
