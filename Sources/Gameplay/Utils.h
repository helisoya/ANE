#pragma once

using namespace DirectX::SimpleMath;

Vector4 ToVec4(const Vector3& v);
Vector4 ToVec4Normal(const Vector3& v);

// Gets the sign of a float
float sign(float v);

// Gets the sign of an int
int signInt(int v);

/// <summary>
/// Casts a char array to a wchar array
/// </summary>
/// <param name="str">The char array</param>
/// <returns>The wchar array</returns>
wchar_t* toWChar(const char* str);

/// <summary>
/// Casts a wchar array to a char array
/// </summary>
/// <param name="str">The wchar array</param>
/// <returns>The char array</returns>
char* toChar(const wchar_t* str);

/// <summary>
/// Starts a raycast
/// </summary> 
/// <param name="pos">The start position</param>
/// <param name="dir">The direction</param>
/// <param name="maxDist">The maximum distance</param>
/// <returns>The points used by the raycast</returns>
std::vector<std::array<int, 3>> Raycast(Vector3 pos, Vector3 dir, float maxDist);