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

	__forceinline int GetVertexIndex(const string& indexStr)
	{
		int result = stoi(indexStr);
		return result < 0 ? result : result - 1;
	}

	void CalculateNormals(const vector<int>& indices, OUT vector<VertexInfo>& mesh) {
		size_t size =  mesh.size();
		// Initialize normals
		for (size_t i = 0; i < size; ++i) {
			mesh[i].nx = 0.0f;
			mesh[i].ny = 0.0f;
			mesh[i].nz = 0.0f;
		}

		// Calculate face normals and add to vertex normals
		for (size_t i = 0; i < indices.size(); i += 3) {
			int index0 = indices[i];
			int index1 = indices[i + 1];
			int index2 = indices[i + 2];

			XMFLOAT3 v0(mesh[index0].x, mesh[index0].y, mesh[index0].z);
			XMFLOAT3 v1(mesh[index1].x, mesh[index1].y, mesh[index1].z);
			XMFLOAT3 v2(mesh[index2].x, mesh[index2].y, mesh[index2].z);

			XMVECTOR vec0 = XMLoadFloat3(&v0);
			XMVECTOR vec1 = XMLoadFloat3(&v1);
			XMVECTOR vec2 = XMLoadFloat3(&v2);

			XMVECTOR edge1 = XMVectorSubtract(vec1, vec0);
			XMVECTOR edge2 = XMVectorSubtract(vec2, vec0);
			XMVECTOR faceNormal = XMVector3Cross(edge1, edge2);
			faceNormal = XMVector3Normalize(faceNormal);

			XMFLOAT3 normal;
			XMStoreFloat3(&normal, faceNormal);

			mesh[index0].nx += normal.x;
			mesh[index0].ny += normal.y;
			mesh[index0].nz += normal.z;

			mesh[index1].nx += normal.x;
			mesh[index1].ny += normal.y;
			mesh[index1].nz += normal.z;

			mesh[index2].nx += normal.x;
			mesh[index2].ny += normal.y;
			mesh[index2].nz += normal.z;
		}

		// Normalize the vertex normals
		for (size_t i = 0; i < size; ++i) {
			XMVECTOR normal = XMVectorSet(mesh[i].nx, mesh[i].ny, mesh[i].nz, 0.0f);
			normal = XMVector3Normalize(normal);
			XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&mesh[i].nx), normal);
		}
	}

public:

	bool Load(const string& filename, OUT vector<VertexInfo>& mesh, OUT int& vertexCount, OUT int& indexCount)
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

		indexCount = positionIndices.size();
		vertexCount = positions.size();

		mesh.resize(positions.size());


		auto setVertex = [&](VertexInfo& vertex, int posIndex, int texIndex, int normIndex)
		{
			vertex.x = positions[posIndex].x;
			vertex.y = positions[posIndex].y;
			vertex.z = positions[posIndex].z;
			vertex.tu = texCoords[texIndex].x;
			vertex.tv = texCoords[texIndex].y;
			vertex.nx = normals[normIndex].x;
			vertex.ny = normals[normIndex].y;
			vertex.nz = normals[normIndex].z;
		};

		//adding Clock-wise
		for (size_t i = 0; i < indexCount; i += 3)
		{
			VertexInfo vertex1, vertex2, vertex3;

			vertex1.x = positions[positionIndices[i] - 1].x;
			vertex1.y = positions[positionIndices[i] - 1].y;
			vertex1.z = positions[positionIndices[i] - 1].z;
			vertex1.tu = texCoords[texCoordIndices[i] - 1].x;
			vertex1.tv = texCoords[texCoordIndices[i] - 1].y;
			vertex1.nx = normals[normalIndices[i] - 1].x;
			vertex1.ny = normals[normalIndices[i] - 1].y;
			vertex1.nz = normals[normalIndices[i] - 1].z;

			vertex2.x = positions[positionIndices[i + 1] - 1].x;
			vertex2.y = positions[positionIndices[i + 1] - 1].y;
			vertex2.z = positions[positionIndices[i + 1] - 1].z;
			vertex2.tu = texCoords[texCoordIndices[i + 1] - 1].x;
			vertex2.tv = texCoords[texCoordIndices[i + 1] - 1].y;
			vertex2.nx = normals[normalIndices[i + 1] - 1].x;
			vertex2.ny = normals[normalIndices[i + 1] - 1].y;
			vertex2.nz = normals[normalIndices[i + 1] - 1].z;

			vertex3.x = positions[positionIndices[i + 2] - 1].x;
			vertex3.y = positions[positionIndices[i + 2] - 1].y;
			vertex3.z = positions[positionIndices[i + 2] - 1].z;
			vertex3.tu = texCoords[texCoordIndices[i + 2] - 1].x;
			vertex3.tv = texCoords[texCoordIndices[i + 2] - 1].y;
			vertex3.nx = normals[normalIndices[i + 2] - 1].x;
			vertex3.ny = normals[normalIndices[i + 2] - 1].y;
			vertex3.nz = normals[normalIndices[i + 2] - 1].z;

			mesh.push_back(vertex1);
			mesh.push_back(vertex2);
			mesh.push_back(vertex3);
		}

		if (normals.empty())
		{
			CalculateNormals(positionIndices, mesh);
		}

		return true;
	}
};

#endif