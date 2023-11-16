#include "physics.h"

float Physics :: mean_a (float initial_v, float final_v, float time) {
    return (final_v - initial_v)/time;
}

float Physics :: F (float mass, float acceleration) {
    return mass * acceleration;
}
float Physics :: W (float mass) {
    return F (mass, gravity);
}
float Physics :: v (float acceleration, float time, float initial_v) {
    return acceleration * time + initial_v;
}
float Physics :: delta_x (float acceleration, float time, float initial_v, float initial_x) {
    return ((acceleration*time*time)/2) + initial_v*time + initial_x;
}
float Physics :: Em (float mass, float velocity, float height) {
    return Ec (mass, velocity) + Ep (mass, height);
}
float Physics :: Ep (float mass, float height) {
    return mass * gravity * height;
}
float Physics :: Ec (float mass, float velocity) {
    return (mass * (velocity * velocity)) / 2;
}
float Physics :: T (float force, float distance) {
    return force * distance;
}
float Physics :: P (float force, float velocity) {
    return force * velocity;
}