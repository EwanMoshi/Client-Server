#pragma once

#define CLASS_IDENTIFICATION( code, klass) \
enum { klassId = code }; \
virtual uint32_t getClassId() const { return klassId; } \
static GameObject* createInstance() { return static_cast<GameObject*>( new klass() ); } \

class GameObject
{
public:
	CLASS_IDENTIFICATION('GOBJ', GameObject)

	GameObject();
	virtual void update(float deltaTime);
	virtual void render();

	void setNetworkId(int newNetworkId);
	int getNetworkId() const;

	void setIndexInWorld(int newIndex);
	int getIndexInWorld() const;

	// TODO: add partial state rep using mask
	virtual uint32_t write(OutputBitStream& outputStream) const;
	virtual void read(InputBitStream& inputStream);

	void setLocation(uint32_t x, uint32_t y);

	uint32_t getX() const;
	uint32_t getY() const;

private:
	uint32_t x = 0;
	uint32_t y = 0;
	int networkId;

	int indexInWorld;
};

