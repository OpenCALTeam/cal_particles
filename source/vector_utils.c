#include <vector_utils.h>

/* CLeaR Vector */
void clear_vec3(vec3 * v)
{
    int _i;
    for (_i = 0; _i < DIM; _i++)
        (*v)[_i] = 0.0;
}

/* SET Vector */
void set_vec3(vec3 * v,const vec3 u)
{
    int _i;
    for (_i = 0; _i < DIM; _i++)
        (*v)[_i] = (u)[_i];
}

/* ADD Vector */
void add_vec3(vec3 * v,const vec3 u,const vec3 w)
{
    int _i;
    for (_i = 0; _i < DIM; _i++)
        (*v)[_i] = (u)[_i] + (w)[_i];
}

/* SUBtract Vector */
void subtract_vec3(vec3 * v,const vec3 u,const vec3 w)
{
    int _i;
    for (_i = 0; _i < DIM; _i++)
        (*v)[_i] = (u)[_i] - (w)[_i];
}

/* MULtiply Vector by Scalar */
void multiply_by_scalar_vec3(vec3 * v,const vec3 u, const double s)
{
    int _i;
    for (_i = 0; _i < DIM; _i++)
        (*v)[_i] = (u)[_i] * (s);
}

/* DIVide Vector by Scalar */
void divide_by_scalar_vec3(vec3 * v,const vec3 u, const double s)
{
    int _i;
    for (_i = 0; _i < DIM; _i++)
        (*v)[_i] = (u)[_i] / (s);
}

/* DOT Vector Product */
void dot_product_vec3(double * s, const vec3 v,const vec3 u)
{
    int _i;
    (*s) = 0.0;
    for (_i = 0; _i < DIM; _i++)
        (*s) += (v)[_i] * (u)[_i];
}

void cross_product_vec3(vec3 * s, const vec3 u,const vec3 v)
{
   (*s)[0] = u[2]*v[1] - u[1]*v[2];
   (*s)[1] = u[0]*v[2] - u[2]*v[0];
   (*s)[2] = u[1]*v[0] - u[0]*v[1];
}

/* ABSolute value of a Vector */
void absolute_value_vec3(double * s, const vec3 v)
{
    double _tmp;
    int _i;
    _tmp = 0.0;
    for (_i = 0; _i < DIM; _i++)
        _tmp += (v)[_i] * (v)[_i];
    (*s) = sqrt(_tmp);
}

/* DISTance between Vectors */
void distance_vec3(double * s, const vec3 v,const vec3 u)
{
    double _tmp;
    int _i;
    _tmp = 0.0;
    for (_i = 0; _i < DIM; _i++)
        _tmp += ((u)[_i]-(v)[_i]) * ((u)[_i]-(v)[_i]);
    (*s) = sqrt(_tmp);
}

/* DISTance between Vectors */
void distance_squared_vec3(double * s, const vec3 v,const vec3 u)
{
    int _i;
    (*s) = 0.0;
    for (_i = 0; _i < DIM; _i++)
        (*s) += ((u)[_i]-(v)[_i]) * ((u)[_i]-(v)[_i]);
}
