#pragma once

#pragma warning( disable : 4100 4273 )

#include <WinSDKVer.h>
//#define _WIN32_WINNT 0x0602 // Ensure we are using XAudio 2.8!
#include <SDKDDKVer.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <d3d11_1.h>

//#include <wrl/client.h>

#if defined(NTDDI_WIN10_RS2)
#include <dxgi1_6.h>
#else
#include <dxgi1_5.h>
#endif

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

//#include <stdexcept>

#include "BufferHelpers.h"
#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "GraphicsMemory.h"
#include "Keyboard.h"
#include "Model.h"
#include "Mouse.h"
#include "PostProcess.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"

#include "Utils/IO.h"
#include "Utils/Memory.h"
#include "Utils/Timers.h"
#include "Utils/File.h"
#include "Debug.h"

#ifdef _DEBUG
_Check_return_ _CRT_JIT_INTRINSIC inline double fabs( _In_ double _Xx )
{
	if( _Xx < 0.0 )
	{
		_Xx = -_Xx;
	}

	return _Xx;
}
#endif // _DEBUG

static const float Epsilon = 0.001f;

/*namespace DX
{
// Helper class for COM exceptions
class com_exception : public std::exception
{
public:
	com_exception( HRESULT hr ) noexcept : result( hr ) {}

	const char* what() const override
	{
		static char s_str[ 64 ] = {};
		sprintf_s( s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>( result ) );
		return s_str;
	}

private:
	HRESULT result;
};

// Helper utility converts D3D API failures into exceptions.
inline void ThrowIfFailed( HRESULT hr )
{
	if( FAILED( hr ) )
	{
		throw com_exception( hr );
	}
}
}*/
