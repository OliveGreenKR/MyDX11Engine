#pragma once

#include <stdlib.h>
#include <time.h>


class ModelListClass
{
private:
    struct ModelInfoType
    {
        float positionX, positionY, positionZ;
    };

public:
    ModelListClass();
    ModelListClass(const ModelListClass&);
    ~ModelListClass();

    void Initialize(int);
    void Shutdown();

    int GetModelCount() { return m_modelCount; }
    void GetData(int index, float& x, float& y, float& z) 
    { 
        ModelInfoType info = m_ModelInfoList[index]; 
        x = info.positionX;
        y = info.positionY;
        z = info.positionZ;
        return;
    }

private:
    int m_modelCount;
    ModelInfoType* m_ModelInfoList;
};
