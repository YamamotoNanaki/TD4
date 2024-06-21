#include "GraphicsAPI.h"
#include <cassert>
#include <thread>
#include "Debug.h"
#include "IFETime.h"
#include "WindowsAPI.h"
#include <d3dx12.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace Microsoft::WRL;
using namespace std;
using namespace std::chrono;
using namespace IFE;

ComPtr < ID3D12Device> GraphicsAPI::sDevice_ = nullptr;

void IFE::GraphicsAPI::InitializeFixFps()
{
	reference_ = IFETime::GetStartTime();
}

void IFE::GraphicsAPI::UpdateFixFps()
{
	const microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	const microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	steady_clock::time_point now = steady_clock::now();
	microseconds elapsed = duration_cast<microseconds>(now - reference_);

	if (elapsed < kMinCheckTime)
	{
		while (steady_clock::now() - reference_ < kMinTime)
		{
			this_thread::sleep_for(microseconds(1));
		}
	}
	IFETime::Update();
	reference_ = IFETime::GetNowTime();
}

void IFE::GraphicsAPI::ResizeSwapChain()
{
	if (WindowsAPI::Instance()->resize_)
	{
		UINT width = WindowsAPI::Instance()->winWidth_;
		UINT height = WindowsAPI::Instance()->winHeight_;

		// 既存のRTVとDSVをリリース
		for (size_t i = 0; i < backBuffers_.size(); ++i)
		{
			if (backBuffers_[i])
			{
				backBuffers_[i].Reset();
			}
		}
		if (depthBuffer_)
		{
			depthBuffer_.Reset();
		}

		// スワップチェインをリサイズ
		if (swapchain_)
		{
			HRESULT hr = swapchain_->ResizeBuffers(UINT(backBuffers_.size()), width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
			if (FAILED(hr))
			{
				// リサイズに失敗した場合のエラーハンドリング
				return;
			}
		}

		for (size_t i = 0; i < backBuffers_.size(); i++)
		{
			// スワップチェーンからバッファを取得
			swapchain_->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers_[i]));
			// デスクリプタヒープのハンドルの取得
			D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeaps_->GetCPUDescriptorHandleForHeapStart();
			// 裏か表かアドレスがずれる
			rtvHandle.ptr += i * sDevice_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);
			D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
			rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			// レンダーターゲットビューの生成
			sDevice_->CreateRenderTargetView(backBuffers_[i].Get(), &rtvDesc, rtvHandle);
		}

		// 新しいDSVを作成
		D3D12_RESOURCE_DESC depthStencilDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, width, height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
		D3D12_CLEAR_VALUE clearValue = {};
		clearValue.Format = DXGI_FORMAT_D32_FLOAT;
		clearValue.DepthStencil.Depth = 1.0f;
		clearValue.DepthStencil.Stencil = 0;

		auto a = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		sDevice_->CreateCommittedResource(
			&a,
			D3D12_HEAP_FLAG_NONE,
			&depthStencilDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(&depthBuffer_)
		);

		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		sDevice_->CreateDepthStencilView(depthBuffer_.Get(), &dsvDesc, dsvHeap_->GetCPUDescriptorHandleForHeapStart());

		// ビューポートとシザー矩形を更新
		SetNewViewPort(static_cast<float>(width), static_cast<float>(height), 0, 0);
		SetScissorrect(0, static_cast<float>(width), 0, static_cast<float>(height));
	}
}

GraphicsAPI* IFE::GraphicsAPI::Instance()
{
	static GraphicsAPI inst;
	return &inst;
}

void IFE::GraphicsAPI::Finalize()
{
#ifdef _DEBUG
	{
		//ComPtr<ID3D12DebugDevice> debugDevice;
		//if (SUCCEEDED(sDevice_->QueryInterface(IID_PPV_ARGS(&debugDevice))))
		//{
		//	debugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
		//	debugDevice->Release();
		//}
	}
#endif
}

void GraphicsAPI::Initialize(const HWND& hwnd)
{
	IFETime::Initialize();
	InitializeFixFps();
	//初期化
	ComPtr < IDXGIAdapter4> tmpAdapter = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	//深度バッファ
	D3D12_RESOURCE_DESC depthResDesc{};
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	D3D12_CLEAR_VALUE depthClearValue{};
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	// DXGIファクトリーの生成
	HRESULT result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory_));
	// アダプターの列挙用
	vector< ComPtr<IDXGIAdapter4>> adapters;
	// ここに特定の名前を持アダプターオブジェクトを入れる
	for (uint32_t i = 0; dxgiFactory_->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter); //動的配列に追加する
	}

	for (size_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;
		adapters[i]->GetDesc3(&adapterDesc); // アダプターの情報を取得

		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			tmpAdapter = adapters[i]; //採用
			break;
		}
	}
	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

#ifdef _DEBUG
	ID3D12Debug1* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
	if (debugController)debugController->Release();
#endif

	for (size_t i = 0; i < _countof(levels); i++)
	{
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&sDevice_));
		if (result == S_OK)
		{
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}


#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue>infoQueue;
	if (SUCCEEDED(sDevice_->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		D3D12_MESSAGE_ID denyids[] = { D3D12_MESSAGE_ID_RESOURCE_BARRIER_BEFORE_AFTER_MISMATCH, D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_INCOMPATIBLE_RESOURCE_STATE,
		D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE };
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyids);
		filter.DenyList.pIDList = denyids;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		infoQueue->PushStorageFilter(&filter);
		infoQueue->Release();
	}
#endif
	// コマンドアロケータを生成
	result = sDevice_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(result));

	// コマンドリストの生成
	result = sDevice_->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator_.Get(), nullptr,
		IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(result));
	uint32_t window_width = WindowsAPI::Instance()->winWidth_;
	uint32_t window_height = WindowsAPI::Instance()->winHeight_;

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	result = sDevice_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(result));
	// 各種設定をしてスワップチェーンを生成
	swapChainDesc.Width = window_width;
	swapChainDesc.Height = window_height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色情報の書式
	swapChainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファ用
	swapChainDesc.BufferCount = 2; // バッファ数を２つに設定
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; //フリップ後は破壊
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ComPtr <IDXGISwapChain1> swapchain1;

	result = dxgiFactory_->CreateSwapChainForHwnd(commandQueue_.Get(), hwnd, &swapChainDesc, nullptr, nullptr, &swapchain1);

	swapchain1.As(&swapchain_);

	// 各種設定をしてデスクリプタヒープを生成
	rtvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	rtvHeapDesc_.NumDescriptors = swapChainDesc.BufferCount;	// 裏表の２つ
	sDevice_->CreateDescriptorHeap(&rtvHeapDesc_, IID_PPV_ARGS(&rtvHeaps_));

	backBuffers_.resize(swapChainDesc.BufferCount);

	for (size_t i = 0; i < backBuffers_.size(); i++)
	{
		// スワップチェーンからバッファを取得
		swapchain_->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers_[i]));
		// デスクリプタヒープのハンドルの取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeaps_->GetCPUDescriptorHandleForHeapStart();
		// 裏か表かアドレスがずれる
		rtvHandle.ptr += i * sDevice_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// レンダーターゲットビューの生成
		sDevice_->CreateRenderTargetView(backBuffers_[i].Get(), &rtvDesc, rtvHandle);
	}

	result = sDevice_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));

	//リソース設定
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResDesc.Width = window_width;			//レンダーターゲットに合わせる
	depthResDesc.Height = window_height;		//レンダーターゲットに合わせる
	depthResDesc.DepthOrArraySize = 1;
	depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	depthResDesc.SampleDesc.Count = 1;
	depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//デプスステンシル

	//深度地用ヒーププロパティ
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//深度地のクリアを設定
	depthClearValue.DepthStencil.Depth = 1.0f;			//深度値1(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;		//深度値フォーマット

	result = sDevice_->CreateCommittedResource(
		&depthHeapProp, D3D12_HEAP_FLAG_NONE, &depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,					//深度値書き込みに使用
		&depthClearValue, IID_PPV_ARGS(&depthBuffer_));

	//深度ビュー用デスクリプタヒープ作成
	dsvHeapDesc.NumDescriptors = 1;						//深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	//デプスステンシルビュー
	result = sDevice_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap_));

	//深度ビュー作成
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	sDevice_->CreateDepthStencilView(depthBuffer_.Get(), &dsvDesc, dsvHeap_->GetCPUDescriptorHandleForHeapStart());
	//#ifdef _DEBUG
	//	SetNewViewPort(800, 450, 200, 40);
	//#else
	SetNewViewPort(static_cast<float>(window_width), static_cast<float>(window_height), 0, 0);
	//#endif
	SetScissorrect(0, static_cast<float>(window_width), 0, static_cast<float>(window_height));
}

void GraphicsAPI::DrawBefore()
{
	//描画前処理
	uint32_t bbIndex;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH;
	bbIndex = swapchain_->GetCurrentBackBufferIndex();
	// 1．リソースバリアで書き込み可能に変更
	barrierDesc_.Transition.pResource = backBuffers_[bbIndex].Get(); // バックバッファを指定
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示から
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画
	commandList_->ResourceBarrier(1, &barrierDesc_);

	// 2．描画先指定
	// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	rtvHandle = rtvHeaps_->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * sDevice_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	dsvH = dsvHeap_->GetCPUDescriptorHandleForHeapStart();
	commandList_->OMSetRenderTargets(1, &rtvHandle, false, &dsvH);

	commandList_->ClearRenderTargetView(rtvHandle, clearColor_, 0, nullptr);
	commandList_->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	commandList_->RSSetScissorRects(1, &scissorrect_);
}

void IFE::GraphicsAPI::DrawSetViewport(int32_t viewportNum)
{
	commandList_->RSSetViewports(1, &viewport_[viewportNum]);
}

void GraphicsAPI::DrawAfter()
{
	// 5．リソースバリアを戻す
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;   // 表示に
	commandList_->ResourceBarrier(1, &barrierDesc_);

	// 命令のクローズ
	HRESULT result = commandList_->Close();
	assert(SUCCEEDED(result));
	// コマンドリストの実行
	Microsoft::WRL::ComPtr<ID3D12CommandList> commandLists[] = { commandList_.Get() }; // コマンドリストの配列
	commandQueue_->ExecuteCommandLists(1, commandLists->GetAddressOf());

	// バッファをフリップ（裏表の入替え）
	result = swapchain_->Present(1, 0);
	assert(SUCCEEDED(result));
	// コマンドリストの実行完了を待つ
	commandQueue_->Signal(fence_.Get(), ++fenceVal_);

	if (fence_->GetCompletedValue() != fenceVal_)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence_->SetEventOnCompletion(fenceVal_, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	UpdateFixFps();

	result = commandAllocator_->Reset(); // キューをクリア
	assert(SUCCEEDED(result));
	result = commandList_->Reset(commandAllocator_.Get(), nullptr);  // 再びコマンドリストを貯める準備
	assert(SUCCEEDED(result));
}

void IFE::GraphicsAPI::ExecuteCommand()
{
	// 命令のクローズ
	HRESULT result = commandList_->Close();
	assert(SUCCEEDED(result));
	// コマンドリストの実行
	Microsoft::WRL::ComPtr<ID3D12CommandList> commandLists[] = { commandList_.Get() }; // コマンドリストの配列
	commandQueue_->ExecuteCommandLists(1, commandLists->GetAddressOf());

	// コマンドリストの実行完了を待つ
	commandQueue_->Signal(fence_.Get(), ++fenceVal_);

	if (fence_->GetCompletedValue() != fenceVal_)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence_->SetEventOnCompletion(fenceVal_, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	result = commandAllocator_->Reset(); // キューをクリア
	assert(SUCCEEDED(result));
	result = commandList_->Reset(commandAllocator_.Get(), nullptr);  // 再びコマンドリストを貯める準備
	assert(SUCCEEDED(result));
}

uint32_t  IFE::GraphicsAPI::SetNewViewPort(float width, float height, float topX, float topY, float minDepth, float maxDepth)
{
	D3D12_VIEWPORT view{};

	view.Width = width;
	view.Height = height;
	view.TopLeftX = topX;
	view.TopLeftY = topY;
	view.MinDepth = minDepth;
	view.MaxDepth = maxDepth;

	this->viewport_.push_back(view);

	return static_cast<uint32_t>(viewport_.size()) - 1;
}

void IFE::GraphicsAPI::SetScissorrect(float left, float right, float top, float bottom)
{
	scissorrect_.left = static_cast<LONG>(left);
	scissorrect_.right = scissorrect_.left + static_cast<LONG>(right);
	scissorrect_.top = static_cast<LONG>(top);
	scissorrect_.bottom = scissorrect_.top + static_cast<LONG>(bottom);
}

ID3D12GraphicsCommandList* IFE::GraphicsAPI::GetCmdList()
{
	return commandList_.Get();
}

ID3D12Device* IFE::GraphicsAPI::GetDevice()
{
	return sDevice_.Get();
}

void GraphicsAPI::SetClearColor(const Float4& color)
{
	clearColor_[0] = color.x;
	clearColor_[1] = color.y;
	clearColor_[2] = color.z;
	clearColor_[3] = color.w;
}

void GraphicsAPI::SetClearColor(float R, float G, float B)
{
	clearColor_[0] = R;
	clearColor_[1] = G;
	clearColor_[2] = B;
	clearColor_[3] = 1;
}

ID3D12CommandQueue* IFE::GraphicsAPI::GetCommandQueue()
{
	return commandQueue_.Get();
}

ID3D12Fence* IFE::GraphicsAPI::GetFence()
{
	return fence_.Get();
}

uint64_t& IFE::GraphicsAPI::GetFenceVal()
{
	return fenceVal_;
}

ID3D12CommandAllocator* IFE::GraphicsAPI::GetCommandAllocator()
{
	return commandAllocator_.Get();
}
