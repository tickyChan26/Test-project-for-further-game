#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct Platform
{
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Image image;
};

// ���������� ������ ��������
extern std::vector<Platform> Platforms;
