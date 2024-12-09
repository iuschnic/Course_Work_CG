#include "Geom.h"

Matrix look_at(Point eye, Point center, Point up)
{
    Point z = (center - eye).normalize();
    Point x = (up * z).normalize();
    Point y = (x * z).normalize();
    //Point z = (eye - center).normalize();
    //Point x = (up * z).normalize();
    //Point y = (z * x).normalize();
    Matrix M = Matrix::identity(4);
    M[0][0] = x.get_x();
    M[1][0] = y.get_x();
    M[2][0] = z.get_x();
    //M[0][3] = -center.get_x();

    M[0][1] = x.get_y();
    M[1][1] = y.get_y();
    M[2][1] = z.get_y();
    //M[1][3] = -center.get_y();

    M[0][2] = x.get_z();
    M[1][2] = y.get_z();
    M[2][2] = z.get_z();
    //M[2][3] = -center.get_z();
    Matrix move = Matrix::identity(4);
    move[0][3] = -eye.get_x();
    move[1][3] = -eye.get_y();
    move[2][3] = -eye.get_z();
    //std::cout << M;
    //std::cout << "after look\n";
    return M * move;
    //Point z = eye - center;
    /*Point z = center - eye;
    z.normalize();
    Point x = up * z;
    x.normalize();
    Point y = z * x;
    y.normalize();
    Matrix Minv = Matrix::identity(4);
    Matrix Tr   = Matrix::identity(4);
    //std::cout << Minv;
    //std::cout << Tr;
    Minv[0][0] = x.get_x();
    Minv[1][0] = y.get_x();
    Minv[2][0] = z.get_x();
    Tr[0][3] = -center.get_x();
    Minv[0][1] = x.get_y();
    Minv[1][1] = y.get_y();
    Minv[2][1] = z.get_y();
    Tr[1][3] = -center.get_y();
    Minv[0][2] = x.get_z();
    Minv[1][2] = y.get_z();
    Minv[2][2] = z.get_z();
    Tr[2][3] = -center.get_z();
    return Minv * Tr;*/
    //Matrix ModelView = Minv * Tr;
    //return res;
}

Matrix perspective(const double &focus)
{
    Matrix m = Matrix();

}
