#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <wrl.h>
#include <chrono>
#include "IFMath.h"

namespace IFE
{
	class GraphicsAPI final
	{
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		template<class T> using vector = std::vector<T>;

	private:
		static ComPtr < ID3D12Device> sDevice_;
		ComPtr < IDXGIFactory6> dxgiFactory_ = nullptr;
		ComPtr < IDXGISwapChain4> swapchain_ = nullptr;
		ComPtr < ID3D12CommandAllocator> commandAllocator_ = nullptr;
		ComPtr < ID3D12GraphicsCommandList> commandList_ = nullptr;
		ComPtr < ID3D12CommandQueue> commandQueue_ = nullptr;
		ComPtr < ID3D12DescriptorHeap> rtvHeaps_ = nullptr;
		// 裏表の2つ分について
		vector< ComPtr<ID3D12Resource>> backBuffers_ = vector<ComPtr<ID3D12Resource>>(2);
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};
		// フェンスの生成
		ComPtr<ID3D12Fence> fence_ = nullptr;
		uint64_t fenceVal_ = 0;
		vector<D3D12_VIEWPORT> viewport_{};

	private:
		ComPtr<ID3D12DescriptorHeap> dsvHeap_ = nullptr;
		ComPtr < ID3D12Resource> depthBuffer_ = nullptr;

		D3D12_RESOURCE_BARRIER barrierDesc_{};
		D3D12_RECT scissorrect_{};

		// 3．画面クリア        R  G  B   A
		float clearColor_[4] = { 0 ,0 ,0 ,1.0f };

		//FPS関連
	private:
		void InitializeFixFps();
		void UpdateFixFps();
		std::chrono::steady_clock::time_point reference_;

	public:
		static GraphicsAPI* Instance();
		static void Finalize();
		void Initialize(const HWND& hwnd);
		void DrawBefore();
		void DrawSetViewport(int32_t viewportNum = 0);
		void DrawAfter();
		void ExecuteCommand();
		uint32_t SetNewViewPort(float width, float height, float topX, float topY, float minDepth = 0.0f, float maxDepth = 1.0f);
		void SetScissorrect(float left, float right, float top, float bottom);

		//ゲッター
	public:
		static ID3D12Device* GetDevice();
		ID3D12GraphicsCommandList* GetCmdList();
		ID3D12CommandQueue* GetCommandQueue();
		ID3D12Fence* GetFence();
		ID3D12CommandAllocator* GetCommandAllocator();
		uint64_t& GetFenceVal();

	public:
		void SetClearColor(const Float4& color);
		void SetClearColor(float Red, float Green, float Bule);

	private:
		GraphicsAPI() {};
		GraphicsAPI(const GraphicsAPI&) {};
		~GraphicsAPI() {};
		const GraphicsAPI& operator=(const GraphicsAPI&) {};
	};
}