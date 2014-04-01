#pragma once
#include <D3D11.h>
#include <xnamath.h>
#include <vector>
#include <list>
#include <map>
#include <string>

struct ObjRenderData
{
	XMFLOAT3 position;
	UINT iCount;
	UINT startIndLoc;
	INT baseVertLoc;
};

struct Vertex
{
	Vertex(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT2& tc)
		: position(p), normal(n), texCoords(tc){}
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texCoords;
};

struct ModelData
{
	ModelData() {}
	ModelData(std::vector<Vertex> iVData, std::vector<UINT> iIData) {}
	LPCSTR semanticName;
	std::vector<Vertex> vData;
	std::vector<UINT> iData;
};
