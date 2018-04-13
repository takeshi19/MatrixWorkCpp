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

    // we need a second operator[] in order to make our operator+ below work
    // the operator+ function below needs a const Matrix as a parameter
    // thus this second opertor[] function returns a const reference
    const std::vector<T> & operator[](const int index) const; 

    // operator+ has a const reference parameter, promises not to modify this object
    // and returns a const value
    const Matrix<T> operator+(const Matrix<T> &rhs) const;
    
    // operator* has a const referene param, doesn't mod the object, returns const value
    const Matrix<T> operator*(const Matrix<T> &rhs) const;
    
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
    int numRows = 0; //The amount of rows per Matrix.
    
    for (auto rowit = data.begin(); rowit != data.end(); ++rowit) 
        numRows++;
    return numRows;
}

/**
 * @brief Gets the number of columns of this matrix.
 *
 * @return the number of columns in this matrix.
 */
template<typename T>
int Matrix<T>::getCols() {
  //FIXME check ur logic here...  
    
   int numCols = 0;
   for (auto rowit = data.begin(); rowit != data.end(); ++rowit) {
      std::vector<T> rowData = *rowit;
      for (auto colit = rowData.begin(); colit != rowData.end(); ++colit) 
        numCols++;
      break;
    }
    return numCols;
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
//TODO MAKE ME
/**
 * @brief Multiples the Matrix<T> on the right side of the * operator to the matrix on the left
 *        side of the * sign.
 * @param rhs a const reference to the Matrix on the right of the  
 *        multiplication operator.
 * @return a const Matrix that represents the product.
 */
template<typename T>
const Matrix<T> Matrix<T>::operator*(const Matrix<T> &rhs) const {
    Matrix<T> lhs = *this;
    std::cout << lhs.cols << " : col count in mat1\n";
    std::cout << rhs.rows << " : row count in mat2\n";

    if (lhs.cols != rhs.rows)  //Standard rule for multiplying matrices.
        throw DimensionMismatchException();
    
    int sum = 0;  	     //The sum of products btwn 2 matrices.
    int rows = rhs.rows;    //Rows of mat1: outer loop
    int cols = rhs.cols;    //**Equivalent to mat2.rows** 
    int subcols = lhs.cols; //The columns of matrix2, in 3rd loop.
    Matrix<T> product(rows, subcols); //Resulting matrix after multiplication. 
    for (int i = 0; i < rows; i++) {
	//Simultaneously iterating thru rows of mat2.
	for (int j = 0; j < cols; j++) { 
	    int mat1Elmt = rhs[i][j];
	    
            for (int k = 0; k < subcols; k++) {
		int mat2Elmt = lhs[j][k];  
		sum += mat1Elmt * mat2Elmt;
		product[i][k] = sum;
	    }
        }
	sum = 0;  //Clear sum to store in product after each outermost loop.
    }
    return product;
}

/**
 * @brief Subtracts the Matrix<T> on the right side of the - operator to the matrix on the left
 *        side of the - sign.
 * @param rhs a const reference to the Matrix on the right of the - operator
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
            std::cout << "subtracting elements at [" << i << "][" << j << "]" << std::endl;
            result[i][j] = lhs[i][j] - rhs[i][j];
        }
    }
    return result;
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
}

/**
 * @brief Overloaded equality operator to check if 2 matrices  
 *	  are of the same dimensions.
 *
 * @param rhs, (ref) the right-hand-side matrix to check elements and dimensions with
 * 	  lhs matrix.
 * @return true if identical dimensions & identical elements/data. Else, return false.
 */
template<typename T>
bool Matrix<T>::operator==(const Matrix<T> &rhs) {
    Matrix<T> lhs = *this;
    //If the dimensions aren't identical, then == returns false.
    if (!(lhs.rows == rhs.rows) || !(lhs.cols == rhs.cols))
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
