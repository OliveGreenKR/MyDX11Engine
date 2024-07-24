#pragma once
#ifndef _OBJLOADER_H_
#define _OBJLOADER_H_

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Modelclass.h"

using namespace DirectX;
using namespace std;

class ObjLoader
{
    using VertexInfo = ModelClass::VertexInfo;

private:

    __forceinline int GetVertexIndex(const string& indexStr)
	{
		int result = stoi(indexStr);

        return result < 0 ? result : result - 1;
	}

public:
    
    bool Load(const string& filename, OUT VertexInfo* mesh)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cerr << "Failed to open file: " << filename << endl;
            return false;
        }

        vector<XMFLOAT3> positions;
        vector<XMFLOAT2> texCoords;
        vector<XMFLOAT3> normals;
        vector<int> positionIndices, texCoordIndices, normalIndices;

        string line;
        while (getline(file, line))
        {
            istringstream iss(line);
            string prefix;
            iss >> prefix;

            if (prefix == "v")
            {
                XMFLOAT3 position;
                iss >> position.x >> position.y >> position.z;
                positions.push_back(position);
            }
            else if (prefix == "vt")
            {
                XMFLOAT2 texCoord;
                iss >> texCoord.x >> texCoord.y;
                texCoords.push_back(texCoord);
            }
            else if (prefix == "vn")
            {
                XMFLOAT3 normal;
                iss >> normal.x >> normal.y >> normal.z;
                normals.push_back(normal);
            }
            else if (prefix == "f")
            {
                string vertexData;
                for (int i = 0; i < 3; ++i)
                {
                    iss >> vertexData;
                    istringstream viss(vertexData);
                    string index;
                    int posIndex = 0, texIndex = 0, normIndex = 0;

                    getline(viss, index, '/');
                    posIndex = GetVertexIndex(index);

                    if (getline(viss, index, '/'))
                    {
                        if (!index.empty())
                        {
                            texIndex = stoi(index) - 1;
                        }
                        if (getline(viss, index, '/'))
                        {
                            if (!index.empty())
                            {
                                normIndex = stoi(index) - 1;
                            }
                        }
                    }

                    positionIndices.push_back(posIndex);
                    texCoordIndices.push_back(texIndex);
                    normalIndices.push_back(normIndex);
                }
            }
        }

        mesh = new VertexInfo[positionIndices.size()];

        for (size_t i = 0; i < positionIndices.size(); ++i)
        {
            
            positionIndices[i] = positionIndices[i] < 0 ? positionIndices[i] + positions.size() : positionIndices[i];

            XMFLOAT3 position = positions[positionIndices[i]];
            XMFLOAT2 texCoord;
            XMFLOAT3 normal;

            mesh[i].x = position.x;
            mesh[i].x = position.x;
            mesh[i].x = position.x;

            if (!texCoords.empty())
            {
                texCoord = texCoords[texCoordIndices[i]];
                mesh[i].tu = texCoord.x;
                mesh[i].tv = texCoord.y;
            }
            if (!normals.empty())
            {
                normal = normals[normalIndices[i]];
                mesh[i].nx = normal.x;
                mesh[i].ny = normal.y;
                mesh[i].nz = normal.z;
            }
        }

        return true;
    }
};

#endif