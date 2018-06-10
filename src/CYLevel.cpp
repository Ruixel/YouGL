#include "CYLevel.h"

CYLevel::CYLevel()
: m_octree(512) {

}

///////////// WEB FORMAT /////////////
void CYLevel::addWall(const std::smatch& matchGroups)
{
    std::shared_ptr<EditorObject> wall = std::make_shared<CYWall>(matchGroups);
    m_editorObjects.push_back(std::move(wall));
}

void CYLevel::addPlat(const std::smatch& matchGroups)
{
    std::shared_ptr<EditorObject> plat = std::make_shared<CYPlat>(matchGroups);
    m_editorObjects.push_back(std::move(plat));
}

///////////// JSON FORMAT /////////////
void CYLevel::addWall(const json& jObj)
{
	std::shared_ptr<EditorObject> wall = std::make_shared<CYWall>(jObj);
	m_editorObjects.push_back(std::move(wall));
}

void CYLevel::addPlat(const json& jObj)
{
	std::shared_ptr<EditorObject> plat = std::make_shared<CYPlat>(jObj);
	m_editorObjects.push_back(std::move(plat));
}


bool CYLevel::serializeIntoJsonFormat(std::string file_name)
{
    int id = 0;

    json jLevel;

    // Level Headers
    jLevel["NAME"] = m_header.name;
    jLevel["AUTHOR"] = m_header.author;
	jLevel["VERSION"] = roundf(m_header.version * 100);
    jLevel["LEVELS"] = m_header.levels;

    // WALLS
    for (auto obj : this->m_editorObjects)
    {
        if ( obj->type == "WALL")
        {
            obj->toJsonFormat(jLevel["WALLS"][std::to_string(id)], id);
            id++;
        }

    }

    // Write to file
    std::ofstream file(file_name);
    file << jLevel.dump();
    file.close();

    return true;
}

void CYLevel::buildGeometry()
{
    for (auto obj : m_editorObjects)
    {
        obj->createModel();
    }
}

void CYLevel::drawGeometry(Renderer& renderer)
{
    for (auto obj : m_editorObjects)
    {
        obj->render(renderer);
    }
}

void CYLevel::destroyObjects()
{
    // Smart Pointers ;)
    /*for (auto e_obj : m_editorObjects)
    {
        delete e_obj;
    }*/
}
