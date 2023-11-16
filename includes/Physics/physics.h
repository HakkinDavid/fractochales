#ifndef PHYSICS_H
#define PHYSICS_H

namespace Physics {
    float mean_a (float initial_v, float final_v, float time);
    float F (float mass, float acceleration = 0);
    float W (float mass);
    float v (float acceleration = 0, float time = 0, float initial_v = 0);
    float delta_x (float acceleration = 0, float time = 0, float initial_v = 0, float initial_x = 0);
    float Em (float mass, float velocity = 0, float height = 0);
    float Ep (float mass, float height = 0);
    float Ec (float mass, float velocity = 0);
    float T (float force = 0, float distance = 0);
    float P (float force = 0, float velocity = 0);

    const float gravity = 9.81;
}

#endif