//#pragma once
//#include <wrl.h>
//#include "Sprite.h"
//#include "ConstStruct.h"
//#include "ConstBuffer.h"
//#include "EditorMode.h"
//
//namespace IFE
//{
//	class OldPostEffect : public Sprite
//	{
//	private:
//		Microsoft::WRL::ComPtr<ID3D12Resource>texBuff[3];
//		Microsoft::WRL::ComPtr<ID3D12Resource>depthTex;
//		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapSRV;
//		Microsoft::WRL::ComPtr<ID3D12Resource>depthBuff;
//		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapRTV;
//		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapDSV;
//		Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;
//		Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineStateAdd;
//		Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;
//		ConstBuffer<ConstBufferPostEffect> buffer_;
//
//	public:
//		ConstBufferPostEffect* constMapPostEffect = nullptr;
//
//	private:
//		static const float clearColor[4];
//
//	public:
//		OldPostEffect();
//		~OldPostEffect();
//		void Draw(bool add = false);
//		void Update();
//		void Initialize();
//		void DrawBefore();
//		void DrawAfter();
//		/// <summary>
//		/// RGBシフトをします
//		/// </summary>
//		/// <param name="shift">0~0.005以内がちょうど良いと思います</param>
//		void SetRGBShift(float shift);
//		void SetContrast(float contrast);
//		void SetBrightness(float brightness);
//		//void SetGrayscale(bool gray);
//		//void SetSepia(float sepia);
//		void SetVBInit();
//		void SetVBGame();
//
//#ifdef InverseEditorMode
//#else
//		void DebugGUI();
//		void OutputScene();
//#endif
//		void LoadingScene();
//
//	private:
//		void CreateGraphicsPipelineState();
//	};
//}
