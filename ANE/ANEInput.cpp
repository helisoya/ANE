#include "ANEInput.h"

ANEInput::ANEInput()
{
}


ANEInput::~ANEInput()
{
}

void ANEInput::Initialize()
{
	int i;


	// Initialize all the keys to being released and not pressed.
	for (i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}
}

void ANEInput::KeyDown(unsigned int key)
{
	// If a key is pressed then save that state in the key array.
	m_keys[key] = true;
}

void ANEInput::KeyUp(unsigned int key)
{
	// If a key is pressed then save that state in the key array.
	m_keys[key] = false;
}

bool ANEInput::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}
