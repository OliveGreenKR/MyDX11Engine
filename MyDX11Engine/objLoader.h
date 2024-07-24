#pragma once
#ifndef _OBJLOADER_H_
#define _OBJLOADER_H_

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
//#include <directxmath.h>

#include "Modelclass.h"

using namespace DirectX;
using namespace std;


class ObjLoader
{
    using VertexInfo = ModelClass::VertexInfo;

public:

    bool Load(const string& filename, VertexInfo* mesh)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cerr << "Failed to open file: " << filename << endl;
            return false;
        }
        vector<VertexInfo> vertices;

        vector<XMFLOAT3> positions;
        vector<XMFLOAT2> texCoords;
        vector<XMFLOAT3> normals;
        vector<unsigned int> positionIndices, texCoordIndices, normalIndices;

        string line;
        while (getline(file, line))
        {
            istringstream iss(line);
            string prefix;
            iss >> prefix;

            VertexInfo vertex;
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
                    unsigned int posIndex = 0, texIndex = 0, normIndex = 0;

                    getline(viss, index, '/');
                    posIndex = stoi(index) - 1;

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

        for (size_t i = 0; i < positionIndices.size(); ++i)
        {
            Vertex vertex;
            vertex.position = positions[positionIndices[i]];
            if (!texCoords.empty())
            {
                vertex.texCoord = texCoords[texCoordIndices[i]];
            }
            if (!normals.empty())
            {
                vertex.normal = normals[normalIndices[i]];
            }
            mesh[i].x = vertex.position.x;
            mesh[i].x = vertex.position.x;
            mesh[i].x = vertex.position.x;
			mesh[i].tu = vertex.texCoord.x;
			mesh[i].tv = vertex.texCoord.y;
			mesh[i].nx = vertex.normal.x;
			mesh[i].ny = vertex.normal.y;
			mesh[i].nz = vertex.normal.z;
        }

        return true;
    }
};

#endif