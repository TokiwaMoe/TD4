#pragma once
#include<d3dcompiler.h>
#include<DirectXTex.h>
#include<d3dx12.h>
#include<wrl.h>
#include"ShaderManager.h"
using namespace Microsoft::WRL;
using namespace DirectX;
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

enum PipelineType
{
	PipelineNormal,
	PipelineNoShadow,
	PipelineFBX,
};

/// <summary>
/// パイプラインクラス
/// </summary>
class Pipeline
{
public:
	//構造体
	struct PipelineSet
	{
		//パイプラインステートオブジェクト
		ComPtr<ID3D12PipelineState>pipelinestate;
		//ルートシグネチャ
		ComPtr <ID3D12RootSignature>rootsignature;
	};
	//スプライト
	static PipelineSet  SpriteCreateGraphicsPipeline(ID3D12Device* dev, const Shader &shader);
	//2枚テクスチャ用スプライト
	static PipelineSet SpriteCreateDoubleGraphicsPipeline(ID3D12Device* dev, const Shader& shader);

	//パーティクル
	static PipelineSet ParticleCreateGraphicsPipeline(ID3D12Device* dev, const Shader &shader);
	//ポストエフェクト(通常)
	static PipelineSet PostNormalCreateGraphicsPipeline(ID3D12Device* dev, const Shader &shader);
public:
	//２Dスプライト描画
	static PipelineSet SpritePipeline;
	static PipelineSet SpriteBlurPipeline;

	//パーティクル
	static PipelineSet ParticlePiepline;


	//ポストエフェクト
	static PipelineSet PostPipeline;
	static PipelineSet PostWaterPipeline;

	//2枚用
	//仮
	static PipelineSet DoubleTexturePipeline;


	//パイプライン作成
	static void CreatePipeline(ID3D12Device* dev);

	static int pipelineType;

	static bool SetPipeline(int type);
};