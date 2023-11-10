#include <cmath>
#include "../SFML/Graphics.hpp"

// SFML THICK LINE
class thickLine {
    private:
        sf::Vertex vertices[4];
    public:
        thickLine (sf::Vector2f point1, sf::Vector2f point2, sf::Color color = sf::Color::White, float thickness = 1.f) {
            sf::Vector2f direction = point2 - point1;
            sf::Vector2f unitDirection = direction/std::sqrt(direction.x*direction.x+direction.y*direction.y);
            sf::Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

            sf::Vector2f offset = (thickness/2.f)*unitPerpendicular;

            vertices[0].position = point1 + offset;
            vertices[1].position = point2 + offset;
            vertices[2].position = point2 - offset;
            vertices[3].position = point1 - offset;

            for (int i=0; i<4; ++i) {
                vertices[i].color = color;
            }
        }

        void draw(sf::RenderTarget &target) {
            target.draw(vertices, 4, sf::Quads);
        }

        void setColor (int r, int b, int g, int a = 255) {
            sf::Color color (r, b, g, a);
            for (int i=0; i<4; ++i) {
                vertices[i].color = color;
            }
        }
};