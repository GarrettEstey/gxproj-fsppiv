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
#include "./Shaders/Csh/VS_Default.csh"
#include "./Shaders/Csh/VS_Waves.csh"
#include "./Shaders/Csh/VS_Instancing.csh"
#include "./Shaders/Csh/PS_Default.csh"
#include "./Shaders/Csh/PS_SolidColor.csh"
#include "./Shaders/Csh/PS_CustomWaves.csh"
#include "./Shaders/Csh/PS_SkyBox.csh"
#include "./Shaders/Csh/GS_Test.csh"
// Other includes
#include "lightDefines.h"

#define INSTANCECOUNT 27

using namespace DirectX;
using namespace std;

//namespace DrawingStuff
//{
	// Structs
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 tex;
	};

	struct Mesh
	{
		const char* name;
		vector<Vertex> vertexList;
		vector<int> indicesList;
		XMMATRIX mWorld;
		ID3D11Buffer* vertexBuffer = nullptr;
		ID3D11Buffer* indexBuffer = nullptr;
		ID3D11ShaderResourceView* textureRV = nullptr;
	};

	struct DirectionalLight
	{
		XMFLOAT4 col;
		XMFLOAT4 dir;
	};

	struct PointLight
	{
		XMFLOAT4 col;
		XMFLOAT4 pos;
		XMFLOAT4 rad;
	};

	struct SpotLight
	{
		XMFLOAT4 col;
		XMFLOAT4 pos;
		XMFLOAT4 dir;
		XMFLOAT4 rat;
	};

	struct ConstantBuffer
	{
		XMMATRIX mWorld;
		XMMATRIX mView;
		XMMATRIX mProjection;
		DirectionalLight dirLights[DIRLIGHTCOUNT];
		PointLight pointLights[POINTLIGHTCOUNT];
		SpotLight spotLights[SPOTLIGHTCOUNT];
		XMFLOAT4 solidColor;
		XMFLOAT2 time;
		XMFLOAT2 useDirLights;
		XMFLOAT2 usePointLights;
		XMFLOAT2 useSpotLights;
	};

	struct InstanceData
	{
		XMFLOAT4 translation;
	};

	InstanceData	perInstanceData[INSTANCECOUNT];

	// Matrices
	XMMATRIX		testSceneViewMatrix;
	XMMATRIX		vp1ProjectionMatrix;
	XMMATRIX		spaceSceneViewMatrix;
	XMMATRIX		vp2ProjectionMatrix;

	// Meshes
	vector<Mesh>	meshes;
	vector<ID3D11ShaderResourceView*>	planetTextures;

	// Control variables
	POINT			oldCursorPos;
	bool			cameraPaused = false;
	int				inputDelay = 0;
	bool			testGeoShader = false;
	unsigned int	testSceneVp = 0;
	unsigned int	spaceSceneVp = 1;
	float			moveSpeed = 0.005f;
	float			lookSpeed = 0.003f;
	unsigned int	planetLookAt = 0;
//};

//using namespace DrawingStuff;

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
	ID3D11Buffer*					    myConstantBuffer = nullptr;
	ID3D11Buffer*						cubeInstanceBuffer = nullptr;
	ID3D11InputLayout*					myVertexLayout = nullptr;
	ID3D11SamplerState*					mySamplerLinear = nullptr;
	ID3D11VertexShader*					vsDefault = nullptr;
	ID3D11VertexShader*					vsWaves = nullptr;
	ID3D11VertexShader*					vsInstancing = nullptr;
	ID3D11PixelShader*					psDefault = nullptr;
	ID3D11PixelShader*					psSolidColor = nullptr;
	ID3D11PixelShader*					psCustomWaves = nullptr;
	ID3D11PixelShader*					psSkyBox = nullptr;
	ID3D11GeometryShader*				gsTest = nullptr;
	D3D11_VIEWPORT*						viewPorts = nullptr;

	// Function definitions
	void FindMesh(const char* meshName, unsigned int& index);
	void BasicDrawIndexed(unsigned int index, ConstantBuffer cb, D3D_PRIMITIVE_TOPOLOGY topology, ID3D11VertexShader* vShader, ID3D11PixelShader* pShader, ID3D11GeometryShader* gShader);
	void BasicDraw(unsigned int index, ConstantBuffer cb, D3D_PRIMITIVE_TOPOLOGY topology, ID3D11VertexShader* vShader, ID3D11PixelShader* pShader, ID3D11GeometryShader* gShader);
	void LoadMesh(const char* meshFileName, Mesh& mesh);
	void DrawTestScene(ConstantBuffer& cb);
	void DrawSpaceScene(ConstantBuffer& cb);

public:
	// Init
	LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint);
	// Shutdown
	~LetsDrawSomeStuff();
	// Draw
	void Render();
	// Window resized
	void WindowResized(GW::SYSTEM::GWindow* attatchPoint);
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

			GetCursorPos(&oldCursorPos);

			#pragma region Mesh Setup

			// Mesh Loading
			{
				XMMATRIX identity = XMMatrixIdentity();

				// Setup mesh to be used to load in meshes
				Mesh myMesh;
				// Initialize the world matrix for this mesh
				myMesh.mWorld = identity;

				// Set mesh name
				myMesh.name = "Bed";
				// Load a mesh
				LoadMesh("./Assets/Meshes/bed.mesh", myMesh);
				// Push this mesh into the meshes vector
				meshes.push_back(myMesh);

				// Set mesh name
				myMesh.name = "Lantern";
				// Load a mesh
				LoadMesh("./Assets/Meshes/lantern.mesh", myMesh);
				// Push this mesh into the meshes vector
				meshes.push_back(myMesh);

				// Set mesh name
				myMesh.name = "Fan";
				// Load a mesh
				LoadMesh("./Assets/Meshes/fan.mesh", myMesh);
				// Push this mesh into the meshes vector
				meshes.push_back(myMesh);

				// Set mesh name
				myMesh.name = "Cube";
				// Load a mesh
				LoadMesh("./Assets/Meshes/cube.mesh", myMesh);
				// Push this mesh into the meshes vector
				meshes.push_back(myMesh);

				// Set mesh name
				myMesh.name = "TestSkyCube";
				// Load a mesh
				LoadMesh("./Assets/Meshes/skyCube.mesh", myMesh);
				// Push this mesh into the meshes vector
				meshes.push_back(myMesh);

				// Set mesh name
				myMesh.name = "SpaceSkyCube";
				// Load a mesh
				LoadMesh("./Assets/Meshes/skyCube.mesh", myMesh);
				// Push this mesh into the meshes vector
				meshes.push_back(myMesh);

				// Set mesh name
				myMesh.name = "Chest";
				// Load a mesh
				LoadMesh("./Assets/Meshes/chest.mesh", myMesh);
				// Push this mesh into the meshes vector
				meshes.push_back(myMesh);

				// Set mesh name
				myMesh.name = "Planet";
				// Load a mesh
				LoadMesh("./Assets/Meshes/planet.mesh", myMesh);
				// Push this mesh into the meshes vector
				meshes.push_back(myMesh);

				// Set mesh name
				myMesh.name = "Ship";
				// Load a mesh
				LoadMesh("./Assets/Meshes/talonShip.mesh", myMesh);
				// Push this mesh into the meshes vector
				meshes.push_back(myMesh);
			}

			// Grid mesh creation
			{
				// Setup mesh to be used to load in meshes
				Mesh myMesh;
				// Initialize the world matrix for this mesh
				myMesh.mWorld = XMMatrixTranslation(0.0f, -2.0f, 0.0f);

				// Set mesh name
				myMesh.name = "Grid";
				// Establish constant grid points
				float gridScale = 1.0f;
				float gridSize = 10.0f;
				/*for (float x = -25.0f; x <= 25.0f; x += 1.0f)
				{
					myMesh.vertexList.push_back({ XMFLOAT3(x * gridScale, 0.0f, -25.0f * gridScale), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
					myMesh.vertexList.push_back({ XMFLOAT3(x * gridScale, 0.0f, 25.0f * gridScale), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
				}
				for (float z = -25.0f; z <= 25.0f; z += 1.0f)
				{
					myMesh.vertexList.push_back({ XMFLOAT3(-25.0f * gridScale, 0.0f, z * gridScale), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
					myMesh.vertexList.push_back({ XMFLOAT3(25.0f * gridScale, 0.0f, z * gridScale), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
				}*/
				for (float z = -gridSize; z <= gridSize; z += 1.0f)
				{
					for (float x = -gridSize; x < gridSize; x += 1.0f)
					{
						float point = x + z;
						myMesh.vertexList.push_back({ XMFLOAT3(x * gridScale, 0.0f, z * gridScale), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
						myMesh.vertexList.push_back({ XMFLOAT3((x + 1.0f) * gridScale, 0.0f, z * gridScale), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
					}
				}
				for (float z = -gridSize; z < gridSize; z += 1.0f)
				{
					for (float x = -gridSize; x <= gridSize; x += 1.0f)
					{
						float point = x + z;
						myMesh.vertexList.push_back({ XMFLOAT3(x * gridScale, 0.0f, z * gridScale), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
						myMesh.vertexList.push_back({ XMFLOAT3(x * gridScale, 0.0f, (z + 1.0f) * gridScale), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
					}
				}
				// Push this mesh into the meshes vector
				meshes.push_back(myMesh);
			}

			// PointGrid mesh creation
			{
				// Setup mesh to be used to load in meshes
				Mesh myMesh;
				// Initialize the world matrix for this mesh
				myMesh.mWorld = XMMatrixTranslation(0.0f, -5.0f, 0.0f);

				// Set mesh name
				myMesh.name = "PointGrid";
				// Establish constant grid points
				float gridScale = 1.0f;
				float gridSize = 10.0f;
				for (float z = -gridSize; z <= gridSize; z += 1.0f)
				{
					for (float x = -gridSize; x <= gridSize; x += 1.0f)
					{
						float point = x + z;
						myMesh.vertexList.push_back({ XMFLOAT3(x * gridScale, 0.0f, z * gridScale), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) });
					}
				}
				// Push this mesh into the meshes vector
				meshes.push_back(myMesh);
			}

			#pragma endregion

			#pragma region Buffer Setup

			// Bed Buffers
			{
				// Find the mesh in the vector with the correct name
				unsigned int index = 0;
				FindMesh("Bed", index);

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
				bd.ByteWidth = sizeof(Vertex) * meshes[index].vertexList.size();

				// Create the buffer on the device
				D3D11_SUBRESOURCE_DATA InitData = {};
				InitData.pSysMem = meshes[index].vertexList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].vertexBuffer);


				// Index Buffer

				// Create index buffer
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(int) * meshes[index].indicesList.size();
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;
				InitData.pSysMem = meshes[index].indicesList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].indexBuffer);

				// Resource View

				hr = CreateDDSTextureFromFile(myDevice, L"./Assets/Textures/bed.dds", nullptr, &meshes[index].textureRV);
			}

			// Grid Buffers
			{
				// Find the mesh in the vector with the correct name
				unsigned int index = 0;
				FindMesh("Grid", index);

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
				bd.ByteWidth = sizeof(Vertex) * meshes[index].vertexList.size();

				// Create the buffer on the device
				D3D11_SUBRESOURCE_DATA InitData = {};
				InitData.pSysMem = meshes[index].vertexList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].vertexBuffer);

			}

			// Lantern Buffers
			{
				// Find the mesh in the vector with the correct name
				unsigned int index = 0;
				FindMesh("Lantern", index);

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
				bd.ByteWidth = sizeof(Vertex) * meshes[index].vertexList.size();

				// Create the buffer on the device
				D3D11_SUBRESOURCE_DATA InitData = {};
				InitData.pSysMem = meshes[1].vertexList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].vertexBuffer);


				// Index Buffer

				// Create index buffer
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(int) * meshes[index].indicesList.size();
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;
				InitData.pSysMem = meshes[1].indicesList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].indexBuffer);
			}

			// Fan Buffers
			{
				// Find the mesh in the vector with the correct name
				unsigned int index = 0;
				FindMesh("Fan", index);

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
				bd.ByteWidth = sizeof(Vertex) * meshes[index].vertexList.size();

				// Create the buffer on the device
				D3D11_SUBRESOURCE_DATA InitData = {};
				InitData.pSysMem = meshes[index].vertexList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].vertexBuffer);


				// Index Buffer

				// Create index buffer
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(int) * meshes[index].indicesList.size();
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;
				InitData.pSysMem = meshes[index].indicesList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].indexBuffer);

				// Resource View

				hr = CreateDDSTextureFromFile(myDevice, L"./Assets/Textures/fan.dds", nullptr, &meshes[index].textureRV);
			}

			// Cube Buffers
			{
				// Find the mesh in the vector with the correct name
				unsigned int index = 0;
				FindMesh("Cube", index);

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
				bd.ByteWidth = sizeof(Vertex) * meshes[index].vertexList.size();

				// Create the buffer on the device
				D3D11_SUBRESOURCE_DATA InitData = {};
				InitData.pSysMem = meshes[index].vertexList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].vertexBuffer);


				// Index Buffer

				// Create index buffer
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(int) * meshes[index].indicesList.size();
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;
				InitData.pSysMem = meshes[index].indicesList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].indexBuffer);

				// Resource View

				hr = CreateDDSTextureFromFile(myDevice, L"./Assets/Textures/Crate.dds", nullptr, &meshes[index].textureRV);
			}

			// Cube instance buffer
			{
				// This creates an empty buffer description object
				D3D11_BUFFER_DESC bd = {};
				bd.ByteWidth = sizeof(InstanceData) * INSTANCECOUNT;
				bd.Usage = D3D11_USAGE_IMMUTABLE;
				bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				bd.CPUAccessFlags = 0;
				bd.StructureByteStride = sizeof(InstanceData);

				D3D11_SUBRESOURCE_DATA InitData = {};

				float yVal = -5.0f;
				unsigned int iMod = 0;
				for (int i = 0; i < 3; i++)
				{
					perInstanceData[0 + iMod] = { XMFLOAT4(15.0f, yVal, -5.0f, 1.0f) };
					perInstanceData[1 + iMod] = { XMFLOAT4(15.0f, yVal, 0.0f, 1.0f) };
					perInstanceData[2 + iMod] = { XMFLOAT4(15.0f, yVal, 5.0f, 1.0f) };
					perInstanceData[3 + iMod] = { XMFLOAT4(25.0f, yVal, -5.0f, 1.0f) };
					perInstanceData[4 + iMod] = { XMFLOAT4(25.0f, yVal, 0.0f, 1.0f) };
					perInstanceData[5 + iMod] = { XMFLOAT4(25.0f, yVal, 5.0f, 1.0f) };
					perInstanceData[6 + iMod] = { XMFLOAT4(20.0f, yVal, -5.0f, 1.0f) };
					perInstanceData[7 + iMod] = { XMFLOAT4(20.0f, yVal, 0.0f, 1.0f) };
					perInstanceData[8 + iMod] = { XMFLOAT4(20.0f, yVal, 5.0f, 1.0f) };
					yVal += 5.0f;
					iMod += 9;
				}

				InitData.pSysMem = perInstanceData;
				myDevice->CreateBuffer(&bd, &InitData, &cubeInstanceBuffer);
			}

			// Test Scene Skybox Cube Buffers
			{
				// Find the mesh in the vector with the correct name
				unsigned int index = 0;
				FindMesh("TestSkyCube", index);

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
				bd.ByteWidth = sizeof(Vertex) * meshes[index].vertexList.size();

				// Create the buffer on the device
				D3D11_SUBRESOURCE_DATA InitData = {};
				InitData.pSysMem = meshes[index].vertexList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].vertexBuffer);


				// Index Buffer

				// Create index buffer
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(int) * meshes[index].indicesList.size();
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;
				InitData.pSysMem = meshes[index].indicesList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].indexBuffer);

				// Resource View

				hr = CreateDDSTextureFromFile(myDevice, L"./Assets/Textures/oceanSkybox.dds", nullptr, &meshes[index].textureRV);
			}

			// Space Scene Skybox Cube Buffers
			{
				// Find the mesh in the vector with the correct name
				unsigned int index = 0;
				FindMesh("SpaceSkyCube", index);

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
				bd.ByteWidth = sizeof(Vertex) * meshes[index].vertexList.size();

				// Create the buffer on the device
				D3D11_SUBRESOURCE_DATA InitData = {};
				InitData.pSysMem = meshes[index].vertexList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].vertexBuffer);


				// Index Buffer

				// Create index buffer
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(int) * meshes[index].indicesList.size();
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;
				InitData.pSysMem = meshes[index].indicesList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].indexBuffer);

				// Resource View

				hr = CreateDDSTextureFromFile(myDevice, L"./Assets/Textures/spaceSkybox.dds", nullptr, &meshes[index].textureRV);
			}

			// Chest Buffers
			{
				// Find the mesh in the vector with the correct name
				unsigned int index = 0;
				FindMesh("Chest", index);

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
				bd.ByteWidth = sizeof(Vertex) * meshes[index].vertexList.size();

				// Create the buffer on the device
				D3D11_SUBRESOURCE_DATA InitData = {};
				InitData.pSysMem = meshes[index].vertexList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].vertexBuffer);


				// Index Buffer

				// Create index buffer
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(int) * meshes[index].indicesList.size();
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;
				InitData.pSysMem = meshes[index].indicesList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].indexBuffer);

				// Resource View

				hr = CreateDDSTextureFromFile(myDevice, L"./Assets/Textures/treasureChest.dds", nullptr, &meshes[index].textureRV);
			}

			// Point Grid Buffers
			{
				// Find the mesh in the vector with the correct name
				unsigned int index = 0;
				FindMesh("PointGrid", index);

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
				bd.ByteWidth = sizeof(Vertex) * meshes[index].vertexList.size();

				// Create the buffer on the device
				D3D11_SUBRESOURCE_DATA InitData = {};
				InitData.pSysMem = meshes[index].vertexList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].vertexBuffer);

			}

			// Planet Buffers
			{
				// Find the mesh in the vector with the correct name
				unsigned int index = 0;
				FindMesh("Planet", index);

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
				bd.ByteWidth = sizeof(Vertex) * meshes[index].vertexList.size();

				// Create the buffer on the device
				D3D11_SUBRESOURCE_DATA InitData = {};
				InitData.pSysMem = meshes[index].vertexList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].vertexBuffer);


				// Index Buffer

				// Create index buffer
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(int) * meshes[index].indicesList.size();
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;
				InitData.pSysMem = meshes[index].indicesList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].indexBuffer);

				// Resource Views for all planet textures, to be set before drawing

				ID3D11ShaderResourceView* tempRV = nullptr;
				hr = CreateDDSTextureFromFile(myDevice, L"./Assets/Textures/sunmap.dds", nullptr, &tempRV);
				planetTextures.push_back(tempRV);
				hr = CreateDDSTextureFromFile(myDevice, L"./Assets/Textures/mercurymap.dds", nullptr, &tempRV);
				planetTextures.push_back(tempRV);
				hr = CreateDDSTextureFromFile(myDevice, L"./Assets/Textures/venusmap.dds", nullptr, &tempRV);
				planetTextures.push_back(tempRV);
				hr = CreateDDSTextureFromFile(myDevice, L"./Assets/Textures/earthmap.dds", nullptr, &tempRV);
				planetTextures.push_back(tempRV);
				hr = CreateDDSTextureFromFile(myDevice, L"./Assets/Textures/moonmap.dds", nullptr, &tempRV);
				planetTextures.push_back(tempRV);
				hr = CreateDDSTextureFromFile(myDevice, L"./Assets/Textures/marsmap.dds", nullptr, &tempRV);
				planetTextures.push_back(tempRV);
				hr = CreateDDSTextureFromFile(myDevice, L"./Assets/Textures/jupitermap.dds", nullptr, &tempRV);
				planetTextures.push_back(tempRV);
				hr = CreateDDSTextureFromFile(myDevice, L"./Assets/Textures/saturnmap.dds", nullptr, &tempRV);
				planetTextures.push_back(tempRV);
				hr = CreateDDSTextureFromFile(myDevice, L"./Assets/Textures/uranusmap.dds", nullptr, &tempRV);
				planetTextures.push_back(tempRV);
				hr = CreateDDSTextureFromFile(myDevice, L"./Assets/Textures/neptunemap.dds", nullptr, &tempRV);
				planetTextures.push_back(tempRV);
				hr = CreateDDSTextureFromFile(myDevice, L"./Assets/Textures/plutomap.dds", nullptr, &tempRV);
				planetTextures.push_back(tempRV);
			}

			// Chest Buffers
			{
				// Find the mesh in the vector with the correct name
				unsigned int index = 0;
				FindMesh("Ship", index);

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
				bd.ByteWidth = sizeof(Vertex) * meshes[index].vertexList.size();

				// Create the buffer on the device
				D3D11_SUBRESOURCE_DATA InitData = {};
				InitData.pSysMem = meshes[index].vertexList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].vertexBuffer);


				// Index Buffer

				// Create index buffer
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(int) * meshes[index].indicesList.size();
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;
				InitData.pSysMem = meshes[index].indicesList.data();
				hr = myDevice->CreateBuffer(&bd, &InitData, &meshes[index].indexBuffer);

				// Resource View

				hr = CreateDDSTextureFromFile(myDevice, L"./Assets/Textures/talonShip.dds", nullptr, &meshes[index].textureRV);
			}

			#pragma endregion

			#pragma region Vertex Shaders

			myDevice->CreateVertexShader(VS_Default, sizeof(VS_Default), nullptr, &vsDefault);
			myDevice->CreateVertexShader(VS_Waves, sizeof(VS_Waves), nullptr, &vsWaves);
			myDevice->CreateVertexShader(VS_Instancing, sizeof(VS_Instancing), nullptr, &vsInstancing);

			#pragma endregion

			#pragma region Pixel Shaders

			myDevice->CreatePixelShader(PS_Default, sizeof(PS_Default), nullptr, &psDefault);
			myDevice->CreatePixelShader(PS_SolidColor, sizeof(PS_SolidColor), nullptr, &psSolidColor);
			myDevice->CreatePixelShader(PS_CustomWaves, sizeof(PS_CustomWaves), nullptr, &psCustomWaves);
			myDevice->CreatePixelShader(PS_SkyBox, sizeof(PS_SkyBox), nullptr, &psSkyBox);

			#pragma endregion

			#pragma region Geometry Shaders

			myDevice->CreateGeometryShader(GS_Test, sizeof(GS_Test), nullptr, &gsTest);

			#pragma endregion

			// Sample state
			{
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

			// Input Layout
			{
				// Create the input layout
				const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "INSTANCEDATA", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
				};
				// Grab the size of the layout dynamically, in case it's changed. This size is required to create the layout on the device
				UINT elements = ARRAYSIZE(inputElementDesc);

				// Create layout on the device
				hr = myDevice->CreateInputLayout(inputElementDesc, elements, VS_Default, sizeof(VS_Default), &myVertexLayout);

				// Set layout in the context
				myContext->IASetInputLayout(myVertexLayout);
			}

			#pragma region Constant Buffer

			D3D11_BUFFER_DESC bd = {};
			// Create the constant buffer
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(ConstantBuffer);
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			hr = myDevice->CreateBuffer(&bd, nullptr, &myConstantBuffer);

			#pragma endregion

			// Set viewports
			{
				// Large viewport

				// Initialize the view matrix
				XMMATRIX transl = XMMatrixTranslation(0.0f, 3.0f, -10.0f);
				//testSceneViewMatrix = XMMatrixLookAtLH(Eye, At, Up);
				testSceneViewMatrix = XMMatrixMultiply(XMMatrixIdentity(), transl);

				// Initialize the projection matrix
				UINT height;
				UINT width;
				attatchPoint->GetClientHeight(height);
				attatchPoint->GetClientWidth(width);
				vp1ProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);

				// Viewport structure constructed by topLeftX, topLeftY, width, height, minDepth, maxDepth
				D3D11_VIEWPORT largeViewport = {0.0f, 0.0f, width, height, 0.5f, 1.0f};


				// Small viewport

				float vp2Height = height / 4.0f;
				float vp2Width = width / 4.0f;

				// Viewport structure constructed by topLeftX, topLeftY, width, height, minDepth, maxDepth
				D3D11_VIEWPORT smallViewport = { 5.0f, 5.0f, vp2Width, vp2Height, 0.0f, 0.5f };
				
				// Initialize the view matrix
				spaceSceneViewMatrix = XMMatrixMultiply(XMMatrixIdentity(), XMMatrixRotationX(0.7f));
				spaceSceneViewMatrix = XMMatrixMultiply(spaceSceneViewMatrix, XMMatrixTranslation(0.0f, 25.0f, -25.0f));

				// Initialize the projection matrix
				vp2ProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, vp2Width / vp2Height, 0.01f, 100.0f);;

				viewPorts = new D3D11_VIEWPORT[2];
				viewPorts[0] = largeViewport;
				viewPorts[1] = smallViewport;
			}

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

	// Release buffers
	if (myConstantBuffer)
		myConstantBuffer->Release();
	if (cubeInstanceBuffer)
		cubeInstanceBuffer->Release();
	
	for each (Mesh var in meshes)
	{
		if (var.indexBuffer)
			var.indexBuffer->Release();
		if (var.vertexBuffer)
			var.vertexBuffer->Release();
		if (var.textureRV)
			var.textureRV->Release();
	}

	// Release Shaders
	if (vsDefault)
		vsDefault->Release();
	if (vsWaves)
		vsWaves->Release();
	if (vsInstancing)
		vsInstancing->Release();

	if (psDefault)
		psDefault->Release();
	if (psSolidColor)
		psSolidColor->Release();
	if (psCustomWaves)
		psCustomWaves->Release();
	if (psSkyBox)
		psSkyBox->Release();
	
	if (gsTest)
		gsTest->Release();
	
	// Release other stuff
	if (myVertexLayout)
		myVertexLayout->Release();
	if (mySamplerLinear)
		mySamplerLinear->Release();
	
	delete[] viewPorts;
	planetTextures[0]->Release();
	planetTextures[1]->Release();
	planetTextures[2]->Release();
	planetTextures[3]->Release();
	planetTextures[4]->Release();
	planetTextures[5]->Release();
	planetTextures[6]->Release();
	planetTextures[7]->Release();
	planetTextures[8]->Release();
	planetTextures[9]->Release();
	planetTextures[10]->Release();

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

			// Unsigned int used to point to meshes in the vector of meshes
			unsigned int meshIndex = 0;

			// Set the proper viewport
			myContext->RSSetViewports(1, &viewPorts[testSceneVp]);

			// Modify the view matrix based on user input
			{
				// Setup values
				POINT cursorPos;
				GetCursorPos(&cursorPos);
				float xDiff = oldCursorPos.x - cursorPos.x;
				float yDiff = oldCursorPos.y - cursorPos.y;
				oldCursorPos = cursorPos;
				// Input delay to prevent asyncKeyState from spamming inputs. Primitive, but it works
				if (inputDelay > 0)
				{
					inputDelay -= 1;
				}
				// Escape locks the camera
				if (GetAsyncKeyState(VK_ESCAPE) && inputDelay == 0)
				{
					cameraPaused = !cameraPaused;
					inputDelay = 250;
				}
				// G activates or deactivates the geometry shader
				if (GetAsyncKeyState('G') && inputDelay == 0)
				{
					testGeoShader = !testGeoShader;
					inputDelay = 250;
				}
				if (GetAsyncKeyState('C') && inputDelay == 0)
				{
					if (testSceneVp == 0)
					{
						testSceneVp = 1;
						spaceSceneVp = 0;
					}
					else
					{
testSceneVp = 0;
spaceSceneVp = 1;
					}
					inputDelay = 250;
				}

				// Set which camera is moving
				XMMATRIX mainCamera;
				if (testSceneVp == 0)
					mainCamera = testSceneViewMatrix;
				else
					mainCamera = spaceSceneViewMatrix;

				// Planet lookat controls
				if (inputDelay == 0)
				{
					if (GetAsyncKeyState('1'))
					{
						if (planetLookAt != 1)
							planetLookAt = 1;
						else
							planetLookAt = 0;
						inputDelay = 250;
					}
					if (GetAsyncKeyState('2'))
					{
						if (planetLookAt != 2)
							planetLookAt = 2;
						else
							planetLookAt = 0;
						inputDelay = 250;
					}
					if (GetAsyncKeyState('3'))
					{
						if (planetLookAt != 3)
							planetLookAt = 3;
						else
							planetLookAt = 0;
						inputDelay = 250;
					}
					if (GetAsyncKeyState('4'))
					{
						if (planetLookAt != 4)
							planetLookAt = 4;
						else
							planetLookAt = 0;
						inputDelay = 250;
					}
					if (GetAsyncKeyState('5'))
					{
						if (planetLookAt != 5)
							planetLookAt = 5;
						else
							planetLookAt = 0;
						inputDelay = 250;
					}
					if (GetAsyncKeyState('6'))
					{
						if (planetLookAt != 6)
							planetLookAt = 6;
						else
							planetLookAt = 0;
						inputDelay = 250;
					}
					if (GetAsyncKeyState('7'))
					{
						if (planetLookAt != 7)
							planetLookAt = 7;
						else
							planetLookAt = 0;
						inputDelay = 250;
					}
					if (GetAsyncKeyState('8'))
					{
						if (planetLookAt != 8)
							planetLookAt = 8;
						else
							planetLookAt = 0;
						inputDelay = 250;
					}
					if (GetAsyncKeyState('9'))
					{
						if (planetLookAt != 9)
							planetLookAt = 9;
						else
							planetLookAt = 0;
						inputDelay = 250;
					}
					if (GetAsyncKeyState('0'))
					{
						if (planetLookAt != 10)
							planetLookAt = 10;
						else
							planetLookAt = 0;
						inputDelay = 250;
					}
				}

				// Camera controls, so long as the camera is not locked
				if (!cameraPaused)
				{
					// R resets the camera to a default position
					if (GetAsyncKeyState('R'))
					{
						// Initialize the view matrix
						XMMATRIX transl = XMMatrixTranslation(0.0f, 3.0f, -10.0f);
						mainCamera = XMMatrixMultiply(XMMatrixIdentity(), transl);
					}
					// Move to the left using A
					if (GetAsyncKeyState('A'))
					{
						XMMATRIX transl = XMMatrixTranslation(-moveSpeed, 0.0f, 0.0f);
						mainCamera = XMMatrixMultiply(transl, mainCamera);
					}
					// Move to the right using D
					if (GetAsyncKeyState('D'))
					{
						XMMATRIX transl = XMMatrixTranslation(moveSpeed, 0.0f, 0.0f);
						mainCamera = XMMatrixMultiply(transl, mainCamera);
					}
					// Move forward using W
					if (GetAsyncKeyState('W'))
					{
						XMMATRIX transl = XMMatrixTranslation(0.0f, 0.0f, moveSpeed);
						mainCamera = XMMatrixMultiply(transl, mainCamera);
					}
					// Move backward using S
					if (GetAsyncKeyState('S'))
					{
						XMMATRIX transl = XMMatrixTranslation(0.0f, 0.0f, -moveSpeed);
						mainCamera = XMMatrixMultiply(transl, mainCamera);
					}
					// Move up using SPACE
					if (GetAsyncKeyState(VK_SPACE))
					{
						XMMATRIX transl = XMMatrixTranslation(0.0f, moveSpeed, 0.0f);
						mainCamera = XMMatrixMultiply(transl, mainCamera);
					}
					// Move down using CTRL
					if (GetAsyncKeyState(VK_CONTROL))
					{
						XMMATRIX transl = XMMatrixTranslation(0.0f, -moveSpeed, 0.0f);
						mainCamera = XMMatrixMultiply(transl, mainCamera);
					}
					// Rotate based on cursor position
					if (xDiff != 0)
					{
						// Save original location
						XMMATRIX old = mainCamera;
						// Prepare y rotation matrix
						XMMATRIX transl = XMMatrixRotationY(-xDiff * lookSpeed);
						// Apply y rotation
						mainCamera = XMMatrixMultiply(mainCamera, transl);
						// Move back to saved position
						mainCamera.r[3].m128_f32[0] = old.r[3].m128_f32[0];
						mainCamera.r[3].m128_f32[1] = old.r[3].m128_f32[1];
						mainCamera.r[3].m128_f32[2] = old.r[3].m128_f32[2];
					}
					if (yDiff != 0)
					{
						// Save original location
						XMMATRIX old = mainCamera;
						// Prepare y rotation matrix
						XMMATRIX transl = XMMatrixRotationX(-yDiff * lookSpeed);
						// Place matrix at origin
						mainCamera.r[3].m128_f32[0] = 0.0f;
						mainCamera.r[3].m128_f32[1] = 0.0f;
						mainCamera.r[3].m128_f32[2] = 0.0f;
						// Mutiply matrices in reverse order
						mainCamera = XMMatrixMultiply(transl, mainCamera);
						// Move back to saved position
						mainCamera.r[3].m128_f32[0] = old.r[3].m128_f32[0];
						mainCamera.r[3].m128_f32[1] = old.r[3].m128_f32[1];
						mainCamera.r[3].m128_f32[2] = old.r[3].m128_f32[2];
					}
				}

				// Send the modified camera data back out to the respective view matrix
				if (testSceneVp == 0)
					testSceneViewMatrix = mainCamera;
				else
					spaceSceneViewMatrix = mainCamera;
			}

			// Set up initial constant buffer values
			ConstantBuffer cb1;
			cb1.time = XMFLOAT2(t, t);
			cb1.mView = XMMatrixTranspose(XMMatrixInverse(nullptr,testSceneViewMatrix));
			cb1.mProjection = XMMatrixTranspose(vp1ProjectionMatrix);
			// Dir light 1
			cb1.dirLights[0].col = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
			cb1.dirLights[0].dir = XMFLOAT4(-1.0f, 0.75f, 0.0f, 1.0f);
			// Point light 1
			cb1.pointLights[0].col = XMFLOAT4(0.7f, 0.5f, 0.0f, 1.0f);
			cb1.pointLights[0].pos = XMFLOAT4(3.5f, 0.0f, 0.0f, 1.0f);
			cb1.pointLights[0].rad = XMFLOAT4(5.0f, 0.0f, 0.0f, 1.0f);
			// Point light 2
			cb1.pointLights[1].col = XMFLOAT4(0.0f, 0.7f, 0.0f, 1.0f);
			cb1.pointLights[1].pos = XMFLOAT4(0.0f, 2.5f, 0.0f, 1.0f);
			cb1.pointLights[1].rad = XMFLOAT4(10.0f, 0.0f, 0.0f, 1.0f);

			// Set the proper viewport
			myContext->RSSetViewports(1, &viewPorts[0]);
			if (testSceneVp == 0)
			{
				DrawTestScene(cb1);
				myContext->RSSetViewports(1, &viewPorts[1]);
				DrawSpaceScene(cb1);
			}
			else
			{
				DrawSpaceScene(cb1);
				myContext->RSSetViewports(1, &viewPorts[1]);
				DrawTestScene(cb1);
			}

			// Present Backbuffer using Swapchain object
			// Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.
			mySwapChain->Present(0, 0); // set first argument to 1 to enable vertical refresh sync with display

			// Free any temp DX handles aquired this frame
			myRenderTargetView->Release();
		}
	}
}

// Loads a mesh from a given file path
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

// Finds a mesh in the Mesh vector "meshes"
void LetsDrawSomeStuff::FindMesh(const char* meshName, unsigned int& index)
{
	index = 0;
	for each (Mesh var in meshes)
	{
		if (var.name == meshName)
		{
			break;
		}
		index++;
	}
}

// Calls DrawIndexed with given parameters
void LetsDrawSomeStuff::BasicDrawIndexed(unsigned int index, ConstantBuffer cb, D3D_PRIMITIVE_TOPOLOGY topology, ID3D11VertexShader* vShader, ID3D11PixelShader* pShader, ID3D11GeometryShader* gShader)
{
	// Set vertex buffer in the context
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	myContext->IASetVertexBuffers(0, 1, &meshes[index].vertexBuffer, &stride, &offset);

	// Set index buffer
	myContext->IASetIndexBuffer(meshes[index].indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set primitive topology
	myContext->IASetPrimitiveTopology(topology);

	// Update constant buffer
	cb.mWorld = XMMatrixTranspose(meshes[index].mWorld);
	// Send updated constant buffer
	myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);

	// Setup shaders
	myContext->VSSetShader(vShader, nullptr, 0);
	myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
	myContext->PSSetShader(pShader, nullptr, 0);
	myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer);
	myContext->PSSetShaderResources(0, 1, &meshes[index].textureRV);
	myContext->PSSetSamplers(0, 1, &mySamplerLinear);
	myContext->GSSetShader(gShader, nullptr, 0);
	myContext->GSSetConstantBuffers(0, 1, &myConstantBuffer);

	// Draw indexed object
	myContext->DrawIndexed((UINT)meshes[index].indicesList.size(), 0, 0);
}

// Calls Draw with given parameters
void LetsDrawSomeStuff::BasicDraw(unsigned int index, ConstantBuffer cb, D3D_PRIMITIVE_TOPOLOGY topology, ID3D11VertexShader* vShader, ID3D11PixelShader* pShader, ID3D11GeometryShader* gShader)
{
	// Set vertex buffer in the context
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	myContext->IASetVertexBuffers(0, 1, &meshes[index].vertexBuffer, &stride, &offset);

	// Set primitive topology
	myContext->IASetPrimitiveTopology(topology);

	// Set index buffer in the context
	myContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);

	// Update constant buffer
	cb.mWorld = XMMatrixTranspose(meshes[index].mWorld);
	// Send updated constant buffer
	myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);

	// Setup to render normal object
	myContext->VSSetShader(vShader, nullptr, 0);
	myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
	myContext->PSSetShader(pShader, nullptr, 0);
	myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer);
	myContext->PSSetShaderResources(0, 1, &meshes[index].textureRV);
	myContext->PSSetSamplers(0, 1, &mySamplerLinear);
	myContext->GSSetShader(gShader, nullptr, 0);
	myContext->GSSetConstantBuffers(0, 1, &myConstantBuffer);

	// Draw indexed object
	myContext->Draw((UINT)meshes[index].vertexList.size(), 0);
}

void LetsDrawSomeStuff::DrawTestScene(ConstantBuffer& cb1)
{
	// Set matrices to test scene matrices
	cb1.mView = XMMatrixTranspose(XMMatrixInverse(nullptr, testSceneViewMatrix));
	cb1.mProjection = XMMatrixTranspose(vp1ProjectionMatrix);

	// Move lights
	{
		cb1.dirLights[0].col = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
		cb1.dirLights[0].dir = XMFLOAT4(-1.0f, 0.75f, 0.0f, 1.0f);

		// Establish how many lights to render in this scene
		cb1.useDirLights = XMFLOAT2(1.0f, 0.0f);
		cb1.usePointLights = XMFLOAT2(0.0f, 2.0f);
		cb1.useSpotLights = XMFLOAT2(0.0f, 3.0f);

		// Spot light 1
		cb1.spotLights[0].col = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		cb1.spotLights[0].pos = XMFLOAT4(-42.0f, 10.0f, 0.0f, 1.0f);
		cb1.spotLights[0].dir = XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f);
		cb1.spotLights[0].rat = XMFLOAT4(fabsf(sinf(cb1.time.x)), 0.5f, 0.5f, 1.0f);
		// Spot light 2
		cb1.spotLights[1].col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		cb1.spotLights[1].pos = XMFLOAT4(-42.0f, 10.0f, 0.0f, 1.0f);
		cb1.spotLights[1].dir = XMFLOAT4(0.0f, -fabsf(sinf(cb1.time.x)), cosf(cb1.time.x), 1.0f);
		cb1.spotLights[1].rat = XMFLOAT4(0.9f, 0.5f, 0.5f, 1.0f);
		// Spot light 3
		cb1.spotLights[2].col = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		cb1.spotLights[2].pos = XMFLOAT4((cosf(cb1.time.x * 1.7f) * 12.0f) - 42.0f, 10.0f, (cosf(cb1.time.x * 1.7f) * 12.0f), 1.0f);
		cb1.spotLights[2].dir = XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f);
		cb1.spotLights[2].rat = XMFLOAT4(0.9f, 0.5f, 0.5f, 1.0f);
		
		// Rotate the yellow point light around the origin
		XMMATRIX mModify = XMMatrixRotationY(-2.0f * cb1.time.x);
		XMVECTOR vLightDir = XMLoadFloat4(&cb1.pointLights[0].pos);
		vLightDir = XMVector3Transform(vLightDir, mModify);
		XMStoreFloat4(&cb1.pointLights[0].pos, vLightDir);

		// Move the blue point light back and forth
		mModify = XMMatrixTranslation(0.0f, 0.0f, sinf(cb1.time.x) * 10.0f);
		vLightDir = XMLoadFloat4(&cb1.pointLights[1].pos);
		vLightDir = XMVector3Transform(vLightDir, mModify);
		XMStoreFloat4(&cb1.pointLights[1].pos, vLightDir);
	}

	unsigned int meshIndex = 0;

	// Draw skybox
	{
		float skyboxScale = 1.0f;
		// Find the test scene skybox cube
		FindMesh("TestSkyCube", meshIndex);
		// Set that cube's location to the location of the camera
		meshes[meshIndex].mWorld = XMMatrixMultiply(XMMatrixIdentity(), XMMatrixScaling(skyboxScale, skyboxScale, skyboxScale));
		meshes[meshIndex].mWorld.r[3].m128_f32[0] = testSceneViewMatrix.r[3].m128_f32[0];
		meshes[meshIndex].mWorld.r[3].m128_f32[1] = testSceneViewMatrix.r[3].m128_f32[1];
		meshes[meshIndex].mWorld.r[3].m128_f32[2] = testSceneViewMatrix.r[3].m128_f32[2];
		// Set vertex buffer in the context
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		myContext->IASetVertexBuffers(0, 1, &meshes[meshIndex].vertexBuffer, &stride, &offset);

		// Set index buffer
		myContext->IASetIndexBuffer(meshes[meshIndex].indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set primitive topology
		myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Update constant buffer
		cb1.mWorld = XMMatrixTranspose(meshes[meshIndex].mWorld);
		// Send updated constant buffer
		myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb1, 0, 0);

		// Setup shaders
		myContext->VSSetShader(vsDefault, nullptr, 0);
		myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
		myContext->PSSetShader(psSkyBox, nullptr, 0);
		myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer);
		myContext->PSSetShaderResources(1, 1, &meshes[meshIndex].textureRV);
		myContext->PSSetSamplers(0, 1, &mySamplerLinear);
		myContext->GSSetShader(nullptr, nullptr, 0);
		myContext->GSSetConstantBuffers(0, 1, &myConstantBuffer);

		// Draw indexed object
		myContext->DrawIndexed((UINT)meshes[meshIndex].indicesList.size(), 0, 0);

		ID3D11DepthStencilView* myDepthStencilView = nullptr;
		// Grab the Z Buffer if one was requested
		if (G_SUCCESS(mySurface->GetDepthStencilView((void**)&myDepthStencilView)))
		{
			myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.
			myDepthStencilView->Release();
		}
	}

	// Drawing the grid
	{
		FindMesh("Grid", meshIndex);
		cb1.solidColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		BasicDraw(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_LINELIST, vsWaves, psSolidColor, nullptr);
	}

	// Drawing Lanterns
	{
		FindMesh("Lantern", meshIndex);
		// Lantern 1
		XMMATRIX mLight = XMMatrixTranslationFromVector(5.0f * XMLoadFloat4(&cb1.pointLights[0].pos));
		meshes[meshIndex].mWorld = XMMatrixMultiply(mLight, XMMatrixScaling(0.2f, 0.2f, 0.2f));
		cb1.solidColor = cb1.pointLights[0].col;
		BasicDrawIndexed(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vsDefault, psSolidColor, nullptr);
		// Lantern 2
		mLight = XMMatrixTranslationFromVector(5.0f * XMLoadFloat4(&cb1.pointLights[1].pos));
		meshes[meshIndex].mWorld = XMMatrixMultiply(mLight, XMMatrixScaling(0.2f, 0.2f, 0.2f));
		cb1.solidColor = cb1.pointLights[1].col;
		BasicDrawIndexed(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vsDefault, psSolidColor, nullptr);
	}

	// Drawing the bed
	{
		FindMesh("Bed", meshIndex);
		BasicDrawIndexed(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vsDefault, psDefault, nullptr);
	}

	// Drawing the chest
	{
		FindMesh("Chest", meshIndex);
		meshes[meshIndex].mWorld = XMMatrixTranslation(0.0f, 0.0f, 6.0f);
		BasicDrawIndexed(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vsDefault, psDefault, nullptr);
	}

	// Drawing the special cube
	{
		FindMesh("Cube", meshIndex);
		meshes[meshIndex].mWorld = XMMatrixMultiply(XMMatrixScaling(0.1f, 0.1f, 0.1f), XMMatrixRotationY(cb1.time.x));
		meshes[meshIndex].mWorld = XMMatrixMultiply(meshes[meshIndex].mWorld, XMMatrixTranslation(-7.0f, 0.0f, sinf(cb1.time.x * 1.337) * 5.0f));
		BasicDrawIndexed(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vsDefault, psCustomWaves, nullptr);
	}

	// Drawing special cube instances
	{
		FindMesh("Cube", meshIndex);
		// Set up the vertex buffers. We have 2 streams:
		// Stream 1 contains per-primitive vertices defining the cubes.
		// Stream 2 contains the per-instance data for scale, position and orientation
		UINT Strides[] = { sizeof(Vertex), sizeof(InstanceData) };
		UINT Offsets[] = { 0, 0 };
		ID3D11Buffer* Buffers[] = { meshes[meshIndex].vertexBuffer, cubeInstanceBuffer };
		myContext->IASetVertexBuffers(0, _countof(Strides), Buffers, Strides, Offsets);

		myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		myContext->IASetIndexBuffer(meshes[meshIndex].indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		myContext->VSSetShader(vsInstancing, nullptr, 0);
		myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
		myContext->PSSetShader(psCustomWaves, nullptr, 0);
		myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer);
		myContext->PSSetShaderResources(0, 1, &meshes[meshIndex].textureRV);
		myContext->PSSetSamplers(0, 1, &mySamplerLinear);
		myContext->GSSetShader(nullptr, nullptr, 0);
		myContext->GSSetConstantBuffers(0, 1, &myConstantBuffer);

		myContext->DrawIndexedInstanced(meshes[meshIndex].indicesList.size(), INSTANCECOUNT, 0, 0, 0);
	}

	// Drawing the point grid as squares using the geometry shader
	{
		FindMesh("PointGrid", meshIndex);
		cb1.solidColor = XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f);
		// If the geometry shader is enabled, call draw with the geo shader
		if (testGeoShader)
			BasicDraw(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, vsDefault, psSolidColor, gsTest);
		// Otherwise, call it without
		else
			BasicDraw(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, vsDefault, psSolidColor, nullptr);
	}

	// Drawing a beeg cube
	{
		FindMesh("Cube", meshIndex);
		meshes[meshIndex].mWorld = XMMatrixMultiply(XMMatrixIdentity(), XMMatrixScaling(2.5f, 0.4f, 2.5f));
		meshes[meshIndex].mWorld = XMMatrixMultiply(meshes[meshIndex].mWorld, XMMatrixTranslation(-42.0f, -10.0f, 0.0f));
		BasicDrawIndexed(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vsDefault, psDefault, nullptr);
	}
}

void LetsDrawSomeStuff::DrawSpaceScene(ConstantBuffer& cb1)
{
	// Set matrices to space scene matrices
	cb1.mView = XMMatrixTranspose(XMMatrixInverse(nullptr, spaceSceneViewMatrix));
	cb1.mProjection = XMMatrixTranspose(vp2ProjectionMatrix);

	unsigned int meshIndex = 0;
	// Skybox for space viewport
	{
		float skyboxScale = 1.0f;
		cb1.mView = XMMatrixTranspose(XMMatrixInverse(nullptr, spaceSceneViewMatrix));
		cb1.mProjection = XMMatrixTranspose(vp2ProjectionMatrix);

		// Set the proper viewport
		myContext->RSSetViewports(1, &viewPorts[spaceSceneVp]);
		// Find the space scene skybox cube
		FindMesh("SpaceSkyCube", meshIndex);
		// Set that cube's location to the location of the camera
		meshes[meshIndex].mWorld = XMMatrixMultiply(XMMatrixIdentity(), XMMatrixScaling(skyboxScale, skyboxScale, skyboxScale));
		meshes[meshIndex].mWorld.r[3].m128_f32[0] = spaceSceneViewMatrix.r[3].m128_f32[0];
		meshes[meshIndex].mWorld.r[3].m128_f32[1] = spaceSceneViewMatrix.r[3].m128_f32[1];
		meshes[meshIndex].mWorld.r[3].m128_f32[2] = spaceSceneViewMatrix.r[3].m128_f32[2];
		// Set vertex buffer in the context
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		myContext->IASetVertexBuffers(0, 1, &meshes[meshIndex].vertexBuffer, &stride, &offset);

		// Set index buffer
		myContext->IASetIndexBuffer(meshes[meshIndex].indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set primitive topology
		myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Update constant buffer
		cb1.mWorld = XMMatrixTranspose(meshes[meshIndex].mWorld);
		// Send updated constant buffer
		myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb1, 0, 0);

		// Setup shaders
		myContext->VSSetShader(vsDefault, nullptr, 0);
		myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
		myContext->PSSetShader(psSkyBox, nullptr, 0);
		myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer);
		myContext->PSSetShaderResources(1, 1, &meshes[meshIndex].textureRV);
		myContext->PSSetSamplers(0, 1, &mySamplerLinear); 
		myContext->GSSetShader(nullptr, nullptr, 0);
		myContext->GSSetConstantBuffers(0, 1, &myConstantBuffer);

		// Draw indexed object
		myContext->DrawIndexed((UINT)meshes[meshIndex].indicesList.size(), 0, 0);

		ID3D11DepthStencilView* myDepthStencilView = nullptr;
		// Grab the Z Buffer if one was requested
		if (G_SUCCESS(mySurface->GetDepthStencilView((void**)&myDepthStencilView)))
		{
			myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.
			myDepthStencilView->Release();
		}
	}

	cb1.dirLights[0].col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	cb1.dirLights[0].dir = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	cb1.useDirLights = XMFLOAT2(1.0f, 0.0f);
	cb1.usePointLights = XMFLOAT2(0.0f, 0.0f);
	cb1.useSpotLights = XMFLOAT2(0.0f, 0.0f);

	// Lots of matrix math
	// The sun just has to rotate
	XMMATRIX sunWorld = XMMatrixMultiply(XMMatrixScaling(0.2f, 0.2f, 0.2f), XMMatrixRotationY(cb1.time.x));
	// Mercury just has to rotate and orbit the sun
	XMMATRIX mercuryWorld = XMMatrixMultiply(XMMatrixScaling(0.01f, 0.01f, 0.01f), XMMatrixRotationY(cb1.time.x * 0.5f));
	mercuryWorld = XMMatrixMultiply(mercuryWorld, XMMatrixTranslation(4.0f, 0.0f, 0.0f));
	mercuryWorld = XMMatrixMultiply(mercuryWorld, XMMatrixRotationY(0.0f - (cb1.time.x * 1.07f)));
	// Venus
	XMMATRIX venusWorld = XMMatrixMultiply(XMMatrixScaling(0.02f, 0.02f, 0.02f), XMMatrixRotationY(cb1.time.x * 0.5f));
	venusWorld = XMMatrixMultiply(venusWorld, XMMatrixTranslation(6.0f, 0.0f, 0.0f));
	venusWorld = XMMatrixMultiply(venusWorld, XMMatrixRotationY(0.0f - (cb1.time.x * 0.78f)));
	// Earth
	XMMATRIX earthWorld = XMMatrixMultiply(XMMatrixScaling(0.04f, 0.04f, 0.04f), XMMatrixRotationY(cb1.time.x));
	earthWorld = XMMatrixMultiply(earthWorld, XMMatrixTranslation(8.0f, 0.0f, 0.0f));
	earthWorld = XMMatrixMultiply(earthWorld, XMMatrixRotationY(0.0f - (cb1.time.x * 0.66f)));
	// Moon is very special
	XMMATRIX moonWorld = XMMatrixMultiply(XMMatrixScaling(0.005f, 0.005f, 0.005f), XMMatrixRotationY(cb1.time.x * 0.5f));
	moonWorld.r[3].m128_f32[0] = earthWorld.r[3].m128_f32[0] + (sinf(cb1.time.x) * 0.5f);
	moonWorld.r[3].m128_f32[1] = earthWorld.r[3].m128_f32[1] + (sinf(cb1.time.x) * 0.5f);
	moonWorld.r[3].m128_f32[2] = earthWorld.r[3].m128_f32[2] + (cosf(cb1.time.x) * 0.5f);
	// Mars
	XMMATRIX marsWorld = XMMatrixMultiply(XMMatrixScaling(0.03f, 0.03f, 0.03f), XMMatrixRotationY(cb1.time.x * 0.5f));
	marsWorld = XMMatrixMultiply(marsWorld, XMMatrixTranslation(11.0f, 0.0f, 0.0f));
	marsWorld = XMMatrixMultiply(marsWorld, XMMatrixRotationY(0.0f - (cb1.time.x * 0.53f)));
	// Jupiter
	XMMATRIX jupiterWorld = XMMatrixMultiply(XMMatrixScaling(0.1f, 0.1f, 0.1f), XMMatrixRotationY(cb1.time.x * 2.0f));
	jupiterWorld = XMMatrixMultiply(jupiterWorld, XMMatrixTranslation(15.0f, 0.0f, 0.0f));
	jupiterWorld = XMMatrixMultiply(jupiterWorld, XMMatrixRotationY(0.0f - (cb1.time.x * 0.29f)));
	// Saturn
	XMMATRIX saturnWorld = XMMatrixMultiply(XMMatrixScaling(0.08f, 0.08f, 0.08f), XMMatrixRotationY(cb1.time.x * 2.0f));
	saturnWorld = XMMatrixMultiply(saturnWorld, XMMatrixTranslation(19.0f, 0.0f, 0.0f));
	saturnWorld = XMMatrixMultiply(saturnWorld, XMMatrixRotationY(0.0f - (cb1.time.x * 0.21f)));
	// Uranus
	XMMATRIX uranusWorld = XMMatrixMultiply(XMMatrixScaling(0.065f, 0.065f, 0.065f), XMMatrixRotationY(cb1.time.x * 3.0f));
	uranusWorld = XMMatrixMultiply(uranusWorld, XMMatrixTranslation(23.0f, 0.0f, 0.0f));
	uranusWorld = XMMatrixMultiply(uranusWorld, XMMatrixRotationY(0.0f - (cb1.time.x * 0.15f)));
	// Neptune
	XMMATRIX neptuneWorld = XMMatrixMultiply(XMMatrixScaling(0.06f, 0.06f, 0.06f), XMMatrixRotationY(cb1.time.x * 3.0f));
	neptuneWorld = XMMatrixMultiply(neptuneWorld, XMMatrixTranslation(27.0f, 0.0f, 0.0f));
	neptuneWorld = XMMatrixMultiply(uranusWorld, XMMatrixRotationY(0.0f - (cb1.time.x * 0.12f)));
	// Pluto
	XMMATRIX plutoWorld = XMMatrixMultiply(XMMatrixScaling(0.01f, 0.01f, 0.01f), XMMatrixRotationY(cb1.time.x));
	plutoWorld = XMMatrixMultiply(plutoWorld, XMMatrixTranslation(31.0f, 0.0f, 0.0f));
	plutoWorld = XMMatrixMultiply(plutoWorld, XMMatrixRotationY(0.0f - (cb1.time.x * 0.1f)));

	switch (planetLookAt)
	{
	case 1:
		cb1.mView = XMMatrixTranspose(XMMatrixLookAtLH(spaceSceneViewMatrix.r[3], mercuryWorld.r[3], {0,1,0}));
		break;
	case 2:
		cb1.mView = XMMatrixTranspose(XMMatrixLookAtLH(spaceSceneViewMatrix.r[3], venusWorld.r[3], { 0,1,0 }));
		break;
	case 3:
		cb1.mView = XMMatrixTranspose(XMMatrixLookAtLH(spaceSceneViewMatrix.r[3], earthWorld.r[3], { 0,1,0 }));
		break;
	case 4:
		cb1.mView = XMMatrixTranspose(XMMatrixLookAtLH(spaceSceneViewMatrix.r[3], moonWorld.r[3], { 0,1,0 }));
		break;
	case 5:
		cb1.mView = XMMatrixTranspose(XMMatrixLookAtLH(spaceSceneViewMatrix.r[3], marsWorld.r[3], { 0,1,0 }));
		break;
	case 6:
		cb1.mView = XMMatrixTranspose(XMMatrixLookAtLH(spaceSceneViewMatrix.r[3], jupiterWorld.r[3], { 0,1,0 }));
		break;
	case 7:
		cb1.mView = XMMatrixTranspose(XMMatrixLookAtLH(spaceSceneViewMatrix.r[3], saturnWorld.r[3], { 0,1,0 }));
		break;
	case 8:
		cb1.mView = XMMatrixTranspose(XMMatrixLookAtLH(spaceSceneViewMatrix.r[3], uranusWorld.r[3], { 0,1,0 }));
		break;
	case 9:
		cb1.mView = XMMatrixTranspose(XMMatrixLookAtLH(spaceSceneViewMatrix.r[3], neptuneWorld.r[3], { 0,1,0 }));
		break;
	case 10:
		cb1.mView = XMMatrixTranspose(XMMatrixLookAtLH(spaceSceneViewMatrix.r[3], plutoWorld.r[3], { 0,1,0 }));
		break;
	default:
		break;
	}

	// Drawing the sun
	{
		FindMesh("Planet", meshIndex);
		meshes[meshIndex].textureRV = planetTextures[0];
		meshes[meshIndex].mWorld = sunWorld;
		BasicDrawIndexed(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vsDefault, psDefault, nullptr);
	}

	// Drawing Mercury
	{
		FindMesh("Planet", meshIndex);
		meshes[meshIndex].textureRV = planetTextures[1];
		meshes[meshIndex].mWorld = mercuryWorld;
		BasicDrawIndexed(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vsDefault, psDefault, nullptr);
	}

	// Drawing Venus
	{
		FindMesh("Planet", meshIndex);
		meshes[meshIndex].textureRV = planetTextures[2];
		meshes[meshIndex].mWorld = venusWorld;
		BasicDrawIndexed(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vsDefault, psDefault, nullptr);
	}

	// Drawing Earth
	{
		FindMesh("Planet", meshIndex);
		meshes[meshIndex].textureRV = planetTextures[3];
		meshes[meshIndex].mWorld = earthWorld;
		BasicDrawIndexed(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vsDefault, psDefault, nullptr);
	}

	// Drawing Earth's moon
	{
		FindMesh("Planet", meshIndex);
		meshes[meshIndex].textureRV = planetTextures[4];
		meshes[meshIndex].mWorld = moonWorld;
		BasicDrawIndexed(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vsDefault, psDefault, nullptr);
	}

	// Drawing mars
	{
		FindMesh("Planet", meshIndex);
		meshes[meshIndex].textureRV = planetTextures[5];
		meshes[meshIndex].mWorld = marsWorld;
		BasicDrawIndexed(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vsDefault, psDefault, nullptr);
	}

	// Drawing jupiter
	{
		FindMesh("Planet", meshIndex);
		meshes[meshIndex].textureRV = planetTextures[6];
		meshes[meshIndex].mWorld = jupiterWorld;
		BasicDrawIndexed(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vsDefault, psDefault, nullptr);
	}

	// Drawing saturn
	{
		FindMesh("Planet", meshIndex);
		meshes[meshIndex].textureRV = planetTextures[7];
		meshes[meshIndex].mWorld = saturnWorld;
		BasicDrawIndexed(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vsDefault, psDefault, nullptr);
	}

	// Drawing uranus
	{
		FindMesh("Planet", meshIndex);
		meshes[meshIndex].textureRV = planetTextures[8];
		meshes[meshIndex].mWorld = uranusWorld;
		BasicDrawIndexed(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vsDefault, psDefault, nullptr);
	}

	// Drawing neptune
	{
		FindMesh("Planet", meshIndex);
		meshes[meshIndex].textureRV = planetTextures[9];
		meshes[meshIndex].mWorld = neptuneWorld;
		BasicDrawIndexed(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vsDefault, psDefault, nullptr);
	}

	// Drawing pluto
	{
		FindMesh("Planet", meshIndex);
		meshes[meshIndex].textureRV = planetTextures[10];
		meshes[meshIndex].mWorld = plutoWorld;
		BasicDrawIndexed(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vsDefault, psDefault, nullptr);
		// After drawing pluto, make sure to clear the planet mesh texture RV to nullptr
		meshes[meshIndex].textureRV = nullptr;
	}

	// Drawing the ship
	{
		FindMesh("Ship", meshIndex);
		meshes[meshIndex].mWorld = XMMatrixMultiply(XMMatrixIdentity(), XMMatrixRotationY(cb1.time.x * -0.0025f));
		meshes[meshIndex].mWorld = XMMatrixMultiply(meshes[meshIndex].mWorld, XMMatrixTranslation(9.5f, 0.0f, 0.0f));
		meshes[meshIndex].mWorld = XMMatrixMultiply(meshes[meshIndex].mWorld, XMMatrixRotationY(0.0f - (cb1.time.x * 0.25f)));
		BasicDrawIndexed(meshIndex, cb1, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vsDefault, psDefault, nullptr);
	}
}

void LetsDrawSomeStuff::WindowResized(GW::SYSTEM::GWindow* attatchPoint)
{
	// Large viewport

	// Initialize the projection matrix
	UINT height;
	UINT width;
	attatchPoint->GetClientHeight(height);
	attatchPoint->GetClientWidth(width);
	vp1ProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);

	// Viewport structure constructed by topLeftX, topLeftY, width, height, minDepth, maxDepth
	D3D11_VIEWPORT largeViewport = { 0.0f, 0.0f, width, height, 0.5f, 1.0f };


	// Small viewport

	float vp2Height = height / 4.0f;
	float vp2Width = width / 4.0f;

	// Viewport structure constructed by topLeftX, topLeftY, width, height, minDepth, maxDepth
	D3D11_VIEWPORT smallViewport = { 5.0f, 5.0f, vp2Width, vp2Height, 0.0f, 0.5f };

	// Initialize the projection matrix
	vp2ProjectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, vp2Width / vp2Height, 0.01f, 100.0f);;

	viewPorts = new D3D11_VIEWPORT[2];
	viewPorts[0] = largeViewport;
	viewPorts[1] = smallViewport;
}