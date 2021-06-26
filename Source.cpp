#include "Particle.h"

float getContactTime(particle* A, particle* B, float timeSlice) {
    sf::Vector2f delta;
    float distanceSquared;
    float timeU, timeL;
    int i = 0;
    float uS = timeSlice;
    timeU = 0;
    timeL = timeSlice;
    float radiSquared = pow((A->getSize() + B->getSize()), 2);

    //((A->getSize() * A->getSize()) + (B->getSize() * B->getSize()) + 2 * (A->getSize() * B->getSize()));
    delta.x = (A->getNextPosition(0).x - B->getNextPosition(0).x);
    delta.y = (A->getNextPosition(0).y - B->getNextPosition(0).y);
    distanceSquared = (delta.x * delta.x) + (delta.y * delta.y);
    //std::cout << "Starting distance: " << distanceSquared - radiSquared << "\n";

    if (distanceSquared - radiSquared <= -contactThreshold) {
        float a = pow(A->getVelocity().x, 2) + pow(B->getVelocity().x, 2) - 2 * A->getVelocity().x * B->getVelocity().x;
        a += pow(A->getVelocity().y, 2) + pow(B->getVelocity().y, 2) - 2 * A->getVelocity().y * B->getVelocity().y;

        float b = (2 * A->getNextPosition(0).x * A->getVelocity().x) + (2 * B->getNextPosition(0).x * B->getVelocity().x) - (2 * A->getNextPosition(0).x * B->getVelocity().x) - (2 * A->getVelocity().x * B->getNextPosition(0).x);
        b += (2 * A->getNextPosition(0).y * A->getVelocity().y) + (2 * B->getNextPosition(0).y * B->getVelocity().y) - (2 * A->getNextPosition(0).y * B->getVelocity().y) - (2 * A->getVelocity().y * B->getNextPosition(0).y);

        float c = pow(A->getNextPosition(0).x, 2) + pow(B->getNextPosition(0).x, 2) - (2 * A->getNextPosition(0).x * B->getNextPosition(0).x);
        c += pow(A->getNextPosition(0).y, 2) + pow(B->getNextPosition(0).y, 2) - (2 * A->getNextPosition(0).y * B->getNextPosition(0).y);
        //c -= (A->getSize() * A->getSize()) + (B->getSize() * B->getSize()) + (2 * A->getSize() * B->getSize());
        c -= radiSquared;


        //std::cout << "uS: " << (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a) << "\n";
        //std::cout << "uS: " << (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a) << "\n";
        uS = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
        delta.x = (A->getNextPosition(uS).x - B->getNextPosition(uS).x);
        delta.y = (A->getNextPosition(uS).y - B->getNextPosition(uS).y);
        distanceSquared = (delta.x * delta.x) + (delta.y * delta.y);
        //std::cout << "Calculated distance: " << distanceSquared - radiSquared << "\n";
        uS = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
        delta.x = (A->getNextPosition(uS).x - B->getNextPosition(uS).x);
        delta.y = (A->getNextPosition(uS).y - B->getNextPosition(uS).y);
        distanceSquared = (delta.x * delta.x) + (delta.y * delta.y);
        //std::cout << "Calculated distance: " << distanceSquared - radiSquared << "\n";
        //getchar();
    }


    // Binary Search with Threshold
    while ((distanceSquared - radiSquared > contactThreshold) || (distanceSquared - radiSquared < -contactThreshold)) { // Is abs needed?
        i++;
        uS = timeL + (timeU - timeL) / 2;
        //std::cout << "uS: " << uS << "\n";
        delta.x = (A->getNextPosition(uS).x - B->getNextPosition(uS).x);
        delta.y = (A->getNextPosition(uS).y - B->getNextPosition(uS).y);
        distanceSquared = (delta.x * delta.x) + (delta.y * delta.y);
        //std::cout << i << " - distance: " << distanceSquared - radiSquared << "\n";
        if (i > 20) {
            //getchar();
        }
        if (distanceSquared - radiSquared > 0) // 2x Radius
            timeU = uS;
        else
            timeL = uS;
    }

    //std::cout << i << " - Final distance: " << distanceSquared - radiSquared << "\n";
    //std::cout << "uS: " << uS << "\n";
    return uS;
}

sf::Vector2f getCollisionVelocity(particle* A, particle* B, unsigned int particle) {
    sf::Vector2f velocity;
    float totalMass = A->getMass() + B->getMass();
    sf::Vector2f momentumA = A->getMomentum();
    sf::Vector2f momentumB = B->getMomentum();

    if (particle == 0) {
        velocity.x = (2 * momentumB.x + momentumA.x - (B->getMass() * A->getVelocity().x)) / totalMass;
        velocity.y = (2 * momentumB.y + momentumA.y - (B->getMass() * A->getVelocity().y)) / totalMass;
    }
    else if (particle == 1) {
        velocity.x = (2 * momentumA.x + momentumB.x - (A->getMass() * B->getVelocity().x)) / totalMass;
        velocity.y = (2 * momentumA.y + momentumB.y - (A->getMass() * B->getVelocity().y)) / totalMass;
    }
    else
        std::cout << "Unexpected value passed to 'getCollisionVelocity'!\n";
    return velocity;
}

void setupGas(particle* particles[particleCount], sf::RectangleShape* container) {
    int i, j;
    bool collisionFlag;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float mass;
    int size;
    sf::Vector2f nextPosition;

    float radiSquared;
    sf::Vector2f delta;
    float distanceSquared;
    sf::Vector2f momentum;

    // Generate Particles
    for (i = 0; i < particleCount; i++) {
        collisionFlag = true;

        //mass = particleMass + rand() % (int)particleMass;
        mass = 1;
        size = mass * particleSize;//Uniform Density

        while (collisionFlag) {
            collisionFlag = false;
            position.x = rand() % ((int)container->getGlobalBounds().left + containerWidth);
            position.y = rand() % ((int)container->getGlobalBounds().top + containerHeight);
            // Wall Positioning
            if ((position.x + (2 * size) >= container->getGlobalBounds().left + containerWidth) || (position.x - (2 * size) <= container->getGlobalBounds().left))
                collisionFlag = true;
            if ((position.y + (2 * size) >= container->getGlobalBounds().top + containerHeight) || (position.y - (2 * size) <= container->getGlobalBounds().top))
                collisionFlag = true;

            // Inter-particle Positioning
            for (j = 0; j < i; j++) {
                if (j != i) {
                    radiSquared = pow((size + particles[j]->getSize()), 2);
                    delta.x = (particles[j]->getPosition().x - position.x - (size));//?
                    delta.y = (particles[j]->getPosition().y - position.y - (size));
                    distanceSquared = (delta.x * delta.x) + (delta.y * delta.y);
                    if (distanceSquared <= radiSquared) // 2x Radius
                        collisionFlag = true;
                }
            }
            if (rand() % 2)
                velocity.x = rand() % (int)maxSpeed;
            else
                velocity.x = -(rand() % (int)maxSpeed);
            if (rand() % 2)
                velocity.y = rand() % (int)maxSpeed;
            else
                velocity.y = -(rand() % (int)maxSpeed);
        }

        particles[i] = new particle(i, position, velocity, mass, size);
        std::cout << i + 1 << " particles placed.\n";
    }
}

void setupLiquid(particle* particles[particleCount], sf::RectangleShape* container) {
    int i, j;
    sf::Vector2f position;

    float radiSquared;
    sf::Vector2f delta;
    float distanceSquared;
    sf::Vector2f momentum;
    j = 0;

    position.x = (containerWidth / 2);
    position.y = (containerHeight / 2);

    // Generate Particles
    for (i = 0; i < particleCount; i++) {
        particles[i] = new particle(i, position, sf::Vector2f(0,0), particleMass, particleSize);
        std::cout << i + 1 << " particles placed.\n";

        position.x += 2 * particleSize + 1;
        if (position.x - (containerWidth / 2) + 2 * particleSize >= containerWidth) {
            if (j % 2 == 0)
                position.x = (containerWidth / 2) + particleSize;
            else
                position.x = (containerWidth / 2);
            position.y += 2 * particleSize;
            j += 1;
        }
    }
}

int main()
{
    // Init
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Particle Simulation");
    sf::Event event;
    sf::Clock simClock;
    sf::Font fontLucidaFax;
    sf::Text textFrameRate;
    sf::Text valueFrameRate;
    sf::RectangleShape container;
    particle* particles[particleCount];
    float lastFrameTime;
    float timeSlice;
    int i, j;
    bool collisionFlag;

    sf::Vector2f tmpPosition;
    sf::Vector2f tmpVelocity;
    float tmpMass;
    int tmpSize;
    sf::Vector2f tmpNextPosition;

    float radiSquared;
    sf::Vector2f delta;
    float distanceSquared;
    sf::Vector2f momentum;

    // Setup
    container.setSize(sf::Vector2f(containerWidth, containerHeight));
    container.setOutlineColor(sf::Color::White);
    container.setOutlineThickness(1);
    container.setFillColor(sf::Color(0,0,0,0));
    container.setPosition(sf::Vector2f((windowWidth - containerWidth) / 2, (windowHeight - containerHeight) / 2));
    fontLucidaFax.loadFromFile("LFAX.TTF");
    textFrameRate.setFont(fontLucidaFax);
    textFrameRate.setCharacterSize(16);
    textFrameRate.setFillColor(sf::Color::Yellow);
    textFrameRate.setPosition(0, 0);
    textFrameRate.setString("FPS: ");
    std::srand(std::time(0));
    window.setActive(false); // Also improves performance?
    //antialiasing context
    //window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(60);

    // Opitmisations
    // Mulitplying by itself twice is faster than pow function.

    //setupGas(particles, &container);
    setupLiquid(particles, &container);

    lastFrameTime = simClock.getElapsedTime().asMicroseconds();
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (simClock.getElapsedTime().asMicroseconds() - lastFrameTime >= frameInterval) {

            for (i = 0; i < particleCount; i++) {
                timeSlice = frameInterval * timeMultiplier;

                // Apply Force
                particles[i]->applyForce(sf::Vector2f(0, 9.81), timeSlice);
                
                // Inter-particle Collision
                tmpNextPosition = particles[i]->getNextPosition(timeSlice);
                tmpMass = particles[i]->getMass();
                tmpSize = particles[i]->getSize();
                for (j = 0; j < particleCount; j++) {
                    if (j != i) {
                        radiSquared = pow((tmpSize + particles[j]->getSize()), 2);
                        delta.x = (tmpNextPosition.x - particles[j]->getNextPosition(timeSlice).x);
                        delta.y = (tmpNextPosition.y - particles[j]->getNextPosition(timeSlice).y);
                        distanceSquared = (delta.x*delta.x) + (delta.y*delta.y);
                        if (distanceSquared <= radiSquared)
                        {
                            //std::cout << "\nProjected distance: " << distanceSquared - radiSquared << "\n";
                            //timeSlice = getContactTime(particles[i], particles[j], timeSlice);
                            tmpVelocity = getCollisionVelocity(particles[i], particles[j], 1);// SHOULD BE POST UPDATE
                            particles[i]->setVelocity(getCollisionVelocity(particles[i], particles[j], 0));
                            particles[j]->setVelocity(tmpVelocity);
                            //particles[i]->body.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
                            //particles[j]->body.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
                        }
                    }
                }
                // Wall Collision
                tmpNextPosition = particles[i]->getNextPosition(timeSlice);
                tmpVelocity = particles[i]->getVelocity();
                if ((tmpNextPosition.x + tmpSize >= container.getGlobalBounds().left + containerWidth) || (tmpNextPosition.x - tmpSize <= container.getGlobalBounds().left))
                    tmpVelocity.x = -tmpVelocity.x * restitution;
                if ((tmpNextPosition.y + tmpSize >= container.getGlobalBounds().top + containerHeight) || (tmpNextPosition.y - tmpSize <= container.getGlobalBounds().top))
                    tmpVelocity.y = -tmpVelocity.y * restitution;
                particles[i]->setVelocity(tmpVelocity);

                particles[i]->updatePosition(timeSlice);
            }

            window.clear();

            window.draw(container);

            for (i = 0; i < particleCount; i++) {
                window.draw(particles[i]->body);
            }

            textFrameRate.setString("FPS: " + std::to_string(static_cast<int>(round(timeScale / (simClock.getElapsedTime().asMicroseconds() - lastFrameTime)))));
            lastFrameTime = simClock.getElapsedTime().asMicroseconds();
            window.draw(textFrameRate);

            window.display();
        } 
    }

    std::cout << "Simulation exiting!\n";
    for (i = 0; i < particleCount; i++) {
        delete particles[i];
    }

    return 0;
}
