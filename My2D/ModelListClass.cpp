#include "ModelListClass.h"

ModelListClass::ModelListClass()
{
	m_transformList = nullptr;
}


ModelListClass::ModelListClass(const ModelListClass& other)
{
}


ModelListClass::~ModelListClass()
{
}

void ModelListClass::Initialize(int numModels)
{
	int i;
	float x, y, z;
	// Store the number of models.
	m_modelCount = numModels;

	m_transformList = new Transform[m_modelCount];

	// Seed the random generator with the current time.
	srand((unsigned int)time(NULL));

	// Go through all the models and randomly generate the position.
	for (i = 0; i < m_modelCount; i++)
	{
		// Generate a random position in front of the viewer for the mode.
		 x = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		 y = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		 z = ((((float)rand() - (float)rand()) / RAND_MAX) * 10.0f) + 5.0f;
		 m_transformList[i].SetPosition(x, y, z);
	}

	return;
}

void ModelListClass::Shutdown()
{
	// Release the model information list.
	if (m_transformList)
	{
		delete[] m_transformList;
		m_transformList = nullptr;
	}

	return;
}

