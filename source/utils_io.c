#include <utils_io.h>
#include <stdlib.h>

CALint number_of_particles = 0;
CALreal total_energy = 0.0;
CALreal max_velocity = 0.0;
CALreal max_omega = 0.0;
CALreal max_displacement = 0.0;

CALreal total_kinetic_energy = 0.0;
CALreal total_rotational_energy = 0.0;
CALreal total_potential_energy = 0.0;
CALreal total_elastic_pp_energy = 0.0;
CALreal total_elastic_pw_energy = 0.0;

void summary(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{
    vec3 omega, velocity;
    CALreal v = 0.0;
    CALreal w = 0.0;

    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
        if (calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z) > NULL_ID)
        {
#pragma omp critical
            {
                number_of_particles++;
            }
            calGet3Dr_vec3(ca, Q.wx[slot], Q.wy[slot], Q.wz[slot], cell_x, cell_y, cell_z, &omega );
            calGet3Dr_vec3(ca, Q.vx[slot], Q.vy[slot], Q.vz[slot], cell_x, cell_y, cell_z, &velocity );
#ifdef ENERGY
#pragma omp critical
            {
                total_energy += calGet3Dr(ca,Q.total_energy[slot],cell_x,cell_y,cell_z);
                total_kinetic_energy += calGet3Dr(ca,Q.kinetic_energy[slot],cell_x,cell_y,cell_z);
                total_rotational_energy += calGet3Dr(ca,Q.rotational_energy[slot],cell_x,cell_y,cell_z);
                total_potential_energy += calGet3Dr(ca,Q.potential_energy[slot],cell_x,cell_y,cell_z);
                total_elastic_pp_energy += calGet3Dr(ca,Q.elastic_pp_energy[slot],cell_x,cell_y,cell_z);
                total_elastic_pw_energy += calGet3Dr(ca,Q.elastic_pw_energy[slot],cell_x,cell_y,cell_z);
            }
#endif
            v = sqrt(velocity[0]*velocity[0] + velocity[1]*velocity[1] + velocity[2]*velocity[2]);
            w = sqrt(omega[0]*omega[0] + omega[1]*omega[1] + omega[2]*omega[2]);

#pragma omp critical
            {
                if (max_velocity < v)
                {
                    max_velocity = v;
                    max_displacement = v*DELTA_T;
                }

                if (max_omega < w)
                {
                    max_omega = w;
//                    max_displacement = v*DELTA_T;
                }
            }
        }
}

void printSummary(struct CALModel3D* ca)
{
    number_of_particles = 0;
    total_energy = 0.0;
    max_velocity = 0.0;
    max_omega = 0.0;
    max_displacement = 0.0;

    total_kinetic_energy = 0.0;
    total_rotational_energy = 0.0;
    total_potential_energy = 0.0;
    total_elastic_pp_energy = 0.0;
    total_elastic_pw_energy = 0.0;

    calApplyElementaryProcess3D(ca,summary);






#ifdef ENERGY
    printf("step %6d, elapsed_time: %.6f s, n_of_particles: %d, tot_energy: %.9f, max_v: %.6f, max_w: %.6f, max_displacement: %e\n", a_simulazioni->step, elapsed_time, number_of_particles, total_energy, max_velocity,max_omega, max_displacement);
#else
    printf("step %6d, elapsed_time: %.6f s, n_of_particles: %d, max_v: %.6f, max_w: %.6f, max_displacement: %e\n", a_simulazioni->step, elapsed_time, number_of_particles, max_velocity, max_omega, max_displacement);
#endif

}

void saveTotalEnergy(struct CALModel3D *ca, CALint step, CALreal elapsed_time, FILE *f)
{

    fprintf(f, "%d %.10f %.10f %.10f %.10f %.10f %.10f %.10f\n",step, elapsed_time,
            total_kinetic_energy, total_rotational_energy, total_potential_energy,
            total_elastic_pp_energy, total_elastic_pw_energy, total_energy);
}

void saveParticles(struct CALModel3D *ca, CALint step, CALreal elapsed_time, double CPU_time, char* path)
{
    FILE* f;
    f = fopen(path, "w");

    if ( !f )
    {
        printf ("Unable to open %s.\n", path);
        exit(EXIT_FAILURE);
    }

    CALint number_of_particles = 0;
    CALreal total_energy = 0.0;

    CALreal p[3], v[3], F[3];
    CALint particle_id;


    fprintf(f, "Current step: %d\n", step);
    fprintf(f, "Physical elapsed time: %.9f\n", elapsed_time);
    fprintf(f, "CPU time: %.9f\n", CPU_time);
    fprintf(f, "id     \tp[0]     \tp[1]     \tp[2]     \tv[0]     \tv[1]     \tv[2]     \tF[0]     \tF[1]     \tF[2]\n");

    for (int cell_x=0; cell_x<ca->rows; cell_x++)
        for (int cell_y=0; cell_y<ca->columns; cell_y++)
            for (int cell_z = 0; cell_z<ca->slices; cell_z++)
                for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
                    if (calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z) > NULL_ID)
                    {
                        F[0] = calGet3Dr(ca, Q.Fx[slot],cell_x,cell_y,cell_z);
                        F[1] = calGet3Dr(ca, Q.Fy[slot],cell_x,cell_y,cell_z);
                        F[2] = calGet3Dr(ca, Q.Fz[slot],cell_x,cell_y,cell_z);

                        p[0] = calGet3Dr(ca, Q.px[slot],cell_x,cell_y,cell_z);
                        p[1] = calGet3Dr(ca, Q.py[slot],cell_x,cell_y,cell_z);
                        p[2] = calGet3Dr(ca, Q.pz[slot],cell_x,cell_y,cell_z);

                        v[0] = calGet3Dr(ca, Q.vx[slot],cell_x,cell_y,cell_z);
                        v[1] = calGet3Dr(ca, Q.vy[slot],cell_x,cell_y,cell_z);
                        v[2] = calGet3Dr(ca, Q.vz[slot],cell_x,cell_y,cell_z);

                        particle_id = calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z);

                        number_of_particles++;
                        total_energy += PARTICLE_MASS*G*p[2];
                        total_energy += 0.5*PARTICLE_MASS*v[0]*v[0];
                        total_energy += 0.5*PARTICLE_MASS*v[1]*v[1];
                        total_energy += 0.5*PARTICLE_MASS*v[2]*v[2];

                        fprintf(f, "%d\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\n", particle_id, p[0], p[1], p[2], v[0], v[1], v[2], F[0], F[1], F[2]);
                    }

    fprintf(f, "Total number of particles: %d\n", number_of_particles);
    fprintf(f, "Total energy: %.9f\n", total_energy);

    if (!f)
        fclose(f);
}
