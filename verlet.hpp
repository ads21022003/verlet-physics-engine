#include<SFML/Graphics.hpp>
#include <vector>
#pragma once

struct verletObject {
	 sf::Vector2f position_old;
	 sf::Vector2f position_current;
	 float radius; 
	 verletObject(sf::Vector2f position, float radius) :position_old(position), position_current(position), radius(radius) {};
	 sf::Vector2f acceleration = { 0,0};
	 void updatePosition(float dt) {
		 sf::Vector2f displacement = position_current - position_old;
		 position_old = position_current;
		 position_current = acceleration*dt*dt + position_old+displacement;
		 
		 acceleration = {};
	 }
	 void setvelocity(sf::Vector2f x,float dt) {
		 position_old -= x;
	 }


};

class solver {
public:
	void setframerate(int f) { frame_rate = f; }
	void setSubstep(int f) { sub_steps = f; }
	float get_dt () { return dt; }
	void set_dt() { dt = 1 / (float(frame_rate) * float(sub_steps)); } 
	
	int getObjectCount() { return m_object.size(); }
	void add_object(sf::Vector2f position, float radius) { m_object.push_back(verletObject(position,radius)); }
	void add_object(verletObject a) { m_object.push_back(a); }
	std::vector<verletObject>* get_object() { return &m_object; }
	int objectNo(){ return m_object.size(); }
	void update();
	void gridCreate();
	void gridInitialize();
	
private :
	int frame_rate;
	int sub_steps;
	float dt ;
	std::vector<verletObject> m_object;
	std::vector<std::vector <std::vector<int>>> grid;
	sf::Vector2f gravity = { 0,500 };
	void apply_gravity();
	
	void applyconstraint(verletObject* object);
	void applyboxconstraint();
	void updatePosition(float dt);
	sf::Vector2i gridDecider(verletObject a);
	std::vector<int> nearbyObjects(sf::Vector2i v);
	void checkCollision();
	void iterateNearby(std::vector<int> list);
	float distance(sf::Vector2f a);
	void resolveCollison(int i, int j);
	void step(float dt);
	




};
void solver::step(float dt) {
	apply_gravity();
	updatePosition(dt);
	applyboxconstraint();
	gridInitialize();
	gridCreate();
	checkCollision();
	
		
		
}
void solver::update() {
	for (int i = 0; i < sub_steps; i++) {
		step(dt);
	}

}
void solver::applyconstraint(verletObject* object) {
	sf::Vector2f constraint_center = { 500,500 };
	float radius = 450;
		sf::Vector2f distance = constraint_center - object->position_current;
		float d = sqrt(distance.x * distance.x + distance.y * distance.y);
		if (d > radius) {
			float diff = radius - d;
			object->position_current -= distance * (float(diff / d));
		}
}
void solver::applyboxconstraint() {
	for (int i = 0; i < m_object.size(); i++) {
		if (m_object[i].position_current.x < m_object[i].radius) {
			//float temp = m_object[i].position_old.x;
			//m_object[i].position_old.x = m_object[i].position_current.x;
			//m_object[i].position_current.x = temp;
			m_object[i].position_old.x =  m_object[i].radius;
			m_object[i].position_current.x =  m_object[i].radius;
		}
		if (m_object[i].position_current.x > (1000 - m_object[i].radius)) {
			m_object[i].position_old.x = 1000-m_object[i].radius;
			m_object[i].position_current.x =1000- m_object[i].radius;
		}
		if (m_object[i].position_current.y < m_object[i].radius) {
			//float temp = m_object[i].position_old.y;
			//m_object[i].position_old.y = m_object[i].position_current.y;
			//m_object[i].position_current.y = temp;
			m_object[i].position_old.y =  m_object[i].radius;
			m_object[i].position_current.y =  m_object[i].radius;
		}
		if (m_object[i].position_current.y > (1000-m_object[i].radius)) {
			//float temp = m_object[i].position_old.y;
			//m_object[i].position_old.y = m_object[i].position_current.y;
			//m_object[i].position_current.y = temp;
			m_object[i].position_old.y = 1000 - m_object[i].radius;
			m_object[i].position_current.y = 1000 - m_object[i].radius;
		}

	}
}
void solver::updatePosition(float dt) {
	for (int i = 0; i < m_object.size(); i++) {
		m_object[i].updatePosition(dt);
	}
}
void solver::apply_gravity() {
	for (int i = 0; i < m_object.size(); i++) {
		m_object[i].acceleration += gravity;
	}
}
sf::Vector2i solver::gridDecider(verletObject a) {
	sf::Vector2i v;
	if (a.position_current.x < 0 + a.radius ) {
		v.x = 0;
	} else if (a.position_current.x > 1000 - a.radius) {
		v.x = int (1000/ (2 * a.radius)) -1;
	}
	else {
		v.x = int(a.position_current.x / (2*a.radius));

	}
	if (a.position_current.y < 0 + a.radius) {
		v.y = 0;
	}
	else if (a.position_current.y >(1000 - a.radius)){
		v.y = int(1000 / (2*a.radius)) - 1;
	}
	else {
		v.y = int(a.position_current.y / (2 * a.radius));

	}
	return v;
}
void solver::gridCreate() {
	for (int i = 0; i < m_object.size(); i++) {
		sf::Vector2i v = gridDecider(m_object[i]);
		grid[v.x][v.y].push_back(i);
	}
}
void solver::gridInitialize() {
	std::vector <std::vector<std::vector<int>>> p(int(1000 / (2 * m_object[0].radius)));
	for (int i = 0; i < p.size(); i++) {
		std::vector<std::vector<int>> j (int(1000 / (2 * m_object[0].radius)));
		p[i] = j;
	}
	grid = p;
}
std::vector<int> solver::nearbyObjects(sf::Vector2i v) {
	std::vector<int> out;
	for (int i = v.x - 1; i < v.x + 2; i++) {
		for (int j = v.y - 1; j < v.y + 2; j++) {
			if (grid[i][j].size() != 0) {
				out.insert(out.end(), grid[i][j].begin(), grid[i][j].end());
			}
		}
	}
	return out;
}
float solver::distance(sf::Vector2f a) {
	return (sqrtf(a.x * a.x + a.y * a.y));
}
void solver::resolveCollison(int i, int j) {
	sf::Vector2f disp = m_object[i].position_current - m_object[j].position_current;
	float resposnse_coff = 0.75;
	float dst = distance(disp);
	float min_dst = (m_object[i].radius + m_object[j].radius);
	if (dst < min_dst ) {
		sf::Vector2f n = disp / dst;
		float delta = 0.5 * resposnse_coff * (dst - min_dst);
		m_object[i].position_current -= n * delta;
		m_object[j].position_current += n * delta;

	}
}
void solver::iterateNearby(std::vector<int> list) {
	
	for (int i = 0; i < list.size(); i++) {
		for (int j = 0; j < list.size(); j++){
			if (i != j) {
				resolveCollison(list[i], list[j]);
			}
		}
	}
}
void solver::checkCollision() {
	for (int i = 1; i < (1000 / (2 * m_object[0].radius) - 1); i++) {
		for (int j = 1; j < (1000 / (2 * m_object[0].radius) - 1); j++) {
			sf::Vector2i v = { i,j };
			std::vector<int> g = nearbyObjects(v);
			iterateNearby(g);
		}
	}
}
     