#include <collision_pp.h>

void copyCollisionPP(struct CollisionPP * _to, struct CollisionPP * _from)
{
    _to->id_i = _from->id_i;
    _to->id_j = _from->id_j;

    set_vec3(&_to->pos_i_0, _from->pos_i_0);
    set_vec3(&_to->pos_j_0, _from->pos_j_0);

    set_vec3(&_to->theta_i_0, _from->theta_i_0);
    set_vec3(&_to->theta_j_0, _from->theta_j_0);

    set_vec3(&_to->vers_R_c_0, _from->vers_R_c_0);

    set_vec3(&_to->F_collision_i, _from->F_collision_i);
    set_vec3(&_to->F_collision_j, _from->F_collision_j);

    set_vec3(&_to->moment_collision_i, _from->moment_collision_i);
    set_vec3(&_to->moment_collision_j, _from->moment_collision_j);
}

void updateCollisionPP(struct CollisionPP * _to, struct CollisionPP * _from)
{
    set_vec3(&_to->theta_i_0, _from->theta_i_0);
    set_vec3(&_to->theta_j_0, _from->theta_j_0);

    set_vec3(&_to->F_collision_i, _from->F_collision_i);
    set_vec3(&_to->F_collision_j, _from->F_collision_j);

    set_vec3(&_to->moment_collision_i, _from->moment_collision_i);
    set_vec3(&_to->moment_collision_j, _from->moment_collision_j);
}

//this structure is indexed using global particles' indices
void initCollisionsPP (struct Collisions * collisions)
{
    collisions->collisions_PP_current = (struct CollisionPP***)malloc(sizeof(struct CollisionPP**)*collisions->N_PARTICLES);
    collisions->collisions_PP_next = (struct CollisionPP***)malloc(sizeof(struct CollisionPP**)*collisions->N_PARTICLES);
    for (int i = 0; i < collisions->N_PARTICLES; i++)
    {
        collisions->collisions_PP_current [i] = (struct CollisionPP**)malloc(sizeof(struct CollisionPP*)*(collisions->N_PARTICLES-i));
        collisions->collisions_PP_next [i] = (struct CollisionPP**)malloc(sizeof(struct CollisionPP*)*(collisions->N_PARTICLES-i));
        for (int j = 0; j < collisions->N_PARTICLES-i; ++j) {
            collisions->collisions_PP_current[i][j] = NULL;
            collisions->collisions_PP_next[i][j] = NULL;
        }
    }
}

bool existsCollisionPP(struct Collisions * collisions, const int i, const int j)
{
    if (j<i || i>= collisions->N_PARTICLES)
    {
        return false; //ERROR
    }
    return collisions->collisions_PP_current[i][j-i] == NULL;
}

struct CollisionPP* findCollision (struct Collisions * collisions, const int i, const int j)
{
    if (j<i || i>= collisions->N_PARTICLES)
    {
        return NULL; //ERROR
    }
    return collisions->collisions_PP_current[i][j-i];
}

void initializeCollisions_PP(struct CollisionPP* collision_PP, const int i, const int j)
{
    collision_PP->id_i = i;
    collision_PP->id_j = j;

    clear_vec3 (&collision_PP->pos_i_0);
    clear_vec3 (&collision_PP->pos_j_0);

    clear_vec3 (&collision_PP->vers_R_c_0);

    clear_vec3 (&collision_PP->theta_i_0);
    clear_vec3 (&collision_PP->theta_j_0);

    clear_vec3 (&collision_PP->F_collision_i);
    clear_vec3 (&collision_PP->F_collision_j);

    clear_vec3 (&collision_PP->moment_collision_i);
    clear_vec3 (&collision_PP->moment_collision_j);

}

bool deleteCollision (struct Collisions * collisions, const int i, const int j)
{
    if (j<i || i>= collisions->N_PARTICLES)
    {
        return false; //ERROR
    }

    printf("sto cancellando la collisione tra (%d,%d)\n", i,j);
    //we set the collision in the next matrix null, then during the update phase memory will be freed
    free (collisions->collisions_PP_next[i][j-i]);
    collisions->collisions_PP_next[i][j-i] = NULL;
    return true;
}

void setInitCollsionValues (struct CollisionPP* collision_ij,
                            vec3*  pi, vec3 *pj, vec3* theta_i, vec3* theta_j, vec3* vi, vec3* vj,
                            vec3* wi, vec3* wj, CALreal dtp)
{
    vec3 _toAdd;
    multiply_by_scalar_vec3(&_toAdd, *vi, -dtp);
    add_vec3(&collision_ij->pos_i_0, *pi, _toAdd);

    multiply_by_scalar_vec3(&_toAdd, *wi, -dtp);
    add_vec3(&collision_ij->theta_i_0, *theta_i, _toAdd);

    multiply_by_scalar_vec3(&_toAdd, *vj, -dtp);
    add_vec3(&collision_ij->pos_j_0, *pj, _toAdd);

    multiply_by_scalar_vec3(&_toAdd, *wj, -dtp);
    add_vec3(&collision_ij->theta_j_0, *theta_j, _toAdd);

    subtract_vec3(&collision_ij->vers_R_c_0, collision_ij->pos_j_0, collision_ij->pos_i_0);

    CALreal absVec = 0.0;
    absulute_value_vec3(&absVec, collision_ij->vers_R_c_0);

    divide_by_scalar_vec3(&collision_ij->vers_R_c_0, collision_ij->vers_R_c_0, absVec);
}

struct CollisionPP* addCollision (struct Collisions* collisions, const int i, const int j,
                                  vec3*  pi, vec3 *pj, vec3* theta_i, vec3* theta_j, vec3* vi, vec3* vj,
                                  vec3* wi, vec3* wj, CALreal dtp)
{
    if (j<i || i>=collisions->N_PARTICLES)
    {
        return false; //ERROR
    }

    printf("sto aggiungendo la collisione tra (%d,%d)\n", i,j);

    collisions->collisions_PP_next[i][j-i] = (struct CollisionPP*)malloc(sizeof(struct CollisionPP));
    initializeCollisions_PP(collisions->collisions_PP_next[i][j-i], i, j);

    setInitCollsionValues(collisions->collisions_PP_next[i][j-i], pi,pj,theta_i, theta_j, vi, vj, wi, wj, dtp);

    return collisions->collisions_PP_next[i][j-i];
}

void setTheta_i (struct Collisions* collisions, const int i, const int j,vec3* newTheta)
{
    if (j<i || i>= collisions->N_PARTICLES)
    {
        return; //ERROR
    }
    set_vec3(&collisions->collisions_PP_next[i][j-i]->theta_i_0, *newTheta);
}
void setTheta_j (struct Collisions* collisions, const int i, const int j,vec3* newTheta)
{
    if (j<i || i>= collisions->N_PARTICLES)
    {
        return; //ERROR
    }
    set_vec3(&collisions->collisions_PP_next[i][j-i]->theta_j_0, *newTheta);
}

void setForce_i (struct Collisions* collisions, const int i, const int j,vec3* force)
{
    if (j<i || i>= collisions->N_PARTICLES)
    {
        return; //ERROR
    }
    set_vec3(&collisions->collisions_PP_next[i][j-i]->F_collision_i, *force);
}

void setForce_j (struct Collisions* collisions, const int i, const int j,vec3* force)
{
    if (j<i || i>= collisions->N_PARTICLES)
    {
        return; //ERROR
    }
    set_vec3(&collisions->collisions_PP_next[i][j-i]->F_collision_j, *force);
}

void setMoment_i (struct Collisions* collisions, const int i, const int j,vec3* moment)
{
    if (j<i || i>= collisions->N_PARTICLES)
    {
        return; //ERROR
    }
    set_vec3(&collisions->collisions_PP_next[i][j-i]->moment_collision_i, *moment);
}

void setMoment_j (struct Collisions* collisions, const int i, const int j,vec3* moment)
{
    if (j<i || i>= collisions->N_PARTICLES)
    {
        return; //ERROR
    }
    set_vec3(&collisions->collisions_PP_next[i][j-i]->moment_collision_j, *moment);
}



void updateCollisionsPP (struct Collisions* collisions)
{
    for (int i = 0; i < collisions->N_PARTICLES; i++)
    {
        for (int j = 0; j < collisions->N_PARTICLES-i; ++j) {
            //deleted collision
            if (collisions->collisions_PP_next[i][j] == NULL &&
                    collisions->collisions_PP_current[i][j] != NULL)
            {
                free(collisions->collisions_PP_current[i][j]);
                collisions->collisions_PP_current[i][j] = NULL;
            }
            //new collision
            else if(collisions->collisions_PP_next[i][j] != NULL &&
                    collisions->collisions_PP_current[i][j] == NULL)
            {
                collisions->collisions_PP_current[i][j] = (struct CollisionPP*)malloc(sizeof(struct CollisionPP));
                copyCollisionPP(collisions->collisions_PP_current[i][j], collisions->collisions_PP_next[i][j]);
            }
            //already present collision
            else if (collisions->collisions_PP_next[i][j] != NULL &&
                     collisions->collisions_PP_current[i][j] != NULL)
            {
                updateCollisionPP(collisions->collisions_PP_current[i][j], collisions->collisions_PP_next[i][j]);
            }
        }
    }
}

//is i a valid particle?

//VA PRESO DAL CURRENT?
void totalMomentCollisionPP(struct Collisions* collisions, vec3* moment_tot_i, const int i)
{
    for (int j = 0; j < collisions->N_PARTICLES - i; ++j) {
        if(collisions->collisions_PP_current[i][j] != NULL)
            add_vec3(moment_tot_i, *moment_tot_i, collisions->collisions_PP_current[i][j]->moment_collision_i);
    }
    int k = i, t= 0;

    while (k>=0 && t < i)
    {
        if(collisions->collisions_PP_current[k][t] != NULL)
            add_vec3(moment_tot_i, *moment_tot_i, collisions->collisions_PP_current[k][t]->moment_collision_j);
        k--;
        t++;
    }
}

//is i a valid particle?
//VA PRESO DAL CURRENT?
void totalForceCollisionPP(struct Collisions* collisions, vec3* F_tot_i, const int i)
{
    for (int j = 0; j < collisions->N_PARTICLES - i; ++j) {
        if(collisions->collisions_PP_current[i][j] != NULL)
            add_vec3(F_tot_i, *F_tot_i, collisions->collisions_PP_current[i][j]->F_collision_i);
    }

    int k = i, t= 0;

    while (k>=0 && t < i)
    {
        if(collisions->collisions_PP_current[k][t] != NULL)
            add_vec3(F_tot_i, *F_tot_i, collisions->collisions_PP_current[k][t]->F_collision_j);
        k--;
        t++;
    }
}

void clearForces(struct Collisions* collisions)
{
    for (int i = 0; i < collisions->N_PARTICLES; i++)
    {
        for (int j = 0; j < collisions->N_PARTICLES-i; ++j) {
            if(collisions->collisions_PP_next[i][j] != NULL)
            {
                clear_vec3(&collisions->collisions_PP_next[i][j]->F_collision_i);
                clear_vec3(&collisions->collisions_PP_next[i][j]->F_collision_j);
                clear_vec3(&collisions->collisions_PP_next[i][j]->moment_collision_i);
                clear_vec3(&collisions->collisions_PP_next[i][j]->moment_collision_j);
            }
        }
    }

}
