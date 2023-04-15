#include  <SFML/Graphics.hpp>
#include<iostream>
#include "verlet.hpp"
#include"render.hpp"

int main()
{  
    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Hello");
    window.setFramerateLimit(60);
    solver solver;
    solver.setframerate(60);
    solver.setSubstep(8);
    solver.set_dt();
    verletObject a(sf::Vector2f(900, 300), 25);
    a.setvelocity(sf::Vector2f(-4, 0), solver.get_dt());
    solver.add_object(a);
    
    solver.gridInitialize();
    renderer renderer(solver.get_object(), window);
    




    sf::Event event;

    while (window.isOpen()) {

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {

                window.close();
            }
        }
        
        //physics
        if (solver.getObjectCount() < 400 and clock.getElapsedTime().asSeconds() >= 0.25) {
            clock.restart();
            verletObject D(sf::Vector2f(500, 100), 25);
            D.setvelocity(sf::Vector2f(-1, 0), solver.get_dt());
            solver.add_object(D);
        }
        
        //render
        window.clear(sf::Color::Cyan);
        solver.update();
        renderer.render();
        window.display();

    }


    return 0;
}