//
// Copyright (c) 2017-2020 the rbfx project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "../Math/RandomEngine.h"
#include "../Math/MathDefs.h"

#include <sstream>

namespace Urho3D
{

RandomEngine::RandomEngine(unsigned seed /*= 1*/)
    : engine_(seed)
{
}

RandomEngine::RandomEngine(const ea::string& state)
{
    Load(state);
}

RandomEngine& RandomEngine::GetDefault()
{
    thread_local RandomEngine instance;
    return instance;
}

void RandomEngine::Load(const ea::string& state)
{
    std::istringstream stream{ state.c_str() };
    stream >> engine_;
}

ea::string RandomEngine::Save() const
{
    std::ostringstream stream;
    stream << engine_;
    return stream.str().c_str();
}

int RandomEngine::RandomInt(int min, int max)
{
    assert(min < max);
    std::uniform_int_distribution<int> dist{ min, max - 1 };
    return dist(engine_);
}

unsigned RandomEngine::RandomUint(unsigned min, unsigned max)
{
    assert(min < max);
    std::uniform_int_distribution<unsigned> dist{ min, max - 1 };
    return dist(engine_);
}

ea::pair<float, float> RandomEngine::RandomStandardNormalFloatPair()
{
    const double u1 = RandomDouble(M_EPSILON, 1.0);
    const double u2 = RandomDouble();

    const double z0 = Sqrt(-2.0 * Ln(u1)) * Cos(360.0f * u2);
    const double z1 = Sqrt(-2.0 * Ln(u1)) * Sin(360.0f * u2);
    return ea::make_pair(static_cast<float>(z0), static_cast<float>(z1));
}

void RandomEngine::RandomStandardNormalFloatArray(ea::span<float> array)
{
    const unsigned num = array.size();
    for (unsigned i = 0; i < num / 2; ++i)
    {
        const auto p = RandomStandardNormalFloatPair();
        array[i * 2 + 0] = p.first;
        array[i * 2 + 1] = p.second;
    }
    if (num % 2 != 0)
        array[num - 1] = RandomStandardNormalFloat();
}

void RandomEngine::RandomDirection(ea::span<float> direction)
{
    assert(direction.size() > 0);

    for (unsigned i = 0; i < MaxIterations; ++i)
    {
        RandomStandardNormalFloatArray(direction);

        // Compute length
        float length = 0.0f;
        for (float x : direction)
            length += x * x;

        // Normalize and return if long enough
        if (length > M_EPSILON)
        {
            const float rsqrLength = 1.0f / Sqrt(length);
            for (float& x : direction)
                x *= rsqrLength;
            return;
        }
    }

    // Return default direction
    for (float& x : direction)
        x = 0.0f;
    direction[0] = 1.0f;
}

Vector2 RandomEngine::RandomDirectionVector2()
{
    float dir[2] = {};
    RandomDirection(dir);
    return Vector2(dir[0], dir[1]);
}

Vector3 RandomEngine::RandomDirectionVector3()
{
    float dir[3] = {};
    RandomDirection(dir);
    return Vector3(dir[0], dir[1], dir[2]);

}

Quaternion RandomEngine::RandomQuaternion()
{
    float dir[4] = {};
    RandomDirection(dir);
    return Quaternion(dir[0], dir[1], dir[2], dir[3]);
}

Vector2 RandomEngine::RandomVector2(const Vector2& min, const Vector2& max)
{
    Vector2 result;
    result.x_ = RandomFloat(min.x_, max.x_);
    result.y_ = RandomFloat(min.y_, max.y_);
    return result;
}

Vector3 RandomEngine::RandomVector3(const Vector3& min, const Vector3& max)
{
    Vector3 result;
    result.x_ = RandomFloat(min.x_, max.x_);
    result.y_ = RandomFloat(min.y_, max.y_);
    result.z_ = RandomFloat(min.z_, max.z_);
    return result;
}

}
