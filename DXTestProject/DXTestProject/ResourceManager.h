#pragma once

#include <map>
#include <string>
#include "GraphicsIncludes.h"

struct ID3D11ShaderResourceView;
struct ID3D11PixelShader;
struct ID3D11VertexShader;
struct ID3D11DeviceChild;
struct IUnknown;

struct D3D11_INPUT_ELEMENT_DESC;

typedef struct VertexPositionColorNormalTextureUVDescription
{
  XMFLOAT3 Position;
  XMFLOAT3 Color;
  XMFLOAT3 Normal;
  XMFLOAT2 TextureUV;
} VPCNTDesc;

struct MODEL_DESC
{
  VPCNTDesc *modelData; // all of the actual vertices that make up the model.
  unsigned int sizeOfData; // the total size of all of the model data.
  unsigned int vertexCount; // how many individual vertices we're working with.
  D3D11_INPUT_ELEMENT_DESC *elementDescs; // the description of how the elements in the vertices map to semantics in the shader.
  unsigned int elementCount; // how many elements are in the description.
  unsigned int sizeOfVertexDesc; // how big each vertex is. (eg. Color (4b), Position (4b) = 8b)
};

struct TEXTURE_DESC
{
  TCHAR *filepath;
  D3D11_SAMPLER_DESC *samplerDesc;
};

struct SHADER_DESC
{
  TCHAR *filepath; // the filepath of the shader (eg. L"./basicShader.fx")
  const char *entryPoint; // the 'main' entry point of the shader (eg. BasicVertexShader)
  const char *profile; // the profile with which to compile it for (eg. vs_4_0 = Vertex Shader, Shader Model 4.0)
};

class ResourceManager
{
public:
  static ResourceManager* GetInstance();
  static void Shutdown();

  ~ResourceManager();

  ID3D11ShaderResourceView* LoadTexture(TCHAR* filename);
  ID3D11VertexShader* LoadVertexShaderAndInputLayout(SHADER_DESC desc, MODEL_DESC* input = NULL, ID3D11InputLayout** layout = NULL);
  ID3D11PixelShader* LoadPixelShader(SHADER_DESC desc);

  template <class T> bool UnloadResource(T **resource)
  {
    return UnloadCachedResource((ID3D11DeviceChild **)resource);
  }

protected:
  ResourceManager();
  typedef std::pair<ID3D11DeviceChild *, int> ResourcePair;
  typedef std::map<std::wstring, ResourcePair> ResourceMap;
  ResourceMap mLoadedResources;

  ID3D11DeviceChild* LoadShaderAndInputLayout(SHADER_DESC desc, MODEL_DESC* input = NULL, ID3D11InputLayout** layout = NULL);
  bool UnloadCachedResource(ID3D11DeviceChild **resource);

private:
  static ResourceManager *sResourceManagerInstance;
};