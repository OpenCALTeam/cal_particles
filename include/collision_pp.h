#ifndef COLLISION_P_P_H
#define COLLISION_P_P_H

#include<common.hpp>
#include <vector_utils.h>

struct CollisionPP
{

    int id_i, id_j;
    vec3 pos_i_0;
    vec3 pos_j_0;

    vec3 theta_i_0;
    vec3 theta_j_0;

    vec3 vers_R_c_0;

    vec3 F_collision_i;
    vec3 F_collision_j;

    vec3 moment_collision_i;
    vec3 moment_collision_j;
};

//the object collision that is present in the two matrices MUST be different
struct CollisionPP *** collisions_PP_current;
struct CollisionPP *** collisions_PP_next;

void initCollisionPP(struct CollisionPP * _to, struct CollisionPP * _from);
void updateCollisionPP(struct CollisionPP * _to, struct CollisionPP * _from);

//this structure is indexed using global particles' indices
void initCollisionsPP (struct CollisionPP *** collisions_PP, const int N_PARTICLES);

bool existsCollisionPP(struct CollisionPP *** collisions_PP_current, const int N_PARTICLES, const int i, const int j);

struct CollisionPP* findCollision (struct CollisionPP *** collisions_PP_current, const int N_PARTICLES, const int i, const int j);

void initializeCollisions_PP(struct CollisionPP* collision_PP, const int i, const int j);

bool deleteCollision (struct CollisionPP *** collisions_PP, const int N_PARTICLES, const int i, const int j);

struct CollisionPP* addCollision (struct CollisionPP *** collisions_PP_next, const int N_PARTICLES, const int i, const int j);

void updateCollisionsPP (struct CollisionPP *** collisions_PP_current, struct CollisionPP *** collisions_PP_next, const int N_PARTICLES);

//is i a valid particle?
vec3* totalMomentCollisionPP(struct CollisionPP *** collisions_PP, const int i, const int N_PARTICLES);

//is i a valid particle?
vec3* totalForceCollisionPP(struct CollisionPP *** collisions_PP, const int i, const int N_PARTICLES);





#endif
