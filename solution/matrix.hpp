#ifndef MYTEST_HPP
#define MYTEST_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace matrix_interpreter {

struct Matrix {
private:
    int rows_;
    int columns_;
    std::vector<std::vector<int>> matrix_;

public:
    Matrix() : rows_(0), columns_(0) {
    }

    Matrix(int n, int m, std::vector<std::vector<int>> matrix)
        : rows_(n), columns_(m), matrix_(std::move(matrix)) {
    }

    Matrix(const Matrix &other) = default;

    Matrix &operator=(const Matrix &other);

    Matrix(Matrix &&other) noexcept
        : rows_(other.rows_),
          columns_(other.columns_),
          matrix_(std::move(other.matrix_)) {
        other.rows_ = 0;
        other.columns_ = 0;
    }

    Matrix &operator=(Matrix &&other) noexcept;

    ~Matrix() = default;

    [[nodiscard]] bool check_element(int i, int j) const;

    void set_element(int i, int j, int value);

    [[nodiscard]] int get_element(int i, int j) const;

    [[nodiscard]] size_t number_of_rows() const {
        return rows_;
    }

    [[nodiscard]] size_t number_of_columns() const {
        return columns_;
    }

    void add(const Matrix &other);

    [[nodiscard]] Matrix multiply(const Matrix &other) const;
};

}  // namespace matrix_interpreter

#endif  // MYTEST_HPP
