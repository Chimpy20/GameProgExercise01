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

private:
	DeviceResources*				m_deviceResources;
	ID3D11Buffer*					m_vpConstantBuffer;

	DirectX::XMMATRIX				m_viewMatrix;
	DirectX::XMMATRIX				m_projectionMatrix;
};

} // namespace DX