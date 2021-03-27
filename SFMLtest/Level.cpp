#include "Level.h"

#include "Timer.h"
#include <iostream>

static vec2f gravity = { 0, 10.0f };

static int steps = 300; 

static float delta = 0.01f / steps;

void World::Update()
{
	sf::Vector2f newPos = (sf::Vector2f)(window->getPosition() / 8);
	gravity = vec2f( 0, 10.0f ) + vec2f(lastPos - newPos) * 10;
	lastPos = newPos;

	for (int i = 0; i < steps; i++) {
		for (auto& connection : connections) {
			connection.UpdateForce();
		}
		for (auto& point : points) {
			if (point.isStatic)
				continue;
			point.addForces();
			point.updatePosition();
		}
		//CheckCollisions();
	}
}

static vec2f cam;

void World::Render()
{
	sf::VertexArray lines(sf::Lines, connections.size()*2);
	for (size_t i = 0; i < connections.size(); i++) {
		float size = (connections[i].currentLength / connections[i].length - 1.0f) * 4.0f;
		uint8_t b = 255 - (uint8_t)std::max((-size) * 255.0f, 0.0f); // compression
		uint8_t r = 255 - (uint8_t)std::max((size) * 255.0f, 0.0f);  // stretch
		sf::Color color(b, std::min(b, r), r, 255 * connections[i].active);

		lines[i * 2 + 0] = (sf::Vector2f)connections[i].points[0]->pos;
		lines[i * 2 + 0].color = color;
		lines[i * 2 + 1] = (sf::Vector2f)connections[i].points[1]->pos;
		lines[i * 2 + 1].color = color;
	}

	window->draw(lines);
}

void World::Load(const std::string& s)
{
	try {
		size_t space1 = s.find(' ', 0);
		size_t endln = s.find('\n', space1 + 1);

		int  pointsCount = std::stoi(s.substr(0, space1));
		int connectCount = std::stoi(s.substr(space1 + 1, endln - (space1 + 1)));

		points.reserve(pointsCount);
		connections.reserve(connectCount);

		for (size_t i = endln + 1; i < s.size(); i++) {
			if (s[i] == '\n')
				continue;

			size_t space = s.find(' ', i + 2);
			size_t endln = s.find('\n', space + 1);

			int x = std::stoi(s.substr(i + 2, space - i - 2));
			int y = std::stoi(s.substr(space + 1, endln - space - 1));

			if (s[i] == 'c') {
				connections.emplace_back(&points[x], &points[y]);
			}else{
				points.emplace_back(vec2f((float)x, (float)-y), s[i] == 's');
			}
			i = endln;
		}

	}
	catch (std::exception e) {
		std::cerr << "ERROR: wrong world format!";
	}
}

float normal(const vec2f& A, const vec2f& B, const vec2f& C) {
	vec2f a = A - B;
	vec2f b = A - C;
	return a.x * b.y - b.x * a.y;
}

void World::CheckCollisions() {
	for (auto&& i : connections) {
		for (auto&& j : points) {
			float curr = normal(i.points[0]->pos, i.points[1]->pos, j.pos);
			float prev = normal(i.points[0]->prevPos, i.points[1]->prevPos, j.prevPos);
			if (curr * prev < 0) {
				i.points[0]->pos = i.points[0]->prevPos;
				i.points[1]->pos = i.points[1]->prevPos;
				j.pos = j.prevPos;
			}
		}
	}
}

void Connection::UpdateLength() {
	currentLength = abs((points[0]->pos - points[1]->pos).length());
	center = (points[0]->pos + points[1]->pos) / 2;
}

void Connection::UpdateForce() {
	if (!active)
		return;

	UpdateLength();

	float stiffForce = ((currentLength - length) * abs(currentLength - length) * stiffness * delta);

	vec2f V = (points[0]->velocity - points[1]->velocity) * (points[0]->pos - points[1]->pos);
	float dampForce = damping * (V.x + V.y) / currentLength * delta;

	currentForce = stiffForce + dampForce;

	if (currentLength / length > lenMax || currentLength / length < lenMin)
	{
		active = false;
		currentForce = 0;
	}
}

void Point::addForces()
{
	prevForces.resize(connections.size());

	vec2f totalForce;
	for (int i = 0; i < connections.size(); i++) {
		vec2f force;
		force += gravity * connections[i]->mass * delta;
		force += (connections[i]->center - this->pos) * (connections[i]->currentForce/2);

		totalForce += force / connections.size();
		prevForces[i] = force;
	}
	this->velocity += totalForce;
}

void Point::updatePosition()
{
	prevPos = pos;
	pos += velocity * delta;
}