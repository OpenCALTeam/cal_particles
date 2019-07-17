#include <configuration.h>

#include <common.hpp>
#include <utils_io.h>
#include<math.h>

void initConfig(char* file_name, struct Configuration * config)
{
    readProperties(file_name, config);

    config->PARTICLE_VOLUME = PIG43 * config->PARTICLE_RADIUS * config->PARTICLE_RADIUS * config->PARTICLE_RADIUS;


    if(config->DENSITY > 0.0)
    {
        config->PARTICLE_MASS = config->DENSITY * config->PARTICLE_VOLUME;
    }

    if(config->COEF_REST > 0.0)
    {
        config->AL_PP = sqrt((4*config->PARTICLE_MASS*config->KN_PP)/ (1+ pow((PIG/log(config->COEF_REST)),2)));
        config->AL_PW = sqrt((4*config->PARTICLE_MASS*config->KN_PW)/ (1+ pow((PIG/log(config->COEF_REST)),2)));

    }

    printf(" config->AL_PP %.7f config->COEF_REST = %.7f \n", config->AL_PP, config->COEF_REST);

    config->MAX_NUMBER_OF_PARTICLES_PER_CELL = (int)(((MAX_OCCUPANCY_VOLUME)/(config->PARTICLE_VOLUME))+1);
    config->DM = 2* config->PARTICLE_RADIUS;
    config->DM_2 = config->DM*config->DM;

#ifdef OMP
    sprintf(config->file_energy_name, "%s_%d_omp.dat", config->file_energy_name,OPTIMITAZION_ACTIVE_CELLS);
    sprintf(config->file_particlesInfo_name, "%s_%d_omp.dat", config->file_particlesInfo_name,OPTIMITAZION_ACTIVE_CELLS);
#else
    sprintf(config->file_energy_name, "%s_%d.dat", config->file_energy_name,OPTIMITAZION_ACTIVE_CELLS);
    sprintf(config->file_particlesInfo_name, "%s_%d.dat", config->file_particlesInfo_name,OPTIMITAZION_ACTIVE_CELLS);
#endif

    if(config->STEPS <= 0.0)
    {
        config-> STEPS =(int)((double)(TOTAL_SIMULATION_TIME)/(double)(config->DELTA_T));
    }

    config->MOMENT_INERTIA= ((0.4 * config->PARTICLE_MASS) * config->PARTICLE_RADIUS*config->PARTICLE_RADIUS);
}

void printProperties(struct Configuration * config)
{

    printf("++++++++++++++++++++++++++++++++++++++++\n");
    printf("KA=%.10f\n", config->KA);
    printf("FRICTION_COEF_PP=%.10f\n", config->FRICTION_COEF_PP);
    printf("FRICTION_COEF_PW=%.10f\n", config->FRICTION_COEF_PW);
    printf("KN_PP=%.10f\n", config->KN_PP);
    printf("KN_PW=%.10f\n", config->KN_PW);

    printf("AL_PP=%.10f\n", config->AL_PP);
    printf("AL_PW=%.10f\n", config->AL_PW);

    printf("COEF_REST=%.10f\n", config->COEF_REST);

    printf("PARTICLE_MASS=%.10f\n", config->PARTICLE_MASS);
    printf("PARTICLE_RADIUS=%.10f\n", config->PARTICLE_RADIUS);
    printf("PARTICLE_VOLUME=%.10f\n", config->PARTICLE_VOLUME);
    printf("DELTA_T=%.10f\n", config->DELTA_T);
    printf("G=%.5f\n", config->G);
    printf("STEPS=%d\n", config->STEPS);
    printf("MAX_NUMBER_OF_PARTICLES_PER_CELL=%d\n", config->MAX_NUMBER_OF_PARTICLES_PER_CELL);
    printf("DENSITY=%.10f\n", config->DENSITY);
    printf("MOMENT_INERTIA=%.17f\n", config->MOMENT_INERTIA);
    printf("file_energy_name=%s\n", config->file_energy_name);
    printf("file_particlesInfo_name=%s\n", config->file_particlesInfo_name);
    printf("++++++++++++++++++++++++++++++++++++++++\n\n");

}
