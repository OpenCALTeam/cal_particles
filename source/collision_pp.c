#include <collision_pp.h>

void initCollisionPP(struct CollisionPP * _to, struct CollisionPP * _from)
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
void initCollisionsPP (struct CollisionPP *** collisions_PP, const int N_PARTICLES)
{
    collisions_PP = (struct CollisionPP***)malloc(sizeof(struct CollisionPP**)*N_PARTICLES);
    for (int i = 0; i < N_PARTICLES; i++)
    {
        collisions_PP [i] = (struct CollisionPP**)malloc(sizeof(struct CollisionPP*)*(N_PARTICLES-i));
        for (int j = 0; j < N_PARTICLES-i; ++j) {
            collisions_PP[i][j] = NULL;
        }
    }
}

bool existsCollisionPP(struct CollisionPP *** collisions_PP_current, const int N_PARTICLES, const int i, const int j)
{
    if (j>=i || i>=N_PARTICLES)
    {
        return false; //ERROR
    }
    return collisions_PP_current[i][j-i-1] == NULL;
}

struct CollisionPP* findCollision (struct CollisionPP *** collisions_PP_current, const int N_PARTICLES, const int i, const int j)
{
    if (j>=i || i>=N_PARTICLES)
    {
        return NULL; //ERROR
    }
    return collisions_PP_current[i][j-i-1];
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

bool deleteCollision (struct CollisionPP *** collisions_PP, const int N_PARTICLES, const int i, const int j)
{
    if (j>=i || i>=N_PARTICLES)
    {
        return false; //ERROR
    }

    //we set the collision in the next matrix null, then during the update phase memory will be freed
    free (collisions_PP[i][j-i-1]);
    collisions_PP[i][j-i-1] = NULL;
    return true;
}

struct CollisionPP* addCollision (struct CollisionPP *** collisions_PP_next, const int N_PARTICLES, const int i, const int j)
{
    if (j>=i || i>=N_PARTICLES)
    {
        return false; //ERROR
    }

    collisions_PP_next[i][j-i-1] = (struct CollisionPP*)malloc(sizeof(struct CollisionPP));
    initializeCollisions_PP(collisions_PP_next[i][j-i-1], i, j);
    return collisions_PP_next[i][j-i-1];
}



void updateCollisionsPP (struct CollisionPP *** collisions_PP_current, struct CollisionPP *** collisions_PP_next, const int N_PARTICLES)
{
    for (int i = 0; i < N_PARTICLES; i++)
    {
        for (int j = 0; j < N_PARTICLES-i; ++j) {
            //deleted collision
            if (collisions_PP_next[i][j] == NULL &&
                    collisions_PP_current[i][j] != NULL)
            {
                free(collisions_PP_current[i][j]);
                collisions_PP_current[i][j] = NULL;
            }
            //new collision
            else if(collisions_PP_next[i][j] != NULL &&
                    collisions_PP_current[i][j] == NULL)
            {
                collisions_PP_current[i][j] = (struct CollisionPP*)malloc(sizeof(struct CollisionPP));
                initCollisionPP(collisions_PP_current[i][j], collisions_PP_next[i][j]);
            }
            //already present collision
            else if (collisions_PP_next[i][j] != NULL &&
                     collisions_PP_current[i][j] != NULL)
            {
                updateCollisionPP(collisions_PP_current[i][j], collisions_PP_next[i][j]);
            }
        }
    }
}

//is i a valid particle?
vec3* totalMomentCollisionPP(struct CollisionPP *** collisions_PP, const int i, const int N_PARTICLES)
{
    vec3 moment_tot_i = {0.0, 0.0, 0.0};
    vec3* moment_tot_i_prt = &moment_tot_i;

    for (int j = 0; j < N_PARTICLES - i; ++j) {
        if(collisions_PP[i][j] != NULL)
            add_vec3(&moment_tot_i, moment_tot_i, collisions_PP[i][j]->moment_collision_i);
    }
    int k = i, t= 0;

    while (k>=0 && t > i)
    {
        if(collisions_PP[t][k] != NULL)
            add_vec3(&moment_tot_i, moment_tot_i, collisions_PP[t][k]->moment_collision_j);
        k--;
        t++;
    }

    return moment_tot_i_prt;

}

//is i a valid particle?
vec3* totalForceCollisionPP(struct CollisionPP *** collisions_PP, const int i, const int N_PARTICLES)
{
    vec3 F_tot_i = {0.0, 0.0, 0.0};
    vec3*  F_tot_i_prt = &F_tot_i;

    for (int j = 0; j < N_PARTICLES - i; ++j) {
        if(collisions_PP[i][j] != NULL)
            add_vec3(&F_tot_i, F_tot_i, collisions_PP[i][j]->F_collision_i);
    }
    int k = i, t= 0;

    while (k>=0 && t > i)
    {
        if(collisions_PP[t][k] != NULL)
            add_vec3(&F_tot_i, F_tot_i, collisions_PP[t][k]->F_collision_j);
        k--;
        t++;
    }

    return F_tot_i_prt;

}
