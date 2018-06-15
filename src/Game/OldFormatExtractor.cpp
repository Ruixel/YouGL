#include "OldFormatUtil.h"

#include <regex>
#include <iostream>

namespace
{
    const Property::Colour WHITE = { 255, 255, 255 };

    Property::Colour stringToColour(const std::string& colourString)
    {
        size_t valueEnd;
        int pointer = 7;
        auto getComponent = [&](sf::Uint8& comp)
        {
            valueEnd = colourString.find(",", pointer);
            comp = (sf::Uint8)std::stoi(colourString.substr(pointer, valueEnd - pointer));
            pointer = valueEnd + 2;
        };

        Property::Colour colour;
        getComponent(colour.r);
        getComponent(colour.g);
        getComponent(colour.b);
        return colour;
    }

    Property::Position extractVector(const std::smatch& match, int firstIndex)
    {
        Property::Position vector;
        //Extract x
        vector.x = (u16)std::stoi(match[firstIndex]);

        //Extract y
        vector.y = (u16)std::stoi(match[firstIndex + 1]);

        return vector;
    }

    Property::Material extractMaterial(const std::smatch& match, int firstIndex)
    {
        Property::Material material;

        if (match[firstIndex] != "") { //Colour
            material.colour = stringToColour(match[firstIndex].str());
        }
        else {  //Texture
            material.textureId = (u8)std::stoi(match[firstIndex + 1].str());
            material.colour = WHITE;
        }
        return material;
    }
}

namespace OldFormat
{
    LevelHeader extractHeader(std::string& levelData)
    {
        LevelHeader header;
        std::smatch matchGroups;
        std::regex headerRegex("#name: \"([^\"]+)\", #levels: (\\d+), #version: ([^,]+), #creator: \"([^\"]+)\"");

        if (std::regex_search(levelData, matchGroups, headerRegex)) {
            header.gameName = matchGroups[1].str();
            header.floorCount = (u8)std::stoi(matchGroups[2].str());
            header.gameAuthor = matchGroups[4].str();
        }
        else {
            std::cout << "Cannot match header\n";
            return {};
        }
        return header;
    }

    std::vector<Wall> extractWalls(std::string& wallData)
    {
        std::vector<Wall> walls;
        std::smatch matchGroups;
        std::regex wallRegex("\\[(-?\\d+), (-?\\d+), (-?\\d+), (-?\\d+), \\[(c[^\\)]*\\))?(\\d+)?, (c[^\\)]*\\))?(\\d+)?\\]?, (\\d+)\\],? ?(\\d+)?");
        while (std::regex_search(wallData, matchGroups, wallRegex))
        {
            Wall wall;
            //Wall position
            auto displacement = extractVector(matchGroups, 1);
            wall.startPosition = extractVector(matchGroups, 3);
            wall.endPosition = { displacement.x + wall.startPosition.x,
                displacement.y + wall.startPosition.y };

            //Texture
            wall.frontMaterial = extractMaterial(matchGroups, 5);
            wall.backMaterial = extractMaterial(matchGroups, 7);

            //Height and floor
            if (matchGroups[10] != "") {
                wall.height = (u8)std::stoi(matchGroups[9].str());
                wall.floor  = (u8)std::stoi(matchGroups[10].str());
            }
            else {//Old versions have no wall height
                wall.height = 1;
                wall.floor = (u8)std::stoi(matchGroups[9].str());
            }
            walls.push_back(wall);
            wallData = matchGroups.suffix();


        }
        return walls;
    }

    std::vector<Platform> extractPlatforms(std::string& platformData)
    {
        std::vector<Platform> platforms;
        //std::smatch matchGroups;
        //std::regex reg_plats("\\[\\[([\\d\\.]+), ([\\d\\.]+)\\], \\[(\\d+),? ?(c[^\\)]*\\))?(\\d+)?,? ?(\\d+)?\\], (\\d+)");
        return platforms;
    }
}

/*
#include "CYPlat.h"

CYPlat::CYPlat(const std::smatch& matchGroups)
{
std::ssub_match subMatch;

subMatch = matchGroups[1];
//std::cout << "X: " << subMatch.str() << std::endl;

subMatch = matchGroups[2];
//std::cout << "Y: " << subMatch.str() << std::endl;

// If group 4 and 5 are empty then it's
// [X, Y, [Size], Level]
if (matchGroups[4] == "" && matchGroups[5] == "") {
subMatch = matchGroups[3];
//std::cout << "Texture: " << subMatch.str() << std::endl;
} else {
// Else, 4/5 are texture and if 6 exists, it's the Z_Index
subMatch = matchGroups[3];
//std::cout << "Size: " << subMatch.str() << std::endl;

// 4 = Colour, 5 = TextureID
if (matchGroups[4] != "")
subMatch = matchGroups[4];
else
subMatch = matchGroups[5];
//std::cout << "Texture: " << subMatch.str() << std::endl;

if (matchGroups[6] != "") {
subMatch = matchGroups[6];
//std::cout << "Z_Index: " << subMatch.str() << std::endl;
}
}

subMatch = matchGroups[7];
//std::cout << "Level: " << subMatch.str() << std::endl;
}

CYPlat::CYPlat(const json & jObj)
{

}

void CYPlat::createModel() {
}

void CYPlat::render(Renderer& renderer)
{

}

void CYPlat::toJsonFormat(json& jLevel, int id)
{
return;
}



// PLATFORMS
// V2.13 = [X_Pos, Y_Pos, [Texture], Level]
// V3.06-3.27 = [X_Pos, Y_Pos, [Size, Texture], Level]
// V3.37+ = [X_Pos, Y_Pos, [Size, Texture, Z_Index], Level]
// Tested on Version 2.13, 3.06, 3.09, 3.13, 3.27. 3.37, 3.52, 3.68
std::string platCode = cyTable.at("Plat");
std::regex reg_plats("\\[\\[([\\d\\.]+), ([\\d\\.]+)\\], \\[(\\d+),? ?(c[^\\)]*\\))?(\\d+)?,? ?(\\d+)?\\], (\\d+)");

while (std::regex_search(platCode, matchGroups, reg_plats))
{
cyLevel.addPlat(matchGroups);

platCode = matchGroups.suffix();
//std::cout << "------------------" << std::endl;
}



*/