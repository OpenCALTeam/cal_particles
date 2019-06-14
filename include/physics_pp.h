#ifndef PHYSICS_PP_H
#define PHYSICS_PP_H

#include <model.h>
#include <vector_utils.h>
#include <collision_pp.h>

void defPart_PP_ij (vec3 * DefN, vec3 * DefT, CALreal overlap, vec3 enij, vec3 theta_i, vec3 theta_j,
              struct CollisionPP* collision_ij);

void defPart_PP_ji (vec3 * DefN, vec3 * DefT, CALreal overlap, vec3 enji, vec3 theta_i, vec3 theta_j,
              struct CollisionPP* collision_ij);

void resetPart_PP (vec3 deT, vec3 theta_i, vec3 theta_j, vec3 enij,
                struct CollisionPP* collision_ij, struct Collisions* collisions);

void forcePart_PP (vec3* Fn, vec3* Ft, CALreal overlap, vec3 DefN,
                vec3 DefT, vec3 enij, vec3 Wi, vec3 Wj, CALreal vnij, vec3 vrij,
                vec3 theta_i, vec3 theta_j,  struct CollisionPP* collision_ij, struct Collisions* collisions);

void updateForces_PP (vec3 Ft, vec3 Fn, vec3 enij, struct CollisionPP* collision_ij,
                      struct Collisions* collisions);


#endif
