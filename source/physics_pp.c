#include <physics_pp.h>

void defPart_PP (vec3 * DefN, vec3 * DefT, CALreal overlap, vec3 enij, vec3 theta_i, vec3 theta_j,
              struct CollisionPP* collision_ij)
{
    vec3 delta_theta_vers, delta_theta_i, delta_theta_j, delta_t_i, delta_t_j, vr_i, vr_j;
    multiply_by_scalar_vec3(DefN, enij, overlap);

   cross_product_vec3(&delta_theta_vers, collision_ij->vers_R_c_0, enij);


   subtract_vec3(&delta_theta_i, theta_i, collision_ij->theta_i_0);
   add_vec3(&delta_theta_i, delta_theta_i, delta_theta_vers);

   subtract_vec3(&delta_theta_j, theta_j, collision_ij->theta_j_0);
   add_vec3(&delta_theta_j, delta_theta_j, delta_theta_vers);

   multiply_by_scalar_vec3(&vr_i, enij, PARTICLE_RADIUS);
   multiply_by_scalar_vec3(&vr_j, enij, -PARTICLE_RADIUS);

   cross_product_vec3(&delta_t_i, delta_theta_i, vr_i);
   cross_product_vec3(&delta_t_j, delta_theta_j, vr_j);

   subtract_vec3(DefT, delta_t_i, delta_t_j);

}

void resetPart_PP (vec3 deT, vec3 theta_i, vec3 theta_j, vec3 enij,  struct CollisionPP* collision_ij, struct Collisions* collisions)
{
    vec3 delta_theta_vers, delta_theta_j, deTn, newTheta, toAdd;

    cross_product_vec3(&delta_theta_vers, collision_ij->vers_R_c_0, enij);

    subtract_vec3(&delta_theta_j, theta_j, collision_ij->theta_j_0);
    add_vec3(&delta_theta_j, delta_theta_j, delta_theta_vers);

    add_vec3(&newTheta, theta_i, delta_theta_vers);

    cross_product_vec3(&deTn, deT, enij);
    multiply_by_scalar_vec3(&toAdd, delta_theta_j, PARTICLE_RADIUS);
    add_vec3(&toAdd, toAdd, deTn);

    add_vec3(&newTheta, toAdd, newTheta);

    setTheta_i_PP(collisions, collision_ij->id_i, collision_ij->id_j, &newTheta);
}

void forcePart_PP (vec3* Fn, vec3* Ft, CALreal overlap, vec3 DefN,
                vec3 DefT, vec3 enij, vec3 Wi, vec3 Wj, CALreal vnij, vec3 vrij,
                vec3 theta_i, vec3 theta_j,  struct CollisionPP* collision_ij,
                struct Collisions* collisions)
{
    CALreal ddt_2, ddt_max_2, ddt, ddt_max;
    vec3 WixRi, WjxRj, vrc, vrcn, vrct, vrcn_AL, vrct_AL, Et, deT;

    multiply_by_scalar_vec3(Fn, DefN, -KN );

    dot_product_vec3(&ddt, DefT, DefT);

    ddt_max = (FRICTION_COEF*overlap)/KA;
    ddt_max_2 = ddt_max * ddt_max;

    if(ddt_2 > ddt_max_2)
    {
        divide_by_scalar_vec3(&Et, DefT, ddt);
        multiply_by_scalar_vec3(Ft, Et, -FRICTION_COEF *KN * overlap);
        multiply_by_scalar_vec3(&deT, Et, ddt_max);

        resetPart_PP(deT, theta_i, theta_j, enij, collision_ij, collisions);

    }
    else
    {
        multiply_by_scalar_vec3(Ft, DefT, (-KN*KA));
    }

    multiply_by_scalar_vec3(&WixRi, enij, PARTICLE_RADIUS);
    cross_product_vec3(&WixRi, WixRi, Wi);

    multiply_by_scalar_vec3(&WjxRj, enij, -PARTICLE_RADIUS);
    cross_product_vec3(&WjxRj, WjxRj, Wj);

    add_vec3(&vrc, vrij, WixRi);
    subtract_vec3(&vrc, vrc, WjxRj);

    multiply_by_scalar_vec3(&vrcn, enij, vnij);
    subtract_vec3(&vrct, vrc, vrcn);

    multiply_by_scalar_vec3(&vrcn_AL, vrcn, -AL);
    subtract_vec3(Fn, *Fn, vrcn_AL);

    multiply_by_scalar_vec3(&vrct_AL, vrct, -AL);
    subtract_vec3(Ft, *Ft, vrct_AL);

}

void updateForces_PP (vec3 Ft, vec3 Fn, vec3 enij, struct CollisionPP* collision_ij)
{
    vec3 F, enijXFt;
    add_vec3(&F, Ft, Fn);

    add_vec3(&collision_ij->F_collision_i, F, collision_ij->F_collision_i);
    subtract_vec3(&collision_ij->F_collision_j, collision_ij->F_collision_j, F);

    cross_product_vec3(&enijXFt, enij, Ft);
    multiply_by_scalar_vec3(&enijXFt, enijXFt, PARTICLE_RADIUS);

    add_vec3(&collision_ij->moment_collision_i, enijXFt, collision_ij->moment_collision_i);
    add_vec3(&collision_ij->moment_collision_j, enijXFt, collision_ij->moment_collision_j);

}



