#include<collision_pw.h>

#include<model.h>

//TODO portare a zero l'altezza
//TODO sistemare gli indici dei wall
void initWalls(struct Wall walls[N_WALLS])
{
    walls[WALL_WEST].indx= 0;
    walls[WALL_WEST].sign= -1;
    walls[WALL_WEST].border_id = WALL_WEST;
    clear_vec3(&walls[WALL_WEST].pos);
    walls[WALL_WEST].pos[0] = cnfg.PARTICLE_RADIUS;

    walls[WALL_EAST].indx= 0;
    walls[WALL_EAST].sign= 1;
    walls[WALL_EAST].border_id = WALL_EAST;
    clear_vec3(&walls[WALL_EAST].pos);
    walls[WALL_EAST].pos[0] = X_CELLS * CELL_SIDE - cnfg.PARTICLE_RADIUS;

    walls[WALL_NORTH].indx= 1;
    walls[WALL_NORTH].sign= 1;
    walls[WALL_NORTH].border_id = WALL_NORTH;
    clear_vec3(&walls[WALL_NORTH].pos);
    walls[WALL_NORTH].pos[1] = Y_CELLS * CELL_SIDE - cnfg.PARTICLE_RADIUS;

    walls[WALL_SOUTH].indx= 1;
    walls[WALL_SOUTH].sign= -1;
    walls[WALL_SOUTH].border_id = WALL_SOUTH ;
    clear_vec3(&walls[WALL_SOUTH].pos);
    walls[WALL_SOUTH].pos[1] = cnfg.PARTICLE_RADIUS;

    walls[WALL_REAR].indx= 2;
    walls[WALL_REAR].sign= 1;
    walls[WALL_REAR].border_id = WALL_REAR;
    clear_vec3(&walls[WALL_REAR].pos);
    walls[WALL_REAR].pos[2] = Z_CELLS * CELL_SIDE - cnfg.PARTICLE_RADIUS;

    walls[WALL_FRONT].indx= 2;
    walls[WALL_FRONT].sign= -1;
    walls[WALL_FRONT].border_id = WALL_FRONT;
    clear_vec3(&walls[WALL_FRONT].pos);
    walls[WALL_FRONT].pos[2] = cnfg.PARTICLE_RADIUS;
}

void copyCollisionPW(struct CollisionPW * _to, struct CollisionPW * _from)
{
    _to->id_particle = _from->id_particle;
    _to->wall_ID = _from->wall_ID;

    set_vec3(&_to->pos_0, _from->pos_0);

    set_vec3(&_to->theta_0, _from->theta_0);

    set_vec3(&_to->F_collision, _from->F_collision);

    set_vec3(&_to->moment_collision, _from->moment_collision);

#ifdef ENERGY
    _to->energy = _from->energy;
#endif
}

void updateCollisionPW(struct CollisionPW * _to, struct CollisionPW * _from)
{
    set_vec3(&_to->theta_0, _from->theta_0);

    set_vec3(&_to->F_collision, _from->F_collision);

    set_vec3(&_to->moment_collision, _from->moment_collision);

#ifdef ENERGY
    _to->energy = _from->energy;
#endif
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
    if (i>= collisions->N_PARTICLES || WALL_ID >= N_WALLS)
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
    collision_PW->wall_ID = WALL_ID;

    clear_vec3 (&collision_PW->pos_0);
    clear_vec3 (&collision_PW->theta_0);

    clear_vec3 (&collision_PW->F_collision);

    clear_vec3 (&collision_PW->moment_collision);

#ifdef ENERGY
    collision_PW->energy = 0.0;
#endif
}

bool deleteCollision_PW (struct Collisions * collisions, const int i, const int WALL_ID)
{
    if (i>= collisions->N_PARTICLES || WALL_ID >= N_WALLS)
    {
        return false; //ERROR
    }

    printf("sto cancellando la collisione wall tra (%d,%d | %d)\n", i,WALL_ID, WALL_ID);
    //we set the collision in the next matrix null, then during the update phase memory will be freed
    free (collisions->collisions_PW_next[i][WALL_ID]);
    collisions->collisions_PW_next[i][WALL_ID] = NULL;
    return true;
}

void setInitCollisionValues_PW (struct CollisionPW* collision_PW,
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
    if (i>= collisions->N_PARTICLES || WALL_ID >= N_WALLS)
    {
        return false; //ERROR
    }

    printf("sto aggiungendo la collisione p-wall tra (%d,%d)\n", i,WALL_ID);

    collisions->collisions_PW_next[i][WALL_ID] = (struct CollisionPW*)malloc(sizeof(struct CollisionPW));
    initializeCollisions_PW(collisions->collisions_PW_next[i][WALL_ID], i, WALL_ID);

    setInitCollisionValues_PW(collisions->collisions_PW_next[i][WALL_ID], p, theta, v, w, dtp);

    return collisions->collisions_PW_next[i][WALL_ID];
}

void setPos_i_PW(struct Collisions* collisions, const int i, const int WALL_ID, vec3* new_pos)
{
    if (i>= collisions->N_PARTICLES || WALL_ID >= N_WALLS)
    {
        return; //ERROR
    }
    set_vec3(&collisions->collisions_PW_next[i][WALL_ID]->pos_0, *new_pos);
}

void setForce_i_PW (struct Collisions* collisions, const int i, const int WALL_ID, vec3* force)
{
    if (i>= collisions->N_PARTICLES || WALL_ID >= N_WALLS)
    {
        return; //ERROR
    }
    set_vec3(&collisions->collisions_PW_next[i][WALL_ID]->F_collision, *force);
}


void setEnergy_i_PW (struct Collisions* collisions, const int i, const int WALL_ID, CALreal energy)
{
#ifdef ENERGY
    if (i>= collisions->N_PARTICLES || WALL_ID >= N_WALLS)
    {
        return; //ERROR
    }
    collisions->collisions_PW_next[i][WALL_ID]->energy= energy;
#endif
}


void updateForce_i_PW (struct Collisions* collisions, const int i, const int WALL_ID, vec3* force)
{
    if (i>= collisions->N_PARTICLES || WALL_ID >= N_WALLS)
    {
        return; //ERROR
    }
    add_vec3(&collisions->collisions_PW_next[i][WALL_ID]->F_collision,
             collisions->collisions_PW_next[i][WALL_ID]->F_collision, *force);
}

void setMoment_i_PW (struct Collisions* collisions, const int i, const int WALL_ID,vec3* moment)
{
    if (i>= collisions->N_PARTICLES || WALL_ID >= N_WALLS)
    {
        return; //ERROR
    }
    set_vec3(&collisions->collisions_PW_next[i][WALL_ID]->moment_collision, *moment);
}

void updateMoment_i_PW (struct Collisions* collisions, const int i, const int WALL_ID,vec3* moment)
{
    if (i>= collisions->N_PARTICLES || WALL_ID >= N_WALLS)
    {
        return; //ERROR
    }
    add_vec3(&collisions->collisions_PW_next[i][WALL_ID]->moment_collision,
             collisions->collisions_PW_next[i][WALL_ID]->moment_collision, *moment);

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

void totalElasticEnergyCollisionPW(struct Collisions* collisions, CALreal * energy, const int i)
{
#ifdef ENERGY
    for (int j = 0; j < N_WALLS; ++j) {
        if(collisions->collisions_PW_current[i][j] != NULL)
            *energy += collisions->collisions_PW_current[i][j]->energy;
    }
#endif
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

void cleanupCollisions_PW(struct Collisions* collisions)
{
    for (int i = 0; i < collisions->N_PARTICLES; i++)
    {
        for (int j = 0; j < N_WALLS; ++j) {
            if(collisions->collisions_PW_next[i][j] != NULL)
            {
                free (collisions->collisions_PW_next[i][j]);
                collisions->collisions_PW_next[i][j] = NULL;

            }
            if(collisions->collisions_PW_current[i][j] != NULL)
            {
                free (collisions->collisions_PW_current[i][j]);
                collisions->collisions_PW_current[i][j] = NULL;

            }
        }
        free (collisions->collisions_PW_next[i]);
        free (collisions->collisions_PW_current[i]);

    }
    free(collisions->collisions_PW_next);
    free(collisions->collisions_PW_current);

}


