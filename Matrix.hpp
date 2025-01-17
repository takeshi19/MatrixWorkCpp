//////////////////////////////////////////////
// 
// Your Name:  Manuel T. Gomez
// 
// CS 368, Spring 2018
// Matrix.hpp
//
// This code creates overloaded arithmetic 
// operators in order to faciliate the 
// math operations on matrices.  
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

/**
 * @brief This class derives from runtime_error
 *	  It is thrown when user tries accessing an invalid row idx.
 */
class RowIndexOutOfBoundsException : public std::runtime_error {
public:
    RowIndexOutOfBoundsException() : std::runtime_error("row index out of bounds")
    {
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
    Matrix(int r, int c);  //Default constructor
    int getRows();
    int getCols();

    // we need operator[] to return a modifiable L-value, that is, a non-const reference
    // so that we can store a value in a cell of a Matrix
    std::vector<T> & operator[](const int index); 

    const std::vector<T> & operator[](const int index) const; 
    
    //Assignment and addition operator in 1. Ca-ca-combo breaker!
    const Matrix<T> operator+=(const Matrix<T> &rhs);
    
    // operator+ has a const reference parameter, promises not to modify this object
    // and returns a const value
    const Matrix<T> operator+(const Matrix<T> &rhs) const;
   
    //Assignment and multiplication operator.
    const Matrix<T> operator*=(const Matrix<T> &rhs);
   
    //Assignment and multiplication operator with a SCALAR.
    const Matrix<T> operator*=(const T scalar);
 
    //For scalars that are on the RHS of the * operator (matrix on LHS of *)
    //Return the modified vector after scalar multiplication.
    const Matrix<T> operator*(const T scalar); 
 
    // operator* has a const reference param, doesn't mod the object, returns const value
    const Matrix<T> operator*(const Matrix<T> &rhs) const;
    
    //Assignment and subtraction operator in 1.    
    const Matrix<T> operator-=(const Matrix<T> &rhs);
 
    // operator- for subtraction of matrices.
    const Matrix<T> operator-(const Matrix<T> &rhs) const;

    //An overloaded inequality operator to compare 2 matrices.
    bool operator!=(const Matrix<T> &rhs);

    //An overloaded equality operator to compare 2 matrices.
    bool operator==(const Matrix<T> &rhs);

    /**
     * @brief overrides the << operator for Matrix<T>
     *        because this is a friend function, and because we have a templated class
     *        we need to define the function inside our class declaration
     * @param os the stream that we are using << with
     * @param obj  the Matrix<T> we are trying to insert into the stream
     * @return a non-const reference, which allows us to chain << operators
     */
    friend std::ostream& operator<<(std::ostream& os, const Matrix<T> &obj) {
        for (auto rowIt = obj.data.begin(); rowIt != obj.data.end(); ++rowIt) {
            for (auto colIt = rowIt->begin(); colIt != rowIt->end(); ++colIt) {
                if (colIt != rowIt->end() - 1) {
                    os << *colIt << " ";
                } else {
                    os << *colIt;
                }
            }
            os << std::endl;
        }
        return os;
    }
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
 * @brief Gets the number of rows of this matrix.
 *
 * @return the number of rows in this matrix.
 */
template<typename T>
int Matrix<T>::getRows() {
    return rows;  
}

/**
 * @brief Gets the number of columns of this matrix.
 *
 * @return the number of columns in this matrix.
 */
template<typename T>
int Matrix<T>::getCols() {
    return cols;  
}

	
/**
 * @brief accesses the row index of this Matrix
 *        we assume row-major ordering and a start index of 0
 *        this non-const function returns a non-const reference
 *
 * @param index, the index which corresponds to a vector (row) in the matrix
 * @return a non-const vector reference that is suitable for a Left-value
 */
template<typename T>
std::vector<T> & Matrix<T>::operator[](const int index) {
    //We don't use const because matrix[x][y] is on LHS of eqtn and it can be edited with arithmetic.
    if (index >= rows || index < 0) //Throw rowIndexOOBE if accessing invalid row.
	throw RowIndexOutOfBoundsException();

    return data[index];
}


/**
 * @brief accesses the row index of this Matrix
 *        we assume row-major ordering and a start index of 0
 *
 * @param index, the index which corresponds to a vector (row) in the matrix
 * @return data, a const vector referene that is suitable for a Right-value
 */
template<typename T>
const std::vector<T> & Matrix<T>::operator[](const int index) const {
    //We use const because we tell compiler we wont change matrix[x][y] when its on RHS of equation.
    if (index >= rows || index < 0) //Throw rowIndexOOBE if accessing invalid row.
	throw RowIndexOutOfBoundsException();
    return data[index]; 
}

/**
 * @brief adds the Matrix<T> on the right side of the + operator to the matrix on the left
 *        side of the + sign.
 *
 * @param rhs, a const reference to the Matrix on the right of the + operator
 * @return a const Matrix that represents the sum
 */
template<typename T>
const Matrix<T> Matrix<T>::operator+(const Matrix<T> &rhs) const {
    Matrix<T> lhs = *this; 
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
            result[i][j] = lhs[i][j] + rhs[i][j];
        }
    }
    return result;
}

/**
 * @brief Adds the instance of this class to the rhs Matrix parameter,
 *        then assigns the sum to the instance of this class. Smart because
 *	  it calls on the above + operator.
 *
 * @param rhs, the matrix being added to the instance of this class.
 * @return the summed instance of class Matrix.
 */ 
template<typename T>
const Matrix<T> Matrix<T>::operator+=(const Matrix<T> &rhs) {
    Matrix<T> lhs = *this;
    *this = lhs + rhs;
    return *this;
}

/**
 * @brief multiplies the object on LHS of * operator by the scalar param
 *	  on the RHS.
 *
 * @param scalar, the scalar we are multiplying matrix by.
 * @return the modified LHS Object (Matrix) being multiplied.
 */
template<typename T>
const Matrix<T> Matrix<T>::operator*(const T scalar) {
    Matrix<T> lhs = *this;
    
    for (int i = 0; i < lhs.rows; i++) 
        for (int j = 0; j < lhs.cols; j++) 
	    lhs[i][j] *= scalar;
   
    return lhs;
}  

/**
 * @brief Compound multiplication WITH a scalar.
 *
 * @param scalar, the scalar to multiply matrix with.
 * @return this matrix after compound multiplication.
 */
template<typename T>
const Matrix<T> Matrix<T>::operator*=(const T scalar) {
    Matrix<T> lhs = *this;
    *this = lhs * scalar;
    return *this;
}  

/**
 * @brief Does Matrix multiplication for equations where the scalar is on 
 *	  the LHS and the Matrix is on RHS.
 *
 * @param scalar, the T scalar to multiply our 2nd param by
 * @param &rhs, the 3nd param (Matrix) being multiplied by scalar.
 * @return the multiplied Matrix.
 */
template<typename T>
const Matrix<T> operator*(const T scalar, Matrix<T> &rhs) {
    return rhs * scalar;  //Just call on the * operator above.
}

/**
 * @brief Multiplies the Matrix<T> on the right side of the * operator to the matrix on the left
 *        side of the * sign.
 * 
 * @param &rhs a const reference to the Matrix on the right of the  
 *        multiplication operator.
 * @return a const Matrix that represents the product.
 */
template<typename T>
const Matrix<T> Matrix<T>::operator*(const Matrix<T> &rhs) const {
    Matrix<T> lhs = *this;

    if (lhs.cols != rhs.rows)  //Standard rule for multiplying matrices.
        throw DimensionMismatchException();

    Matrix<T> product(lhs.rows, rhs.cols); //Resulting matrix after multiplication. 
    //**Initializing the product array to zeros.**
    for (int i = 0; i < product.rows; i++) 
	for (int j = 0; j < product.cols; j++) 
 	    product[i][j] = 0;
 
    for (int i = 0; i < lhs.rows; i++) 
	for (int j = 0; j < rhs.cols; j++) 
            for (int k = 0; k < lhs.cols; k++) 
		product[i][j] += lhs[i][k] * rhs[k][j];
      
    return product;
}

/**
 * @brief Assignment and multiplication in one operation. Calls on the
 *        standard * operator above.
 * 
 * @param &rhs, a const reference to Matrix on right of *= op.
 * @return a reference to this class that was multiplied by rhs & 
 *         assigned to itself.
 */
template<typename T>
const Matrix<T> Matrix<T>::operator*=(const Matrix<T> &rhs) {
    Matrix<T> lhs = *this;
    *this = lhs * rhs;
    return *this;
}

/**
 * @brief Subtracts the Matrix<T> on the right side of the - operator to the matrix on the left
 *        side of the - sign.
 * 
 * @param &rhs, a const reference to the Matrix on the right of the - operator
 * @return a const Matrix that represents the result.
 */
template<typename T>
const Matrix<T> Matrix<T>::operator-(const Matrix<T> &rhs) const {
    Matrix<T> lhs = *this;
    //Need the matrices to be of same size for the multiplication to happen.
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
            result[i][j] = lhs[i][j] - rhs[i][j];
        }
    }
    return result;
}

/**
 * @brief Assigns and subtracts the rhs matrix to the instance of this class.
 *
 * @param &rhs, reference to the Matrix on right side of -= operator.
 * @return an instance of this class where -= did its work.
 */
template<typename T>
const Matrix<T> Matrix<T>::operator-=(const Matrix<T> &rhs) {
    Matrix<T> lhs = *this;
    *this = lhs - rhs;
    return *this;
}

/**
 * @brief Overloaded inequality operator to check if 2 matrices
 *	  have differing dimensions.
 *
 * @param rhs, (ref) the right-hand-side matrix to check dimensions with lhs matrix.
 * @return true if != applies. 
 */
template<typename T>
bool Matrix<T>::operator!=(const Matrix<T> &rhs) {
    Matrix<T> lhs = *this;
    //If the dimensions aren't identical per matrix, then != returns true. 
    if ((lhs.rows != rhs.rows) || (lhs.cols != rhs.cols)) 
	return true;
   	
    if (!(lhs == rhs)) 
	return true; 
    return false;  //Else, dimensions are equal & values identical.
}

/**
 * @brief Overloaded equality operator to check if 2 matrices  
 *	  are of the same dimensions.
 *
 * @param &rhs, (ref) the right-hand-side matrix to check elements and dimensions with
 * 	  lhs matrix.
 * @return true if identical dimensions & identical elements/data. Else, return false.
 */
template<typename T>
bool Matrix<T>::operator==(const Matrix<T> &rhs) {
    Matrix<T> lhs = *this;
    //If the dimensions aren't identical, then == returns false.
    if ((lhs.rows != rhs.rows) || (lhs.cols != rhs.cols)) 
        return false;
    //Else, dimensions are equivalent, then check if all elements are identical.
    for (int i = 0; i < lhs.rows; i++) {
        for (int j = 0; j < lhs.cols; j++) {
	    if (rhs[i][j] != lhs[i][j]) 
		return false;
        }
    }
    return true;  //If all elements identical btwn matrices, return true.
}

#endif 
