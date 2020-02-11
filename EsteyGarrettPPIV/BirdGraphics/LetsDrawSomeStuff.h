// This file can be used to contain very basic DX11 Loading, Drawing & Clean Up. (Start Here, or Make your own set of classes)
#pragma once
// Include our DX11 middle ware 
#include "../../Gateware/Interface/G_Graphics/GDirectX11Surface.h"
// Other possible Gateware libraries:
// GMatrix, GAudio, GController, GInput, GLog, GFile... +more
// While these libraries won't directly add any points to your project they can be used to make them more "gamelike"
// To add a library just include the relevant "Interface" & add any related *.cpp files from the "Source" folder to the Gateware filter->.
// Gateware comes with MSDN style docs too: "Gateware\Documentation\html\index.html"

// Include DirectX11 for interface access
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <fstream>
#include "DDSTextureLoader.h"
// Shader includes
#include "VS_Default.csh"
#include "PS_Default.csh"
#include "PS_SolidColor.csh"

using namespace DirectX;
using namespace std;

// Structs
struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 tex;
};

struct Mesh
{
	vector<Vertex> vertexList;
	vector<int> indicesList;
	XMMATRIX mWorld;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 dirLightDir;
	XMFLOAT4 dirLightCol;
};

// Global variables
XMMATRIX	myViewMatrix;
XMMATRIX	myProjectionMatrix;

Mesh		myMesh1;
Mesh		gridMesh;

// Simple Container class to make life easier/cleaner
class LetsDrawSomeStuff
{
	// variables here
	GW::GRAPHICS::GDirectX11Surface* mySurface = nullptr;
	// Gettting these handles from GDirectX11Surface will increase their internal refrence counts, be sure to "Release()" them when done!
	ID3D11Device*						myDevice = nullptr;
	IDXGISwapChain*						mySwapChain = nullptr;
	ID3D11DeviceContext*				myContext = nullptr;

	// TODO: Add your own D3D11 variables here (be sure to "Release()" them when done!)
	ID3D11Buffer*						myVertexBuffer = nullptr;
	ID3D11Buffer*						gridVertexBuffer = nullptr;
	ID3D11Buffer*						myIndexBuffer = nullptr;
	ID3D11Buffer*					    myConstantBuffer = nullptr;
	ID3D11InputLayout*					myVertexLayout = nullptr;
	ID3D11VertexShader*					myVertexShader = nullptr;
	ID3D11PixelShader*					psDefault = nullptr;
	ID3D11PixelShader*					psSolidColor = nullptr;
	ID3D11ShaderResourceView*           myTextureRV = nullptr;
	ID3D11SamplerState*                 mySamplerLinear = nullptr;

public:
	// Init
	LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint);
	// Shutdown
	~LetsDrawSomeStuff();
	// Draw
	void Render();
	// Load mesh
	void LoadMesh(const char* meshFileName, Mesh& mesh);
};

// Init
LetsDrawSomeStuff::LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint)
{
	if (attatchPoint) // valid window?
	{
		// Create surface, will auto attatch to GWindow
		if (G_SUCCESS(GW::GRAPHICS::CreateGDirectX11Surface(attatchPoint, GW::GRAPHICS::DEPTH_BUFFER_SUPPORT, &mySurface)))
		{
			// Grab handles to all DX11 base interfaces
			mySurface->GetDevice((void**)&myDevice);
			mySurface->GetSwapchain((void**)&mySwapChain);
			mySurface->GetContext((void**)&myContext);

			// TODO: Create new DirectX stuff here! (Buffers, Shaders, Layouts, Views, Textures, etc...)
			HRESULT hr = S_OK;

			#pragma region myMesh1 Loading

			// Load a mesh
			LoadMesh("./Assets/Meshes/axe.mesh", myMesh1);
			// Initialize the world matrix for this mesh
			myMesh1.mWorld = XMMatrixIdentity();

			#pragma endregion

			// gridMesh Creation
			{
				// Initialize the world matrix for this mesh
				gridMesh.mWorld = XMMatrixIdentity();

				// Establish constant grid points
				float gridScale = 1.0f;
				for (float x = -25.0f; x <= 25.0f; x += 1.0f)
				{
					gridMesh.vertexList.push_back({ XMFLOAT3(x * gridScale, 0.0f, -25.0f * gridScale), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
					gridMesh.vertexList.push_back({ XMFLOAT3(x * gridScale, 0.0f, 25.0f * gridScale), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
				}
				for (float z = -25.0f; z <= 25.0f; z += 1.0f)
				{
					gridMesh.vertexList.push_back({ XMFLOAT3(-25.0f * gridScale, 0.0f, z * gridScale), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
					gridMesh.vertexList.push_back({ XMFLOAT3(25.0f * gridScale, 0.0f, z * gridScale), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
				}
			}

			// myMesh1 Buffers
			{
				// Vertex Buffer

				// This creates an empty buffer description object
				D3D11_BUFFER_DESC bd = {};
				// The usage flag informs how the data will be used. IMMUTABLE means that this is a constant, never changing buffer
				// You can also use DEFAULT, which allows the GPU to alter this data, but not the CPU
				// There is also DYNAMIC, meaning it can be changed at any time by the CPU or GPU
				bd.Usage = D3D11_USAGE_IMMUTABLE;
				// The bindFlags inform what the buffer will be used to store. In this case, verts
				bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				// CPUAccessFlags inform what kind of acess the CPU has to this buffer. None. 0. This is immutable
				bd.CPUAccessFlags = 0;
				// ByteWidth is just informing how large the buffer is
				bd.ByteWidth = sizeof(Vertex) * myMesh1.vertexList.size();

				// Create the buffer on the device
				D3D11_SUBRESOURCE_DATA InitData = {};
				InitData.pSysMem = myMesh1.vertexList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &myVertexBuffer);


				// Index Buffer

				// Create index buffer
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(int) * myMesh1.indicesList.size();        // 36 vertices needed for 12 triangles in a triangle list
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;
				InitData.pSysMem = myMesh1.indicesList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &myIndexBuffer);
			}

			// gridMesh Buffers
			{

				// This creates an empty buffer description object
				D3D11_BUFFER_DESC bd = {};
				// The usage flag informs how the data will be used. IMMUTABLE means that this is a constant, never changing buffer
				// You can also use DEFAULT, which allows the GPU to alter this data, but not the CPU
				// There is also DYNAMIC, meaning it can be changed at any time by the CPU or GPU
				bd.Usage = D3D11_USAGE_IMMUTABLE;
				// The bindFlags inform what the buffer will be used to store. In this case, verts
				bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				// CPUAccessFlags inform what kind of acess the CPU has to this buffer. None. 0. This is immutable
				bd.CPUAccessFlags = 0;
				// ByteWidth is just informing how large the buffer is
				bd.ByteWidth = sizeof(Vertex) * gridMesh.vertexList.size();

				// Create the buffer on the device
				D3D11_SUBRESOURCE_DATA InitData = {};
				InitData.pSysMem = gridMesh.vertexList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &gridVertexBuffer);

			}


			#pragma region Vertex Shaders

			myDevice->CreateVertexShader(VS_Default, sizeof(VS_Default), nullptr, &myVertexShader);

			#pragma endregion

			#pragma region Pixel Shaders

			myDevice->CreatePixelShader(PS_Default, sizeof(PS_Default), nullptr, &psDefault);
			myDevice->CreatePixelShader(PS_SolidColor, sizeof(PS_SolidColor), nullptr, &psSolidColor);

			#pragma endregion

			// Input Layout
			{
				// Create the input layout
				const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};
				// Grab the size of the layout dynamically, in case it's changed. This size is required to create the layout on the device
				UINT elements = ARRAYSIZE(inputElementDesc);

				// Create layout on the device
				hr = myDevice->CreateInputLayout(inputElementDesc, elements, VS_Default, sizeof(VS_Default), &myVertexLayout);

				// Set layout in the context
				myContext->IASetInputLayout(myVertexLayout);
			}

			// Sample State
			{
				// Create the sample state
				D3D11_SAMPLER_DESC sampDesc = {};
				sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
				sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
				sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
				sampDesc.MinLOD = 0;
				sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
				hr = myDevice->CreateSamplerState(&sampDesc, &mySamplerLinear);
			}

			#pragma region Shader Resource View

			hr = CreateDDSTextureFromFile(myDevice, L"./Assets/Textures/axe.dds", nullptr, &myTextureRV);

			#pragma endregion

			#pragma region Constant Buffer

			D3D11_BUFFER_DESC bd = {};
			// Create the constant buffer
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(ConstantBuffer);
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			hr = myDevice->CreateBuffer(&bd, nullptr, &myConstantBuffer);

			#pragma endregion

			#pragma region Matrices

			// Initialize the view matrix
			XMVECTOR Eye = XMVectorSet(0.0f, 4.0f, -10.0f, 0.0f);
			XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			myViewMatrix = XMMatrixLookAtLH(Eye, At, Up);

			// Initialize the projection matrix
			UINT height;
			UINT width;
			attatchPoint->GetClientHeight(height);
			attatchPoint->GetClientWidth(width);
			myProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);

			#pragma endregion

		}
	}
}

// Shutdown
LetsDrawSomeStuff::~LetsDrawSomeStuff()
{
	// Release DX Objects aquired from the surface
	myDevice->Release();
	mySwapChain->Release();
	myContext->Release();

	// TODO: "Release()" more stuff here!
	if (myVertexShader)
		myVertexShader->Release();
	if (psDefault)
		psDefault->Release();
	if (psSolidColor)
		psSolidColor->Release();
	if (myVertexLayout)
		myVertexLayout->Release();
	if (myVertexBuffer)
		myVertexBuffer->Release();
	if (gridVertexBuffer)
		gridVertexBuffer->Release();
	if (myTextureRV)
		myTextureRV->Release();
	if (mySamplerLinear)
		mySamplerLinear->Release();
	if (myIndexBuffer)
		myIndexBuffer->Release();
	if (myConstantBuffer)
		myConstantBuffer->Release();

	if (mySurface) // Free Gateware Interface
	{
		mySurface->DecrementCount(); // reduce internal count (will auto delete on Zero)
		mySurface = nullptr; // the safest way to fly
	}
}

// Draw
void LetsDrawSomeStuff::Render()
{
	if (mySurface) // valid?
	{
		// this could be changed during resolution edits, get it every frame
		ID3D11RenderTargetView *myRenderTargetView = nullptr;
		ID3D11DepthStencilView *myDepthStencilView = nullptr;
		if (G_SUCCESS(mySurface->GetRenderTarget((void**)&myRenderTargetView)))
		{
			// Grab the Z Buffer if one was requested
			if (G_SUCCESS(mySurface->GetDepthStencilView((void**)&myDepthStencilView)))
			{
				myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.
				myDepthStencilView->Release();
			}

			// Set active target for drawing, all array based D3D11 functions should use a syntax similar to below
			ID3D11RenderTargetView* const targets[] = { myRenderTargetView };
			myContext->OMSetRenderTargets(1, targets, myDepthStencilView);

			// Clear the screen to green
			const float clearColor[] = { 0.3f, 0.3f, 0.3f, 1 };
			myContext->ClearRenderTargetView(myRenderTargetView, clearColor);
			
			// TODO: Set your shaders, Update & Set your constant buffers, Attatch your vertex & index buffers, Set your InputLayout & Topology & Draw!

			// Update our time
			static float t = 0.0f;
			static ULONGLONG timeStart = 0;
			ULONGLONG timeCur = GetTickCount64();
			if (timeStart == 0)
				timeStart = timeCur;
			t = (timeCur - timeStart) / 1000.0f;

			// Drawing myMesh1
			{
				// Set vertex buffer in the context
				UINT stride = sizeof(Vertex);
				UINT offset = 0;
				myContext->IASetVertexBuffers(0, 1, &myVertexBuffer, &stride, &offset);

				// Set index buffer
				myContext->IASetIndexBuffer(myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

				// Rotate mesh1 around the origin
				myMesh1.mWorld = XMMatrixRotationY(t);

				// Set primitive topology
				myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				// Update constant buffer
				ConstantBuffer cb1;
				cb1.mWorld = XMMatrixTranspose(myMesh1.mWorld);
				cb1.mView = XMMatrixTranspose(myViewMatrix);
				cb1.mProjection = XMMatrixTranspose(myProjectionMatrix);
				cb1.dirLightCol = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
				cb1.dirLightDir = XMFLOAT4(-1.0f, 0.5f, -0.5f, 1.0f);
				// Send updated constant buffer
				myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb1, 0, 0);

				// Setup to render normal object
				myContext->VSSetShader(myVertexShader, nullptr, 0);
				myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
				myContext->PSSetShader(psDefault, nullptr, 0);
				myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer);
				myContext->PSSetShaderResources(0, 1, &myTextureRV);
				myContext->PSSetSamplers(0, 1, &mySamplerLinear);

				// Draw indexed object
				myContext->DrawIndexed((UINT)myMesh1.indicesList.size(), 0, 0);
			}

			// Drawing gridMesh
			{
				// Set vertex buffer in the context
				UINT stride = sizeof(Vertex);
				UINT offset = 0;
				myContext->IASetVertexBuffers(0, 1, &gridVertexBuffer, &stride, &offset);

				// Set index buffer in the context
				myContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);

				// Set primitive topology
				myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

				// Update constant buffer
				ConstantBuffer cb1;
				cb1.mWorld = XMMatrixTranspose(gridMesh.mWorld);
				cb1.mView = XMMatrixTranspose(myViewMatrix);
				cb1.mProjection = XMMatrixTranspose(myProjectionMatrix);
				cb1.dirLightCol = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
				cb1.dirLightDir = XMFLOAT4(-1.0f, 0.5f, -0.5f, 1.0f);
				// Send updated constant buffer
				myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb1, 0, 0);

				// Setup to render normal object
				myContext->VSSetShader(myVertexShader, nullptr, 0);
				myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
				myContext->PSSetShader(psSolidColor, nullptr, 0);
				myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer);

				// Draw normal object
				myContext->Draw((UINT)gridMesh.vertexList.size(), 0);
			}

			// Present Backbuffer using Swapchain object
			// Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.
			mySwapChain->Present(0, 0); // set first argument to 1 to enable vertical refresh sync with display

			// Free any temp DX handles aquired this frame
			myRenderTargetView->Release();
		}
	}
}

void LetsDrawSomeStuff::LoadMesh(const char* meshFileName, Mesh& mesh)
{
	std::fstream file{ meshFileName, std::ios_base::in | std::ios_base::binary };

	assert(file.is_open());

	uint32_t player_index_count;
	file.read((char*)&player_index_count, sizeof(uint32_t));

	mesh.indicesList.resize(player_index_count);

	file.read((char*)mesh.indicesList.data(), sizeof(uint32_t) * player_index_count);

	uint32_t player_vertex_count;
	file.read((char*)&player_vertex_count, sizeof(uint32_t));

	mesh.vertexList.resize(player_vertex_count);

	file.read((char*)mesh.vertexList.data(), sizeof(Vertex) * player_vertex_count);

	// Example mesh conditioning if needed - this flips handedness
	for (auto& v : mesh.vertexList)
	{
		v.pos.x = -v.pos.x;
		v.normal.x = -v.normal.x;
		v.tex.y = 1.0f - v.tex.y;
	}

	int tri_count = (int)(mesh.indicesList.size() / 3);

	for (int i = 0; i < tri_count; ++i)
	{
		int* tri = mesh.indicesList.data() + i * 3;

		int temp = tri[0];
		tri[0] = tri[2];
		tri[2] = temp;
	}
	file.close();
}