#pragma once

#include "DX\DeviceResources.h"

namespace DX
{
	class View;
}

namespace scene
{
	class Scene;
}

class Core final : public DX::IDeviceNotify
{
public:
	Core() noexcept( false );
	~Core();

	static Core*			Get()
	{
		return g_core;
	}

	const DX::DeviceResources*	GetDeviceResources() const
	{
		return m_deviceResources;
	}

	void					Initialise( HWND window, int width, int height );
	void					Shutdown();

	void					Update();
	void					Render();

	virtual void			OnDeviceLost() override;
	virtual void			OnDeviceRestored() override;

private:
	void					Clear(); // Clear the screen

	void					CreateDeviceDependentResources();
	void					CreateWindowSizeDependentResources();

	static Core*			g_core;

	DX::DeviceResources*	m_deviceResources; // The D3D objects
	DX::View*				m_view; // Code relating the the camera

	scene::Scene*			m_scene; // An object that contains all the game world entities
};
