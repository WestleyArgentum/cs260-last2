#pragma once
class IWeapon
{
public:
	virtual void Fire(const float x, const float y, const float rotation, Controller* controller) = 0;

	virtual void Update() = 0;
	
	virtual void SetFireDuration(const int duration) = 0;
	
	//virtual void CeaseFire() = 0;
	;
};

