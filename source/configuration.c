#include <configuration.h>

#include <common.hpp>
#include <utils_io.h>

void initConfig(char* file_name, struct Configuration * config)
{
    readProperties(file_name, config);

    if(config->DENSITY > 0.0)
    {
        double volume = PIG43 * config->PARTICLE_RADIUS;
        volume = volume * volume * volume;
        config->PARTICLE_MASS = config->DENSITY * volume;

    }

    config->DM = 2* config->PARTICLE_RADIUS;
    config->DM_2 = config->DM*config->DM;

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

    printf("PARTICLE_MASS=%.10f\n", config->PARTICLE_MASS);
    printf("PARTICLE_RADIUS=%.10f\n", config->PARTICLE_RADIUS);
    printf("DELTA_T=%.10f\n", config->DELTA_T);
    printf("DELTA_T=%.5f\n", config->G);
    printf("STEPS=%d\n", config->STEPS);
    printf("DENSITY=%.10f\n", config->DENSITY);
    printf("MOMENT_INERTIA=%.17f\n", config->MOMENT_INERTIA);
    printf("file_energy_name=%s\n", config->file_energy_name);
    printf("file_particlesInfo_name=%s\n", config->file_particlesInfo_name);
    printf("++++++++++++++++++++++++++++++++++++++++\n\n");

}
