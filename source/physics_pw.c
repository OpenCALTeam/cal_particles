#include <physics_pw.h>

void defPart_PW (vec3 * DefN, vec3 * DefT, CALreal overlap, vec3 vec_r, vec3 p, vec3 theta,
                 struct CollisionPW* collision_pw)
{

    vec3 delta_theta, delta_theta_X_vec_r;
    clear_vec3 (DefN);
    clear_vec3(&delta_theta_X_vec_r);
    clear_vec3(DefT);

    int sign = walls[collision_pw->wall_ID].sign;
    unsigned int indx = walls[collision_pw->wall_ID].indx;

    subtract_vec3(&delta_theta, theta, collision_pw->theta_0);
    (*DefN)[indx] = sign * overlap;

    cross_product_vec3(&delta_theta_X_vec_r, delta_theta, vec_r);

    subtract_vec3(DefT, p, collision_pw->pos_0);

    add_vec3(DefT, *DefT, delta_theta_X_vec_r);

    (*DefT)[indx] = 0.0;

}

void forcePart_PW (vec3* Fn, vec3* Ft, CALreal overlap, vec3 DefN,
                   vec3 DefT, vec3 theta, vec3 p, vec3 v, vec3 vec_r,
                   struct CollisionPW* collision_pw, struct Collisions* collisions)
{

    CALreal ddt_2, ddt_max_2, ddt, ddt_max;
    vec3 Et, deT; //va aggiunta la parte della velocità nel punto di contatto?

    clear_vec3(Fn);

    multiply_by_scalar_vec3(Fn, DefN, - KN_PW);

    unsigned int indx = walls[collision_pw->wall_ID].indx;

    dot_product_vec3(&ddt_2, DefT, DefT);



    ddt_max = (FRICTION_COEF_PW*overlap)/KA;
    ddt_max_2 = ddt_max * ddt_max;
    if(ddt_2 > ddt_max_2)
    {
        ddt = sqrt(ddt_2);
        divide_by_scalar_vec3(&Et, DefT, ddt);
        multiply_by_scalar_vec3(Ft, Et, -FRICTION_COEF_PW *KN_PW * overlap);
        multiply_by_scalar_vec3(&deT, Et, ddt_max);

        resetPart_PW(deT, theta, p, vec_r, collision_pw, collisions );
    }
    else
    {
        multiply_by_scalar_vec3(Ft, DefT, (-KN_PW*KA));
    }

    (*Fn) [indx] = (*Fn) [indx] - AL_PW * v[indx];

}





void resetPart_PW (vec3 deT, vec3 theta,  vec3 p, vec3 vec_r,
                   struct CollisionPW* collision_iw, struct Collisions* collisions)
{

    vec3 delta_theta, delta_theta_X_vec_r, p_new;
    subtract_vec3(&delta_theta, theta, collision_iw->theta_0);

    cross_product_vec3(&delta_theta_X_vec_r, delta_theta, vec_r);

    clear_vec3(&p_new);

    add_vec3(&p_new, p, delta_theta_X_vec_r);

    subtract_vec3(&p_new, p_new, deT);

    setPos_i_PW(collisions, collision_iw->id_particle, collision_iw->wall_ID, &p_new);

}

void updateForces_PW (vec3 Ft, vec3 Fn, vec3 vec_r, struct CollisionPW* collision_iw,
                      struct Collisions* collisions)
{
    vec3 F, vec_r_X_F;
    add_vec3(&F, Ft, Fn);

    //COSA CAVOLO È FORZE WALL

    updateForce_i_PW(collisions, collision_iw->id_particle, collision_iw->wall_ID, &F);

    cross_product_vec3(&vec_r_X_F, vec_r, Ft);

    updateMoment_i_PW(collisions, collision_iw->id_particle, collision_iw->wall_ID, &vec_r_X_F);

}

