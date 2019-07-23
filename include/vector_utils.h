#ifndef VECTOR_UTILS_H
#define VECTOR_UTILS_H
#include <common.hpp>

/* CLeaR Vector */
void clear_vec3(vec3 * v);

/* SET Vector */
void set_vec3(vec3 * v,const vec3 u);

/* ADD Vector */
void add_vec3(vec3 * v,const vec3 u,const vec3 w);

/* SUBtract Vector */
void subtract_vec3(vec3 * v,const vec3 u,const vec3 w);

/* MULtiply Vector by Scalar */
void multiply_by_scalar_vec3(vec3 * v,const vec3 u, const double s);

/* DIVide Vector by Scalar */
void divide_by_scalar_vec3(vec3 * v,const vec3 u, const double s);

/* DOT Vector Product */
void dot_product_vec3(double * s, const vec3 v,const vec3 u);

void cross_product_vec3(vec3 * s, const vec3 v,const vec3 u);
/* ABSolute value of a Vector */
void absolute_value_vec3(double * s, const vec3 v);

/* DISTance between Vectors */
void distance_vec3(double * s, const vec3 v,const vec3 u);

void distance_squared_vec3(double * s, const vec3 v,const vec3 u);




#endif
