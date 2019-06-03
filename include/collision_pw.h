#ifndef COLLISION_PW_H
#define COLLISION_PW_H
#include <common.hpp>
#include <vector_utils.h>

struct CollisionPW
{
    int id_particle, border_id;
    vec3 pos_0;

    vec3 theta_0;

    vec3 F_collision;

    vec3 moment_collision;

};

struct Collisions
{
    //the object collision that is present in the two matrices MUST be different
    struct CollisionPP *** collisions_PP_current;
    struct CollisionPP *** collisions_PP_next;
    CALint N_PARTICLES;

    struct CollisionPW *** collisions_PW_current;
    struct CollisionPW *** collisions_PW_next;
};

void copyCollisionPW(struct CollisionPW * _to, struct CollisionPW * _from);
void updateCollisionPW(struct CollisionPW * _to, struct CollisionPW * _from);

void initCollisionsPW (struct Collisions * collisions);
bool existsCollisionPW (struct Collisions * collisions, const int i, const int WALL_ID);
struct CollisionPW* findCollision_PW (struct Collisions * collisions, const int i, const int WALL_ID);

void initializeCollisions_PW(struct CollisionPW* collision_PW, const int i, const int WALL_ID);
bool deleteCollision_PW (struct Collisions * collisions, const int i, const int WALL_ID);

struct CollisionPW* addCollision_PW (struct Collisions* collisions, const int i, const int WALL_ID,
                                  vec3*  p, vec3* theta, vec3* v,
                                  vec3* w, CALreal dtp);

void setTheta_i_PW (struct Collisions* collisions, const int i, const int WALL_ID, vec3* newTheta);
void setForce_i_PW (struct Collisions* collisions, const int i, const int WALL_ID, vec3* force);

void setMoment_i_PW (struct Collisions* collisions, const int i, const int j,vec3* moment);

void updateCollisionsPW (struct Collisions* collisions);

void totalMomentCollisionPW(struct Collisions* collisions, vec3* moment_tot, const int i);

void totalForceCollisionPW(struct Collisions* collisions, vec3* F_tot, const int i);

void clearForces_PW(struct Collisions* collisions);



#endif
