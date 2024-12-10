#include "Geom.h"

Matrix look_at(Point eye, Point center, Point up)
{
    Point z = (center - eye).normalize();
    Point x = (up * z).normalize();
    Point y = (x * z).normalize();
    Matrix M = Matrix::identity(4);
    M[0][0] = x.get_x();
    M[1][0] = y.get_x();
    M[2][0] = z.get_x();

    M[0][1] = x.get_y();
    M[1][1] = y.get_y();
    M[2][1] = z.get_y();

    M[0][2] = x.get_z();
    M[1][2] = y.get_z();
    M[2][2] = z.get_z();
    Matrix move = Matrix::identity(4);
    move[0][3] = -eye.get_x();
    move[1][3] = -eye.get_y();
    move[2][3] = -eye.get_z();
    return M * move;
}

Matrix perspective(const double &focus)
{
    Matrix m = Matrix();

}
