#pragma once

#include "GraphicsIncludes.h"

struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;
struct ID3D10Blob;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11InputLayout;
class Camera;

class EasyFont
{
public:
  EasyFont();
  bool Initialize(ID3D11Device *device, ID3D11DeviceContext *dc);

  //Font Stuff
  bool DrawString(ID3D11DeviceContext *dc, Camera *camera, char* text, float posX, float posY);

private:
  ID3D11Buffer*	vertexBuffer;		//VertexBuffer
  ID3D11Buffer*   indexBuffer;		//IndexBuffer
  ID3D11VertexShader* VS;						//VertexShader Interface
  ID3D11PixelShader* PS;						//PixelShader Interface


  ID3D11ShaderResourceView * fontAtlas;
  ID3D11SamplerState * fontAtlasSampler;
  ID3D11Buffer			 * fontVertexBuffer;
  ID3D11Buffer* cbPerObjectBuffer;
  ID3D10Blob* FONT_VS_Buffer, *FONT_PS_Buffer;
  ID3D11VertexShader * FONT_VS;
  ID3D11PixelShader *FONT_PS;
  ID3D11InputLayout *mInputLayout;
};