#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct Platform
{
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Image image;
};

// Глобальный вектор платформ
extern std::vector<Platform> Platforms;
