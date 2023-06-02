#include "../header/main.h"
#include "../header/Menu.h"
#include "../header/Math.h"
#include "../header/Memory.h"
#include "../header/Config.h"
#include "../header/ESP.h"
#include "../header/Misc.h"

#include <vector>
#include <iostream>

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;
D3D11ResizeBuffersHook phookD3D11ResizeBuffers;
D3D11DrawIndexedHook phookD3D11DrawIndexed;
D3D11DrawIndexedInstancedHook phookD3D11DrawIndexedInstanced;

ID3D11DepthStencilState* DepthStencilState_FALSE; //depth off
ID3D11DepthStencilState* DepthStencilState_ORIG; //depth on

ID3D11RasterizerState* DEPTHBIASState_FALSE;
ID3D11RasterizerState* DEPTHBIASState_TRUE;
ID3D11RasterizerState* DEPTHBIASState_ORIG;

//vertex
ID3D11Buffer* veBuffer;
UINT Stride;
UINT veBufferOffset;
D3D11_BUFFER_DESC vedesc;

//index
ID3D11Buffer* inBuffer;
DXGI_FORMAT inFormat;
UINT        inOffset;
D3D11_BUFFER_DESC indesc;

//psgetConstantbuffers
UINT pscStartSlot;
ID3D11Buffer* pscBuffer;
D3D11_BUFFER_DESC pscdesc;

//vsgetconstantbuffers
UINT vscStartSlot;
ID3D11Buffer* vscBuffer;
D3D11_BUFFER_DESC vscdesc;

//pssetshaderresources
UINT pssrStartSlot;
ID3D11Resource* Resource;
D3D11_SHADER_RESOURCE_VIEW_DESC Descr;
D3D11_TEXTURE2D_DESC texdesc;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	menu::def_main = io.Fonts->AddFontFromMemoryTTF(myfont, myfont_size, 15.f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	menu::med_main = io.Fonts->AddFontFromMemoryTTF(myfont, myfont_size, 20.f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	menu::big_main = io.Fonts->AddFontFromMemoryTTF(myfont, myfont_size, 30.f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	menu::icons = io.Fonts->AddFontFromMemoryTTF(iconfont, iconfont_size, 15.f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	ImGui::GetIO().Fonts->Build();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	io.IniFilename = NULL;
	menu::SetupImGuiStyle();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

void InitDxStuff() {
	////////////////////////////////////////////////////////////////////////////////
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.StencilEnable = FALSE;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	// Stencil operations if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	pDevice->CreateDepthStencilState(&depthStencilDesc, &DepthStencilState_FALSE);

	//create depthbias rasterizer state
	D3D11_RASTERIZER_DESC rasterizer_desc;
	ZeroMemory(&rasterizer_desc, sizeof(rasterizer_desc));
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_NONE; //D3D11_CULL_FRONT;
	rasterizer_desc.FrontCounterClockwise = false;
	float bias = 1000.0f;
	float bias_float = static_cast<float>(-bias);
	bias_float /= 10000.0f;
	rasterizer_desc.DepthBias = DEPTH_BIAS_D32_FLOAT(*(DWORD*)&bias_float);
	rasterizer_desc.SlopeScaledDepthBias = 0.0f;
	rasterizer_desc.DepthBiasClamp = 0.0f;
	rasterizer_desc.DepthClipEnable = true;
	rasterizer_desc.ScissorEnable = false;
	rasterizer_desc.MultisampleEnable = false;
	rasterizer_desc.AntialiasedLineEnable = false;
	pDevice->CreateRasterizerState(&rasterizer_desc, &DEPTHBIASState_FALSE);

	//create normal rasterizer state
	D3D11_RASTERIZER_DESC nrasterizer_desc;
	ZeroMemory(&nrasterizer_desc, sizeof(nrasterizer_desc));
	nrasterizer_desc.FillMode = D3D11_FILL_SOLID;
	//nrasterizer_desc.CullMode = D3D11_CULL_BACK; //flickering
	nrasterizer_desc.CullMode = D3D11_CULL_NONE;
	nrasterizer_desc.FrontCounterClockwise = false;
	nrasterizer_desc.DepthBias = 0.0f;
	nrasterizer_desc.SlopeScaledDepthBias = 0.0f;
	nrasterizer_desc.DepthBiasClamp = 0.0f;
	nrasterizer_desc.DepthClipEnable = true;
	nrasterizer_desc.ScissorEnable = false;
	nrasterizer_desc.MultisampleEnable = false;
	nrasterizer_desc.AntialiasedLineEnable = false;
	pDevice->CreateRasterizerState(&nrasterizer_desc, &DEPTHBIASState_TRUE);
	/////////////////////////////////////////////////////////////////////////////////
}

HRESULT __stdcall hookD3D11ResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	if (mainRenderTargetView) {
		pContext->OMSetRenderTargets(0, 0, 0);
		mainRenderTargetView->Release();
	}

	HRESULT hr = phookD3D11ResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

	ID3D11Texture2D* pBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);

	pDevice->CreateRenderTargetView(pBuffer, NULL, &mainRenderTargetView);

	pBuffer->Release();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);

	D3D11_VIEWPORT vp;
	vp.Width = Width;
	vp.Height = Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	pContext->RSSetViewports(1, &vp);

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)Width, (float)Height);
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	ImGui::Render();

	memory::scrsize = { (float)Width, (float)Height };

	return hr;
}

void __stdcall hookD3D11DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	pContext->IAGetVertexBuffers(0, 1, &veBuffer, &Stride, &veBufferOffset);
	if (veBuffer != NULL)
		veBuffer->GetDesc(&vedesc);
	if (veBuffer != NULL) { veBuffer->Release(); veBuffer = NULL; }


	pContext->IAGetIndexBuffer(&inBuffer, &inFormat, &inOffset);
	if (inBuffer != NULL)
		inBuffer->GetDesc(&indesc);
	if (inBuffer != NULL) { inBuffer->Release(); inBuffer = NULL; }

	pContext->PSGetConstantBuffers(pscStartSlot, 1, &pscBuffer);
	if (pscBuffer != NULL)
		pscBuffer->GetDesc(&pscdesc);
	if (pscBuffer != NULL) { pscBuffer->Release(); pscBuffer = NULL; }


	pContext->VSGetConstantBuffers(vscStartSlot, 1, &vscBuffer);
	if (vscBuffer != NULL)
		vscBuffer->GetDesc(&vscdesc);
	if (vscBuffer != NULL) { vscBuffer->Release(); vscBuffer = NULL; }

	/*if (GetAsyncKeyState(VK_NUMPAD1) & 1)
	{
		nstride += 1;
		printf("Stride: %d \n", nstride);
	}
	if (GetAsyncKeyState(VK_NUMPAD4) & 1)
	{
		nstride -= 1;
		printf("Stride: %d \n", nstride);
	}
	if (GetAsyncKeyState(VK_NUMPAD2) & 1)
	{
		count += 1;
		printf("Count: %d \n", count);
	}
	if (GetAsyncKeyState(VK_NUMPAD5) & 1)
	{
		count -= 1;
		printf("Count: %d \n", count);
	}
	*/

	if (cfg::remove_smokes)
	{
		if (Stride == 0 && IndexCount == 6)
		{
			pContext->RSSetState(DEPTHBIASState_FALSE);
			phookD3D11DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
			pContext->RSSetState(DEPTHBIASState_TRUE);
		}
	}


	if (cfg::remove_smokes)
	{
		if (Stride == 0 && IndexCount == 6)
			return;
	}

	return phookD3D11DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);

}

void __stdcall hookD3D11DrawIndexedInstanced(ID3D11DeviceContext* pContext, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation)
{

	pContext->IAGetVertexBuffers(0, 1, &veBuffer, &Stride, &veBufferOffset);
	if (veBuffer != NULL)
		veBuffer->GetDesc(&vedesc);
	if (veBuffer != NULL) { veBuffer->Release(); veBuffer = NULL; }

	pContext->IAGetIndexBuffer(&inBuffer, &inFormat, &inOffset);
	if (inBuffer != NULL)
		inBuffer->GetDesc(&indesc);
	if (inBuffer != NULL) { inBuffer->Release(); inBuffer = NULL; }

	pContext->PSGetConstantBuffers(pscStartSlot, 1, &pscBuffer);
	if (pscBuffer != NULL)
		pscBuffer->GetDesc(&pscdesc);
	if (pscBuffer != NULL) { pscBuffer->Release(); pscBuffer = NULL; }

	pContext->VSGetConstantBuffers(vscStartSlot, 1, &vscBuffer);
	if (vscBuffer != NULL)
		vscBuffer->GetDesc(&vscdesc);
	if (vscBuffer != NULL) { vscBuffer->Release(); vscBuffer = NULL; }

	if (cfg::remove_smokes)
	{
		if (Stride == 0 && IndexCountPerInstance == 6)
		{
			pContext->RSSetState(DEPTHBIASState_FALSE); //depthbias off
			phookD3D11DrawIndexedInstanced(pContext, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation); //redraw
			pContext->RSSetState(DEPTHBIASState_TRUE); //depthbias true
		}
	}

	if (cfg::remove_smokes)
	{
		if (Stride == 0 && IndexCountPerInstance == 6)
			return;
	}

	return phookD3D11DrawIndexedInstanced(pContext, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
}


LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (cfg::block_input)
	{
	case(true):
		if (menu::open)
		{
			ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
			return true;
		};

	case(false):
		if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		{
			return true;
		};
	}
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			memory::InitMemory();
			InitDxStuff();
			init = true;

		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::GetIO().MouseDrawCursor = menu::open;
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		menu::open = !menu::open;
	}
#ifdef _DEBUG
	menu::agree = true;
#endif // _DEBUG
	if (!menu::agree)
		menu::showWarningwindow();

	if (menu::agree)
	{
		if (menu::open)
		{
			menu::showMenu();
		}
	}
#ifdef _DEBUG
	if (cfg::esp_status)
		debug();
#endif // _DEBUG

	ESP();
	Misc();

	ImGui::Render();


	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}