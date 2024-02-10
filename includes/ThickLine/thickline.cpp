#include <cmath>
#include "../SFML/Graphics.hpp"

// SFML THICK LINE
class thickLine {
    private:
        sf::Vertex vertices[4];
        sf::Vector3f start, end;
        sf::Color color;
        float thickness;
    public:
        thickLine (const sf::Vector3f start, const sf::Vector3f end, const sf::Color color = sf::Color::White, const float thickness = 1.f) {
            this->start = start;
            this->end = end;
            this->color = color;
            this->thickness = thickness;
        }

        void recalculate () {
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
                vertices[i].color = color;
            }
        }

        void draw (sf::RenderTarget &target) {
            recalculate ();
            target.draw(vertices, 4, sf::TriangleStrip);
        }

        void setColor (int r, int b, int g, int a = 255) {
            sf::Color color (r, b, g, a);
            for (int i=0; i<4; ++i) {
                vertices[i].color = color;
            }
        }

        sf::Vector3f& accessStart () {
            return start;
        }

        sf::Vector3f& accessEnd () {
            return end;
        }
};