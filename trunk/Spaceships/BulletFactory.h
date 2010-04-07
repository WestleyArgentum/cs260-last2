#pragma once
class BulletFactory
{
public:
	BulletFactory(void);
	virtual ~BulletFactory(void);
	GameObject* GetNewObject();
	GameObject* GetBullet(float x, float y, float rotation, GameObject* owner, Controller* owningController);
	virtual void SetBulletStats(int colRadius, int dmg, IRenderable* renderable);
private:
	IRenderable* bulletRenderable;
	int collisionRadius;
	int damage;

};

