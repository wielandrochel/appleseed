
//
// This source file is part of appleseed.
// Visit http://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2010-2013 Francois Beaune, Jupiter Jazz Limited
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

// Interface header.
#include "light.h"

// appleseed.foundation headers.
#include "foundation/math/distance.h"

using namespace foundation;

namespace renderer
{

//
// Light class implementation.
//

struct Light::Impl
{
    Transformd m_transform;

    Impl()
      : m_transform(Transformd::identity())
    {
    }
};

namespace
{
    const UniqueID g_class_uid = new_guid();
}

Light::Light(
    const char*         name,
    const ParamArray&   params)
  : ConnectableEntity(g_class_uid, params)
  , impl(new Impl())
  , m_flags(0)
{
    set_name(name);
}

Light::~Light()
{
    delete impl;
}

void Light::set_transform(const Transformd& transform)
{
    impl->m_transform = transform;
    bump_version_id();
}

const Transformd& Light::get_transform() const
{
    return impl->m_transform;
}

bool Light::on_frame_begin(
    const Project&      project,
    const Assembly&     assembly)
{
    m_flags = 0;

    if (m_params.get_optional<bool>("cast_indirect_light", true))
        m_flags |= CastIndirectLight;

    return true;
}

void Light::on_frame_end(
    const Project&      project,
    const Assembly&     assembly)
{
}

double Light::compute_distance_attenuation(
    const Vector3d&     target,
    const Vector3d&     position) const
{
    return 1.0 / square_distance(target, position);
}

}   // namespace renderer
