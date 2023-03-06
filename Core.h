#pragma once

#include "DX\DeviceResources.h"

// Forward declarations
namespace DX
{
	class View;
	class Input;
}

namespace scene
{
	class Scene;
}

// Handles everything in the demo, the "root" object in some ways
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

	DX::Input*				GetInput() const
	{
		return m_input;
	}

	DX::View*				GetView() const
	{
		return m_view;
	}

	scene::Scene* GetScene() const
	{
		return m_scene;
	}

	bool					Initialise( HWND window, int width, int height );
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
	DX::Input*				m_input; // Wraps input handling

	scene::Scene*			m_scene; // An object that contains all the game world entities
};
