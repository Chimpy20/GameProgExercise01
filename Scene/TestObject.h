#pragma once

namespace scene
{

class TestObject
{
public:
	TestObject();

	void				Initialise();
	void				Shutdown();

	void				Update();
	void				Render();

private:
	static const UINT NumVertices = 3;

	ID3D11InputLayout*	m_inputLayout;
	ID3D11Buffer*		m_vertexBuffer;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader*	m_pixelShader;

};

} // namespace scene
