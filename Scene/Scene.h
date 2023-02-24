#pragma once

class Scene
{
public:
	Scene();

	void				Update();
	void				Render();

private:

	ID3D11Buffer*      m_constantBuffer;
};
