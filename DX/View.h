#pragma once

namespace DX
{

class DeviceResources;

// Class that encapsulates some viewport and shader constant functionality
// probably best to have separate camera class
class View
{
public:
	struct VPConstantBuffer
	{
		DirectX::XMMATRIX viewProjectionMatrix;
		DirectX::XMFLOAT4 vAmbient;
		DirectX::XMFLOAT4 vDirLight0;
		DirectX::XMFLOAT4 vDirLight0Col;
	};
	static_assert( ( sizeof( VPConstantBuffer ) % 16 ) == 0, "Constant buffer must always be 16-byte aligned" );

	struct WorldConstantBuffer
	{
		DirectX::XMMATRIX worldMatrix;
	};
	static_assert( ( sizeof( WorldConstantBuffer ) % 16 ) == 0, "Constant buffer must always be 16-byte aligned" );

	View( DeviceResources* const deviceResources );
	~View();

	void							Initialise();
	void							Refresh();
	void							Shutdown();

	inline void						SetViewPosition( const DirectX::XMVECTOR viewPosition )
	{
		m_viewPosition = viewPosition;
	}

	inline void						SetViewDirection( const DirectX::XMVECTOR viewDirection )
	{
#ifdef _DEBUG
		const DirectX::XMVECTOR vecLength = DirectX::XMVector3LengthEst( viewDirection );
		ASSERT( ( vecLength.m128_f32[ 0 ] < ( 1.0f + Epsilon ) ) && ( vecLength.m128_f32[ 0 ] > ( 1.0f - Epsilon ) ), "View direction vector isn't normalised.\n" );
#endif
		m_viewDirection = viewDirection;
	}

private:
	DeviceResources*				m_deviceResources;
	ID3D11Buffer*					m_vpConstantBuffer;

	DirectX::XMMATRIX				m_viewMatrix;
	DirectX::XMMATRIX				m_projectionMatrix;

	DirectX::XMVECTOR				m_viewPosition;
	DirectX::XMVECTOR				m_viewDirection;

	DirectX::XMFLOAT4				m_ambient;
	DirectX::XMVECTOR				m_dirLightDir;
	DirectX::XMFLOAT4				m_dirLightCol;
};

} // namespace DX