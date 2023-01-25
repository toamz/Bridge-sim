#include "Level.h"
#include <Windows.h>
#include <iostream>

void WorldEditor::TryDeletePoint(Point* p) {
	if (p == nullptr)
		return;
	if (p->connections.size() == 0) {
		if (p == w.points.end()._Ptr - 1)
			w.points.pop_back();
		return;
	}
}

Point* WorldEditor::GetClosestPoint(vec2f pos) {
	float distance = 1.f;
	Point* closest = nullptr;

	for (auto& node : w.points) {
		auto newDist = (node.pos - (vec2f)pos).length();
		if (newDist < distance) {
			distance = newDist;
			closest = &node;
		}
	}

	return closest;
}

void WorldEditor::Click(MouseEvent e) {
	if (play) {
		if (e.down)
			return;
		if (e.button == sf::Mouse::Middle) {
			play = false;
			selected = nullptr;
			w.connections.clear();
			w.points.clear();
			Load(data);
		}
		return;
	}
	e.pos.x = round(e.pos.x);
	e.pos.y = round(e.pos.y);

	if (e.button == sf::Mouse::Middle) { // Middle = play
		if (e.down)
			return;

		TryDeletePoint(selected);
		data = GetData();
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		std::cout << "Level Data:\n\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << data;
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "\nDataEnd;\n\n";
		w.Load(data);
		w.Reset();

		play = true;

		return;
	}

	Point* closest = GetClosestPoint(e.pos);

	if (e.button == sf::Mouse::Right) { // Right = Select
		if (e.down) { // Select / Delete
			if (closest != nullptr) {
				if (closest == selected) { // The same = delete
					if (closest->connections.size() > 0 && w.connections.end()._Ptr - 1 == *(closest->connections.end()._Ptr - 1)) {
						Connection& connection = *closest->connections.back();
						connection.points[0]->connections.pop_back();
						connection.points[1]->connections.pop_back();
						TryDeletePoint(connection.points[0]);
						TryDeletePoint(connection.points[1]);
						TryDeletePoint(connection.points[0]);
						w.connections.pop_back();
						if (w.connections.size() > 0)
							selected = w.connections.back().points[1];
						else selected = nullptr;
						return;
					}
					TryDeletePoint(selected);
					selected = w.connections.back().points[1];
					return;
				}
				//std::printf("x: %f y:%f\n", closest->pos.x, closest->pos.y);
			}
			if (selected != nullptr)
				TryDeletePoint(selected);
			selected = closest;
			if (selected != nullptr)
				movingPoint = true;
			return;
		}
		else {
			if (movingPoint) {
				selected->pos = e.pos;
			}
			movingPoint = false;
			return;
		}
	}

	if (e.down)
		return;

	//Left = Create

	if (closest == nullptr) {
		if (selected == nullptr) { // Create Point
			w.points.emplace_back(e.pos, false);
			selected = &w.points.back();
		}
		else { // Create Point and Link
			w.points.emplace_back(e.pos, false);
			w.connections.emplace_back(selected, &w.points.back());
			selected = &w.points.back();
		}
		return;
	}
	// Closest != nullptr
	if (selected != nullptr) {
		if (selected == closest) { // Switch Static
			selected->isStatic = !selected->isStatic;
		}
		else {
			w.connections.emplace_back(selected, closest);
			selected = closest;
		}
		return;
	}
}

void WorldEditor::RenderGrid() {
	sf::VertexArray lines(sf::Lines, 100 * 4);
	for (int i = 0; i < 100; i++) {
		int opacity = 30 + (i % 5 == 0) * 50;
		lines[i * 4 + 0] = sf::Vertex(sf::Vector2f((i - 50), -100), sf::Color(0, 0, 0, opacity));
		lines[i * 4 + 1] = sf::Vertex(sf::Vector2f((i - 50), 100), sf::Color(0, 0, 0, opacity));
		lines[i * 4 + 2] = sf::Vertex(sf::Vector2f(-100, (i - 50)), sf::Color(0, 0, 0, opacity));
		lines[i * 4 + 3] = sf::Vertex(sf::Vector2f(100, (i - 50)), sf::Color(0, 0, 0, opacity));
	}
	w.window->draw(lines);
}

void WorldEditor::Render(sf::Vector2f mousePos) {
	if (play) {
		w.Update();
		w.Render();
		return;
	}

	if (movingPoint) {
		mousePos.x = round(mousePos.x);
		mousePos.y = round(mousePos.y);
		selected->pos = mousePos;
	}

	RenderGrid();
	w.Render();

	sf::CircleShape circle(0.5f, 16);
	circle.setOrigin(sf::Vector2f(0.5f, 0.5f));
	circle.setFillColor(sf::Color(128, 128, 255, 255));
	for (auto& i : w.points) {
		circle.setPosition(i.pos);
		circle.setFillColor(sf::Color(128, 128 * !i.isStatic, 255 * !i.isStatic, 255));
		w.window->draw(circle);
	}

	if (selected != nullptr) {
		circle.setPosition(selected->pos);
		circle.setRadius(1);
		circle.setOrigin(sf::Vector2f(1, 1));
		circle.setFillColor(sf::Color(0, 0, 0, 64));
		w.window->draw(circle);
	}
}
std::string WorldEditor::GetData() {
	std::string out;
	out += std::to_string(w.points.size()) + " " + std::to_string(w.connections.size()) + "\n\n";
	for (const auto& i : w.points) {
		out += (i.isStatic ? "s " : "r ") + std::to_string((int)i.pos.x) + " " + std::to_string(-(int)i.pos.y) + "\n";
	}
	Point* pointsBegin = w.points.begin()._Ptr;

	out += "\n";

	for (const Connection& i : w.connections) {
		out += "c " + std::to_string(i.points[0] - pointsBegin) + " " + std::to_string(i.points[1] - pointsBegin) + "\n";
	}

	return out;
}