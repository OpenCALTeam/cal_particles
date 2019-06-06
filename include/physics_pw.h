#ifndef PHYSICS_PW_H
#define PHYSICS_PW_H

#include <model.h>
#include <vector_utils.h>
#include <collision_pw.h>

void defPart_PW (vec3 * DefN, vec3 * DefT, CALreal overlap, vec3 vec_r, vec3 p, vec3 theta,
                 struct CollisionPW* collision_iw);

void resetPart_PW (vec3 deT, vec3 theta,  vec3 p, vec3 vec_r,
                struct CollisionPW* collision_iw, struct Collisions* collisions);

void forcePart_PW (vec3* Fn, vec3* Ft, CALreal overlap, vec3 DefN,
                   vec3 DefT, vec3 theta, vec3 p, vec3 v, vec3 vec_r,
                   struct CollisionPW* collision_pw, struct Collisions* collisions);

void updateForces_PW (vec3 Ft, vec3 Fn, vec3 vec_r, struct CollisionPW* collision_pw, struct Collisions* collisions);


#endif
