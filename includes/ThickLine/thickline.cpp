#include <cmath>
#include "../SFML/Graphics.hpp"
#include "../Physics/physics.h"

// SFML THICK LINE
class thickLine {
    private:
        sf::Vertex vertices[4];
        sf::Vector3f start, end;
        sf::Vector3f original [2];
        sf::Color color;
        float thickness;
        float color_factor;
    public:
        thickLine (const sf::Vector3f start, const sf::Vector3f end, const sf::Color color = sf::Color::White, const float thickness = 1.f) {
            this->start = start;
            this->end = end;
            this->color = color;
            this->thickness = thickness;
            this->original[0] = start;
            this->original[1] = end;
            this->color_factor = 1.f;
        }

        void recalculate () {
            float   z_r, z_g, z_b;
            do {
                z_r = color.r * color_factor,
                z_g = color.g * color_factor,
                z_b = color.b * color_factor;
                color_factor -= 0.01f;
            } while (z_r > 255 || z_g > 255 || z_b > 255);
            sf::Color z_color (z_r, z_g, z_b, color.a);
            sf::Vector2f point1(start.x, start.y), point2(end.x, end.y);
            sf::Vector2f direction = point2 - point1;
            sf::Vector2f unitDirection = direction/std::sqrt(direction.x*direction.x+direction.y*direction.y);
            sf::Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

            sf::Vector2f offset = (thickness/2.f)*unitPerpendicular;

            vertices[0].position = point1 - offset;
            vertices[1].position = point2 - offset;
            vertices[2].position = point1 + offset;
            vertices[3].position = point2 + offset;

            for (int i=0; i<4; ++i) {
                vertices[i].color = z_color;
            }
        }

        void pre_draw (sf::Vector3f *centroid = nullptr, float x_rad = 0.f, float y_rad = 0.f, float z_rad = 0.f) {
            start = original[0];
            end = original[1];
            color_factor = 0.8f;
            if (centroid != nullptr && (x_rad != 0.f || y_rad != 0.f || z_rad != 0.f)) {
                // esto es contraintuitivo pero tl;dr: -z es hacia la pantalla (según la profe)
                // iluminar más
                if (
                        (centroid->z > start.z || centroid->z > end.z) // si está detrás del centroide
                        || ((centroid->x > start.x || centroid->x > end.x) && y_rad > 0 && y_rad < Physics::PI) // si pertenece a la izquierda y se rotó entre 0 y 180 grados respecto al eje y
                        || ((centroid->x < start.x || centroid->x < end.x) && y_rad > Physics::PI && y_rad < 2*Physics::PI) // si pertenece a la derecha y se rotó entre 180 y 360 grados respecto al eje y
                        || ((centroid->y > start.y || centroid->y > end.y) && x_rad > 0 && x_rad < Physics::PI) // si pertenece a la mitad superior y se rotó entre 0 y 180 grados respecto al eje x
                        || ((centroid->y < start.y || centroid->y < end.y) && x_rad > Physics::PI && x_rad < 2*Physics::PI) // si pertenece a la mitad inferior y se rotó entre 180 y 360 grados respecto al eje x
                ) {
                    color_factor = (std::sqrt(pow(centroid->x - (start.x + end.x)/2, 2) + pow(centroid->y - (start.y + end.y)/2, 2) + pow(centroid->z - (start.z + end.z)/2, 2)))/500.f;
                    if (color_factor < 0.85f) color_factor = 0.85f;
                    else if (color_factor > 1.05f) color_factor = 1.05f;
                }
                // oscurecer
                else if (
                        (centroid->z < start.z || centroid->z < end.z) // si está detrás del centroide
                        || ((centroid->x < start.x || centroid->x < end.x) && y_rad > 0 && y_rad < Physics::PI) // si pertenece a la derecha y se rotó entre 0 y 180 grados respecto al eje y
                        || ((centroid->x > start.x || centroid->x > end.x) && y_rad > Physics::PI && y_rad < 2*Physics::PI) // si pertenece a la izquierda y se rotó entre 180 y 360 grados respecto al eje y
                        || ((centroid->y < start.y || centroid->y < end.y) && x_rad > 0 && x_rad < Physics::PI) // si pertenece a la mitad inferior y se rotó entre 0 y 180 grados respecto al eje x
                        || ((centroid->y > start.y || centroid->y > end.y) && x_rad > Physics::PI && x_rad < 2*Physics::PI) // si pertenece a la mitad superior y se rotó entre 180 y 360 grados respecto al eje x
                ) {
                    color_factor = 100.f/(std::sqrt(pow(centroid->x - (start.x + end.x)/2, 2) + pow(centroid->y - (start.y + end.y)/2, 2) + pow(centroid->z - (start.z + end.z)/2, 2)));
                    if (color_factor > 0.75f) color_factor = 0.75f;
                }
                Physics::rotate(&start, centroid, x_rad, y_rad, z_rad);
                Physics::rotate(&end, centroid, x_rad, y_rad, z_rad);
            }
            recalculate ();
        }

        void draw (sf::RenderTarget &target) {
            target.draw(vertices, 4, sf::TriangleStrip);
        }

        void setColor (int r, int b, int g, int a = 255) {
            sf::Color color (r, b, g, a);
            for (int i=0; i<4; ++i) {
                vertices[i].color = color;
            }
        }

        int getZOrder () {
            return (start.z + end.z)/2.f;
        }
};