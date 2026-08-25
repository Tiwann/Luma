#pragma once

struct ID3D12Device;
struct ID3D12Device1;
struct ID3D12Device2;
struct ID3D12Device3;
struct ID3D12Device4;
struct ID3D12Device5;
struct ID3D12Device6;
struct ID3D12Device7;
struct ID3D12Device8;
struct ID3D12Device9;
struct ID3D12Device10;
struct ID3D12Device11;
struct ID3D12Device12;
struct ID3D12Device13;
struct ID3D12Device14;
struct ID3D12Device15;

struct IDXGIFactory;
struct IDXGIFactory1;
struct IDXGIFactory2;
struct IDXGIFactory3;
struct IDXGIFactory4;
struct IDXGIFactory5;
struct IDXGIFactory6;
struct IDXGIFactory7;

struct IDXGIAdapter;
struct IDXGIAdapter1;
struct IDXGIAdapter2;
struct IDXGIAdapter3;
struct IDXGIAdapter4;

struct ID3D12InfoQueue;
struct ID3D12InfoQueue1;

struct ID3D12Debug;
struct ID3D12Debug1;
struct ID3D12Debug2;
struct ID3D12Debug3;
struct ID3D12Debug4;
struct ID3D12Debug5;
struct ID3D12Debug6;

struct ID3D12CommandSignature;
struct ID3D12CommandAllocator;
struct ID3D12CommandQueue;

struct ID3D12Resource;
struct ID3D12Resource1;
struct ID3D12Resource2;

struct IDXGISwapChain;
struct IDXGISwapChain1;
struct IDXGISwapChain2;
struct IDXGISwapChain3;
struct IDXGISwapChain4;

struct ID3D12DescriptorHeap;

struct ID3D12CommandList;

struct ID3D12GraphicsCommandList;
struct ID3D12GraphicsCommandList1;
struct ID3D12GraphicsCommandList2;
struct ID3D12GraphicsCommandList3;
struct ID3D12GraphicsCommandList4;
struct ID3D12GraphicsCommandList5;
struct ID3D12GraphicsCommandList6;
struct ID3D12GraphicsCommandList7;
struct ID3D12GraphicsCommandList8;
struct ID3D12GraphicsCommandList9;
struct ID3D12GraphicsCommandList10;

namespace D3D12MA
{
    class Allocator;
    class Allocation;
}

typedef D3D12MA::Allocation ID3D12Allocation;
typedef D3D12MA::Allocator ID3D12Allocator;

using namespace D3D12MA;