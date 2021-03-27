#include <SFML/Graphics.hpp>

#include <Windows.h>

#include <thread>
#include <functional>
#include <iostream>
#include <math.h>

#include "main.h"
#include "Level.h"

#include "levels.h"
#include "Timer.h"
#include <mutex>

std::pair<MouseEvent, bool> mouseClickPos = { MouseEvent(), false };
static sf::Vector2i mousePos;


INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT) {

}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Hýbej s oknem!!", sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    window.setActive(false);

    bool shouldClose = false;

    int (*renderFunc) (sf::RenderWindow * window, bool* shouldClose);
    if (true) // Should be editor?
        renderFunc = EditorRenderLoop;
    else
        renderFunc = WorldRenderLoop;

    std::thread renderThread(std::bind(renderFunc, &window, &shouldClose));

    sf::Event event;
    while (window.isOpen())
    {
        window.waitEvent(event);
        bool down = false;
        switch (event.type)
        {
        case sf::Event::MouseMoved:
            if (mouseClickPos.second)
                break;
            mouseClickPos.first.pos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
            break;

        case sf::Event::MouseButtonPressed:
            down = true;
        case sf::Event::MouseButtonReleased:
            if (mouseClickPos.second)
                break;

            mouseClickPos.first.button = event.mouseButton.button;
            mouseClickPos.first.pos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
            mouseClickPos.first.down = down;
            mouseClickPos.second = true;
            break;

        case sf::Event::Closed:
            shouldClose = true;
            renderThread.join();
            window.close();
            break;
        }

    }

    return 0;
}

int WorldRenderLoop(sf::RenderWindow* window, bool* shouldClose) {
    World world(window);
    world.Update();
    world.Load(bridgeHugeWorld);

    sf::View view;
    view.setCenter({ 0, 0 });
    view.zoom(0.1f);
    window->setView(view);

    sf::Clock c;

    world.Update();
    window->clear();
    world.Render();
    window->display();

    Sleep(1500);

    while (!*shouldClose)
    {
        //Timer t;
        window->setView(view);
        world.Update();

        window->clear();
        world.Render();

        window->display();
    }

    window->setActive(false);
    return 0;
}

int EditorRenderLoop(sf::RenderWindow* window, bool* shouldClose) {
    WorldEditor worldEditor(window);
    worldEditor.Load(testWorld);

    sf::View view;
    view.setCenter({ 0, 0 });
    view.zoom(0.1f);
    window->setView(view);

    sf::Clock c;

    while (!*shouldClose)
    {
        //Timer t;
        window->setView(view);
        if (mouseClickPos.second) {
            worldEditor.Click(mouseClickPos.first);
            mouseClickPos.second = false;
        }

        window->clear(sf::Color(0xffffffff * (!worldEditor.play)));
        worldEditor.Render(mouseClickPos.first.pos);

        window->display();
    }

    window->setActive(false);
    return 0;
}