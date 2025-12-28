#include "pch.h"

#include "Engine/DefaultResources.h"
#include "Player.h"
#include "Utils.h"
#include <Engine/StepTimer.h>
#include "string"
#include "iostream"

using ButtonState = Mouse::ButtonStateTracker::ButtonState;

void Player::GenerateGPUResources(DeviceResources* deviceRes, GameMode mode) {
	camera.SetRotation(Quaternion::CreateFromYawPitchRoll(currentYaw,0,0));
	camera.SetPosition(position);
	this->mode = mode;

}

void Player::Update(float dt, DirectX::Keyboard::State kb, DirectX::Mouse::State ms) {
	keyboardTracker.Update(kb);
	mouseTracker.Update(ms);

	if (mode == SCENE_EDITOR) {

		if (kb.R) {
			position.x = defaultPosition.x;
			position.y = defaultPosition.y;
			position.z = defaultPosition.z;
			currentYaw = 0;

			camera.SetPosition(position);
			camera.SetRotation(Quaternion::CreateFromYawPitchRoll(0, 0, 0));
		}

		// Movements
		float speed = walkSpeed;
		if (kb.LeftShift) speed *= 2;

		Vector3 delta;
		if (kb.Z) delta += Vector3::Forward;
		if (kb.S) delta += Vector3::Backward;
		if (kb.Q) delta += Vector3::Left;
		if (kb.D) delta += Vector3::Right;
		Vector3 move = Vector3::TransformNormal(delta, camera.GetInverseViewMatrix());
		move.y = 0;
		move.Normalize();
		position += move * speed * dt;


		// Rotation
		int rotationSide = (kb.A ? 1 : 0) - (kb.E ? 1 : 0);
		if (rotationSide != 0) {
			currentYaw += rotationSide * dt * 0.8f;
			if (currentYaw < -360.0f) currentYaw += 360.f;
			else if (currentYaw > 360.0f) currentYaw -= 360.f;

			camera.SetRotation(Quaternion::CreateFromYawPitchRoll(currentYaw, 0, 0));
		}

		// The camera's position

		camera.SetPosition(position);
	}


	// Raycast
	float mouseX = ms.x;
	float mouseY = ms.y;
	float a_width = screenWidth;
	float a_height = screenHeight;
	float a_nearZ = 0.01f;
	float a_farZ = 500.0f;

	Vector3 pickRayViewSpace;
	pickRayViewSpace.x = (((2.0f * ms.x) / screenWidth) - 1) / camera.GetProjectionMatrix()(0, 0);
	pickRayViewSpace.y = -(((2.0f * ms.y) / screenHeight) - 1) / camera.GetProjectionMatrix()(1, 1);
	pickRayViewSpace.z = 1.0f;

	Vector3 pickRayViewSpaceDir(pickRayViewSpace.x, pickRayViewSpace.y, pickRayViewSpace.z);

	pickRayViewSpaceDir = XMVector3TransformNormal(pickRayViewSpaceDir, camera.GetViewMatrix());
	pickRayViewSpaceDir.z *= -1.0f;

	auto cubes = Raycast(camera.GetPosition(), pickRayViewSpaceDir, 100);
	for (int i = 0; i < cubes.size(); i++) {
	}
}

void Player::Draw(DeviceResources* deviceRes) {
	auto gpuRes = DefaultResources::Get();
}

void Player::Reset()
{

}

void Player::SetScreenSize(int width, int height)
{
	screenHeight = height;
	screenWidth = width;
}

void Player::Im(DX::StepTimer const& timer)
{

}
