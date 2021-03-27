#pragma once

#include <cstdint>
#include <vector>
#include "vec.h"
#include "Level.h"
#include <cmath>
#include <SFML/Graphics.hpp>

struct Connection;

struct Point {
	vec2f pos;
	vec2f velocity;
	std::vector<Connection*> connections;

	Point(vec2f pos)
		:pos(pos) {};
};
struct Rigid : public Point {

	std::vector<vec2f> prevForces;

	void addForces();
	void updatePosition();

	Rigid(vec2f pos)
		:Point(pos)	{};
};
struct Static : public Point {
	Static(vec2f pos)
		:Point(pos) {};
};

struct Connection {
	Point* points[2];
	float length;
	float mass = 10;
	float damping = 600; 
	float stiffness = 600000;

	float lenMin = 0.95f;
	float lenMax = 1.05f;

	float currentLength = 0;
	float currentForce = 0;

	vec2f center;

	bool active = true;

	void UpdateLength();
	void UpdateForce();

	Connection(Point* a, Point* b) 
		: Connection(a,b,0)
	{
		length = abs((a->pos - b->pos).length());
		if (length == 0)
			active = false;
	}
	Connection(Point* a, Point* b, float length)
		:length(length)
	{
		points[0] = a;
		points[1] = b;

		a->connections.push_back(this);
		b->connections.push_back(this);
	}
	Connection(const Connection& connection) 
	{
		points[0] = connection.points[0];
		points[1] = connection.points[1];

		length = connection.length;

		for (auto& i : points[0]->connections)
			if (i == &connection)
				i = this;
		for (auto& i : points[1]->connections)
			if (i == &connection)
				i = this;
	}
};

class World {
public:
	std::vector<Static> statics;
	std::vector<Rigid>  rigids;
	std::vector<Connection> connections;

	sf::Vector2f lastPos;
	sf::RenderWindow* window;

	World(sf::RenderWindow* window) 
		:window(window)	{}

	inline void Reset() {
		lastPos = (sf::Vector2f)(window->getPosition() / 8);
	};

	void Update();
	void Render();
	void CheckCollisions();

	void Load(const std::string& s);

private:

};

struct MouseEvent
{
	bool down;
	sf::Mouse::Button button;
	sf::Vector2f pos;
};

class WorldEditor {
public:
	WorldEditor(sf::RenderWindow* window)
		:w(window)
	{}

	void Load(const std::string& s) {
		w.rigids.reserve(128);
		w.statics.reserve(256);
		w.connections.reserve(512);
		w.Load(s);
	}
	void Click(MouseEvent e);
	Point* GetClosestPoint(vec2f pos);
	void Render(sf::Vector2f mousePos);
	void RenderGrid();
	void TryDeletePoint(Point* p);
	std::string GetData();

	World w;
	Point* selected = nullptr;
	bool play = false;
	std::string data = "";
	bool movingPoint = false;
};