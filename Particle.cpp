#include "Particle.h"

particle::particle(int id, sf::Vector2f pos, sf::Vector2f vel, float m, int d) {
    ID = id;
    position = pos;
    velocity= vel;
    mass = m;
    size = d;
    body = sf::CircleShape(size);
    body.setFillColor(sf::Color(255, 255, 255));
    body.setPosition(sf::Vector2f(position.x, position.y));
}

int particle::getID() {
    return ID;
}

sf::Vector2f particle::getPosition() { // Centre Offset
    sf::Vector2f centre;
    centre.x = position.x + size;
    centre.y = position.y + size;
    return centre;
}

sf::Vector2f particle::getVelocity() {
    return velocity;
}

sf::Vector2f particle::getNextPosition(float uS) { // Centre Offset
    sf::Vector2f nextPosition;
    nextPosition.x = position.x + size + (velocity.x * (float) (uS / timeScale));
    nextPosition.y = position.y + size + (velocity.y * (float) (uS / timeScale));
    return nextPosition;
}

sf::Vector2f particle::getMomentum() {
    sf::Vector2f momentum;
    momentum.x = velocity.x * mass;
    momentum.y = velocity.y * mass;
    return momentum;
}

float particle::getMass() {
    return mass;
}

int particle::getSize() {
    return size;
}

void particle::setVelocity(sf::Vector2f vel) {
    velocity = vel;
}

void particle::applyForce(sf::Vector2f force, float uS) {
    velocity.x += force.x * (float)(uS / timeScale);
    velocity.y += force.y * (float)(uS / timeScale);
}

void particle::updatePosition(float uS) { // No Offset
    position.x = position.x + (velocity.x * (float) (uS / timeScale));
    position.y = position.y + (velocity.y * (float) (uS / timeScale));
    body.setPosition(sf::Vector2f(position.x, position.y));
}