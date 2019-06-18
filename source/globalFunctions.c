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

#ifdef ENERGY
void updateEnergy(struct CALModel3D* ca)
{
    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
        calUpdateSubstate3Dr(ca,Q.kinetic_energy[slot]);
        calUpdateSubstate3Dr(ca,Q.potential_energy[slot]);
        calUpdateSubstate3Dr(ca,Q.rotational_energy[slot]);
        calUpdateSubstate3Dr(ca,Q.elastic_pp_energy[slot]);
        calUpdateSubstate3Dr(ca,Q.elastic_pw_energy[slot]);
    }
}

void updateTotalEnergy(struct CALModel3D* ca)
{
    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
        calUpdateSubstate3Dr(ca,Q.total_energy[slot]);
    }
}
#endif

void updateMoment(struct CALModel3D* ca)
{
    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
        calUpdateSubstate3Dr(ca,Q.momentx[slot]);
        calUpdateSubstate3Dr(ca,Q.momenty[slot]);
        calUpdateSubstate3Dr(ca,Q.momentz[slot]);
    }
}

void updateNP(struct CALModel3D* ca)
{
    calUpdateSubstate3Di(ca,Q.nP);
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
void printID_cell (struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{

    for (int slot=0; slot<MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
        printf("%d | ", calGet3Di(ca,Q.ID[slot],cell_x,cell_y,cell_z));

    printf("\n");
}

void printCount (struct CALModel3D* ca)
{

    int count = 0;
    for (int cell_x=0; cell_x<ca->rows; cell_x++)
    {
        for (int cell_y=0; cell_y<ca->columns; cell_y++)
            for (int cell_z = 0; cell_z<ca->slices; cell_z++)
            {

                count += calGet3Di(ca,Q.nP,cell_x,cell_y,cell_z);
                //                    printf("%d | ", calGet3Di(ca,Q.nP,cell_x,cell_y,cell_z));



            }
        //        printf("\n");
    }
    printf("count =  %d\n", count);
}



void transitionFunction(struct CALModel3D* modello)
{

    //    if (a_simulazioni->step >= 1566)
    //    {
    //        printID(modello);
    //    }

#if INTEGRATION_METHOD == LEAP_FROG
    calApplyElementaryProcess3D(modello, leap_frog_velocity);
#endif
#if INTEGRATION_METHOD == EULER_FORWARD_BACKWARD
    calApplyElementaryProcess3D(modello, euler_backward_forward_velocity);
#endif

    updateV(modello);
    updateW(modello);

#if INTEGRATION_METHOD == LEAP_FROG
    calApplyElementaryProcess3D(modello, leap_frog_positions);
#endif
#if INTEGRATION_METHOD == EULER_FORWARD_BACKWARD
    calApplyElementaryProcess3D(modello, euler_backward_forward_position);
#endif
    updateP(modello);
    updateTheta(modello);



    //  calApplyElementaryProcess3D(modello,movili);
    calApplyElementaryProcess3D(modello,moveParticles); //sposta le particelle nei nuovi celloni
    calUpdate3D(modello);

    //trasformare in elementary
    calApplyElementaryProcess3D(modello,countParticles);
    updateNP(modello);

#ifdef ENERGY

    calApplyElementaryProcess3D(modello, compute_kinetic_energy);
    calApplyElementaryProcess3D(modello, compute_rotational_energy);
    calApplyElementaryProcess3D(modello, compute_potential_energy);
    updateEnergy(modello);
    calApplyElementaryProcess3D(modello,compute_total_energy);
    updateTotalEnergy(modello);
#endif

    calApplyElementaryProcess3D(modello,inner_collision);
    calApplyElementaryProcess3D(modello,outer_collision);
    calApplyElementaryProcess3D(modello,walls_collision);

    updateCollisionsPP(&collisions);
    updateCollisionsPW(&collisions);

#ifdef VERBOSE
    printSummary(modello);
#ifdef ENERGY
    saveTotalEnergy(modello, a_simulazioni->step, elapsed_time, total_energy_file);
    saveParticleInfo(modello, a_simulazioni->step, elapsed_time, particle_info_file);
#endif
#endif

#ifdef ENERGY


    calApplyElementaryProcess3D(modello, setToZeroEnergy);
    calApplyElementaryProcess3D(modello,total_elastic_energy_pp);
    calApplyElementaryProcess3D(modello,total_elastic_energy_pw);


//    calApplyElementaryProcess3D(modello,compute_total_energy);
//    updateTotalEnergy(modello);
#endif

    calApplyElementaryProcess3D(modello,applyForce);


    updateF(modello);
    updateMoment(modello);



    clearForces_PP(&collisions);
    clearForces_PW(&collisions);


#if INTEGRATION_METHOD == LEAP_FROG
    calApplyElementaryProcess3D(modello, leap_frog_velocity);
#endif
    updateV(modello);
    updateW(modello);

    //        printID(modello);

    elapsed_time += cnfg.DELTA_T;



    CALint S = cnfg.STEPS;
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




void cleanup (struct CALModel3D* modello)
{
    cleanupCollisions_PP(&collisions);
    cleanupCollisions_PW(&collisions);

#ifdef ENERGY
    if (!total_energy_file)
        fclose(total_energy_file);
#endif
}
