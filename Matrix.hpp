//////////////////////////////////////////////
// 
// Your Name:  Manuel T. Gomez
// 
// CS 368, Spring 2018
// Matrix.hpp
// TODO ADD class description
//////////////////////////////////////////////

#ifndef LECTURE9_MATRIX_HPP
#define LECTURE9_MATRIX_HPP

#include <iostream>
#include <vector>
#include <stdexcept> // includes runtime_error, which is a subclass of exception

/**
 * @brief This class derives from runtime_error
 *        It is thrown in the case of Non-postive matrix dimensions
 */
class NonPositiveDimensionException : public std::runtime_error {
public:
    // constructor that calls the base class constructor in its initializer list
    NonPositiveDimensionException() : std::runtime_error("dimensions must be positive")
    {// no other code in the constructor
    }
};

/**
 * @brief This class derives from runtime_error
 *        It is thrown in the case of any dimension mismatch
 */
class DimensionMismatchException : public std::runtime_error {
public:
    DimensionMismatchException() : std::runtime_error("dimensions do not match")
    {// no other code in the constructor
    }
};

///////////////////////////////////////////////////////
// DECLARATION OF THE MATRIX TEMPLATED CLASS
// YOU WILL NEED TO ADD MORE PROTOTYPES HERE
///////////////////////////////////////////////////////
template<typename T>
class Matrix {
private:
    int rows;
    int cols;
    std::vector<std::vector<T>> data;

public:
    Matrix();
    Matrix(int r, int c);
    void print() const;

    // we need operator[] to return a modifiable L-value, that is, a non-const reference
    // so that we can store a value in a cell of a Matrix
    std::vector<T> & operator[](const int index); 

    // we need a second operator[] in order to make our operator+ below work
    // the operator+ function below needs a const Matrix as a parameter
    // thus this second opertor[] function returns a const reference
    const std::vector<T> & operator[](const int index) const; 

    // operator+ has a const reference parameter, promises not to modify this object
    // and returns a const value
    const Matrix<T> operator+(const Matrix<T> &rhs) const;
};

///////////////////////////////////////////////////////
// IMPLEMENTATION OF THE MATRIX TEMPLATED CLASS
// YOU WILL NEED TO ADD MORE FUNCTION DEFINITIONS HERE
///////////////////////////////////////////////////////

/**
 * @brief default Constructor for Matrix<T>
 *        calls the other constructor through an initializer list
 */

template<typename T>
Matrix<T>::Matrix() : Matrix(1,1)
{
}


/**
 * @brief Constructor for Matrix<T>
 *        Matrices store their data in a single vector called data
 *        We assume the matrix will use row-major ordering
 * @param r the number of rows (non-negative)
 * @param c the number of cols (non-negative)
 */
template<typename T>
Matrix<T>::Matrix(int r, int c) {
    if (r <= 0 || c <= 0) {
        throw NonPositiveDimensionException();
    }
    rows = r;
    cols = c;
    data.resize(rows);
    for (int r = 0; r < rows; ++r) {
        data[r].resize(cols);
    }
}

/**
 * @brief prints out to the terminal the elements of this matrix
 *        we assume row-major ordering of the elements
 */
template<typename T>
void Matrix<T>::print() const {
    for (auto rowit = data.begin(); rowit != data.end(); ++rowit) {
        std::vector<T> rowData = *rowit; //Pointing to each row.
        for (auto colit = rowData.begin(); colit != rowData.end(); ++colit) {
            std::cout  << *colit << " "; //Pointing to each column per row.
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/*We want both "matrix[x][y] = data" and "matrix[x][y] = mat2[x][y]" to work*/

/**
 * @brief accesses the row index of this Matrix
 *        we assume row-major ordering and a start index of 0
 *        this non-const function returns a non-const reference
 * @param index     the index which corresponds to a vector (row) in the matrix
 * @return a non-const vector reference that is suitable for a Left-value
 */
template<typename T>
std::vector<T> & Matrix<T>::operator[](const int index) {
    //By using matrix[row1][row2], we are accessing a whole row (row1), then a subrow (row2)
      //of that whole row. Equivalent to accessing elements at coordinates x,y of matrix.
    std::cout << " calling non-const operator[] with index = " << index << std::endl;
    //We don't use const because matrix[x][y] is on LHS of eqtn and it can be edited with arithmetic.
    return data[index];
}


/**
 * @brief accesses the row index of this Matrix
 *        we assume row-major ordering and a start index of 0
 * @param index, the index which corresponds to a vector (row) in the matrix
 * @return data, a const vector referene that is suitable for a Right-value
 */
template<typename T>
const std::vector<T> & Matrix<T>::operator[](const int index) const {
    std::cout << "     calling const operator[] with index = " << index << std::endl;
    //We use const because we tell compiler we wont change matrix[x][y] when its on RHS of equation.
    return data[index]; 
}

/**
 * @brief adds the Matrix<T> on the right side of the + operator to the matrix on the left
 *        side of the + sign.
 * @param rhs a const reference to the Matrix on the right of the + operator
 * @return a const Matrix that represents the sum
 */
template<typename T>
const Matrix<T> Matrix<T>::operator+(const Matrix<T> &rhs) const {
    Matrix<T> lhs = *this; //TODO findout how this worx.
    //Need the matrices to be of same size for the addition to happen.
    if (lhs.rows != rhs.rows || lhs.cols != rhs.cols) {
        throw DimensionMismatchException();
    }
    int rows = rhs.rows;
    int cols = rhs.cols;
    Matrix<T> result(rows, cols);
    //Adding the matrices by summing up all of their elements and storing them 
	//into a new matrix of an equivalent size.
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << "adding elements at [" << i << "][" << j << "]" << std::endl;
            result[i][j] = lhs[i][j] + rhs[i][j];
        }
    }
    return result;
}

/**
 * @brief 
 *
 *
 *
 */
template<typename T>
bool & Matrix<T>::operator!=(const Matrix<T> &rhs) const {
    Matrix<T> lhs = *this;
     //If the dimensions aren't identical per matrix, then != returns true. 
    if (lhs.rows != rhs.rows || lhs.cols != rhs.cols)
	return true;  
    return false;  //Else, != doesn't apply in this case, return false.
}







#endif 
