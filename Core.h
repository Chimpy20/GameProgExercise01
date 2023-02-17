#pragma once

#include "DX\DeviceResources.h"

class Scene;

class Core final : public DX::IDeviceNotify
{
public:
	Core() noexcept( false );
	~Core();

	void			Initialise( HWND window, int width, int height );
	void			Shutdown();

	void			Update();
	void			Render();

	virtual void	OnDeviceLost() override;
	virtual void	OnDeviceRestored() override;

private:
	void			Clear(); // Clear the screen

	void			CreateDeviceDependentResources();
	void			CreateWindowSizeDependentResources();

	DX::DeviceResources*	m_deviceResources; // The D3D objects

	Scene*			m_scene; // An object that contains all the game world entities
};
