#pragma once
#ifndef _OBJLOADER_H_
#define _OBJLOADER_H_

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <directxmath.h>
#include "Modelclass.h"

using namespace DirectX;
using namespace std;

class ObjLoader
{
	using VertexInfo = ModelClass::VertexInfo;

private:

	void CalculateNormals(const vector<size_t>& vertexIndices, OUT vector<VertexInfo>& verteces) {
		size_t size = verteces.size();
		// Initialize normals
		for (size_t i = 0; i < size; ++i) {
			verteces[i].nx = 0.0f;
			verteces[i].ny = 0.0f;
			verteces[i].nz = 0.0f;
		}

		// Calculate face normals and add to vertex normals
		for (size_t i = 0; i < vertexIndices.size(); i += 3) {
			auto index0 = vertexIndices[i];
			auto index1 = vertexIndices[i + 1];
			auto index2 = vertexIndices[i + 2];

			XMFLOAT3 v0(verteces[index0].x, verteces[index0].y, verteces[index0].z);
			XMFLOAT3 v1(verteces[index1].x, verteces[index1].y, verteces[index1].z);
			XMFLOAT3 v2(verteces[index2].x, verteces[index2].y, verteces[index2].z);

			XMVECTOR vec0 = XMLoadFloat3(&v0);
			XMVECTOR vec1 = XMLoadFloat3(&v1);
			XMVECTOR vec2 = XMLoadFloat3(&v2);

			XMVECTOR edge1 = XMVectorSubtract(vec1, vec0);
			XMVECTOR edge2 = XMVectorSubtract(vec2, vec0);
			XMVECTOR faceNormal = XMVector3Cross(edge1, edge2);
			faceNormal = XMVector3Normalize(faceNormal);

			XMFLOAT3 normal;
			XMStoreFloat3(&normal, faceNormal);

			verteces[index0].nx += normal.x;
			verteces[index0].ny += normal.y;
			verteces[index0].nz += normal.z;

			verteces[index1].nx += normal.x;
			verteces[index1].ny += normal.y;
			verteces[index1].nz += normal.z;

			verteces[index2].nx += normal.x;
			verteces[index2].ny += normal.y;
			verteces[index2].nz += normal.z;
		}

		// Normalize the vertex normals
		for (size_t i = 0; i < size; ++i) {
			XMVECTOR normal = XMVectorSet(verteces[i].nx, verteces[i].ny, verteces[i].nz, 0.0f);
			normal = XMVector3Normalize(normal);
			XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&verteces[i].nx), normal);
		}
	}

public:

	bool Load(const string& filename, OUT vector<VertexInfo>& vertices, OUT vector<unsigned long>& indices)
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
		vector<size_t> vertexIndices, texCoordIndices, normalIndices;

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
					int vertexIndex = 0, texIndex = 0, normIndex = 0;

					getline(viss, index, '/');
					vertexIndex = stoi(index) - 1;

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

					vertexIndices.push_back(vertexIndex);
					texCoordIndices.push_back(texIndex);
					normalIndices.push_back(normIndex);
				}
			}
		}

		size_t vertexCount = vertexIndices.size();
		vertices.resize(vertexCount);
		indices.resize(vertexCount);

		for (size_t i = 0; i < vertexCount; ++i)
		{
			VertexInfo& vertex = vertices[i];
			int posIndex = vertexIndices[i];
			int texIndex = texCoordIndices[i];
			int normIndex = normalIndices[i];

			vertex.x = positions[posIndex].x;
			vertex.y = positions[posIndex].y;
			vertex.z = positions[posIndex].z;

			if (!texCoords.empty())
			{
				vertex.tu = texCoords[texIndex].x;
				vertex.tv = texCoords[texIndex].y;
			}
			else
			{
				vertex.tu = 0.0f;
				vertex.tv = 0.0f;
			}

			if (!normals.empty())
			{
				vertex.nx = normals[normIndex].x;
				vertex.ny = normals[normIndex].y;
				vertex.nz = normals[normIndex].z;
			}

			indices[i] = static_cast<unsigned long>(i);
		}

		if (normals.empty())
		{
			CalculateNormals(vertexIndices, vertices);
		}

		return true;
	}
};

#endif