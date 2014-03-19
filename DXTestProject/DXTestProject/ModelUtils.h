#pragma once

#include "ModelBuilder.h"
#include "GraphicsIncludes.h"

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
  static MODEL_DESC CreateCubeModelPCNDescription();
};