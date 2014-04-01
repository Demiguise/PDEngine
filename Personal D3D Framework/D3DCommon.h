#include <D3DX11.h>

struct bufferData
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
};

//Helpful Macros (Shameless 'Stolen' from Frank D Luna)

#define ReleaseCOM(x) { if(x) { x->Release(); x = 0; } }

//Need to have the '\' at the end of the line to say it's a
//line macro. Since they are usually just a single line.
#define HR(x)												\
{															\
	HRESULT hr = x;											\
	if (FAILED(hr))											\
	{														\
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true);	\
	}														\
}
