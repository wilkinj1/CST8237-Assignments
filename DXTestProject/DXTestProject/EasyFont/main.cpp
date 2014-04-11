/*-------------------------------------
------ Preprocessor Direktiven --------
--------------------------------------*/
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "DXErr.lib")

#include <Windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>
#include <DxErr.h>
#include <string.h>
#include <string>
#include <iostream>


/*-------------------------------------
-- Konstanten und Globale Variablen----
--------------------------------------*/
// Window Klasse und Handle
LPCTSTR WndClassName = L"firstwindow";
HWND hwnd = NULL;

// Konstanten die Höhe und Breite des Fensters festlegen.
const int Width = 800;
const int Height = 600;

// Benötigte Interfaces
IDXGISwapChain* SwapChain;				// Swapchain ermöglicht Doublebuffering
ID3D11Device* d3d11Device;				// D3D Device 
ID3D11DeviceContext* d3d11DevCon;		// Context für Rendering	
ID3D11RenderTargetView* renderTargetView; //Textur für den Backbuffer
ID3D11Buffer*	vertexBuffer;		//VertexBuffer
ID3D11Buffer*   indexBuffer;		//IndexBuffer
ID3D11VertexShader* VS;						//VertexShader Interface
ID3D11PixelShader* PS;						//PixelShader Interface
ID3D10Blob* VS_Buffer;						//VS Information Buffer
ID3D10Blob* PS_Buffer;						//PS Information Buffer
ID3D11InputLayout* vertLayout;				// Input Layout
ID3D11DepthStencilView * depthStencilView;   // depth and stencil view
ID3D11Texture2D*		depthStencilBuffer;  // Actual Buffer as 2d Texture
ID3D11Buffer* cbPerObjectBuffer;		// Buffer für die WVP Matrix
ID3D11RasterizerState* CCWcullMode,*CWcullMode,*noCull;  // RenderStates
ID3D11ShaderResourceView * Texture;			// Textur
ID3D11SamplerState* TexSamplerState;		//TextureSampler
ID3D11BlendState* Transparency;
ID3D10Blob* ErrorMsg;

// FONT RENDERING STUFF
//----------------------------------------
ID3D11ShaderResourceView * fontAtlas;
ID3D11SamplerState * fontAtlasSampler;
ID3D11Buffer			 * fontVertexBuffer;
ID3D10Blob* FONT_VS_Buffer,*FONT_PS_Buffer;
ID3D11VertexShader * FONT_VS;
ID3D11PixelShader *FONT_PS;

//-----------------------------------------

// Vertex Strukt
struct Vertex
{
	Vertex(){}
	Vertex(float x,float y, float z,
		   float u, float v)
		   : pos(x,y,z), texCoord(u,v){}
	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
};

// Input Layout Descriptor
D3D11_INPUT_ELEMENT_DESC layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
UINT numElements = ARRAYSIZE(layout);

//Struct für den Constant Buffer , muss gleiches Layout wie in der Effect datei haben
struct cbPerObject
{
	XMMATRIX WVP;
};

//Erstelle einen Constant Buffer;
cbPerObject cbPerObj;

//HRESULT für Error Checks
HRESULT hr;

// Matrizen für Transformationen
XMMATRIX WVP;
XMMATRIX cube1World;
XMMATRIX cube2World;
XMMATRIX camView;
XMMATRIX camProjection;

XMMATRIX Rotation;
XMMATRIX Scale;
XMMATRIX Translation;

float rot = 0.01f;

// Vectoren für Kamera 
XMVECTOR camPosition;
XMVECTOR camTarget;
XMVECTOR camUp;

// Font Rendering
//----------------------------
int numLetters = 32;
float textureWidth = 1024.0f;
//-----------------------------

/*------------------------------------
--------- Funktionsprototypen --------
------------------------------------*/
bool InitializeWindow(HINSTANCE hInstance,
					  int ShowWnd,
					  int width, int height,
					  bool windowed);

int messageloop();

LRESULT CALLBACK WndProc(HWND hWnd,
						 UINT msg,
						 WPARAM wParam,
						 LPARAM lParam);

bool InitializeDirect3d11App(HINSTANCE hInstance);
void ReleaseObjects();
bool InitScene();
void UpdateScene();
void DrawScene();
void ShaderErrorOutput(ID3D10Blob* ErrMsg,std::string shader);

//Font Stuff
bool DrawString(char* text,float posX,float posY);

/*------------------------------------
--------- WinMain() Funktion  --------
Einstiegspunkt für Win32 Applikationen
------------------------------------*/
int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nShowCmd)
{
	// Initialisiere und Registriere das Fenster.
	if(!InitializeWindow(hInstance, nShowCmd, Width, Height, true))
	{
		MessageBox(0, L"Window Initialization - Failed",
        			L"Error", MB_OK);
		return 0;
	}

	//Initialisiere Direct3D
	if(!InitializeDirect3d11App(hInstance))	
	{
		MessageBox(0, L"Direct3D Initialization - Failed",
			L"Error", MB_OK);
		return 0;
	}
	
	//Initialisiere die Szene
	if(!InitScene())	
	{
		MessageBox(0, L"Scene Initialization - Failed",
			L"Error", MB_OK);
		return 0;
	}

	// im der messageloop() Schleife läuft das eigentliche Programm ab.
	messageloop();

	//Räume auf
	ReleaseObjects();

	return 0;

}


/*------------------------------------
----Initialize Window Funktion  -----
Erstellt und Registriert eine Window Klasse
------------------------------------*/

bool InitializeWindow(HINSTANCE hInstance,
						int ShowWnd,
						int width, int height,
						bool windowed)
{
	// Erstelle die Window Klasse
	WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = NULL;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = WndClassName;
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


	// Registriere die Window Klasse
		if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Error registering class",	
			L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	// Erstelle das Fenster
	hwnd = CreateWindowEx(NULL,
						  WndClassName,
						  L"Window Title",
						  WS_OVERLAPPEDWINDOW,
						  CW_USEDEFAULT, CW_USEDEFAULT,
						  width, height,
						  NULL,
						  NULL,
						  hInstance,
						  NULL);
	
	if (!hwnd)
	{
		MessageBox(NULL, L"Error creating window",
			L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	//Zeige das Fenster und Aktualisiere es
	ShowWindow(hwnd,ShowWnd);
	UpdateWindow(hwnd);

	return true;
}


/*------------------------------------
------ messageloop() Funktion --------
Hier läuft das Programm ab
------------------------------------*/
int messageloop(){
	// Struktur um die message zu speichern
	MSG msg;
	ZeroMemory(&msg,sizeof(MSG));

	
	while(true)
	{
		//Schau nach ob es eine Nachricht gibt
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				break;
			// Bearbeite die message
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Wenn nicht :
		else
		{
			UpdateScene();
			DrawScene();

		}
	}
	return msg.wParam;
}


/*------------------------------------
------ WndProc() Funktion --------
Callback Funktion bearbeitet die Nachrichten
------------------------------------*/
LRESULT CALLBACK WndProc(HWND hwnd,
						 UINT msg,
						 WPARAM wParam,
						 LPARAM lParam)
{
	// Messages die wir selbst behandeln
	switch( msg )
	{

		case WM_KEYDOWN:
			if( wParam == VK_ESCAPE ){
				if(MessageBox(0, L"Are you sure you want to exit?",
                	L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
			DestroyWindow(hwnd);
			}
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
		return 0;
	}

	// Alle anderen werden vom Default Programm behandelt
	return DefWindowProc(hwnd,msg,wParam,lParam);
}


/*------------------------------------
------ InitializeDirect3DApp() -------
Initialisiert Direct3d :D
------------------------------------*/
bool InitializeDirect3d11App(HINSTANCE hInstance)
{


	//Beschreibe den BackBuffer 
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
		bufferDesc.Width = Width;
		bufferDesc.Height = Height;
		bufferDesc.RefreshRate.Numerator = 60;
		bufferDesc.RefreshRate.Denominator = 1;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		//32Bit float 
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//Beschreibe die SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc; 		
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		swapChainDesc.BufferDesc = bufferDesc;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = hwnd; 
		swapChainDesc.Windowed = TRUE; 
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//Erstelle Device und SwapChain
	hr = D3D11CreateDeviceAndSwapChain(NULL,
									   D3D_DRIVER_TYPE_HARDWARE,
									   NULL,
									   NULL,
									   NULL,
									   NULL,
									   D3D11_SDK_VERSION,
									   &swapChainDesc,
									   &SwapChain,
									   &d3d11Device,
									   NULL,
									   &d3d11DevCon);
	if(FAILED(hr))
	{	
		MessageBox(NULL, DXGetErrorDescription(hr),
			TEXT(" D3D11CreateDeviceAndSwapChain"), MB_OK);	
		return 0;	
	}

	//Erstelle den BackBuffer ( Das ist eine einfache 2D Textur)
	ID3D11Texture2D * BackBuffer;
	hr = SwapChain->GetBuffer(0,__uuidof( ID3D11Texture2D ),(void**)&BackBuffer);
	if(FAILED(hr))
	{	
		MessageBox(NULL, DXGetErrorDescription(hr),
			TEXT("SwapChain->GetBuffer"), MB_OK);	
		return 0;	
	}

	//Erstelle das RenderTarget
	hr = d3d11Device->CreateRenderTargetView(BackBuffer,NULL,&renderTargetView);
	if(FAILED(hr))
	{	
		MessageBox(NULL, DXGetErrorDescription(hr),
			TEXT("d3d11Device->CreateRenderTargetView"), MB_OK);	
		return 0;	
	}
	BackBuffer->Release();
	
	//Beschreibe den DepthStencilBuffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width     = Width;
		depthStencilDesc.Height    = Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count   = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0; 
		depthStencilDesc.MiscFlags      = 0;

	//Erstelle den DepthStencilBuffer
	d3d11Device->CreateTexture2D(&depthStencilDesc,NULL,&depthStencilBuffer);
	d3d11Device->CreateDepthStencilView(depthStencilBuffer,NULL,&depthStencilView);

	// Binde die RenderTargets an den Output Merger
	d3d11DevCon->OMSetRenderTargets(1,&renderTargetView,NULL);

	//Binde den DepthStencilView an den Output Merger
	d3d11DevCon->OMSetRenderTargets(1,&renderTargetView,depthStencilView);

	return true;
}


/*------------------------------------
------ initScene() Funktion ------
Scene Initialsieren :D
------------------------------------*/
bool InitScene()
{
	//Compiliere Shader
	hr=D3DX11CompileFromFile(L"Effects.fx",NULL,NULL,"VS","vs_5_0",0,0,0,&VS_Buffer,&ErrorMsg,0);
			if(FAILED(hr))
	{	
		if(ErrorMsg)
		{
			ShaderErrorOutput(ErrorMsg,"VertexShader");
		}
		MessageBox(NULL, DXGetErrorDescription(hr),
			TEXT(" Vertexshader"), MB_OK);	
		return 0;	
	}
	hr=D3DX11CompileFromFile(L"Effects.fx",NULL,NULL,"PS","ps_5_0",0,0,0,&PS_Buffer,&ErrorMsg,0);
			if(FAILED(hr))
	{	
		if(ErrorMsg)
		{
			ShaderErrorOutput(ErrorMsg,"VertexShader");
		}
		MessageBox(NULL, DXGetErrorDescription(hr),
			TEXT(" Vertexshader"), MB_OK);	
		return 0;	
	}
	
	//Erstelle Shader
	hr=d3d11Device->CreateVertexShader(VS_Buffer->GetBufferPointer(),VS_Buffer->GetBufferSize(),NULL,&VS);
	hr=d3d11Device->CreatePixelShader(PS_Buffer->GetBufferPointer(),PS_Buffer->GetBufferSize(),NULL,&PS);

	//Setze Shaders als aktuelle Pipeline Shader
	d3d11DevCon->VSSetShader(VS,NULL,0);
	d3d11DevCon->PSSetShader(PS,NULL,0);

	//Font Stuff
	//--------------------------------------------
	hr=D3DX11CompileFromFile(L"Effects.fx",NULL,NULL,"FONT_VS","vs_5_0",0,0,0,&FONT_VS_Buffer,&ErrorMsg,0);
			if(FAILED(hr))
	{	
		if(ErrorMsg)
		{
			ShaderErrorOutput(ErrorMsg,"VertexShader");
		}
		MessageBox(NULL, DXGetErrorDescription(hr),
			TEXT(" Vertexshader"), MB_OK);	
		return 0;	
	}
	hr=D3DX11CompileFromFile(L"Effects.fx",NULL,NULL,"FONT_PS","ps_5_0",0,0,0,&FONT_PS_Buffer,&ErrorMsg,0);
			if(FAILED(hr))
	{	
		if(ErrorMsg)
		{
			ShaderErrorOutput(ErrorMsg,"VertexShader");
		}
		MessageBox(NULL, DXGetErrorDescription(hr),
			TEXT(" Vertexshader"), MB_OK);	
		return 0;	
	}
	
	//Erstelle Shader
	hr=d3d11Device->CreateVertexShader(FONT_VS_Buffer->GetBufferPointer(),FONT_VS_Buffer->GetBufferSize(),NULL,&FONT_VS);
	hr=d3d11Device->CreatePixelShader(FONT_PS_Buffer->GetBufferPointer(),FONT_PS_Buffer->GetBufferSize(),NULL,&FONT_PS);




	//---------------------------------------

	// Erstelle ein paar vertices Für einen Würfel 8 Ecken
	Vertex v[]=
	{
			// Front Face
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		Vertex( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		Vertex( 1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		// Back Face
		Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f),
		Vertex( 1.0f, -1.0f, 1.0f, 0.0f, 1.0f),
		Vertex( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f),
		Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f),

		// Top Face
		Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f),
		Vertex( 1.0f, 1.0f,  1.0f, 1.0f, 0.0f),
		Vertex( 1.0f, 1.0f, -1.0f, 1.0f, 1.0f),

		// Bottom Face
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
		Vertex( 1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex( 1.0f, -1.0f,  1.0f, 0.0f, 0.0f),
		Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f),

		// Left Face
		Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f),
		Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		// Right Face
		Vertex( 1.0f, -1.0f, -1.0f, 1.0f, 0.0f),
		Vertex( 1.0f,  1.0f, -1.0f, 1.0f, 1.0f),
		Vertex( 1.0f,  1.0f,  1.0f, 0.0f, 1.0f),
		Vertex( 1.0f, -1.0f,  1.0f, 0.0f, 0.0f),	
		
	};

	// Erstelle ein paar indices
	DWORD indices[] ={
			// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 22, 23,
		
		20, 21, 22,
		};
	
	//Beschreibe den Vertex Buffer
	D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.ByteWidth = sizeof(Vertex)*24;
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

	//Erstelle Datenstruktur IM VertexBuffer  
	D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData,sizeof(vertexBufferData));
		vertexBufferData.pSysMem = v; // Daten die in den Buffer geschrieben werden

	// Erstelle den VertexBuffer
	hr = d3d11Device->CreateBuffer(&vertexBufferDesc,&vertexBufferData,&vertexBuffer);
	
	//Font Stuff
	//-----------------------------
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.CPUAccessFlags= D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.ByteWidth = sizeof(Vertex)*6*numLetters;

	hr=d3d11Device->CreateBuffer(&vertexBufferDesc,NULL,&fontVertexBuffer);
	//-------------------------------

	
	//Beschreibe den IndexBuffer
	D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(DWORD)*12*3;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

	// Erstelle Datenstruktur für IndexBuffer
	D3D11_SUBRESOURCE_DATA indexBufferData;
		ZeroMemory(&indexBufferData,sizeof(indexBufferData));
		indexBufferData.pSysMem = indices; // Daten die in den Buffer geschrieben werden
	
	//Erstelle Index Buffer
	d3d11Device->CreateBuffer(&indexBufferDesc,&indexBufferData,&indexBuffer);


	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	//Binde den VertexBuffer an den Input Assembler
	d3d11DevCon->IASetVertexBuffers(0,2,&vertexBuffer,&stride,&offset);

	//Binde den IndexBuffer an den Input Assambler
	d3d11DevCon->IASetIndexBuffer(indexBuffer,DXGI_FORMAT_R32_UINT,0);

	//Erstelle Input Layout
	hr = d3d11Device->CreateInputLayout(layout,2,VS_Buffer->GetBufferPointer(),VS_Buffer->GetBufferSize(), &vertLayout);
	
	// Binde das Layout an den Input Assembler
	d3d11DevCon->IASetInputLayout(vertLayout);
	
	//Lege die Input Topologie fest (points , triangles etc)
	d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Erstelle den Viewport
	D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport,sizeof(viewport));
		viewport.TopLeftX=0;
		viewport.TopLeftY=0;
		viewport.Width=Width;
		viewport.Height=Height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

	//Binde den Vieport an den Rasterizer
	d3d11DevCon->RSSetViewports(1,&viewport);

	//Beschreibe Rendering State 
	D3D11_RASTERIZER_DESC ccwdesc;
		ZeroMemory(&ccwdesc,sizeof(D3D11_RASTERIZER_DESC));
		ccwdesc.FillMode = D3D11_FILL_SOLID;
		ccwdesc.CullMode = D3D11_CULL_BACK;
		ccwdesc.FrontCounterClockwise = true;
		
	D3D11_RASTERIZER_DESC cwdesc;
		ZeroMemory(&cwdesc,sizeof(D3D11_RASTERIZER_DESC));
		cwdesc.FillMode=D3D11_FILL_SOLID;
		cwdesc.CullMode=D3D11_CULL_BACK;
		cwdesc.FrontCounterClockwise = false;
		
	D3D11_RASTERIZER_DESC noCulldesc;
		ZeroMemory(&noCulldesc,sizeof(D3D11_RASTERIZER_DESC));
		noCulldesc.FillMode=D3D11_FILL_SOLID;
		noCulldesc.CullMode=D3D11_CULL_NONE;
		
	

	// Erstelle Rendering State
	hr = d3d11Device->CreateRasterizerState(&ccwdesc, &CCWcullMode);
	hr = d3d11Device->CreateRasterizerState(&cwdesc,&CWcullMode);
	hr = d3d11Device->CreateRasterizerState(&noCulldesc,&noCull);
	//Beschreibe den ConstantBuffer
	D3D11_BUFFER_DESC cbbd;
		ZeroMemory(&cbbd,sizeof(D3D11_BUFFER_DESC));
		cbbd.Usage = D3D11_USAGE_DEFAULT;
		cbbd.ByteWidth= sizeof(cbPerObject);
		cbbd.BindFlags= D3D11_BIND_CONSTANT_BUFFER;
		cbbd.CPUAccessFlags=0;
		cbbd.MiscFlags = 0;

	//Erstelle Buffer
	hr = d3d11Device->CreateBuffer(&cbbd,NULL,&cbPerObjectBuffer);

	// Lader Textur aus Datei
	hr=D3DX11CreateShaderResourceViewFromFile(d3d11Device,L"./textures/Cage.png"
												,NULL,NULL,&Texture,NULL);

	// Erstelle Sampler Desciptor
	D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory( &sampDesc, sizeof(sampDesc) );
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr=d3d11Device->CreateSamplerState(&sampDesc,&TexSamplerState);
	
	//Font stuff
	//--------------------------------
	hr=D3DX11CreateShaderResourceViewFromFile(d3d11Device,L"ArialFont.dds",NULL
												,NULL,&fontAtlas,NULL);
	hr=d3d11Device->CreateSamplerState(&sampDesc,&fontAtlasSampler);
	//--------------------------------------------------

	// Erstelle Blend Descriptor
	D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc,sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
		ZeroMemory( &rtbd, sizeof(rtbd));
		rtbd.BlendEnable			 = true;
		rtbd.SrcBlend				 = D3D11_BLEND_SRC_COLOR;
		rtbd.DestBlend				 = D3D11_BLEND_BLEND_FACTOR;
		rtbd.BlendOp				 = D3D11_BLEND_OP_ADD;
		rtbd.SrcBlendAlpha			 = D3D11_BLEND_ONE;
		rtbd.DestBlendAlpha			 = D3D11_BLEND_ZERO;
		rtbd.BlendOpAlpha			 = D3D11_BLEND_OP_ADD;
		rtbd.RenderTargetWriteMask	 = D3D10_COLOR_WRITE_ENABLE_ALL;	

		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.RenderTarget[0] = rtbd;

	// Erstelle Blend State
	d3d11Device->CreateBlendState(&blendDesc,&Transparency);
	//Initialisiere die Kamera
	camPosition = XMVectorSet(0.0f,3.0f,-8.0f,0.0f);
	camTarget= XMVectorSet(0.0f,0.0f,0.0f,0.0f);
	camUp =    XMVectorSet(0.0f,1.0f,0.0f,0.0f);

	// Initialisiere die ViewMatrix
	camView= XMMatrixLookAtLH(camPosition,camTarget,camUp);
	

	// Initialisiere die Projection Matrix
	camProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f,(float)Width/Height,1.0f,1000.0f);

	return true;
}


/*------------------------------------
------ UpdateScene() Funktion ------
Hier wird die Scene verändert
------------------------------------*/
void UpdateScene()
{
	// Rotation der Würfel:
	rot += 0.00005f;
	if (rot > 6.28f)
		rot = 0.0f;

	// Setze die cube1World Matrix wieder auf die Identität zurück
	cube1World = XMMatrixIdentity();

	XMVECTOR rotaxis1 = XMVectorSet(1.0f,1.0f,0.0f,0.0f);
	Rotation = XMMatrixRotationAxis(rotaxis1,rot);
	Translation= XMMatrixTranslation(0.0f,0.0f,4.0f);
	// Bestimme die worldmatrix für würfel1
	cube1World = Translation*Rotation;

	//Jetzt cube2

	cube2World = XMMatrixIdentity();
	
	XMVECTOR rotaxis2 = XMVectorSet(0.0f,1.0f,0.0f,0.0f);
	Rotation = XMMatrixRotationAxis(rotaxis2,-rot);
	Scale = XMMatrixScaling(1.3f,1.3f,1.3f);

	cube2World = Scale*Rotation ;
	
}


/*------------------------------------
------ DrawScene() Funktion ------
Zeichne in den Backbuffer und Swappe
------------------------------------*/
void DrawScene()
{	
	//FONT STUFF
	//-----------------------------------
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	d3d11DevCon->IASetVertexBuffers(0,2,&vertexBuffer,&stride,&offset);
	d3d11DevCon->VSSetShader(VS,NULL,0);
	d3d11DevCon->PSSetShader(PS,NULL,0);
	//---------------------------------------
	//Cleare den Backbuffer bevor irgendwas gezeichnet wird
	D3DXCOLOR bgColor(0.0f,0.0f,0.0f,0.0f);
	d3d11DevCon->ClearRenderTargetView(renderTargetView,bgColor);

	//Setze DepthStencilView zurück
	d3d11DevCon->ClearDepthStencilView(depthStencilView,D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f,0);
    
	// Default Render State
	d3d11DevCon->RSSetState(NULL);

	// Zeichne Objekte mit Backface Culling

	// No Culling Render State
	d3d11DevCon->RSSetState(noCull);

	// Zeichne den Ersten würfel
	WVP = cube1World*camView*camProjection;	
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer,0,NULL,&cbPerObj,0,0);	
	d3d11DevCon->VSSetConstantBuffers(0,1, &cbPerObjectBuffer );

	//Schicke Textur an Pixelshader
	d3d11DevCon->PSSetShaderResources( 0, 1, &Texture );
	d3d11DevCon->PSSetSamplers( 0, 1, &TexSamplerState );
		
	d3d11DevCon->DrawIndexed(36,0,0);

	// Zeichne den 2ten Würfel;
	WVP = cube2World*camView*camProjection;	
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer,0,NULL,&cbPerObj,0,0);	
	d3d11DevCon->VSSetConstantBuffers(0,1, &cbPerObjectBuffer );
	
	d3d11DevCon->PSSetShaderResources( 0, 1, &Texture );
	d3d11DevCon->PSSetSamplers( 0, 1, &TexSamplerState );

	d3d11DevCon->DrawIndexed(36,0,0);

	//Font Stuff
	//-----------------------------
	d3d11DevCon->VSSetShader(FONT_VS,0,0);
	d3d11DevCon->PSSetShader(FONT_PS,0,0);
	//d3d11DevCon->RSSetState(NULL);

	WVP = camProjection;
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer,0,NULL,&cbPerObj,0,0);	
	d3d11DevCon->VSSetConstantBuffers(0,1, &cbPerObjectBuffer );

	d3d11DevCon->IASetVertexBuffers(0,1,&fontVertexBuffer,&stride,&offset);
	d3d11DevCon->PSSetShaderResources(0,1,&fontAtlas);
	d3d11DevCon->PSSetSamplers(0,1,&fontAtlasSampler);
	DrawString("EASY FONT",-0.95f,0.65);
	//----------------------------
	//Swap
	SwapChain->Present(0,0);
}

// FONT STUFF
//---------------------------
bool DrawString(char*text,float posX,float posY){
	
	// size of one letter and string size
	int letterSize = sizeof(Vertex)*6;
	int textSize = strlen(text);

	// cut string if its to long
	if (textSize > numLetters)
		textSize = numLetters;

	// size of one char on screen
	float cScreenWidth = 32.0f/Width;
	float cScreenHeight = 32.0f/Height;
	// texel Size
	float texelWidth = 32.0f/textureWidth;

	//Create Mapped Subresource
	D3D11_MAPPED_SUBRESOURCE mappedSub;
	hr= d3d11DevCon->Map(fontVertexBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mappedSub);

	// Pointer to buffer subresource
	Vertex * sprite = (Vertex*)mappedSub.pData;
	
	// Indices of a A and z
	const int indexA = static_cast<int>('A');
	const int indexZ = static_cast<int>('Z');
	
	//loop to build the string
	for(int i=0; i< textSize; i++)
	{
		/*Get starting position of the quad. First Quad is just the posX value , 
		then characterwidth is added.*/

		float thisStartX = posX +(cScreenWidth * static_cast<char>(i));
		float thisEndX =thisStartX + cScreenWidth;
		float thisStartY = posY;
		float thisEndY = thisStartY + cScreenHeight;

		// Write the position of each 6 verticess to subresource
		sprite[0].pos = XMFLOAT3( thisEndX, thisEndY, 1.0f );
		sprite[1].pos = XMFLOAT3( thisEndX, posY, 1.0f );
		sprite[2].pos = XMFLOAT3( thisStartX, posY, 1.0f );
		sprite[3].pos = XMFLOAT3( thisStartX, posY, 1.0f );
		sprite[4].pos = XMFLOAT3( thisStartX, thisEndY, 1.0f );
		sprite[5].pos = XMFLOAT3( thisEndX, thisEndY, 1.0f );

		// calculate texture coordinates from ASCII values.
		int texLookup = 0;
		int letter = static_cast<int>(text[i]);

		// Blank Space ( As the Blank space is just added after Z, 
		//it needs some extra treatment
		if (letter < indexA || letter > indexZ)
		{
			texLookup = (indexZ - indexA)+1;
		}
		else
		// Characters
		{
			texLookup=(letter-indexA);
		}


		float texStart = 0.0f +( texelWidth * static_cast<float>(texLookup));
		float texEnd = texStart + texelWidth;
		
		// Apply texture coordinates to subresource
		sprite[0].texCoord = XMFLOAT2(texEnd, 0.0f);
		sprite[1].texCoord = XMFLOAT2( texEnd, 1.0f );
		sprite[2].texCoord = XMFLOAT2( texStart, 1.0f );
		sprite[3].texCoord = XMFLOAT2( texStart, 1.0f );
		sprite[4].texCoord = XMFLOAT2( texStart, 0.0f );
		sprite[5].texCoord = XMFLOAT2( texEnd, 0.0f );

		//set sprite pointer for next sprite
		sprite = sprite + 6;
	}
	d3d11DevCon->Unmap(fontVertexBuffer,0);
	d3d11DevCon->Draw(6*textSize,0);
		
	return true;
}


//---------------------------

/*------------------------------------
------ ReleaseObject() Funktion ------
Räume auf -> COM Objecte Releasen
------------------------------------*/
void ReleaseObjects()
{
	
	if(SwapChain)SwapChain->Release();
	if(d3d11Device)d3d11Device->Release();
	if(d3d11DevCon)d3d11DevCon->Release();
	if(vertexBuffer)vertexBuffer->Release();
	if(indexBuffer)indexBuffer->Release();
	if(VS)VS->Release();
	if(PS)PS->Release();
	if(VS_Buffer)VS_Buffer->Release();
	if(PS_Buffer)PS_Buffer->Release();
	if(vertLayout)vertLayout->Release();
	if(depthStencilView)depthStencilView->Release();
	if(depthStencilBuffer)depthStencilBuffer->Release();
	if(cbPerObjectBuffer)cbPerObjectBuffer->Release();
	if(CCWcullMode)CCWcullMode->Release();
	if(CWcullMode)CWcullMode->Release();
	if(Texture)Texture->Release();
	if(TexSamplerState)TexSamplerState->Release();
	if(Transparency)Transparency->Release();
	if(noCull)noCull->Release();

	//Font Stuff

	if(fontAtlas)fontAtlas->Release();
	if(fontAtlasSampler)fontAtlasSampler->Release();
	if(fontVertexBuffer)fontVertexBuffer->Release();
}


void ShaderErrorOutput(ID3D10Blob* ErrMsg, std::string shader)
{
	
	char* errors;
	std::string header;
	std::string footer;
	std::string errorStr;
	
	size_t length,convertedChars;

	header = "\n***   "+ shader +" Debug Output   ***\n\n\n";
	footer = "\n\n\n***   Shader Debug Output End   ***\n";
	errors = (char*)(ErrMsg->GetBufferPointer());	
	errorStr = errors;
	errorStr = header + errorStr + footer;

	length = strlen(errorStr.c_str())+1;

	wchar_t * wErrors = new wchar_t[length]; 
	mbstowcs_s(&convertedChars,wErrors, length, errorStr.c_str(),_TRUNCATE);
	

	OutputDebugString(wErrors);
}