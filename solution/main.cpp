#include "matrix.hpp"
#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace matrix_interpreter {

int static check_register(std::string &token) {
    if (token.length() != 2) {
        throw std::runtime_error("'" + token + "'" + " is not a register");
    }
    const std::string register_number = token.substr(1, 1);
    const std::string numbers = "0123456789";
    if (numbers.find(register_number) != std::string::npos) {
        return static_cast<int>(numbers.find(register_number));
    } else {
        throw std::runtime_error("'" + token + "'" + " is not a register");
    }
}

int check_number(const std::string &token) {
    const std::string digits = "-+0123456789";
    if (token.find_first_not_of(digits) == std::string::npos) {
        return std::stoi(token);
    } else {
        throw std::runtime_error("Invalid command format");
    }
}

void load_matrix(
    matrix_interpreter::Matrix &matrix,
    int n,
    int m,
    std::ifstream &is
) {
    int element = 0;
    for (int number = 0; number < n * m; number++) {
        if (is >> element) {
            matrix.set_element(number / m, number % m, element);
        } else {
            if (is.eof()) {
                throw std::runtime_error("Invalid file format");
            }
        }
    }
}

bool check_args_size(const std::string &command, int args_size) {
    if ((command == "load" and args_size != 3) or
        (command == "print" and args_size != 2) or
        (command == "elem" and args_size != 4) or
        (command == "add" and args_size != 3) or
        (command == "mul" and args_size != 3)) {
        throw std::runtime_error("Invalid command format");
    } else {
        return true;
    }
}

void execution(
    std::vector<std::string> &tokens,
    std::vector<matrix_interpreter::Matrix> &registers
) {
    check_args_size(tokens[0], static_cast<int>(tokens.size()));
    if (tokens[0] == "exit") {
        // NOLINTNEXTLINE(concurrency-mt-unsafe,-warnings-as-errors)
        exit(0);
    } else if (tokens[0] == "load") {
        const int register_number = check_register(tokens[1]);
        const std::filesystem::path filename(tokens[2]);
        std::ifstream is(filename);
        if (!is) {
            const std::string output =
                "Unable to open file '" + tokens[2] + "'";
            throw std::runtime_error(output);
        }
        int n = 0;
        int m = 0;
        if (!(is >> n >> m)) {
            throw std::runtime_error("Invalid file format");
        }
        if (n == 0 && m == 0) {
            const Matrix matrix;
            registers[register_number] = matrix;
        } else {
            Matrix matrix(
                n, m, std::vector<std::vector<int>>(n, std::vector<int>(m, 0))
            );
            load_matrix(matrix, n, m, is);
            registers[register_number] = matrix;
        }
    } else if (tokens[0] == "print") {
        const int register_number = check_register(tokens[1]);
        for (size_t i = 0; i < registers[register_number].number_of_rows();
             i++) {
            for (size_t j = 0;
                 j < registers[register_number].number_of_columns(); j++) {
                std::cout << registers[register_number].get_element(
                    static_cast<int>(i), static_cast<int>(j)
                );
                if (j < registers[register_number].number_of_columns() - 1) {
                    std::cout << " ";
                }
            }
            std::cout << "\n";
        }
    } else if (tokens[0] == "elem") {
        const int register_number = check_register(tokens[1]);
        const int i = check_number(tokens[2]);
        const int j = check_number(tokens[3]);
        std::cout << registers[register_number].get_element(i, j) << "\n";
    } else if (tokens[0] == "add") {
        const int register_number_A = check_register(tokens[1]);
        const int register_number_B = check_register(tokens[2]);
        registers[register_number_A].add(registers[register_number_B]);

    } else if (tokens[0] == "mul") {
        const int register_number_A = check_register(tokens[1]);
        const int register_number_B = check_register(tokens[2]);
        registers[register_number_A] =
            registers[register_number_A].multiply(registers[register_number_B]);
    } else {
        throw std::runtime_error("Unknown command: '" + tokens[0] + "'");
    }
}
}  // namespace matrix_interpreter

int main() {
#ifdef _MSC_VER
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif
    std::vector<matrix_interpreter::Matrix> all_registers(10);
    std::string command = {};
    while (std::getline(std::cin, command)) {
        std::stringstream sstream(command);
        std::vector<std::string> tokens;
        std::string token;
        while (sstream >> token) {
            tokens.push_back(token);
        }
        try {
            matrix_interpreter::execution(tokens, all_registers);
        } catch (const std::bad_alloc &) {
            std::cout << "Unable to allocate memory\n";
        } catch (const std::exception &e) {
            std::cout << e.what() << "\n";
        }
    }
    return 0;
}
