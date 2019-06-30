#include "restart_manager.h"
#include <string.h>
#include<init_pp.h>

void saveConfigurationParticles(struct CALModel3D *ca, CALint step, char* path)
{
    FILE* f;
    f = fopen(path, "w");

    if ( !f )
    {
        printf ("Unable to open %s.\n", path);
        exit(EXIT_FAILURE);
    }

    vec3 p, v, w, theta;
    CALint particle_id;


    fprintf(f, "Current_step: %d\n", step);
    fprintf(f, "id     \tp[0]     \tp[1]     \tp[2]     \tv[0]     \tv[1]     \tv[2]     \ttheta[0]     \ttheta[1]     \ttheta[2] \tw[0]     \tw[1]     \tw[2]\n");

    for (int cell_x=0; cell_x<ca->rows; cell_x++)
        for (int cell_y=0; cell_y<ca->columns; cell_y++)
            for (int cell_z = 0; cell_z<ca->slices; cell_z++)
                for (int slot = 0; slot < cnfg.MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
                    if (calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z) > NULL_ID)
                    {
                        p[0] = calGet3Dr(ca, Q.px[slot],cell_x,cell_y,cell_z);
                        p[1] = calGet3Dr(ca, Q.py[slot],cell_x,cell_y,cell_z);
                        p[2] = calGet3Dr(ca, Q.pz[slot],cell_x,cell_y,cell_z);

                        v[0] = calGet3Dr(ca, Q.vx[slot],cell_x,cell_y,cell_z);
                        v[1] = calGet3Dr(ca, Q.vy[slot],cell_x,cell_y,cell_z);
                        v[2] = calGet3Dr(ca, Q.vz[slot],cell_x,cell_y,cell_z);

                        theta[0] = calGet3Dr(ca, Q.thetax[slot],cell_x,cell_y,cell_z);
                        theta[1] = calGet3Dr(ca, Q.thetay[slot],cell_x,cell_y,cell_z);
                        theta[2] = calGet3Dr(ca, Q.thetaz[slot],cell_x,cell_y,cell_z);


                        w[0] = calGet3Dr(ca, Q.wx[slot],cell_x,cell_y,cell_z);
                        w[1] = calGet3Dr(ca, Q.wy[slot],cell_x,cell_y,cell_z);
                        w[2] = calGet3Dr(ca, Q.wz[slot],cell_x,cell_y,cell_z);

                        particle_id = calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z);



                        fprintf(f, "%d\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\n", particle_id, p[0], p[1], p[2], v[0], v[1], v[2], theta[0], theta[1], theta[2], w[0], w[1], w[2]);
                    }


    fclose(f);
}



void saveConfigurationCollisionsWall(struct Collisions * collisions, CALint step, char* path)
{
    FILE* f;
    f = fopen(path, "w");

    if ( !f )
    {
        printf ("Unable to open %s.\n", path);
        exit(EXIT_FAILURE);
    }
    fprintf(f, "Current_step: %d\n", step);
    fprintf(f, "id_particle     \tid_wall     \tp_0[0]     \tp_0[1] \tp_0[2]     \ttheta_0[0]     \ttheta_0[1]     \ttheta_0[2]\n");


    for (int i = 0; i < collisions->N_PARTICLES; i++)
    {
        for (int j = 0; j < N_WALLS; ++j)
        {
            if(collisions->collisions_PW_current[i][j] != NULL)
                fprintf(f, "%d\t%d\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\n", collisions->collisions_PW_current[i][j]->id_particle,
                        collisions->collisions_PW_current[i][j]->wall_ID, collisions->collisions_PW_current[i][j]->pos_0[0],
                        collisions->collisions_PW_current[i][j]->pos_0[1], collisions->collisions_PW_current[i][j]->pos_0[2],
                        collisions->collisions_PW_current[i][j]->theta_0[0],collisions->collisions_PW_current[i][j]->theta_0[1],
                        collisions->collisions_PW_current[i][j]->theta_0[2]);

        }
    }


    fclose(f);


}



void saveConfigurationCollisionsParticles(struct Collisions * collisions, CALint step, char* path)
{

    FILE* f;
    f = fopen(path, "w");

    if ( !f )
    {
        printf ("Unable to open %s.\n", path);
        exit(EXIT_FAILURE);
    }
    fprintf(f, "Current_step: %d\n", step);
    fprintf(f, "id_particle_i     \id_particle_j     \tpos_i_0[0]     \tpos_i_0[1] \tpos_i_0[2]     \tpos_j_0[0]     \tpos_j_0[1]     \tpos_j_0[2]"
               "\ttheta_i_0[0]     \ttheta_i_0[1] \ttheta_i_0[2]     \ttheta_j_0[0]     \ttheta_j_0[1]     \ttheta_j_0[2]"
               "\tvers_R_c_0[0]     \tvers_R_c_0[1]     \tvers_R_c_0[2]\n");


    for (int i = 0; i < collisions->N_PARTICLES; i++)
    {
        for (int j = 0; j < collisions->N_PARTICLES-i; ++j)
        {
            if(collisions->collisions_PP_current[i][j] != NULL)
            {
                fprintf(f, "%d\t%d\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\n",
                        collisions-> collisions_PP_current[i][j]->id_i,
                        collisions->collisions_PP_current[i][j]->id_j, collisions->collisions_PP_current[i][j]->pos_i_0[0],
                        collisions->collisions_PP_current[i][j]->pos_i_0[1], collisions->collisions_PP_current[i][j]->pos_i_0[2],
                        collisions->collisions_PP_current[i][j]->pos_j_0[0], collisions->collisions_PP_current[i][j]->pos_j_0[1],
                        collisions->collisions_PP_current[i][j]->pos_j_0[2], collisions->collisions_PP_current[i][j]->theta_i_0[0],
                        collisions->collisions_PP_current[i][j]->theta_i_0[1], collisions->collisions_PP_current[i][j]->theta_i_0[2],
                        collisions->collisions_PP_current[i][j]->theta_j_0[0], collisions->collisions_PP_current[i][j]->theta_j_0[1],
                        collisions->collisions_PP_current[i][j]->theta_j_0[2], collisions->collisions_PP_current[i][j]->vers_R_c_0[0],
                        collisions->collisions_PP_current[i][j]->vers_R_c_0[1], collisions->collisions_PP_current[i][j]->vers_R_c_0[2]);
            }

        }
    }

    fclose(f);


}

int loadSnapshotParticles (struct CALModel3D *ca, char* file_name, int *numberOfParticles)
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        printf ("Unable to open %s\n", file_name);
        exit(EXIT_FAILURE);

    }

    double value;

    vec3 p, v, w, theta;
    CALint particle_id, steps;

    while ((read = getline(&line, &len, fp)) != -1) {


        char *value_string = strtok(line, " \t\n");
        if(strcmp(value_string, "Current_step:") == 0)
        {

            value_string = strtok(NULL, " \t\n");
            steps = strtod(value_string,NULL);

            printf("step: %f\n ", steps);
            //            a_simulazioni->step = value;


        }
        else if(strcmp(value_string, "id") == 0)
        {
            continue;
        }
        else
        {
            particle_id =(int) strtod(value_string,NULL);
            printf("particle_id: %d\n ", particle_id);

            value_string = strtok(NULL, " \t\n");
            p[0] =strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            p[1] = strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            p[2] = strtod(value_string,NULL);


            value_string = strtok(NULL, " \t\n");
            v[0] = strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            v[1] = strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            v[2] = strtod(value_string,NULL);

            value_string = strtok(NULL, " \t\n");
            theta[0] = strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            theta[1] = strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            theta[2] = strtod(value_string,NULL);

            value_string = strtok(NULL, " \t\n");
            w[0] = strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            w[1] = strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            w[2] = strtod(value_string,NULL);

            if(addParticleWithFullConfiguration(ca,p,v,w,theta,particle_id,numberOfParticles ))
            {
                printf( "aggiunta particella a pos : %.6f %.6f %.6f con id %d \n", p[0],p[1],p[2], particle_id);
            }


        }

    }

    fclose(fp);
    if (line)
        free(line);
    return steps;
}

void loadSnapshotCollisionsParticles (struct Collisions * collisions, char* file_name)
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        printf ("Unable to open %s\n", file_name);
        exit(EXIT_FAILURE);

    }

    double value;

    vec3 pos_i_0, pos_j_0, theta_i_0, theta_j_0, vers_R_c_0, zero_vec;
    CALint id_particle_i, id_particle_j;

    clear_vec3(&zero_vec);

    while ((read = getline(&line, &len, fp)) != -1) {



        char *value_string = strtok(line, " \t\n");
        if( strcmp(value_string, "Current_step:") == 0 || strcmp(value_string, "id_particle_i") == 0)
        {
            continue;
        }
        else
        {
            id_particle_i =(int) strtod(value_string,NULL);
            printf("id_particle_i: %d\n ", id_particle_i);
            value_string = strtok(NULL, " \t\n");
            id_particle_j =(int) strtod(value_string,NULL);
            printf("id_particle_j: %d\n ", id_particle_j);

            value_string = strtok(NULL, " \t\n");
            pos_i_0[0] =strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            pos_i_0[1] = strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            pos_i_0[2] = strtod(value_string,NULL);

            value_string = strtok(NULL, " \t\n");
            pos_j_0[0] =strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            pos_j_0[1] = strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            pos_j_0[2] = strtod(value_string,NULL);


            value_string = strtok(NULL, " \t\n");
            theta_i_0[0] = strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            theta_i_0[1] = strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            theta_i_0[2] = strtod(value_string,NULL);

            value_string = strtok(NULL, " \t\n");
            theta_j_0[0] = strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            theta_j_0[1] = strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            theta_j_0[2] = strtod(value_string,NULL);

            value_string = strtok(NULL, " \t\n");
            vers_R_c_0[0] = strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            vers_R_c_0[1] = strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            vers_R_c_0[2] = strtod(value_string,NULL);

            if(addCollision_PP(collisions, id_particle_i, id_particle_j, &pos_i_0, &pos_j_0, &theta_i_0, &theta_j_0,
                               &zero_vec, &zero_vec,&zero_vec, &zero_vec, 0.0) != NULL)
            {
                printf( "aggiunta collisione tra particelle (%d,%d)\n", id_particle_i, id_particle_j);
            }


        }

    }

    fclose(fp);
    if (line)
        free(line);
}


void loadSnapshotCollisionsWalls (struct Collisions * collisions, char* file_name)
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        printf ("Unable to open %s\n", file_name);
        exit(EXIT_FAILURE);

    }

    double value;

    vec3 pos_0, theta_0, vers_R_c_0, zero_vec;
    CALint id_particle, id_wall;

    clear_vec3(&zero_vec);

    while ((read = getline(&line, &len, fp)) != -1) {



        char *value_string = strtok(line, " \t\n");
        if( strcmp(value_string, "Current_step:") == 0 || strcmp(value_string, "id_particle_i") == 0)
        {
            continue;
        }
        else
        {
            id_particle =(int) strtod(value_string,NULL);
            printf("id_particle: %d\n ", id_particle);
            value_string = strtok(NULL, " \t\n");
            id_wall =(int) strtod(value_string,NULL);
            printf("id_wall: %d\n ", id_wall);

            value_string = strtok(NULL, " \t\n");
            pos_0[0] =strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            pos_0[1] = strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            pos_0[2] = strtod(value_string,NULL);

            value_string = strtok(NULL, " \t\n");
            theta_0[0] =strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            theta_0[1] = strtod(value_string,NULL);
            value_string = strtok(NULL, " \t\n");
            theta_0[2] = strtod(value_string,NULL);


            if(addCollision_PW(collisions, id_particle,id_wall,&pos_0, &theta_0, &zero_vec, &zero_vec, 0.0) != NULL)
            {
                printf( "aggiunta collisione tra p-wall (%d,%d)\n", id_particle, id_wall);
            }


        }

    }

    fclose(fp);
    if (line)
        free(line);
}


