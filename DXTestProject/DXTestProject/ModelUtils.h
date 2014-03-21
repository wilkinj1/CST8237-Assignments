#pragma once

#include "GraphicsIncludes.h"

class Model;

// Define the data-type that
// describes a vertex.
struct VertexPositionColorNormalDescription
{
  XMFLOAT3 Position;
  XMFLOAT3 Color;
  XMFLOAT3 Normal;
};

struct VertexPositionColorNormalTextureUVDescription
{
  XMFLOAT3 Position;
  XMFLOAT3 Color;
  XMFLOAT3 Normal;
  XMFLOAT2 TextureUV;
};

class ModelUtils
{
public:
  static Model* CreateCubeModelPCNT();
};