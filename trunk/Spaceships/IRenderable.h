#pragma once
// ok, I really hate c++ right about now. . .
class View;

struct Vertex
{
	// Position of vertex in 3D space
	float x, y, z; 
	// Color of vertex
    DWORD color;   
};

class IRenderable
{
public:
	
	virtual void RenderObject(View* view) = 0;	

	// calculate center of this sprite
	virtual D3DXVECTOR3 GetCenter() = 0;

	virtual void SetLocalCoordinates(D3DXVECTOR3 localCoords) = 0;

	// object's position in world space (used for world transform.  that's right, I said world transform.
	// someone's been reading about 3D graphics)	
	virtual void SetPosition(float xValue, float yValue) = 0;

	// sets this object's rotation.  Rotation value should be between 0 and 2*PI
	virtual void SetRotation(float rotationValue) = 0;

	virtual void Add(IRenderable* render) = 0;

	virtual void SetOwner(GameObject* owner) = 0;
};

