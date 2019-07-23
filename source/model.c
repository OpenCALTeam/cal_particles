#include <boundary.h>
#include <ep_collision.h>
#include <ep_movili_cazzu.h>
#include <ep_physics.h>
#include <ep_utils.h>
#include <init.h>
#include <model.h>
#include <utils_io.h>
#include <sim_stop.h>
#include <stdlib.h>

#include<init_pp.h>

#include <globalFunctions.h>
#include <restart_manager.h>


void run()
{

    char path_config[256];
    sprintf(path_config, "./config/config%d.properties", TEST_CASE);
    initConfig(path_config, &cnfg);
    printProperties(&cnfg);

    u_modellu = calCADef3D(X_CELLS,Y_CELLS,Z_CELLS,CAL_MOORE_NEIGHBORHOOD_3D,CAL_SPACE_TOROIDAL,OPTIMITAZION_ACTIVE_CELLS);

    Q.Fx = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
    Q.Fy = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
    Q.Fz = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);

    Q.momentx = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
    Q.momenty = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
    Q.momentz = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);

    Q.px = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
    Q.py = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
    Q.pz = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);

    Q.thetax = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
    Q.thetay = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
    Q.thetaz = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);

    Q.vx = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
    Q.vy = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
    Q.vz = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);

    Q.wx = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
    Q.wy = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
    Q.wz = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);

    Q.ID = (struct CALSubstate3Di**)malloc(sizeof(struct CALSubstate3Di*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
    Q.nP = (struct CALSubstate3Di*)malloc(sizeof(struct CALSubstate3Di)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);

#ifdef ENERGY
    Q.total_energy = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
    Q.kinetic_energy = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
    Q.potential_energy = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
    Q.rotational_energy = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
    Q.elastic_pp_energy = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
    Q.elastic_pw_energy = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)* cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL);
#endif
    for(int slot=0;slot< cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL;slot++)
    {
        Q.Fx[slot] = calAddSubstate3Dr(u_modellu);
        Q.Fy[slot] = calAddSubstate3Dr(u_modellu);
        Q.Fz[slot] = calAddSubstate3Dr(u_modellu);

        Q.momentx[slot] = calAddSubstate3Dr(u_modellu);
        Q.momenty[slot] = calAddSubstate3Dr(u_modellu);
        Q.momentz[slot] = calAddSubstate3Dr(u_modellu);

        Q.px[slot] = calAddSubstate3Dr(u_modellu);
        Q.py[slot] = calAddSubstate3Dr(u_modellu);
        Q.pz[slot] = calAddSubstate3Dr(u_modellu);

        Q.thetax[slot] = calAddSubstate3Dr(u_modellu);
        Q.thetay[slot] = calAddSubstate3Dr(u_modellu);
        Q.thetaz[slot] = calAddSubstate3Dr(u_modellu);

        Q.vx[slot] = calAddSubstate3Dr(u_modellu);
        Q.vy[slot] = calAddSubstate3Dr(u_modellu);
        Q.vz[slot] = calAddSubstate3Dr(u_modellu);

        Q.wx[slot] = calAddSubstate3Dr(u_modellu);
        Q.wy[slot] = calAddSubstate3Dr(u_modellu);
        Q.wz[slot] = calAddSubstate3Dr(u_modellu);

        Q.ID[slot] = calAddSubstate3Di(u_modellu);

        Q.nP = calAddSubstate3Di(u_modellu);

#ifdef ENERGY
        Q.total_energy[slot] = calAddSubstate3Dr(u_modellu);
        Q.potential_energy[slot] = calAddSubstate3Dr(u_modellu);
        Q.rotational_energy[slot] = calAddSubstate3Dr(u_modellu);
        Q.kinetic_energy[slot] = calAddSubstate3Dr(u_modellu);
        Q.elastic_pp_energy[slot] = calAddSubstate3Dr(u_modellu);
        Q.elastic_pw_energy[slot] = calAddSubstate3Dr(u_modellu);

#endif

        calInitSubstate3Dr(u_modellu,Q.Fx[slot],0.0);
        calInitSubstate3Dr(u_modellu,Q.Fy[slot],0.0);
        calInitSubstate3Dr(u_modellu,Q.Fz[slot],0.0);

        calInitSubstate3Dr(u_modellu,Q.momentx[slot],0.0);
        calInitSubstate3Dr(u_modellu,Q.momenty[slot],0.0);
        calInitSubstate3Dr(u_modellu,Q.momentz[slot],0.0);

        calInitSubstate3Dr(u_modellu,Q.px[slot],0.0);
        calInitSubstate3Dr(u_modellu,Q.py[slot],0.0);
        calInitSubstate3Dr(u_modellu,Q.pz[slot],0.0);

        calInitSubstate3Dr(u_modellu,Q.thetax[slot],0.0);
        calInitSubstate3Dr(u_modellu,Q.thetay[slot],0.0);
        calInitSubstate3Dr(u_modellu,Q.thetaz[slot],0.0);

        calInitSubstate3Dr(u_modellu,Q.vx[slot],0.0);
        calInitSubstate3Dr(u_modellu,Q.vy[slot],0.0);
        calInitSubstate3Dr(u_modellu,Q.vz[slot],0.0);

        calInitSubstate3Dr(u_modellu,Q.wx[slot],0.0);
        calInitSubstate3Dr(u_modellu,Q.wy[slot],0.0);
        calInitSubstate3Dr(u_modellu,Q.wz[slot],0.0);
        calInitSubstate3Di(u_modellu,Q.ID[slot],NULL_ID);

#ifdef ENERGY
        calInitSubstate3Dr(u_modellu,Q.total_energy[slot],0.0);
        calInitSubstate3Dr(u_modellu,Q.potential_energy[slot],0.0);
        calInitSubstate3Dr(u_modellu,Q.rotational_energy[slot],0.0);
        calInitSubstate3Dr(u_modellu,Q.kinetic_energy[slot],0.0);
        calInitSubstate3Dr(u_modellu,Q.elastic_pp_energy[slot],0.0);
        calInitSubstate3Dr(u_modellu,Q.elastic_pw_energy[slot],0.0);
#endif


    }
    calInitSubstate3Di(u_modellu,Q.nP,0);


    // Boundary
#ifdef GUI
    boundaryCellsSerial(u_modellu);
#endif



    //    exit(EXIT_SUCCESS);


#ifdef ENERGY
    total_energy_file = fopen(cnfg.file_energy_name, "w");
    if ( !total_energy_file )
    {
        printf ("Unable to open %s.\n", cnfg.file_energy_name);
        exit(EXIT_FAILURE);
    }

    particle_info_file = fopen(cnfg.file_particlesInfo_name, "w");
    if ( !particle_info_file )
    {
        printf ("Unable to open %s.\n", cnfg.file_particlesInfo_name);
        exit(EXIT_FAILURE);
    }
#endif



    // Initial conditions
    initial_nummber_of_particles = 1;
    elapsed_time = 0.0;
    int initial_step=0;

    initWalls(walls);




#if TEST_CASE == TEST_CASE_SUPERBALL
    vec3 p_0, v_0, w_0;
    clear_vec3(&v_0);
    v_0[0] = 0.1;
    clear_vec3(&w_0);
    //v_0[2] = -0.1;
    w_0[1]= 500;

    p_0[0]=0.02;
    p_0[1]=0.02;
    p_0[2]=0.001001;

    addParticleWithPosition(u_modellu, p_0, v_0,w_0, &initial_nummber_of_particles);

#elif TEST_CASE == TEST_CASE_TWO_PP_VEL
    vec3 p_0, v_0, w_0;
    clear_vec3(&v_0);
    v_0[0] = 0.8;
    clear_vec3(&w_0);

    p_0[0]=0.02;
    p_0[1]=0.02;
    p_0[2]=0.02;
    addParticleWithPosition(u_modellu, p_0, v_0,w_0, &initial_nummber_of_particles);

    p_0[0]=0.0211;
    p_0[1]=0.02;
    p_0[2]=0.02;
    v_0[0] = -0.8;
    addParticleWithPosition(u_modellu, p_0, v_0,w_0, &initial_nummber_of_particles);

#elif TEST_CASE == TEST_CASE_TWO_PP_VEL_OMEGA
    vec3 p_0, v_0, w_0;
    clear_vec3(&v_0);
    v_0[0] = +1.0;
    clear_vec3(&w_0);
    w_0[2] = 500.0;

    p_0[0]=0.02;
    p_0[1]=0.02;
    p_0[2]=0.02;

//    w_0[0] = 40.0;

    addParticleWithPosition(u_modellu, p_0, v_0,w_0, &initial_nummber_of_particles);

    clear_vec3(&v_0);
    clear_vec3(&w_0);
    p_0[0]=0.02+cnfg.PARTICLE_RADIUS*2.3;
    p_0[1]=0.02;
    p_0[2]=0.02;
//    v_0[0] = -2.2;

//    w_0[1] = -30.0;

    addParticleWithPosition(u_modellu, p_0, v_0,w_0, &initial_nummber_of_particles);

#elif TEST_CASE == TEST_CASE_VEL_WALL
    vec3 p_0, v_0, w_0;
    clear_vec3(&v_0);

    clear_vec3(&w_0);

    p_0[0]=0.039;
    p_0[1]=0.02;
    p_0[2]=0.02;

    v_0[0] = 0.9;

    addParticleWithPosition(u_modellu, p_0, v_0,w_0, &initial_nummber_of_particles);

#elif TEST_CASE == TEST_CASE_VEL_WALL_VERTICAL

    vec3 p_0, v_0, w_0;
    clear_vec3(&v_0);
    //    v_0[0] = 0.1;
    clear_vec3(&w_0);
    //    v_0[2] = -0.1;

    p_0[0]=0.02;
    p_0[1]=0.02;
    p_0[2]=0.001101;

    addParticleWithPosition(u_modellu, p_0, v_0,w_0, &initial_nummber_of_particles);
#elif TEST_CASE == TEST_CASE_VEL_WALL_DIAGONAL
    vec3 p_0, v_0, w_0;
    clear_vec3(&v_0);
    v_0[0] = 0.5;
    clear_vec3(&w_0);
    v_0[2] = -0.5;
    w_0[1]= 0;

    p_0[0]=0.02;
    p_0[1]=0.02;
    p_0[2]=0.0011;

    addParticleWithPosition(u_modellu, p_0, v_0,w_0, &initial_nummber_of_particles);
#elif TEST_CASE == TEST_CASE_VEL_WALL_DIAGONAL_GRAVITY
    vec3 p_0, v_0, w_0;
    clear_vec3(&v_0);
    v_0[0] = 0.5;
    clear_vec3(&w_0);

    p_0[0]=0.02;
    p_0[1]=0.02;
    p_0[2]=0.0011;
    addParticleWithPosition(u_modellu, p_0, v_0,w_0, &initial_nummber_of_particles);


#elif TEST_CASE == TEST_CASE_SANDWICH
    vec3 p_0, v_0, w_0;
    //    clear_vec3(&v_0);
    //    v_0[0] = 10.5;
    //    clear_vec3(&w_0);
    //    w_0[0] = 20.0;

    //    p_0[0]=0.02;
    //    p_0[1]=0.02;
    //    p_0[2]=0.02;
    //    addParticleWithPosition(u_modellu, p_0, v_0,w_0, &initial_nummber_of_particles);

    //    clear_vec3(&v_0);
    //    v_0[0] = -10.5;
    //    clear_vec3(&w_0);
    //    w_0[0] = -20.0;

    //    p_0[0]=0.02 + cnfg.PARTICLE_RADIUS*6;
    //    p_0[1]=0.02;
    //    p_0[2]=0.02;
    //    addParticleWithPosition(u_modellu, p_0, v_0,w_0, &initial_nummber_of_particles);

    clear_vec3(&v_0);

    clear_vec3(&w_0);
    w_0[2] = -50.0;
    v_0[1] = -1.5;

    p_0[0]=0.02 + cnfg.PARTICLE_RADIUS*3;
    p_0[1]=0.02;
    p_0[2]=0.02;
    addParticleWithPosition(u_modellu, p_0, v_0,w_0, &initial_nummber_of_particles);

    //    clear_vec3(&v_0);
    //    v_0[1] = -5.5;
    //    clear_vec3(&w_0);
    //    w_0[1] = -50.0;

    //    p_0[0]=0.02 + cnfg.PARTICLE_RADIUS*3;
    //    p_0[1]=0.02 + cnfg.PARTICLE_RADIUS*3;
    //    p_0[2]=0.02;
    //    addParticleWithPosition(u_modellu, p_0, v_0,w_0, &initial_nummber_of_particles);

    clear_vec3(&v_0);
    v_0[1] = +1.5;
    clear_vec3(&w_0);
    w_0[1] = +50.0;

    p_0[0]=0.02 + cnfg.PARTICLE_RADIUS*3;
    p_0[1]=0.02 - cnfg.PARTICLE_RADIUS*3;
    p_0[2]=0.02;
    addParticleWithPosition(u_modellu, p_0, v_0,w_0, &initial_nummber_of_particles);

#elif TEST_CASE == TEST_CASE_BILIARD_BALL
    vec3 p_0, v_0, w_0;
    clear_vec3(&v_0);
    v_0[0] = +1.0;
    clear_vec3(&w_0);
//    w_0[1] = 0;

    p_0[0]=0.02;
    p_0[1]=0.02+cnfg.PARTICLE_RADIUS;
    p_0[2]=0.02;

//    w_0[0] = 40.0;

    addParticleWithPosition(u_modellu, p_0, v_0,w_0, &initial_nummber_of_particles);

    clear_vec3(&v_0);
    clear_vec3(&w_0);
    p_0[0]=0.02+cnfg.PARTICLE_RADIUS*1.75;
    p_0[1]=0.02;
    p_0[2]=0.02;
//    v_0[0] = -2.2;

//    w_0[1] = -30.0;

    addParticleWithPosition(u_modellu, p_0, v_0,w_0, &initial_nummber_of_particles);
#elif TEST_CASE == TEST_CASE_STRUCTURED
    addStructuredParticles(u_modellu, &initial_nummber_of_particles);


#elif TEST_CASE == RESTART
    initial_step = loadSnapshotParticles(u_modellu, "./snapshots/particle_configuration40300.dat", & initial_nummber_of_particles);
    //    initial_step = loadSnapshotParticles(u_modellu, "./snapshots/particle_configuration_chimici.dat", & initial_nummber_of_particles);

    initial_step++;
    elapsed_time = cnfg.DELTA_T * (initial_step+1);

#else
    mmiscali_nta_cella_seriale(u_modellu);
    cancella_particelle_in_urto(u_modellu);

    //    for (int i = 0; i < TOTAL_NUMBER_PARTICLE; ++i) {
    //        addRandomParticlePosition(u_modellu, &initial_nummber_of_particles);
    //    }

#endif




    printf( "initial_nummber_of_particles = %d\n", initial_nummber_of_particles-1);


    //TODO vedere perchè è +1 è -1 nell'altro caso di inizializzazione
    collisions.N_PARTICLES = initial_nummber_of_particles+1;
    initCollisionsPP(&collisions);


    initCollisionsPW(&collisions);

#if TEST_CASE == RESTART
    //    loadSnapshotCollisionsParticles(&collisions, "./snapshots/collisions_particles_configuration_chimici.dat");
    //    loadSnapshotCollisionsWalls(&collisions, "./snapshots/collisions_wall_configuration_chimici.dat");

    loadSnapshotCollisionsParticles(&collisions, "./snapshots/collisions_particles_configuration40300.dat");
    loadSnapshotCollisionsWalls(&collisions, "./snapshots/collisions_wall_configuration40300.dat");
    updateCollisionsPP(&collisions);
    updateCollisionsPW(&collisions);
#endif

    // Simulation
    a_simulazioni = calRunDef3D(u_modellu,initial_step,CAL_RUN_LOOP,CAL_UPDATE_EXPLICIT);

#if OPTIMITAZION_ACTIVE_CELLS == 1
    //    calRunAddInitFunc3D(a_simulazioni, setActiveCells);
    setActiveCells(u_modellu);
#endif

    calRunAddGlobalTransitionFunc3D(a_simulazioni, transitionFunction);
    calRunAddStopConditionFunc3D(a_simulazioni, caminalu);

    calRunAddFinalizeFunc3D(a_simulazioni, cleanup);


#ifdef VERBOSE
    printf("The 3D particles computational model\n");
#ifdef OMP
    printf("OpenMP parallel execution enabled!\n");
#endif
    printf("A system of %d particles will be simulated for %f s, subdivided in %d steps, each one corresponding to %f s\n", initial_nummber_of_particles, TOTAL_SIMULATION_TIME, cnfg.STEPS, cnfg.DELTA_T);
#endif
}
