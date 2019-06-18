#include <ep_collision.h>
#include <ep_utils.h>
#include <math.h>

#include <model.h>
#include <collision_pp.h>
#include <physics_pp.h>
#include <physics_pw.h>

#ifdef ENERGY
#include <../include_energy/energy.h>
#endif


void evaluate_collision_PP (int id_PARTICLE_i, int id_PARTICLE_j, vec3* pi, vec3* pj, vec3* theta_i, vec3* theta_j, vec3* vi, vec3* vj,
                            vec3* wi, vec3* wj)
{
    CALreal  dij, dij_2, vnij;

    distance_squared_vec3(&dij_2, *pi, *pj);

    if (dij_2 < cnfg.DM_2)
    {

        vec3 rij, enij, vrij, defN, defT, Fn, Ft;
        dij = sqrt(dij_2);

        //        printf("dm[%d %d] = %.7f dij[%d %d] = %.10f \n",id_PARTICLE_i, id_PARTICLE_j,  DM,id_PARTICLE_i, id_PARTICLE_j, dij);

        CALreal overlap = cnfg.DM - dij;

        subtract_vec3(&rij, *pj, *pi);
        divide_by_scalar_vec3(&enij, rij, dij);

        subtract_vec3(&vrij, *vj, *vi);
        dot_product_vec3 (&vnij, vrij, enij);

        struct CollisionPP* collision_ij = findCollision_PP(&collisions, id_PARTICLE_i, id_PARTICLE_j);

        if (collision_ij == NULL)
        {
            CALreal dtp, dtp_dt;
            if(vnij != 0.0)
            {
                dtp = fabs(overlap/vnij);
                dtp_dt = dtp / cnfg.DELTA_T;
                if (dtp_dt > 1.0)
                {
                    dtp = cnfg.DELTA_T;
                }

            }
            else
            {
                dtp = 0.0;
            }

            //al momento in cui viene inserito è solo in next.. è corretto usare quello?
            collision_ij = addCollision_PP(&collisions, id_PARTICLE_i, id_PARTICLE_j,
                                           pi, pj, theta_i, theta_j, vi, vj, wi, wj, dtp);
        }

        defPart_PP_ij(&defN, &defT, overlap, enij, *theta_i, *theta_j,
                      collision_ij);

#ifdef ENERGY
        compute_elastic_energy_pp (&collisions, collision_ij, id_PARTICLE_i, id_PARTICLE_j, overlap, dij, *pi, *pj, *theta_i, *theta_j);
#endif

        forcePart_PP(&Fn, &Ft, overlap, defN, defT, enij,
                     *wi, *wj, vnij, vrij, *theta_i, *theta_j, collision_ij, &collisions);

        updateForces_PP(Ft, Fn, enij, collision_ij, &collisions);

    }
    else
    {
        //si potrebbe controllare solo una volta.. ma questo ha accesso costo costante quindi non dovrebbe pesare
        struct CollisionPP* collision_ij = findCollision_PP(&collisions, id_PARTICLE_i, id_PARTICLE_j);
        if (collision_ij != NULL)
        {
            deleteCollision_PP(&collisions, id_PARTICLE_i, id_PARTICLE_j);
        }
    }

    //------------------------------------------------------

} //if dij < 2*PARTICLE_RADIUS







void inner_collision(struct CALModel3D* ca,
                     int cell_x, int cell_y, int cell_z)
{
    vec3 pi, vi, theta_i, wi;
    vec3 pj, vj, theta_j, wj;
    //    vec3 /*rij, enij,*/ vij;
    CALreal  dij, dij_2, vnij;
    CALint id_PARTICLE_i, id_PARTICLE_j;

    if(calGet3Di(ca, Q.nP,cell_x,cell_y,cell_z) < 2)
        return;

    //    printf("_______________________________________ (%d,%d,%d)\n", cell_x, cell_y, cell_z);
    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {

        CALint id_i = calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z);


        if (id_i > NULL_ID)
        {
            // inner particle-particle collision
            for (int inner_slot=slot+1; inner_slot<MAX_NUMBER_OF_PARTICLES_PER_CELL; inner_slot++)
            {
                CALint id_j = calGet3Di(ca, Q.ID[inner_slot],cell_x,cell_y,cell_z);

                if (id_j <= NULL_ID) //dobbiamo escludere anche le particelle al bordo
                    continue;

                id_PARTICLE_i = id_i;
                id_PARTICLE_j = id_j;

                if (id_PARTICLE_i > id_PARTICLE_j) //swap
                {
                    swap(id_PARTICLE_i, id_PARTICLE_j);
                    calGet3Dr_vec3(ca, Q.px[slot], Q.py[slot], Q.pz[slot], cell_x,cell_y,cell_z, &pj );
                    calGet3Dr_vec3(ca, Q.vx[slot], Q.vy[slot], Q.vz[slot], cell_x,cell_y,cell_z, &vj );
                    calGet3Dr_vec3(ca, Q.thetax[slot], Q.thetay[slot], Q.thetaz[slot], cell_x,cell_y,cell_z, &theta_j );
                    calGet3Dr_vec3(ca, Q.wx[slot], Q.wy[slot], Q.wz[slot], cell_x,cell_y,cell_z, &wj );

                    calGet3Dr_vec3(ca, Q.px[inner_slot], Q.py[inner_slot], Q.pz[inner_slot], cell_x,cell_y,cell_z, &pi );
                    calGet3Dr_vec3(ca, Q.vx[inner_slot], Q.vy[inner_slot], Q.vz[inner_slot], cell_x,cell_y,cell_z, &vi);

                    calGet3Dr_vec3(ca, Q.thetax[inner_slot], Q.thetay[inner_slot], Q.thetaz[inner_slot], cell_x,cell_y,cell_z, &theta_i );
                    calGet3Dr_vec3(ca, Q.wx[inner_slot], Q.wy[inner_slot], Q.wz[inner_slot], cell_x,cell_y,cell_z, &wi );

                }
                else
                {
                    calGet3Dr_vec3(ca, Q.px[slot], Q.py[slot], Q.pz[slot], cell_x,cell_y,cell_z, &pi );
                    calGet3Dr_vec3(ca, Q.vx[slot], Q.vy[slot], Q.vz[slot], cell_x,cell_y,cell_z, &vi );
                    calGet3Dr_vec3(ca, Q.thetax[slot], Q.thetay[slot], Q.thetaz[slot], cell_x,cell_y,cell_z, &theta_i );
                    calGet3Dr_vec3(ca, Q.wx[slot], Q.wy[slot], Q.wz[slot], cell_x,cell_y,cell_z, &wi );

                    calGet3Dr_vec3(ca, Q.px[inner_slot], Q.py[inner_slot], Q.pz[inner_slot], cell_x,cell_y,cell_z, &pj );
                    calGet3Dr_vec3(ca, Q.vx[inner_slot], Q.vy[inner_slot], Q.vz[inner_slot], cell_x,cell_y,cell_z, &vj);

                    calGet3Dr_vec3(ca, Q.thetax[inner_slot], Q.thetay[inner_slot], Q.thetaz[inner_slot], cell_x,cell_y,cell_z, &theta_j );
                    calGet3Dr_vec3(ca, Q.wx[inner_slot], Q.wy[inner_slot], Q.wz[inner_slot], cell_x,cell_y,cell_z, &wj );

                }

                evaluate_collision_PP(id_PARTICLE_i, id_PARTICLE_j, &pi, &pj, &theta_i, &theta_j, &vi, &vj, &wi, &wj);


                //------------------------------------------------------

            } //if dij < 2*PARTICLE_RADIUS
            //                } // if > NULL_ID

            //URTO MURO DA FARE


        } //for inner_slot AND if

    }
}


void outer_collision(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{
    vec3 pi, vi, theta_i, wi;

    //    vec3 /*rij, enij,*/ vij;
    CALreal  dij, dij_2, vnij;

    if(calGet3Di(ca, Q.nP,cell_x,cell_y,cell_z) <= 0)
        return;

    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
        CALint id_PARTICLE_i = calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z);
        if (id_PARTICLE_i > NULL_ID)
        {
            // outer particle-particle collision
            for (int n = 1; n<ca->sizeof_X; n++)
            {
                if(calGetX3Di(ca, Q.nP,cell_x,cell_y,cell_z, n) <= 0)
                    continue;

                //PERCHÈ CON QUESTO CONTROLLO È DIVERSA LA SIMULAZIONE?
                //                if(!isThereAtLeastAParticle(ca, cell_x, cell_y,cell_z, n))
                //                    return;

                for (int outer_slot=0; outer_slot<MAX_NUMBER_OF_PARTICLES_PER_CELL; outer_slot++)
                {
                    CALint id_PARTICLE_j = calGetX3Di(ca, Q.ID[outer_slot],cell_x,cell_y,cell_z,n);
                    //                    printf("valuto (%d,%d) e n = %d e cell (%d,%d,%d) \n",id_PARTICLE_i, id_PARTICLE_j, n, cell_x, cell_y, cell_z  );

                    //consideriamo solo una volta la collisione quando i < j
                    if ( id_PARTICLE_j > NULL_ID  && id_PARTICLE_i < id_PARTICLE_j)
                    {

                        vec3 pj, vj, theta_j, wj;

                        calGet3Dr_vec3(ca, Q.px[slot], Q.py[slot], Q.pz[slot], cell_x,cell_y,cell_z, &pi);
                        calGet3Dr_vec3(ca, Q.vx[slot], Q.vy[slot], Q.vz[slot], cell_x,cell_y,cell_z, &vi);
                        calGet3Dr_vec3(ca, Q.thetax[slot], Q.thetay[slot], Q.thetaz[slot], cell_x,cell_y,cell_z, &theta_i );
                        calGet3Dr_vec3(ca, Q.wx[slot], Q.wy[slot], Q.wz[slot], cell_x,cell_y,cell_z, &wi );

                        calGetX3Dr_vec3(ca, Q.px[outer_slot], Q.py[outer_slot], Q.pz[outer_slot], cell_x,cell_y,cell_z, &pj, n  );
                        calGetX3Dr_vec3(ca, Q.vx[outer_slot], Q.vy[outer_slot], Q.vz[outer_slot], cell_x,cell_y,cell_z, &vj, n);

                        calGetX3Dr_vec3(ca, Q.thetax[outer_slot], Q.thetay[outer_slot], Q.thetaz[outer_slot], cell_x,cell_y,cell_z, &theta_j, n );
                        calGetX3Dr_vec3(ca, Q.wx[outer_slot], Q.wy[outer_slot], Q.wz[outer_slot], cell_x,cell_y,cell_z, &wj, n );


                        //------------------------------------------------------

                        evaluate_collision_PP(id_PARTICLE_i, id_PARTICLE_j, &pi, &pj, &theta_i, &theta_j, &vi, &vj, &wi, &wj);
                    }


                } //for n = 1 ...



            }
        }
    }
}

void walls_collision(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{

    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
        CALint id_PARTICLE_i = calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z);
        if (id_PARTICLE_i > NULL_ID)
        {
            for(int wall_ID=0; wall_ID < N_WALLS; wall_ID++)
            {
                //                printf("______________________________\n");
                vec3 pi, vi, theta_i, wi;
                CALreal external, overlap, dtp =0.0, dpw;
                vec3 vec_r, defN, defT, Fn, Ft;


                unsigned int indx = walls[wall_ID].indx;
                int sign = walls[wall_ID].sign;

                //                pwall = walls[wall_ID].pos;
                calGet3Dr_vec3(ca, Q.px[slot], Q.py[slot], Q.pz[slot], cell_x,cell_y,cell_z, &pi);
                calGet3Dr_vec3(ca, Q.vx[slot], Q.vy[slot], Q.vz[slot], cell_x,cell_y,cell_z, &vi);
                calGet3Dr_vec3(ca, Q.thetax[slot], Q.thetay[slot], Q.thetaz[slot], cell_x,cell_y,cell_z, &theta_i );
                calGet3Dr_vec3(ca, Q.wx[slot], Q.wy[slot], Q.wz[slot], cell_x,cell_y,cell_z, &wi );

                dpw = fabs(walls[wall_ID].pos[indx] - pi[indx]);

                if(dpw < cnfg.PARTICLE_RADIUS) //È COSÌ?
                {

                    //printProperties(&cnfg);
                    external = pi[indx] + sign * cnfg.PARTICLE_RADIUS;


                    //FARE CON VALORE ASSOLUTO
                    if (sign < 0)
                    {
                        overlap = walls[wall_ID].pos[indx] - external;
                    }
                    else
                    {
                        overlap = external - walls[wall_ID].pos[indx];
                    }

                    clear_vec3(&vec_r);

                    vec_r[indx] += sign * cnfg.PARTICLE_RADIUS;
                    struct CollisionPW* collision_pw = findCollision_PW(&collisions, id_PARTICLE_i, wall_ID);
                    if (collision_pw == NULL)
                    {
                        CALreal dtp_dt;
                        if ( vi[indx] != 0.0)
                        {
                            dtp = (sign * overlap) / vi[indx];
                            dtp_dt = dtp / cnfg.DELTA_T;
                            if (dtp_dt > 1.0)
                                dtp = cnfg.DELTA_T;

                        }
                        else
                        {
                            dtp = 0.0;
                        }


                        collision_pw = addCollision_PW(&collisions,
                                                       id_PARTICLE_i, wall_ID, &pi, &theta_i, &vi, &wi, dtp);
                    }

                    defPart_PW(&defN, &defT, overlap, vec_r, pi, theta_i, collision_pw);

#ifdef ENERGY
                    CALreal ddt_2 = 0.0, ddn_2 = 0.0, energy= 0.0 ;
                    dot_product_vec3(&ddt_2, defT, defT);
                    dot_product_vec3(&ddn_2, defN, defN);
                    energy = 0.5 * cnfg.KN_PW * ddn_2 + 0.5 * cnfg.KN_PW * cnfg.KA * ddt_2;
                    setEnergy_i_PW(&collisions, id_PARTICLE_i, wall_ID, energy);
#endif

                    forcePart_PW(&Fn, &Ft, overlap, defN, defT, theta_i, pi, vi, vec_r, collision_pw, &collisions);

                    //                    printf("chiamo update force tra %d e %d e la distance è %.6f e il raggio è %.6f\n",id_PARTICLE_i,  wall_ID, dpw, PARTICLE_RADIUS);

                    updateForces_PW(Ft,Fn, vec_r, collision_pw, &collisions);



                }
                else
                {
                    //si potrebbe controllare solo una volta.. ma questo ha accesso costo costante quindi non dovrebbe pesare
                    struct CollisionPW* collision_pw = findCollision_PW(&collisions, id_PARTICLE_i, wall_ID);
                    if (collision_pw != NULL)
                    {
                        deleteCollision_PW(&collisions, id_PARTICLE_i, wall_ID);
                    }

                }
            }
        }
    }
}


void countParticles(struct CALModel3D* ca, int cell_x, int cell_y, int cell_z)
{
    int count = 0;
    for (int slot=0; slot<MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
        if(calGet3Di(ca,Q.ID[slot],cell_x,cell_y,cell_z) > NULL_ID)
            count ++;

    calSet3Di(ca,Q.nP,cell_x,cell_y,cell_z, count);


}




