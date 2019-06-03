#include<collision_pw.h>


void copyCollisionPW(struct CollisionPW * _to, struct CollisionPW * _from)
{
    _to->id_particle = _from->id_particle;
    _to->border_id = _from->border_id;

    set_vec3(&_to->pos_0, _from->pos_0);

    set_vec3(&_to->theta_0, _from->theta_0);

    set_vec3(&_to->F_collision, _from->F_collision);

    set_vec3(&_to->moment_collision, _from->moment_collision);
}

void updateCollisionPW(struct CollisionPW * _to, struct CollisionPW * _from)
{
    set_vec3(&_to->theta_0, _from->theta_0);

    set_vec3(&_to->F_collision, _from->F_collision);

    set_vec3(&_to->moment_collision, _from->moment_collision);
}

//this structure is indexed using global particles' indices
void initCollisionsPW (struct Collisions * collisions)
{
    collisions->collisions_PW_current = (struct CollisionPW***)malloc(sizeof(struct CollisionPW**)*collisions->N_PARTICLES);
    collisions->collisions_PW_next = (struct CollisionPW***)malloc(sizeof(struct CollisionPW**)*collisions->N_PARTICLES);
    for (int i = 0; i < collisions->N_PARTICLES; i++)
    {
        collisions->collisions_PW_current [i] = (struct CollisionPW**)malloc(sizeof(struct CollisionPW*)*(N_WALLS));
        collisions->collisions_PW_next [i] = (struct CollisionPW**)malloc(sizeof(struct CollisionPW*)*(N_WALLS));
        for (int j = 0; j < N_WALLS; ++j) {
            collisions->collisions_PW_current[i][j] = NULL;
            collisions->collisions_PW_next[i][j] = NULL;
        }
    }
}

bool existsCollisionPW(struct Collisions * collisions, const int i, const int WALL_ID)
{
    if (i>= collisions->N_PARTICLES || WALL_ID>= N_WALLS)
    {
        return false; //ERROR
    }
    return collisions->collisions_PW_current[i][WALL_ID] == NULL;
}

struct CollisionPW* findCollision_PW (struct Collisions * collisions, const int i, const int WALL_ID)
{
    if (i>= collisions->N_PARTICLES || WALL_ID >= N_WALLS)
    {
        return NULL; //ERROR
    }
    return collisions->collisions_PW_current[i][WALL_ID];
}

void initializeCollisions_PW(struct CollisionPW* collision_PW, const int i, const int WALL_ID)
{
    collision_PW->id_particle = i;
    collision_PW->border_id = WALL_ID;

    clear_vec3 (&collision_PW->pos_0);
    clear_vec3 (&collision_PW->theta_0);

    clear_vec3 (&collision_PW->F_collision);

    clear_vec3 (&collision_PW->moment_collision);
}

bool deleteCollision_PW (struct Collisions * collisions, const int i, const int WALL_ID)
{
    if (i>= collisions->N_PARTICLES || WALL_ID>= N_WALLS)
    {
        return false; //ERROR
    }

    printf("sto cancellando la collisione wall tra (%d,%d)\n", i,WALL_ID);
    //we set the collision in the next matrix null, then during the update phase memory will be freed
    free (collisions->collisions_PW_next[i][WALL_ID]);
    collisions->collisions_PW_next[i][WALL_ID] = NULL;
    return true;
}

void setInitCollsionValues_PW (struct CollisionPW* collision_PW,
                            vec3*  p, vec3* theta, vec3* v,
                            vec3* w, CALreal dtp)
{
    vec3 _toAdd;
    multiply_by_scalar_vec3(&_toAdd, *v, -dtp);
    add_vec3(&collision_PW->pos_0, *p, _toAdd);

    multiply_by_scalar_vec3(&_toAdd, *w, -dtp);
    add_vec3(&collision_PW->theta_0, *theta, _toAdd);
}


struct CollisionPW* addCollision_PW (struct Collisions* collisions, const int i, const int WALL_ID,
                                  vec3*  p, vec3* theta, vec3* v,
                                  vec3* w, CALreal dtp)
{
    if (i>= collisions->N_PARTICLES || WALL_ID>= N_WALLS)
    {
        return false; //ERROR
    }

    printf("sto aggiungendo la collisione p-wall tra (%d,%d)\n", i,WALL_ID);

    collisions->collisions_PW_next[i][WALL_ID] = (struct CollisionPW*)malloc(sizeof(struct CollisionPW));
    initializeCollisions_PW(collisions->collisions_PW_next[i][WALL_ID], i, WALL_ID);

    setInitCollsionValues_PW(collisions->collisions_PW_next[i][WALL_ID], p, theta, v, w, dtp);

    return collisions->collisions_PW_next[i][WALL_ID];
}

void setTheta_i_PW (struct Collisions* collisions, const int i, const int WALL_ID, vec3* newTheta)
{
    if (i>= collisions->N_PARTICLES || WALL_ID>= N_WALLS)
    {
        return; //ERROR
    }
    set_vec3(&collisions->collisions_PW_next[i][WALL_ID]->theta_0, *newTheta);
}

void setForce_i_PW (struct Collisions* collisions, const int i, const int WALL_ID, vec3* force)
{
    if (i>= collisions->N_PARTICLES || WALL_ID>= N_WALLS)
    {
        return; //ERROR
    }
    set_vec3(&collisions->collisions_PW_next[i][WALL_ID]->F_collision, *force);
}

void setMoment_i_PW (struct Collisions* collisions, const int i, const int WALL_ID,vec3* moment)
{
    if (i>= collisions->N_PARTICLES || WALL_ID>= N_WALLS)
    {
        return; //ERROR
    }
    set_vec3(&collisions->collisions_PW_next[i][WALL_ID]->moment_collision, *moment);
}

void updateCollisionsPW (struct Collisions* collisions)
{
    for (int i = 0; i < collisions->N_PARTICLES; i++)
    {
        for (int j = 0; j < N_WALLS; ++j) {
            //deleted collision
            if (collisions->collisions_PW_next[i][j] == NULL &&
                    collisions->collisions_PW_current[i][j] != NULL)
            {
                free(collisions->collisions_PW_current[i][j]);
                collisions->collisions_PW_current[i][j] = NULL;
            }
            //new collision
            else if(collisions->collisions_PW_next[i][j] != NULL &&
                    collisions->collisions_PW_current[i][j] == NULL)
            {
                collisions->collisions_PW_current[i][j] = (struct CollisionPW*)malloc(sizeof(struct CollisionPW));
                copyCollisionPW(collisions->collisions_PW_current[i][j], collisions->collisions_PW_next[i][j]);
            }
            //already present collision
            else if (collisions->collisions_PW_next[i][j] != NULL &&
                     collisions->collisions_PW_current[i][j] != NULL)
            {
                updateCollisionPW(collisions->collisions_PW_current[i][j], collisions->collisions_PW_next[i][j]);
            }
        }
    }
}

void totalMomentCollisionPW(struct Collisions* collisions, vec3* moment_tot, const int i)
{
    for (int j = 0; j < N_WALLS; ++j) {
        if(collisions->collisions_PW_current[i][j] != NULL)
            add_vec3(moment_tot, *moment_tot, collisions->collisions_PW_current[i][j]->moment_collision);
    }
}

void totalForceCollisionPW(struct Collisions* collisions, vec3* F_tot, const int i)
{
    for (int j = 0; j < N_WALLS; ++j) {
        if(collisions->collisions_PW_current[i][j] != NULL)
            add_vec3(F_tot, *F_tot, collisions->collisions_PW_current[i][j]->F_collision);
    }
}

void clearForces_PW(struct Collisions* collisions)
{
    for (int i = 0; i < collisions->N_PARTICLES; i++)
    {
        for (int j = 0; j < N_WALLS; ++j) {
            if(collisions->collisions_PW_next[i][j] != NULL)
            {
                clear_vec3(&collisions->collisions_PW_next[i][j]->F_collision);
                clear_vec3(&collisions->collisions_PW_next[i][j]->moment_collision);

            }
        }
    }

}


