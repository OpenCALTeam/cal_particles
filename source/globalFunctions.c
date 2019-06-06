#include <globalFunctions.h>

void updateF(struct CALModel3D* ca)
{
    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
        calUpdateSubstate3Dr(ca,Q.Fx[slot]);
        calUpdateSubstate3Dr(ca,Q.Fy[slot]);
        calUpdateSubstate3Dr(ca,Q.Fz[slot]);
    }
}

void updateMoment(struct CALModel3D* ca)
{
    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
        calUpdateSubstate3Dr(ca,Q.momentx[slot]);
        calUpdateSubstate3Dr(ca,Q.momenty[slot]);
        calUpdateSubstate3Dr(ca,Q.momentz[slot]);
    }
}

void updateP(struct CALModel3D* ca)
{
    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
        calUpdateSubstate3Dr(ca,Q.px[slot]);
        calUpdateSubstate3Dr(ca,Q.py[slot]);
        calUpdateSubstate3Dr(ca,Q.pz[slot]);
    }
}

void updateTheta(struct CALModel3D* ca)
{
    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
        calUpdateSubstate3Dr(ca,Q.thetax[slot]);
        calUpdateSubstate3Dr(ca,Q.thetay[slot]);
        calUpdateSubstate3Dr(ca,Q.thetaz[slot]);
    }
}

void updateV(struct CALModel3D* ca)
{
    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
        calUpdateSubstate3Dr(ca,Q.vx[slot]);
        calUpdateSubstate3Dr(ca,Q.vy[slot]);
        calUpdateSubstate3Dr(ca,Q.vz[slot]);
    }
}

void updateW(struct CALModel3D* ca)
{
    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
        calUpdateSubstate3Dr(ca,Q.wx[slot]);
        calUpdateSubstate3Dr(ca,Q.wy[slot]);
        calUpdateSubstate3Dr(ca,Q.wz[slot]);
    }
}

void transitionFunction(struct CALModel3D* modello)
{
    calApplyElementaryProcess3D(modello, leap_frog_velocity);
    updateV(modello);
    updateW(modello);

    calApplyElementaryProcess3D(modello, leap_frog_positions);
    updateP(modello);
    updateTheta(modello);

    //  calApplyElementaryProcess3D(modello,movili);
    calApplyElementaryProcess3D(modello,moveParticles); //sposta le particelle nei nuovi celloni
    calUpdate3D(modello);

    calApplyElementaryProcess3D(modello,inner_collision);
    calApplyElementaryProcess3D(modello,outer_collision);
    calApplyElementaryProcess3D(modello,walls_collision);

    updateCollisionsPP(&collisions);
    updateCollisionsPW(&collisions);

    calApplyElementaryProcess3D(modello,applyForce);


    updateF(modello);
    updateMoment(modello);

    clearForces_PP(&collisions);
    clearForces_PW(&collisions);

    //    calApplyElementaryProcess3D(modello, leap_frog_velocity);
    //    updateV(modello);
    //    updateW(modello);

    //    printID(modello);

    elapsed_time += DELTA_T;

#ifdef VERBOSE
    printSummary(modello);
#endif

    CALint S = INTEGRITY_CHECK_STEPS;
    if (a_simulazioni->step % S == 0)
    {
        CALint missing_particle = findMissingParticle(modello);
        if (missing_particle)
        {
            printf("ERROR: missing particle with ID %d\n", missing_particle);
            exit(EXIT_FAILURE);
        }
    }
}

void printID (struct CALModel3D* ca)
{
    for (int cell_x=0; cell_x<ca->rows; cell_x++)
    {
        for (int cell_y=0; cell_y<ca->columns; cell_y++)
            for (int cell_z = 0; cell_z<ca->slices; cell_z++)
            {
                for (int slot=0; slot<MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
                    printf("%d | ", calGet3Di(ca,Q.ID[slot],cell_x,cell_y,cell_z));



            }
        printf("\n");
    }
}

void cleanupCollisions (struct CALModel3D* modello)
{
    cleanupCollisions_PP(&collisions);
    cleanupCollisions_PW(&collisions);
}
