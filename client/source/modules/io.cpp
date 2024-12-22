#include "io.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Конструктор
IOManager::IOManager(
    const std::string &path_to_conf,
    const std::string &path_to_in,
    const std::string &path_to_out)
    : path_to_conf(path_to_conf),
      path_to_in(path_to_in),
      path_to_out(path_to_out) {}

// Метод для чтения конфигурационных данных
std::array<std::string, 2> IOManager::conf()
{
    std::ifstream conf_file(this->path_to_conf);
    if (!conf_file.is_open())
    {
        throw IOError(
            "Failed to open configuration file \"" +
                this->path_to_conf + "\"",
            "IOManager.conf()");
    }

    std::array<std::string, 2> credentials;
    std::string line;
    std::getline(conf_file, line);
    conf_file.close();

    std::istringstream iss(line);
    std::getline(iss, credentials[0], ':');
    std::getline(iss, credentials[1]);

    // Проверка на наличие обоих значений
    if (credentials[0].empty() || credentials[1].empty())
    {
        throw DataDecodeError(
            "Missing login or password",
            "IOManager.conf()");
    }

    // Логирование считанных логина и пароля
    std::cout << "Log: \"IOManager.conf()\"\n";
    std::cout << "UserData: " << credentials[0]
              << " " << credentials[1] << "\n";

    return credentials;
}

// Метод для чтения числовых данных с логированием
std::vector<std::vector<uint32_t>> IOManager::read()
{
    std::ifstream input_file(this->path_to_in, std::ios::binary);
    if (!input_file.is_open())
    {
        throw IOError("Failed to open input file for reading.", "IOManager.read()");
    }

    // Чтение количества векторов
    uint32_t num_vectors;
    input_file.read(reinterpret_cast<char *>(&num_vectors), sizeof(num_vectors));

    std::vector<std::vector<uint32_t>> data(num_vectors);

    // Чтение каждого вектора
    for (uint32_t i = 0; i < num_vectors; ++i)
    {
        // Чтение размера вектора
        uint32_t vector_size;
        input_file.read(reinterpret_cast<char *>(&vector_size), sizeof(vector_size));

        // Чтение значений вектора
        std::vector<uint32_t> vec(vector_size);
        input_file.read(reinterpret_cast<char *>(vec.data()), vector_size * sizeof(uint32_t));

        data[i] = vec;
    }

    input_file.close();

    // Логирование всех прочитанных векторов
    std::cout << "Log: IOManager.read()\n";
    std::cout << "Vectors: {";
    for (const auto &vec : data)
    {
        std::cout << "{";
        for (const auto &val : vec)
            std::cout << val << ", ";
        if (!vec.empty())
            std::cout << "\b\b";
        std::cout << "}, ";
    }
    if (!data.empty())
        std::cout << "\b\b";
    std::cout << "}\n";

    return data;
}

// Метод для записи числовых данных
void IOManager::write(const std::vector<uint32_t> &data)
{
    std::ofstream output_file(this->path_to_out, std::ios::binary);
    if (!output_file.is_open())
    {
        throw IOError(
            "Failed to open output file \"" +
                this->path_to_out + "\"",
            "IOManager.write()");
    }

    // Запись количества результатов
    uint32_t count = data.size();
    output_file.write(reinterpret_cast<const char *>(&count), sizeof(count));

    // Запись самих результатов
    for (const auto &num : data)
    {
        output_file.write(reinterpret_cast<const char *>(&num), sizeof(num));
    }
    
    output_file.close();
}
