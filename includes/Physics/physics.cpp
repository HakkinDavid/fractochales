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

float Physics :: deg_to_rad (float deg) {
    return deg * (PI / 180);
}

void Physics :: rotate (sf::Vector3f *point, sf::Vector3f *centroid, float x_rad, float y_rad, float z_rad) {
    (*point) -= (*centroid);

    float rad = 0.0f;

    rad = x_rad;
    (*point).y = std::cos(rad) * (*point).y - std::sin(rad) * (*point).z;
    (*point).z = std::sin(rad) * (*point).y + std::cos(rad) * (*point).z;

    rad = y_rad;
    (*point).x = std::cos(rad) * (*point).x + std::sin(rad) * (*point).z;
    (*point).z = -std::sin(rad) * (*point).x + std::cos(rad) * (*point).z;

    rad = z_rad;
    (*point).x = std::cos(rad) * (*point).x - std::sin(rad) * (*point).y;
    (*point).y = std::sin(rad) * (*point).x + std::cos(rad) * (*point).y;

    (*point) += (*centroid);
}

void Physics :: rotate_deg (sf::Vector3f *point, sf::Vector3f *centroid, float x_deg, float y_deg, float z_deg) {
    rotate(point, centroid, deg_to_rad(x_deg), deg_to_rad(y_deg), deg_to_rad(z_deg));
}