#include  <SFML/Graphics.hpp>
#include "verlet.hpp"
#include <vector>
#pragma once
class renderer {
private:
	std::vector<verletObject>* m_object;
	sf::RenderTarget& target;
	
public:
	renderer(std::vector<verletObject>* a, sf::RenderTarget& target) :m_object(a), target(target) {};
	void render() {
		
		sf::CircleShape circle1(500);
		circle1.setOrigin(500,500);
		circle1.setPosition(500,500);
		circle1.setPointCount(300);
		circle1.setFillColor(sf::Color::Black);
		target.draw(circle1);

		
		sf::CircleShape circle(m_object->at(0).radius);
		circle.setOrigin(m_object->at(0).radius, m_object->at(0).radius);
		for (int i = 0; i < m_object->size(); i++) {
			
			circle.setPosition(m_object->at(i).position_current);
			circle.setFillColor(sf::Color::Blue);
			target.draw(circle);

		}
	 }

};