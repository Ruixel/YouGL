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
    m_renderData.vao			= model.m_renderData.vao;
	m_renderData.indicesCount	= model.m_renderData.indicesCount;
	m_renderData.textureID		= model.m_renderData.textureID;
	m_renderData.m_mode			= model.m_renderData.m_mode;

    model.m_renderData.vao			= 0;
	model.m_renderData.indicesCount = 0;
	model.m_renderData.textureID	= 0;
	model.m_renderData.m_mode		= GL_POLYGON;
}

Model& Model::operator=(Model&& model)
{
	m_renderData.vao			= model.m_renderData.vao;
	m_renderData.indicesCount	= model.m_renderData.indicesCount;
	m_renderData.textureID		= model.m_renderData.textureID;
	m_renderData.m_mode			= model.m_renderData.m_mode;

    m_buffers = std::move(model.m_buffers);

    model.m_renderData.vao = 0;
	model.m_renderData.indicesCount = 0;
	model.m_renderData.textureID = 0;
	model.m_renderData.m_mode = GL_POLYGON;

    return *this;
}

Model::~Model()
{
    if (m_renderData.vao) {
        glDeleteVertexArrays(1, &m_renderData.vao);
        glDeleteBuffers(m_buffers.size(), m_buffers.data());
    }
}

void Model::destroyModel()
{
	if (m_renderData.vao) {
		glDeleteVertexArrays(1, &m_renderData.vao);
		glDeleteBuffers(m_buffers.size(), m_buffers.data());
	}

	m_renderData.vao = 0;
	m_renderData.indicesCount = 0;
	m_renderData.textureID = 0;
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

    GLuint currBuffer = 0;
    glGenBuffers(m_buffers.size(), m_buffers.data());
    createBuffer(currBuffer++, 3, mesh.vertices);
    createBuffer(currBuffer++, 3, mesh.texCoords);
    createBuffer(currBuffer++, 3, mesh.normals);
    createBuffer(currBuffer++, 3, mesh.colour);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[currBuffer]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.indices.size(), mesh.indices.data(), GL_STATIC_DRAW);
    m_renderData.indicesCount = mesh.indices.size();
    mesh.clearData();
}

void Model::create(Mesh& mesh, GLuint textureID)
{
	this->create(mesh);
	m_renderData.textureID = textureID;
}

const RenderData& Model::getRenderData() const
{
    return m_renderData;
}

void Model::setMode(GLenum m)
{
	m_renderData.setMode(m);
}

