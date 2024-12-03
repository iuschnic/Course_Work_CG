#ifndef GEOM_H
#define GEOM_H

#include <cmath>
#include <vector>
#include <assert.h>
#include <iostream>
#include "Point.h"

class Matrix {
private:
    std::vector<std::vector<float>> m;
    int rows, cols;
public:
    Matrix(int r = 4, int c = 4) : m(std::vector<std::vector<float> >(r, std::vector<float>(c, 0.f))), rows(r), cols(c) { }
    Matrix(Point &p) : m(std::vector<std::vector<float> >(4, std::vector<float>(1, 1.f))), rows(4), cols(1) {
        m[0][0] = p.get_x();
        m[1][0] = p.get_y();
        m[2][0] = p.get_z();
    }
    int nrows() {return rows;}
    int ncols() {return cols;}
    static Matrix identity(int dimensions)
    {
        Matrix E(dimensions, dimensions);
        for (int i = 0; i < dimensions; i++) {
            for (int j = 0; j < dimensions; j++) {
                E[i][j] = (i == j ? 1.f : 0.f);
            }
        }
        return E;
    }
    std::vector<float>& operator[](const int i)
    {
        assert(i >= 0 && i < rows);
        return m[i];
    }
    Matrix operator*(const Matrix& a)
    {
        assert(cols == a.rows);
        Matrix result(rows, a.cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < a.cols; j++) {
                result.m[i][j] = 0.f;
                for (int k = 0; k < cols; k++) {
                    result.m[i][j] += m[i][k] * a.m[k][j];
                }
            }
        }
        return result;
    }
    Matrix transpose()
    {
        Matrix result(cols, rows);
        for(int i = 0; i < rows; i++)
            for(int j = 0; j < cols; j++)
                result[j][i] = m[i][j];
        return result;
    }
    Matrix inverse(){
        assert(rows == cols);
        Matrix result(rows, cols * 2);
        for(int i = 0; i < rows; i++)
            for(int j = 0; j < cols; j++)
                result[i][j] = m[i][j];
        for(int i = 0; i < rows; i++)
            result[i][i + cols] = 1;
        // first pass
        for (int i = 0; i < rows - 1; i++) {
            for(int j = result.cols - 1; j >= 0; j--)
                result[i][j] /= result[i][i];
            for (int k = i + 1; k < rows; k++) {
                float coeff = result[k][i];
                for (int j = 0; j < result.cols; j++) {
                    result[k][j] -= result[i][j] * coeff;
                }
            }
        }
        for(int j = result.cols - 1; j >= rows - 1; j--)
            result[rows - 1][j] /= result[rows - 1][rows - 1];
        for (int i = rows - 1; i > 0; i--) {
            for (int k = i - 1; k >= 0; k--) {
                float coeff = result[k][i];
                for (int j = 0; j < result.cols; j++) {
                    result[k][j] -= result[i][j] * coeff;
                }
            }
        }
        Matrix truncate(rows, cols);
        for(int i = 0; i < rows; i++)
            for(int j = 0; j < cols; j++)
                truncate[i][j] = result[i][j + cols];
        return truncate;
    }
    friend std::ostream& operator<<(std::ostream& s, Matrix& m)
    {
        for (int i = 0; i < m.nrows(); i++)  {
            for (int j = 0; j < m.ncols(); j++) {
                s << m[i][j];
                if (j < m.ncols() - 1) s << "\t";
            }
            s << "\n";
        }
        return s;
    }
};

//Считает матрицу перехода к реперу в котором точка eye - центр координат, точка center - направление на ось z, точка up - вертикальный вектор на ось y
Matrix look_at(Point eye, Point center, Point up);

#endif // GEOM_H
