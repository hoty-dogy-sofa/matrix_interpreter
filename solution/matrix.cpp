#include "matrix.hpp"

namespace matrix_interpreter {

Matrix &Matrix::operator=(const Matrix &other) {
    if (this != &other) {
        rows_ = other.rows_;
        columns_ = other.columns_;
        matrix_ = other.matrix_;
    }
    return *this;
}

Matrix &Matrix::operator=(Matrix &&other) noexcept {
    if (this != &other) {
        rows_ = other.rows_;
        columns_ = other.columns_;
        matrix_ = std::move(other.matrix_);
        other.rows_ = 0;
        other.columns_ = 0;
    }
    return *this;
}

[[nodiscard]] bool Matrix::check_element(int i, int j) const {
    if (rows_ == 0 || columns_ == 0) {
        throw std::runtime_error("Requested element is out of bounds");
    }
    if (i < 0 || i >= rows_ || j < 0 || j >= columns_) {
        throw std::runtime_error("Requested element is out of bounds");
    }
    return true;
}

void Matrix::set_element(int i, int j, int value) {
    if (check_element(i, j)) {
        matrix_[i][j] = value;
    }
}

[[nodiscard]] int Matrix::get_element(int i, int j) const {
    if (check_element(i, j)) {
        return matrix_[i][j];
    }
    return 0;
}

void Matrix::add(const Matrix &other) {
    if (rows_ != other.rows_) {
        throw std::runtime_error(
            "Dimension mismatch: lhs=" + std::to_string(rows_) +
            ", rhs=" + std::to_string(other.rows_)
        );
    } else if (columns_ != other.columns_) {
        throw std::runtime_error(
            "Dimension mismatch: lhs=" + std::to_string(columns_) +
            ", rhs=" + std::to_string(other.columns_)
        );
    } else {
        for (int i = 0; i < rows_; i++) {
            for (int j = 0; j < columns_; j++) {
                matrix_[i][j] += other.get_element(i, j);
            }
        }
    }
}

Matrix Matrix::multiply(const Matrix &other) const {
    if (columns_ != other.rows_) {
        throw std::runtime_error(
            "Dimension mismatch: lhs=" + std::to_string(columns_) +
            ", rhs=" + std::to_string(other.rows_)
        );
    }
    std::vector<std::vector<int>> result(
        rows_, std::vector<int>(other.columns_, 0)
    );
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < other.columns_; j++) {
            int new_value = 0;
            for (int k = 0; k < columns_; k++) {
                new_value += get_element(i, k) * other.get_element(k, j);
            }
            result[i][j] = new_value;
        }
    }
    return {rows_, other.columns_, result};
}
}  // namespace matrix_interpreter
