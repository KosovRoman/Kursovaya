#include <UnitTest++/UnitTest++.h>
#include "../../client/source/modules/crypt.h"
#include "../../client/source/modules/network.h"
#include "../../client/source/modules/io.h"
#include "../../client/source/modules/ui.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

/**
 * @file main.cpp
 * @brief Тесты для модулей криптографии, сетевого взаимодействия, ввода-вывода и пользовательского интерфейса.
 * @details Этот файл содержит тесты для проверки различных компонентов программы, включая криптографические операции, сетевое взаимодействие, ввод-вывод и обработку параметров пользовательского интерфейса.
 * @date 20.11.2024
 * @version 1.0
 * @authors Косов Р. С.
 * @copyright ИБСТ ПГУ
 */

// Тест для генерации соли
TEST(GetSalt)
{
    std::string salt1 = CryptManager::get_salt();
    std::string salt2 = CryptManager::get_salt();

    // Проверяем, что соль не пустая и что две разные соли не совпадают
    CHECK(!salt1.empty());
    CHECK(!salt2.empty());
    CHECK(salt1 != salt2);
}

// Тест для вычисления хеша
TEST(GetHash)
{
    std::string salt = "A1B2C3D4E5F6G7H8";
    std::string data1 = "P@ssW0rd";
    std::string hash1 = CryptManager::get_hash(salt, data1);

    // Проверяем, что хеш не пустой
    CHECK(!hash1.empty());

    // Проверяем, что при одинаковых входных данных хеши совпадают
    std::string hash2 = CryptManager::get_hash(salt, data1);
    CHECK_EQUAL(hash1, hash2);

    // Проверяем, что при разных данных хеши не совпадают
    std::string data2 = "dr0Wss@P";
    hash2 = CryptManager::get_hash(salt, data2);
    CHECK(hash1 != hash2);
}

// Тест для конфигурации
TEST(IOManagerConf)
{
    IOManager ioManager(
        "./config/vclient.conf",
        "./input.bin", "./output.bin");
    std::array<std::string, 2> conf = ioManager.conf();

    // Проверяем, что конфигурация не пустая
    CHECK(!conf[0].empty());
    CHECK(!conf[1].empty());

    // Проверяем значения конфигурационных параметров
    CHECK_EQUAL(std::string("user"), conf[0]);
    CHECK_EQUAL(std::string("P@ssW0rd"), conf[1]);
}

// Тест для чтения
TEST(IOManagerRead)
{
    IOManager ioManager(
        "./config/vclient.conf",
        "./input.bin", "./output.bin");
    std::vector<std::vector<uint32_t>> data = ioManager.read();

    // Проверяем, что данные не пустые
    CHECK(!data.empty());
}

// Тест для записи
TEST(IOManagerWrite)
{
    IOManager ioManager(
        "./config/vclient.conf",
        "./input.bin", "./output.bin");
    std::vector<uint32_t> data = {1, 2, 3, 4, 5};

    // Пытаемся записать данные и проверяем успешность операции
    ioManager.write(data);
    // Здесь можно добавить дополнительные проверки, чтобы убедиться, что данные были успешно записаны, если такая функциональность предусмотрена.
}

// Тест для ошибки открытия конфигурационного файла
TEST(IOManagerConfFileNotFound)
{
    IOManager ioManager(
        "./non_exists_path.conf",
        "./input.bin", "./output.bin");
    CHECK_THROW(ioManager.conf(), IOError);
}

// Тест для ошибки отсутствия данных в конфигурационном файле
TEST(IOManagerConfMissingData)
{
    // Подготовка тестового конфигурационного файла без данных
    std::ofstream test_conf("./test.conf");
    test_conf << "username:";
    test_conf.close();

    IOManager ioManager(
        "./test.conf",
        "./input.bin",
        "./output.bin");
    CHECK_THROW(ioManager.conf(), DataDecodeError);

    // Удаление тестового файла после проверки
    std::remove("./test.conf");
}

// Тест для ошибки открытия входного файла
TEST(IOManagerReadFileNotFound)
{
    IOManager ioManager(
        "./config/vclient.conf",
        "./non_exists_path.bin",
        "./output.bin");
    CHECK_THROW(ioManager.read(), IOError);
}

// Тест для ошибки открытия выходного файла
TEST(IOManagerWriteFileNotFound)
{
    IOManager ioManager(
        "./config/vclient.conf", "./input.bin",
        "./non_exists_path/non_exists_path.bin");
    CHECK_THROW(ioManager.write({1, 2, 3, 4, 5}), IOError);
}

// Тест для установки соединения
TEST(NetworkManagerConnect)
{
    // Используйте допустимый адрес и порт для тестирования
    NetworkManager netManager("127.0.0.1", 33333);
    netManager.conn();

    // Проверка значений после установки соединения
    CHECK_EQUAL(std::string("127.0.0.1"), netManager.getAddress());
    CHECK_EQUAL((uint16_t)33333, netManager.getPort());

    netManager.close();
}

// Тест для аутентификации
TEST(NetworkManagerAuth)
{
    NetworkManager netManager("127.0.0.1", 33333);
    netManager.conn();
    netManager.auth("user", "P@ssW0rd");
    netManager.close();
}

// Тест для передачи данных и получения результата
TEST(NetworkManagerCalc)
{
    NetworkManager netManager("127.0.0.1", 33333);
    netManager.conn();
    netManager.auth("user", "P@ssW0rd");

    std::vector<std::vector<uint32_t>> data = {{1, 2, 3}, {4, 5, 6}};
    std::vector<uint32_t> results = netManager.calc(data);

    // Проверяем, что результаты не пустые
    CHECK(!results.empty());
    netManager.close();
}

// Тест для ошибки соединения
TEST(NetworkManagerConnError)
{
    // Используйте недопустимый адрес для тестирования ошибки
    NetworkManager netManager("256.256.256.256", 33333);
    CHECK_THROW(netManager.conn(), NetworkError);
}

// Тест для ошибки аутентификации
TEST(NetworkManagerAuthError)
{
    NetworkManager netManager("127.0.0.1", 33333);
    netManager.conn();

    // Используйте неверные учетные данные для тестирования ошибки
    CHECK_THROW(netManager.auth("invalid_user", "invalid_pass"), AuthError);
}

// Тест для закрытия соединения
TEST(NetworkManagerClose)
{
    NetworkManager netManager("127.0.0.1", 33333);
    netManager.conn();
    netManager.close();
}

// Тест для проверки корректной обработки параметров
TEST(UserInterfaceCorrectArgs)
{
    const char *argv[] = {"vclient", "-a", "192.168.0.1", "-p", "8080", "-i", "input.bin", "-o", "output.bin", "-c", "config.conf"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    UserInterface ui(argc, const_cast<char **>(argv));

    // Проверка значений после парсинга аргументов
    CHECK_EQUAL(std::string("192.168.0.1"), ui.getAddress());
    CHECK_EQUAL((uint16_t)8080, ui.getPort());
    CHECK_EQUAL(std::string("input.bin"), ui.getInputPath());
    CHECK_EQUAL(std::string("output.bin"), ui.getOutputPath());
    CHECK_EQUAL(std::string("config.conf"), ui.getConfigPath());
}

// Тест для проверки отсутствия обязательного параметра input
TEST(UserInterfaceMissingInput)
{
    const char *argv[] = {"vclient", "-a", "192.168.0.1", "-p", "8080", "-o", "output.bin", "-c", "config.conf"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeError);
}

// Тест для проверки отсутствия обязательного параметра output
TEST(UserInterfaceMissingOutput)
{
    const char *argv[] = {"vclient", "-a", "192.168.0.1", "-p", "8080", "-i", "input.bin", "-c", "config.conf"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeError);
}

// Тест для проверки отсутствия значения для параметра address
TEST(UserInterfaceMissingAddressValue)
{
    const char *argv[] = {"vclient", "-a"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeError);
}

// Тест для проверки отсутствия значения для параметра port
TEST(UserInterfaceMissingPortValue)
{
    const char *argv[] = {"vclient", "-p"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeError);
}

// Тест для проверки отсутствия значения для параметра config
TEST(UserInterfaceMissingConfigValue)
{
    const char *argv[] = {"vclient", "-c"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeError);
}

// Тест для проверки неизвестного параметра
TEST(UserInterfaceUnknownParam)
{
    const char *argv[] = {"vclient", "--unknown"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeError);
}

/**
 * @brief Класс логгера для вывода детализированной информации о тестах.
 */
class MyTestReporter : public UnitTest::TestReporter
{
public:
    /**
     * @brief Сообщает о начале выполнения теста.
     * @param test Детали теста.
     */
    void ReportTestStart(UnitTest::TestDetails const &test) override
    {
        std::cout << "Test <" << test.testName << "> started:\n";
    }

    /**
     * @brief Сообщает об окончании выполнения теста.
     * @param test Детали теста.
     * @param secondsElapsed Время выполнения теста в секундах.
     */
    void ReportTestFinish(UnitTest::TestDetails const &test, float secondsElapsed) override
    {
        std::cout << "*passed("
                  << secondsElapsed << " seconds)\n"
                  << "================================"
                  << "================================\n";
    }

    /**
     * @brief Сообщает о сбое теста.
     * @param test Детали теста.
     * @param failure Описание сбоя.
     */
    void ReportFailure(UnitTest::TestDetails const &test, char const *failure) override
    {
        std::cout << "*failed: "
                  << " (" << failure << ")\n"
                  << "================================"
                  << "================================\n";
    }

    /**
     * @brief Сообщает об общей сводке выполнения тестов.
     * @param totalTestCount Общее количество тестов.
     * @param failedTestCount Количество неудачных тестов.
     * @param failureCount Общее количество сбоев.
     * @param secondsElapsed Общее время выполнения тестов в секундах.
     */
    void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed) override
    {
        std::cout << "Summary: "
                  << totalTestCount << " tests, "
                  << failedTestCount << " failed, "
                  << failureCount << " failures, "
                  << secondsElapsed << " seconds\n";
    }
};

/**
 * @brief Главная функция тестирования.
 * @details Инициализирует объект MyTestReporter и запускает тесты с использованием UnitTest++.
 * @param argc Количество аргументов командной строки.
 * @param argv Аргументы командной строки.
 * @return Код завершения программы. 0 - успешное завершение, 1 - ошибка.
 */
int main(int argc, char *argv[])
{
    MyTestReporter reporter;
    UnitTest::TestRunner runner(reporter);
    return runner.RunTestsIf(UnitTest::Test::GetTestList(), nullptr, UnitTest::True(), 0);
}
