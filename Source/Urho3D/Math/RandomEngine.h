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

#pragma once

#include "../Math/BoundingBox.h"
#include "../Math/Quaternion.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"

#include <EASTL/array.h>
#include <EASTL/span.h>
#include <EASTL/string.h>

#include <random>

namespace Urho3D
{

/// Random number generator engine.
class URHO3D_API RandomEngine
{
public:
    /// Underlying STD random engine type.
    using EngineType = std::minstd_rand;
    /// Max number of tries to produce "good" random values.
    static const unsigned MaxIterations = 8;

    /// Construct with seed.
    RandomEngine(unsigned seed = 1);
    /// Construct from state.
    RandomEngine(const ea::string& state);
    /// Return default thread-local random engine.
    static RandomEngine& GetDefault();

    /// Load state from string.
    void Load(const ea::string& state);
    /// Save state as string.
    ea::string Save() const;

    /// Return min random number returned by Random().
    static const unsigned MinValue() { return EngineType::min(); }
    /// Return max random number returned by Random().
    static const unsigned MaxValue() { return EngineType::max(); }

    /// Return random integer number in range [0, 2147483646] with uniform distribution.
    unsigned Random() { return engine_(); }

    /// Random shuffle range.
    template <class RandomIter>
    void RandomShuffle(RandomIter first, RandomIter last)
    {
        const auto n = last - first;
        for (auto i = n - 1; i > 0; --i)
        {
            using std::swap;
            const unsigned j = RandomUint(0u, static_cast<unsigned>(i + 1));
            swap(first[i], first[j]);
        }
    }

    /// Return random double in range [0, 1] with uniform distribution.
    double RandomDouble() { return (Random() - MinValue()) / static_cast<double>(MaxValue()); }
    /// Return random double in range [min, max] with uniform distribution.
    double RandomDouble(double min, double max) { return RandomDouble() * (max - min) + min; }

    /// Return random boolean with given probability.
    bool RandomBool(float probability) { return RandomDouble() <= probability && probability != 0.0f; }

    /// Return random int in exclusive range [min, max) with uniform distribution.
    int RandomInt(int min, int max);
    /// Return random unsigned int in exclusive range [min, max) with uniform distribution.
    unsigned RandomUint(unsigned min, unsigned max);

    /// Return random float in range [0, 1] with uniform distribution.
    float RandomFloat() { return static_cast<float>(RandomDouble()); }
    /// Return random float in range [min, max] with uniform distribution.
    float RandomFloat(float min, float max) { return static_cast<float>(RandomDouble(min, max)); }

    /// Return pair of random floats with standard normal distribution.
    ea::pair<float, float> RandomStandardNormalFloatPair();
    /// Return random float with standard normal distribution.
    float RandomStandardNormalFloat() { return RandomStandardNormalFloatPair().first; }

    /// Return random 2D direction (normalized).
    Vector2 RandomDirectionVector2();
    /// Return random 3D direction (normalized).
    Vector3 RandomDirectionVector3();
    /// Return random quaternion (normalized).
    Quaternion RandomQuaternion();
    /// Return random 2D vector in 2D volume.
    Vector2 RandomVector2(const Vector2& min, const Vector2& max);
    /// Return random 3D vector in 3D volume.
    Vector3 RandomVector3(const Vector3& min, const Vector3& max);
    /// Return random 3D vector in 3D volume.
    Vector3 RandomVector3(const BoundingBox& boundingBox) { return RandomVector3(boundingBox.min_, boundingBox.max_); }

private:
    /// Return random array of floats with standard normal distribution.
    void RandomStandardNormalFloatArray(ea::span<float> array);
    /// Return random N-dimensional direction (normalized).
    void RandomDirection(ea::span<float> direction);

    /// Underlying engine.
    EngineType engine_;
};

}
