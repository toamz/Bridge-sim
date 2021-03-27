#include "Level.h"
#include <Windows.h>
#include <iostream>

void WorldEditor::TryDeletePoint(Point* p) {
	if (p == nullptr)
		return;
	if (p->connections.size() == 0) {
		if (p == w.statics.end()._Ptr - 1)
			w.statics.pop_back();
		else if (p == w.rigids.end()._Ptr - 1)
			w.rigids.pop_back();
		return;
	}
}

Point* WorldEditor::GetClosestPoint(vec2f pos) {
	float distance = 1.f;
	Point* closest = nullptr;

	for (auto& node : w.rigids) {
		auto newDist = (node.pos - (vec2f)pos).length();
		if (newDist < distance) {
			distance = newDist;
			closest = &node;
		}
	}
	for (auto& node : w.statics) {
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
			w.rigids.clear();
			w.statics.clear();
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

	//Left = Create Link

	if (closest == nullptr) {
		if (selected == nullptr) {
			w.rigids.emplace_back(e.pos);
			selected = &w.rigids.back();
		}
		else {

			w.rigids.emplace_back(e.pos);
			w.connections.emplace_back(selected, &w.rigids.back());
			selected = &w.rigids.back();
		}
		return;
	}

	if (selected != nullptr) {
		if (selected == closest) {
			char state = 0;
			if (w.rigids.size() > 0 && closest == &w.rigids.back())
			{
				w.statics.emplace_back(closest->pos);
				selected = &w.statics.back();
				state = 1;
			}
			else if (w.statics.size() > 0 && closest == &w.statics.back())
			{
				w.rigids.emplace_back(closest->pos);
				selected = &w.rigids.back();
				state = 2;
			}
			if (state == 0)
				return;

			for (int i = 0; i < closest->connections.size(); i++) {
				selected->connections.push_back(closest->connections[i]);
				if (selected->connections[i]->points[0] == closest)
					selected->connections[i]->points[0] = selected;
				if (selected->connections[i]->points[1] == closest)
					selected->connections[i]->points[1] = selected;
			}
			if (state == 1)
				w.rigids.pop_back();
			else
				w.statics.pop_back();
			return;
		}
		w.connections.emplace_back(selected, closest);
		selected = closest;
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
	for (auto& i : w.rigids) {
		circle.setPosition(i.pos);
		w.window->draw(circle);
	}
	circle.setFillColor(sf::Color(64, 128, 64, 255));
	for (auto& i : w.statics) {
		circle.setPosition(i.pos);
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
	out += std::to_string(w.statics.size()) + " " + std::to_string(w.rigids.size()) + " " + std::to_string(w.connections.size()) + "\n\n";
	for (const auto& i : w.statics) {
		out += "S" + std::to_string((int)i.pos.x) + " S" + std::to_string(-(int)i.pos.y) + "\n";
	}
	for (const auto& i : w.rigids) {
		out += "R" + std::to_string((int)i.pos.x) + " R" + std::to_string(-(int)i.pos.y) + "\n";
	}
	void* staticBegin = w.statics.begin()._Ptr;
	void* staticEnd = w.statics.end()._Ptr;
	void* rigidBegin = w.rigids.begin()._Ptr;
	void* rigidEnd = w.rigids.end()._Ptr;

	out += "\n";

	for (const Connection& i : w.connections) {
		if (i.points[0] >= staticBegin && i.points[0] < staticEnd)
			out += "s" + std::to_string(((Static*)i.points[0] - staticBegin));
		else
			out += "r" + std::to_string(((Rigid*)i.points[0] - rigidBegin));

		if (i.points[1] >= staticBegin && i.points[1] < staticEnd)
			out += " s" + std::to_string(((Static*)i.points[1] - staticBegin)) + "\n";
		else
			out += " r" + std::to_string(((Rigid*)i.points[1] - rigidBegin)) + "\n";
	}
	//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	//std::cout << out;
	//SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	return out;
}