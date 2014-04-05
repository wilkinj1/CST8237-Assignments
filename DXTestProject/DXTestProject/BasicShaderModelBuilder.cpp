#include "stdafx.h"
#include "BasicShaderModelBuilder.h"
#include "GraphicsIncludes.h"

BasicShaderModelBuilder::BasicShaderModelBuilder() { }

Model* BasicShaderModelBuilder::Create(MODEL_DESC & const modelDesc)
{
  SHADER_DESC vertexShaderDesc;
  vertexShaderDesc.filepath = L"./basicShader.fx";
  vertexShaderDesc.entryPoint = "BasicVertexShader";
  vertexShaderDesc.profile = "vs_4_0";

  SHADER_DESC pixelShaderDesc;
  pixelShaderDesc.filepath = L"./basicShader.fx";
  pixelShaderDesc.entryPoint = "BasicFragmentShader";
  pixelShaderDesc.profile = "ps_4_0";

  D3D11_SAMPLER_DESC samplerDesc;
  ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
  samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

  TEXTURE_DESC textureDesc;
  textureDesc.filepath = L"./IC504917.png";
  textureDesc.samplerDesc = &samplerDesc;

  return ModelBuilder::Create(modelDesc, vertexShaderDesc, pixelShaderDesc, textureDesc);
}

void BasicShaderModelBuilder::Destroy(Model *model)
{
  ModelBuilder::Destroy(model);
}