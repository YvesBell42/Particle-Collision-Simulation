#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <string>

const int windowWidth = 640;//1200
const int windowHeight = 640;
const int maxFrameRate = 60;
const int timeScale = 1000000; // microsecond
const int zoomScale = 1;
const float frameInterval = timeScale / maxFrameRate;// uS
const float timeMultiplier = 10;
const int particleCount = 500;
const int particleSize = 5;// m
const float particleMass = 10;// kg
const float maxSpeed = 10;// m/s
const float restitution = 0.5;
const float contactThreshold = 0.1;// m
const int containerWidth = 320;//480
const int containerHeight = 320;

    /*threadParams params;
    params.window = &window;
    params.container = &container;
    params.particles = *particles;
    sf::Thread thread(&collisionThread, &params);
    thread.launch();*/

/*sf::Mutex GlobalMutex;

struct threadParams {
    sf::RenderWindow* window;
    particle* particles;
    sf::RectangleShape* container;
};

void collisionThread(threadParams* paramsPtr)
{
    threadParams* params = static_cast<threadParams*>(paramsPtr);

    int i, j;
    sf::Vector2f tmpPosition;
    sf::Vector2f tmpVelocity;
    float tmpMass;
    int tmpSize;
    sf::Vector2f tmpNextPosition;
    float timeSlice;
    sf::Vector2f delta;
    float distance;
    while (params->window->isOpen())
    {
        GlobalMutex.lock();
        for (i = 0; i < particleCount; i++) {
            timeSlice = (float)frameInterval * timeMultiplier;

            // Inter-particle Collision
            tmpNextPosition = params->particles[i].getNextPosition(timeSlice);
            for (j = 0; j < particleCount; j++) {
                if (j != i) {
                    delta.x = (tmpNextPosition.x - params->particles[j].getNextPosition(timeSlice).x);
                    delta.y = (tmpNextPosition.y - params->particles[j].getNextPosition(timeSlice).y);
                    distance = (delta.x * delta.x) + (delta.y * delta.y);
                    if (distance <= (4 * particleSize * particleSize)) // 2x Radius
                    {
                        // Do all possible collisions need to be checked recursively before iterating by timeSlice?
                        //timeSlice = getContactTime(particles[i], particles[j], timeSlice);
                        tmpVelocity = getCollisionVelocity(&params->particles[i], &params->particles[j], 1);
                        params->particles[i].setVelocity(getCollisionVelocity(&params->particles[i], &params->particles[j], 0));
                        params->particles[j].setVelocity(tmpVelocity);
                        params->particles[i].body.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
                        params->particles[j].body.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
                    }
                }
            }
            // Wall Collision
            tmpNextPosition = params->particles[i].getNextPosition(timeSlice);
            tmpVelocity = params->particles[i].getVelocity();
            if ((tmpNextPosition.x + particleSize >= params->container->getGlobalBounds().left + containerWidth) || (tmpNextPosition.x - particleSize <= params->container->getGlobalBounds().left))
                tmpVelocity.x = -tmpVelocity.x;
            if ((tmpNextPosition.y + particleSize >= params->container->getGlobalBounds().top + containerHeight) || (tmpNextPosition.y - particleSize <= params->container->getGlobalBounds().top))
                tmpVelocity.y = -tmpVelocity.y;
            params->particles[i].setVelocity(tmpVelocity);

            params->particles[i].updatePosition(timeSlice);
        }
        GlobalMutex.unlock();
    }
}*/