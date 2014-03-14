#pragma once
//#include <d3d11.h>

class Model;
struct D3D11_INPUT_ELEMENT_DESC;

struct MODEL_DESC
{
  void *pData;
  unsigned int sizeOfData;
  unsigned int vertexCount;
  D3D11_INPUT_ELEMENT_DESC *elementDescs;
  unsigned int elementCount;
  unsigned int sizeOfVertexDesc;
};

class ModelBuilder
{
public:
  static Model* create(const MODEL_DESC &modelDesc);
  static void destroy(Model *model);

protected:
  ModelBuilder();
};