#pragma once
#include <d3dcompiler.h>
#include <d3dx12.h>

using namespace Microsoft::WRL;
/// <summary>
/// シェーダークラス
/// </summary>
class Shader
{
public:
	ComPtr<ID3DBlob> vsBlob = nullptr; //頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob = nullptr; //ジオメトリシェーダー
	ComPtr<ID3DBlob> psBlob = nullptr; //ピクセルシェーダオブジェクト
};

ComPtr<ID3DBlob> LoadShader(const LPCWSTR &VshaderName = L"", const LPCSTR &Vtarget = "", const LPCSTR &entryPoint = "main");

class ShaderManager
{
public:
	/// <summary>
	/// スプライト
	/// </summary>
	static Shader spriteShader;		//スプライト
	static Shader spriteBlurShader;//スプライトブラー
	/// <summary>
	/// ポストエフェクト
	/// </summary>
	static Shader postNormalShader;	//ポストエフェクト（通常）
	static Shader postReversalShader;//反射
	static Shader postBlurShader;
	/// <summary>
	/// パーティクル
	/// </summary>
	static Shader particleShader;//パーティクル


	static void LoadShaders();
};