#include "Header.h"

class particle {
public:
    particle(int id, sf::Vector2f pos, sf::Vector2f vel, float m, int d);
    int getID();
    sf::Vector2f getPosition();
    sf::Vector2f getNextPosition(float uS);
    sf::Vector2f getVelocity();
    sf::Vector2f getMomentum();
    float getMass();
    int getSize();
    sf::CircleShape body;
    void setVelocity(sf::Vector2f vel);
    void applyForce(sf::Vector2f force, float uS);
    void updatePosition(float uS);
private:
    int ID;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float mass;
    int size;
};
