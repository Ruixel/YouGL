#include "StatePlaying.h"

#include "../Game.h"

#include <vector>
#include <glad.h>
#include <SFML/Audio.hpp>

#include "../Renderer/Model.h"
#include "../Renderer/Mesh.h"

#include "../CYWebParser.h"

std::vector<GLfloat> vertices =
{
    0.5f,  0.5f, 0.0f,  // Top Right
    0.5f, -0.5f, 0.0f,  // Bottom Right
    -0.5f, -0.5f, 0.0f,  // Bottom Left
    -0.5f,  0.5f, 0.0f   // Top Left
};

std::vector<GLfloat> texCoords =
{
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f
};

std::vector<GLfloat> normals =
{
    1.0f, 1.0f, 1.0f,
    0.5f, 0.6f, 0.7f,
    0.7f, 0.5f, 0.3f,
    1.0f, 0.5f, 0.5f
};

std::vector<GLfloat> colours =
{
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
    0.5f, 1.0f, 0.7f
};



std::vector<GLuint> indices =
{
    0, 1, 3,   // First Triangle
    1, 2, 3    // Second Triangle
};

Mesh mesh;
Model model;

StatePlaying::StatePlaying(Game& game)
:   StateBase   (game)
{
    // Load level
    m_level = CYWebParser::loadFileFromWebsite(100000);

    mesh.vertices = vertices;
    mesh.texCoords = texCoords;
    mesh.normals = normals;
    mesh.colour = colours;

    mesh.indices = indices;
    model.create(mesh);
}

void StatePlaying::handleEvent(sf::Event e)
{

}

void StatePlaying::handleInput()
{

}

void StatePlaying::update(sf::Time deltaTime)
{

}

void StatePlaying::fixedUpdate(sf::Time deltaTime)
{

}

void StatePlaying::render(Renderer& renderer)
{
    renderer.draw(model);

    m_level.drawGeometry(renderer);
}
