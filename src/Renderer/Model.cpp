#include "Model.h"

#include "Mesh.h"
#include <iostream>
Model::Model(Mesh& mesh)
{
    create(mesh);
}

Model::Model(Model&& model)
:   m_buffers(std::move(model.m_buffers))
{
    m_renderData.vao = model.m_renderData.vao;
    model.m_renderData.vao = 0;
}

Model& Model::operator=(Model&& model)
{
    m_renderData.vao = model.m_renderData.vao;
    m_buffers = std::move(model.m_buffers);

    model.m_renderData.vao = 0;
}

Model::~Model()
{
    if (m_renderData.vao) {
        glDeleteVertexArrays(1, &m_renderData.vao);
        glDeleteBuffers(m_buffers.size(), m_buffers.data());
    }
}

void Model::create(Mesh& mesh)
{
    auto createBuffer = [this](int index, int numDataPerPoint, const std::vector<GLfloat>& data)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_buffers[index]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(index, numDataPerPoint, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(index);
    };
    glGenVertexArrays(1, &m_renderData.vao);
    glBindVertexArray(m_renderData.vao);

    glGenBuffers(m_buffers.size(), m_buffers.data());
    createBuffer(0, 3, mesh.verticies);
    createBuffer(1, 2, mesh.texCoords);
    createBuffer(2, 3, mesh.normals);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.indices.size(), mesh.indices.data(), GL_STATIC_DRAW);
    m_renderData.indicesCount = mesh.indices.size();
    mesh.clearData();
}

const RenderData & Model::getRenderData() const
{
    return m_renderData;
}



