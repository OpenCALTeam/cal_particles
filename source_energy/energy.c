#include<../include_energy/energy.h>
#include<physics_pp.h>


void compute_total_energy(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{
    if(calGet3Di(ca, Q.nP,cell_x,cell_y,cell_z) <= 0)
        return;
    CALreal totalenergy;
    for (int slot=0; slot<MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
        if (calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z) <= NULL_ID)
            continue;
        totalenergy = 0.0;
        totalenergy += calGet3Dr(ca,Q.kinetic_energy[slot], cell_x, cell_y, cell_z);
        totalenergy += calGet3Dr(ca,Q.potential_energy[slot], cell_x, cell_y, cell_z);
        totalenergy += calGet3Dr(ca,Q.elastic_pp_energy[slot], cell_x, cell_y, cell_z);
        totalenergy += calGet3Dr(ca,Q.elastic_pw_energy[slot], cell_x, cell_y, cell_z);
        totalenergy += calGet3Dr(ca,Q.rotational_energy[slot], cell_x, cell_y, cell_z);


        calSet3Dr(ca,Q.total_energy[slot], cell_x, cell_y, cell_z, totalenergy);
    }
}

void compute_kinetic_energy(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{
    if(calGet3Di(ca, Q.nP,cell_x,cell_y,cell_z) <= 0)
        return;
    vec3 v;
    CALreal energy = 0.0;
    for (int slot=0; slot<MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
        if (calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z) <= NULL_ID)
            continue;

        calGet3Dr_vec3(ca, Q.vx[slot], Q.vy[slot], Q.vz[slot], cell_x,cell_y,cell_z, &v );
        energy = 0.5 * PARTICLE_MASS * (v[0] * v[0] + v[1] *v[1] + v[2]*v[2]);
        //        printf("velocity : (%.9f, %.9f, %.9f) energy %.15f\n",v[0], v[1],v[2], energy );
        calSet3Dr(ca,Q.kinetic_energy[slot], cell_x, cell_y, cell_z, energy);
    }
}

void compute_rotational_energy(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{
    if(calGet3Di(ca, Q.nP,cell_x,cell_y,cell_z) <= 0)
        return;
    vec3 w;
    CALreal energy = 0.0;
    for (int slot=0; slot<MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
        if (calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z) <= NULL_ID)
            continue;

        calGet3Dr_vec3(ca, Q.wx[slot], Q.wy[slot], Q.wz[slot], cell_x,cell_y,cell_z, &w );

        energy = 0.5 * MOMENT_INERTIA * (w[0] * w[0] + w[1] *w[1] + w[2]*w[2]);

        //        printf("w : (%.9f, %.9f, %.9f) energy %.15f\n",w[0], w[1],w[2], energy );
        calSet3Dr(ca,Q.rotational_energy[slot], cell_x, cell_y, cell_z, energy);
    }
}

void compute_potential_energy(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{

    if(calGet3Di(ca, Q.nP,cell_x,cell_y,cell_z) <= 0)
        return;
    vec3 p;
    CALreal energy = 0.0;
    for (int slot=0; slot<MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
        if (calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z) <= NULL_ID)
            continue;

        calGet3Dr_vec3(ca, Q.px[slot], Q.py[slot], Q.pz[slot], cell_x,cell_y,cell_z, &p );


        energy =  PARTICLE_MASS * G * fabs(p[0] * G_dir[0] + p[1] * G_dir[1] + p[2] * G_dir[2]);
        //        printf("position : (%.9f, %.9f, %.9f) energy %.9f\n",p[0], p[1],p[2], energy );

        calSet3Dr(ca,Q.potential_energy[slot], cell_x, cell_y, cell_z, energy);
    }

}

void compute_elastic_energy_pp(struct Collisions* collisions, struct CollisionPP* collision_ij,
                               const int i, const int j, CALreal overlap, CALreal dij,
                               vec3  pi, vec3 pj, vec3 theta_i, vec3 theta_j)
{
    vec3 rij, enij, defN_i, defT_i, rji, enji, defN_j, defT_j;
    subtract_vec3(&rij, pj, pi);
    divide_by_scalar_vec3(&enij, rij, dij);

    defPart_PP_ij(&defN_i, &defT_i, overlap, enij, theta_i, theta_j, collision_ij);

    CALreal ddt_2 = 0.0, ddn_2 = 0.0, energy= 0.0 ;
    dot_product_vec3(&ddt_2, defT_i, defT_i);
    dot_product_vec3(&ddn_2, defN_i, defN_i);
    energy = 0.5 * KN_PP * ddn_2 + 0.5 * KN_PP * KA * ddt_2;
    setEnergy_i_PP(collisions, i,j, energy);

    //    setEnergy_j_PP(collisions, i,j, energy);

    //    subtract_vec3(&rji, pi, pj);
    //    divide_by_scalar_vec3(&enji, rji, dij);

    //    defPart_PP_ji(&defN_j, &defT_j, overlap, enji, theta_i, theta_j, collision_ij);


    //    ddt_2 = 0.0, ddn_2 = 0.0, energy= 0.0 ;
    //    dot_product_vec3(&ddt_2, defT_j, defT_j);
    //    dot_product_vec3(&ddn_2, defN_j, defN_j);
    //    energy = 0.5 * KN_PP * ddt_2 + 0.5 * KN_PP * KA * ddn_2;
    //    setEnergy_j_PP(collisions, i,j, energy);

}

void total_elastic_energy_pp(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{
    if(calGet3Di(ca, Q.nP,cell_x,cell_y,cell_z) < 1)
        return;
    CALreal energy;

    for (int slot=0; slot<MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {

        CALint id_particle = calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z);
        if ( id_particle > NULL_ID)
        {
            energy = 0.0;
            totalElasticEnergyCollisionPP(&collisions, &energy, id_particle);
            calSet3Dr(ca,Q.elastic_pp_energy[slot], cell_x, cell_y, cell_z, energy);
        }
    }

}

void setToZeroEnergy (struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{
    for (int slot=0; slot<MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
        calSet3Dr(ca,Q.elastic_pw_energy[slot], cell_x, cell_y, cell_z, 0.0);
        calSet3Dr(ca,Q.elastic_pp_energy[slot], cell_x, cell_y, cell_z, 0.0);
        calSet3Dr(ca,Q.potential_energy[slot], cell_x, cell_y, cell_z, 0.0);
        calSet3Dr(ca,Q.rotational_energy[slot], cell_x, cell_y, cell_z, 0.0);
        calSet3Dr(ca,Q.total_energy[slot], cell_x, cell_y, cell_z, 0.0);
        calSet3Dr(ca,Q.kinetic_energy[slot], cell_x, cell_y, cell_z, 0.0);
    }

}

void total_elastic_energy_pw(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{
    if(calGet3Di(ca, Q.nP,cell_x,cell_y,cell_z) < 1)
        return;
    CALreal energy;

    for (int slot=0; slot<MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {

        CALint id_particle = calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z);
        if ( id_particle > NULL_ID)
        {
            energy = 0.0;
            totalElasticEnergyCollisionPW(&collisions, &energy, id_particle);
            calSet3Dr(ca,Q.elastic_pw_energy[slot], cell_x, cell_y, cell_z, energy);
        }
    }

}
