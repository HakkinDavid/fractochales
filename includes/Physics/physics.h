#ifndef PHYSICS_H
#define PHYSICS_H

#include "../SFML/Graphics.hpp"
#include <cmath>

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
    float deg_to_rad (float deg);
    void rotate (sf::Vector3f *point, sf::Vector3f *centroid, float x_rad = 0.f, float y_rad = 0.f, float z_rad = 0.f);
    void rotate_deg (sf::Vector3f *point, sf::Vector3f *centroid, float x_deg = 0.f, float y_deg = 0.f, float z_deg = 0.f);

    const float gravity = 9.81;
    const float PI = 3.14159274101257324219;
}

#endif