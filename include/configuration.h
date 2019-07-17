#ifndef CONFIGURATION_H
#define CONFIGURATION_H


struct Configuration {
    double KA;
    double FRICTION_COEF_PP;
    double FRICTION_COEF_PW;

    double KN_PP;
    double KN_PW;

    double AL_PP;
    double AL_PW;

    double COEF_REST;

    double PARTICLE_MASS;
    double PARTICLE_RADIUS;
    double PARTICLE_VOLUME;
    double DM;
    double DM_2;

    double DELTA_T;

    double DENSITY;

    double G;
    int STEPS;

    int MAX_NUMBER_OF_PARTICLES_PER_CELL;
    double MOMENT_INERTIA;

    char file_energy_name[255];
    char file_particlesInfo_name[255];


};

void initConfig(char* file_name, struct Configuration * config);
void printProperties(struct Configuration * config);

#endif

