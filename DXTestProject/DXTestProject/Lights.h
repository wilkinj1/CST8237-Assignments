#pragma once

#include "GraphicsIncludes.h"

struct Light
{
  XMFLOAT4 ambientColor;
  XMFLOAT4 diffuseColor;
  XMFLOAT4 specularColor;
  XMFLOAT specularity;

  XMFLOAT3 position;
  XMFLOAT3 attenuation;
};

struct PointLight : public Light
{
  XMFLOAT intensity;
};

struct SpotLight : public Light
{
  XMFLOAT3 direction;
  XMFLOAT coneRadius;
  XMFLOAT cutoff;
};

struct DirectionalLight : public Light
{
  XMFLOAT3 direction;
};