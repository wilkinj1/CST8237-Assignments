#pragma once

class Model;
struct D3D11_INPUT_ELEMENT_DESC;

struct MODEL_DESC
{
  void *modelData;
  unsigned int sizeOfData;
  unsigned int vertexCount;
  D3D11_INPUT_ELEMENT_DESC *elementDescs;
  unsigned int elementCount;
  unsigned int sizeOfVertexDesc;
};

struct SHADER_DESC
{
  const TCHAR *filepath;
  const char *entryPoint;
  const char *profile;
};

class ModelBuilder
{
public:
  static Model* Create(const MODEL_DESC &modelDesc, const SHADER_DESC &vertexDesc, const SHADER_DESC &pixelDesc);
  static void Destroy(Model *model);

protected:
  ModelBuilder();
};