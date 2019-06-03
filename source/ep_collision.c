#include <ep_collision.h>
#include <ep_utils.h>
#include <math.h>

#include <model.h>
#include <collision_pp.h>
#include <physics_pp.h>


void evaluate_collision_PP (int id_PARTICLE_i, int id_PARTICLE_j, vec3* pi, vec3* pj, vec3* theta_i, vec3* theta_j, vec3* vi, vec3* vj,
                            vec3* wi, vec3* wj)
{
    CALreal  dij, dij_2, vnij;

    distance_squared_vec3(&dij_2, *pi, *pj);

    if (dij_2 < dm_2)
    {

        vec3 rij, enij, vrij, defN, defT, Fn, Ft;
        dij = sqrt(dij_2);

//        printf("dm[%d %d] = %.7f dij[%d %d] = %.10f \n",id_PARTICLE_i, id_PARTICLE_j,  DM,id_PARTICLE_i, id_PARTICLE_j, dij);

        CALreal overlap = DM - dij;

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
                dtp_dt = dtp / DELTA_T;
                if (dtp_dt > 1.0)
                {
                    dtp = DELTA_T;
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

        defPart_PP(&defN, &defT, overlap, enij, *theta_i, *theta_j,
                collision_ij);

        forcePart_PP(&Fn, &Ft, overlap, defN, defT, enij,
                  *wi, *wj, vnij, vrij, *theta_i, *theta_j, collision_ij, &collisions);

        updateForces_PP(Ft, Fn, enij, collision_ij);

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

    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {

        CALint id_PARTICLE_i = calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z);

        if (id_PARTICLE_i > NULL_ID)
        {

            // inner particle-particle collision
            for (int inner_slot=slot+1; inner_slot<MAX_NUMBER_OF_PARTICLES_PER_CELL; inner_slot++)
            {
                CALint id_PARTICLE_j = calGet3Di(ca, Q.ID[inner_slot],cell_x,cell_y,cell_z);
//                printf("+++++ valuto collisione tra %d %d nella cella (%d,%d,%d)\n", id_PARTICLE_i, id_PARTICLE_j, cell_x,cell_y,cell_z);
                if (id_PARTICLE_j <= NULL_ID) //dobbiamo escludere anche le particelle al bordo
                    continue;


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
    vec3 pj, vj, theta_j, wj;
    //    vec3 /*rij, enij,*/ vij;
    CALreal  dij, dij_2, vnij;


    for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
        CALint id_PARTICLE_i = calGet3Di(ca, Q.ID[slot],cell_x,cell_y,cell_z);
        if (id_PARTICLE_i > NULL_ID)
        {
            // outer particle-particle collision
            for (int n = 1; n<ca->sizeof_X; n++)
            {
                for (int outer_slot=0; outer_slot<MAX_NUMBER_OF_PARTICLES_PER_CELL; outer_slot++)
                {
                    CALint id_PARTICLE_j = calGetX3Di(ca, Q.ID[outer_slot],cell_x,cell_y,cell_z,n);

                    //consideriamo solo una volta la collisione quando i < j
                    if ( id_PARTICLE_j <= NULL_ID  || id_PARTICLE_i > id_PARTICLE_j)
                        continue;

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

                } // for outer_slot
            } //for n = 1 ...



        }
    }
}



