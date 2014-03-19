#include "stdafx.h"

#include <d3d11.h>
#include "ModelUtils.h"

MODEL_DESC ModelUtils::CreateCubeModelPCNDescription()
{
  MODEL_DESC modelDescription;
  ZeroMemory(&modelDescription, sizeof(MODEL_DESC));
  return modelDescription;
}