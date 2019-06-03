#ifndef COLLISION_P_P_H
#define COLLISION_P_P_H

#include<common.hpp>
#include <vector_utils.h>

#include <model.h>

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

struct Collisions
{
    //the object collision that is present in the two matrices MUST be different
    struct CollisionPP *** collisions_PP_current;
    struct CollisionPP *** collisions_PP_next;
    CALint N_PARTICLES;
};


void copyCollisionPP(struct CollisionPP * _to, struct CollisionPP * _from);
void updateCollisionPP(struct CollisionPP * _to, struct CollisionPP * _from);

//this structure is indexed using global particles' indices
void initCollisionsPP (struct Collisions * collisions);

bool existsCollisionPP(struct Collisions * collisions, const int i, const int j);

struct CollisionPP* findCollision (struct Collisions * collisions, const int i, const int j);

void initializeCollisions_PP(struct CollisionPP* collision_PP, const int i, const int j);

bool deleteCollision (struct Collisions * collisions, const int i, const int j);

struct CollisionPP* addCollision (struct Collisions* collisions, const int i, const int j,
                                  vec3* pi, vec3 *pj, vec3* theta_i, vec3* theta_j, vec3* vi, vec3* vj,
                                  vec3* wi, vec3* wj, CALreal dtp);

void updateCollisionsPP (struct Collisions* collisions);

void setTheta_i (struct Collisions* collisions, const int i, const int j,vec3* newTheta);
void setTheta_j (struct Collisions* collisions, const int i, const int j,vec3* newTheta);

void setForce_i (struct Collisions* collisions, const int i, const int j,vec3* force);
void setForce_j (struct Collisions* collisions, const int i, const int j,vec3* force);

void setMoment_i (struct Collisions* collisions, const int i, const int j,vec3* moment);
void setMoment_j (struct Collisions* collisions, const int i, const int j,vec3* moment);

//is i a valid particle?
void totalMomentCollisionPP(struct Collisions* collisions, vec3* moment_tot_i, const int i);

//is i a valid particle?
void totalForceCollisionPP(struct Collisions* collisions, vec3* F_tot_i, const int i);

void clearForces(struct Collisions* collisions);





#endif
