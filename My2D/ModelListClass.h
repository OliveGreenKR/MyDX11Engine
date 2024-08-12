#pragma once

#include <stdlib.h>
#include <time.h>
#include "Transform.h"

class ModelListClass
{

public:
    ModelListClass();
    ModelListClass(const ModelListClass&);
    ~ModelListClass();

    void Initialize(int);
    void Shutdown();

    int GetModelCount() { return m_modelCount; }
    void GetData(int index, float& x, float& y, float& z) 
    { 
        auto position = m_transformList[index].GetPosition();

        x = position.x;
        y = position.y;
        z = position.z;
        return;
    }

private:
    int m_modelCount;
    Transform* m_transformList;
};
