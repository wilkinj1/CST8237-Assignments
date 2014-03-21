#pragma once

#include "GraphicsIncludes.h"

struct Light
{
  XMFLOAT3 ambientColor;
  XMFLOAT3 diffuseColor;
  XMFLOAT3 specularColor;
  float specularity;

  XMFLOAT3 position;
  XMFLOAT3 attenuation;
};

struct PointLight : public Light
{
  float intensity;
};

struct SpotLight : public Light
{
  XMFLOAT3 direction;
  float coneRadius;
  float cutoff;
};

struct DirectionalLight : public Light
{
  XMFLOAT3 direction;
};