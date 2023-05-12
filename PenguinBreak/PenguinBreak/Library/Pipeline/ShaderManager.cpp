#include "ShaderManager.h"

ComPtr<ID3DBlob> LoadShader(const LPCWSTR &VshaderName, const LPCSTR &Vtarget, const LPCSTR &entryPoint)
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> Blob;
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	if (VshaderName != L"")
	{
		// 頂点シェーダの読み込みとコンパイル
		result = D3DCompileFromFile(VshaderName,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint, Vtarget, D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0, &Blob, &errorBlob);
		if (FAILED(result)) {
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());

			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				errstr.begin());
			errstr += "\n";
			OutputDebugStringA(errstr.c_str());
			exit(1);
		}
	}
	return Blob;
}

Shader ShaderManager::spriteShader;
Shader ShaderManager::spriteBlurShader;
Shader ShaderManager::doubleTextureShader;
Shader ShaderManager::particleShader;
Shader ShaderManager::postNormalShader;
Shader ShaderManager::postReversalShader;
Shader ShaderManager::postBlurShader;


void ShaderManager::LoadShaders()
{	
	//スプライト
	spriteShader.vsBlob = LoadShader(L"Resources/shaders/Sprite/SpriteVS.hlsl", "vs_5_0");
	spriteShader.psBlob = LoadShader(L"Resources/shaders/Sprite/SpritePS.hlsl", "ps_5_0");
	//ブラー
	spriteBlurShader.vsBlob = LoadShader(L"Resources/shaders/Sprite/SpriteBlurVS.hlsl", "vs_5_0");
	spriteBlurShader.psBlob = LoadShader(L"Resources/shaders/Sprite/SpriteBlurPS.hlsl", "ps_5_0");
	//２枚テクスチャ用
	doubleTextureShader.vsBlob = LoadShader(L"Resources/shaders/Sprite/DobuleTextureVS.hlsl", "vs_5_0");
	doubleTextureShader.psBlob = LoadShader(L"Resources/shaders/Sprite/DobuleTexturePS.hlsl", "ps_5_0");

	//パーティクル
	particleShader.vsBlob = LoadShader(L"Resources/shaders/ParticleVS.hlsl", "vs_5_0");
	particleShader.psBlob = LoadShader(L"Resources/shaders/ParticlePS.hlsl", "ps_5_0");
	particleShader.gsBlob = LoadShader(L"Resources/shaders/ParticleGS.hlsl", "gs_5_0");
	//ポストエフェクト通常
	postNormalShader.vsBlob = LoadShader(L"Resources/shaders/PostEffect/PostEffectTestVS.hlsl", "vs_5_0");
	postNormalShader.psBlob = LoadShader(L"Resources/shaders/PostEffect/PostEffectTestPS.hlsl", "ps_5_0");
	//ブルーム
	postBlurShader.vsBlob = LoadShader(L"Resources/shaders/PostEffect/PostEffectTestVS.hlsl", "vs_5_0");
	postBlurShader.psBlob = LoadShader(L"Resources/shaders/PostEffect/PostEffectTestPS.hlsl", "ps_5_0");
}
