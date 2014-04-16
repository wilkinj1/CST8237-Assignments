#include "stdafx.h"
#include "EasyFont.h"
#include "ResourceManager.h"
#include "GraphicsIncludes.h"

#include "GameEngine.h"
#include "GraphicsManager.h"
#include "Camera.h"

// Font Rendering
//----------------------------
int numLetters = 32;
float textureWidth = 1024.0f;
//-----------------------------

struct cbPerObject
{
  XMMATRIX WVP;
};

struct Vertex
{
  Vertex(){}
  Vertex(float x, float y, float z,
    float u, float v)
    : pos(x, y, z), texCoord(u, v){}
  XMFLOAT3 pos;
  XMFLOAT2 texCoord;
  //XMFLOAT3 padding;
};

// Input Layout Descriptor
D3D11_INPUT_ELEMENT_DESC layout[] =
{
  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
UINT numElements = ARRAYSIZE(layout);


EasyFont::EasyFont()
{

}

bool EasyFont::Initialize(ID3D11Device *device, ID3D11DeviceContext *dc)
{
  bool success = false;

  GraphicsManager *gm = GameEngine::GetInstance()->GetGraphicsManager();

  D3D11_BUFFER_DESC cbbd;
  ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
  cbbd.Usage = D3D11_USAGE_DEFAULT;
  cbbd.ByteWidth = sizeof(cbPerObject);
  cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  cbbd.CPUAccessFlags = 0;
  cbbd.MiscFlags = 0;

  //Erstelle Buffer
  HRESULT hr = device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

  //Font Stuff
  //-----------------------------
  D3D11_BUFFER_DESC vertexBufferDesc;
  ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
  vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  vertexBufferDesc.ByteWidth = sizeof(Vertex)* 6 * numLetters;

  float vertSize = sizeof(Vertex);

  hr = device->CreateBuffer(&vertexBufferDesc, NULL, &fontVertexBuffer);
  //-------------------------------

  //Erstelle Shader
  SHADER_DESC vertexShaderDesc;
  vertexShaderDesc.filepath = L"./EasyFont/Effects.fx";
  vertexShaderDesc.entryPoint = "FONT_VS";
  vertexShaderDesc.profile = "vs_4_0";

  SHADER_DESC pixelShaderDesc;
  ZeroMemory(&pixelShaderDesc, sizeof(pixelShaderDesc));
  pixelShaderDesc.filepath = L"./EasyFont/Effects.fx";
  pixelShaderDesc.entryPoint = "FONT_PS";
  pixelShaderDesc.profile = "ps_4_0";

  MODEL_DESC modelDesc;
  ZeroMemory(&modelDesc, sizeof(modelDesc));
  modelDesc.elementDescs = layout;
  modelDesc.elementCount = numElements;
  modelDesc.sizeOfVertexDesc = sizeof(Vertex);

  FONT_VS = ResourceManager::GetInstance()->LoadVertexShaderAndInputLayout(vertexShaderDesc, &modelDesc, &mInputLayout);
  FONT_PS = ResourceManager::GetInstance()->LoadPixelShader(pixelShaderDesc);

  // Erstelle Sampler Desciptor
  D3D11_SAMPLER_DESC sampDesc;
  ZeroMemory(&sampDesc, sizeof(sampDesc));
  sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

  fontAtlas = ResourceManager::GetInstance()->LoadTexture(L"./EasyFont/ArialFont.dds");
  hr = device->CreateSamplerState(&sampDesc, &fontAtlasSampler);

  return success;
}

//Font Stuff
bool EasyFont::DrawString(ID3D11DeviceContext *dc, Camera *camera, char* text, float posX, float posY)
{
  UINT stride = sizeof(Vertex);
  UINT offset = 0;

  dc->VSSetShader(FONT_VS, 0, 0);
  dc->PSSetShader(FONT_PS, 0, 0);

  cbPerObject cbPerObj;
  cbPerObj.WVP = XMMatrixTranspose(camera->GetProjectionMatrix());

  dc->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
  dc->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

  dc->IASetInputLayout(mInputLayout);
  dc->IASetVertexBuffers(0, 1, &fontVertexBuffer, &stride, &offset);
  dc->PSSetShaderResources(0, 1, &fontAtlas);
  dc->PSSetSamplers(0, 1, &fontAtlasSampler);

  dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


  // size of one letter and string size
  int letterSize = sizeof(Vertex)* 6;
  int textSize = strlen(text);

  // cut string if its to long
  if (textSize > numLetters)
    textSize = numLetters;

  // size of one char on screen
  float cScreenWidth = 32.0f / camera->GetViewSize().x;
  float cScreenHeight = 32.0f / camera->GetViewSize().y;
  // texel Size
  float texelWidth = 32.0f / textureWidth;

  //Create Mapped Subresource
  D3D11_MAPPED_SUBRESOURCE mappedSub;
  HRESULT hr = dc->Map(fontVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSub);

  // Pointer to buffer subresource
  Vertex * sprite = (Vertex*)mappedSub.pData;

  // Indices of a A and z
  const int indexA = static_cast<int>('A');
  const int indexZ = static_cast<int>('Z');

  //loop to build the string
  for (int i = 0; i< textSize; i++)
  {
    /*Get starting position of the quad. First Quad is just the posX value ,
    then characterwidth is added.*/

    float thisStartX = posX + (cScreenWidth * static_cast<char>(i));
    float thisEndX = thisStartX + cScreenWidth;
    float thisStartY = posY;
    float thisEndY = thisStartY + cScreenHeight;

    // Write the position of each 6 verticess to subresource
    sprite[0].pos = XMFLOAT3(thisEndX, thisEndY, 1.0f);
    sprite[1].pos = XMFLOAT3(thisEndX, posY, 1.0f);
    sprite[2].pos = XMFLOAT3(thisStartX, posY, 1.0f);
    sprite[3].pos = XMFLOAT3(thisStartX, posY, 1.0f);
    sprite[4].pos = XMFLOAT3(thisStartX, thisEndY, 1.0f);
    sprite[5].pos = XMFLOAT3(thisEndX, thisEndY, 1.0f);

    // calculate texture coordinates from ASCII values.
    int texLookup = 0;
    int letter = static_cast<int>(text[i]);

    // Blank Space ( As the Blank space is just added after Z, 
    //it needs some extra treatment
    if (letter < indexA || letter > indexZ)
    {
      texLookup = (indexZ - indexA) + 1;
    }
    else
      // Characters
    {
      texLookup = (letter - indexA);
    }


    float texStart = 0.0f + (texelWidth * static_cast<float>(texLookup));
    float texEnd = texStart + texelWidth;

    // Apply texture coordinates to subresource
    sprite[0].texCoord = XMFLOAT2(texEnd, 0.0f);
    sprite[1].texCoord = XMFLOAT2(texEnd, 1.0f);
    sprite[2].texCoord = XMFLOAT2(texStart, 1.0f);
    sprite[3].texCoord = XMFLOAT2(texStart, 1.0f);
    sprite[4].texCoord = XMFLOAT2(texStart, 0.0f);
    sprite[5].texCoord = XMFLOAT2(texEnd, 0.0f);

    //set sprite pointer for next sprite
    sprite = sprite + 6;
  }
  dc->Unmap(fontVertexBuffer, 0);
  dc->Draw(6 * textSize, 0);

  return true;
}