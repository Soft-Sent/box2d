// MIT License

// Copyright (c) 2019 Erin Catto
// Copyright (c) 2013 Google, Inc.

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#ifndef B2_POLYGON_SHAPE_H
#define B2_POLYGON_SHAPE_H

#include "b2_api.h"
#include "b2_shape.h"

struct b2Hull;

/// A solid convex polygon. It is assumed that the interior of the polygon is to
/// the left of each edge.
/// Polygons have a maximum number of vertices equal to b2_maxPolygonVertices.
/// In most cases you should not need many vertices for a convex polygon.
class B2_API b2PolygonShape : public b2Shape
{
public:
	b2PolygonShape();

	/// Implement b2Shape.
	b2Shape* Clone(b2BlockAllocator* allocator) const override;

	/// @see b2Shape::GetChildCount
	int32 GetChildCount() const override;

	/// Create a convex hull from the given array of local points.
	/// The count must be in the range [3, b2_maxPolygonVertices].
	/// @warning the points may be re-ordered, even if they form a convex polygon
	/// @warning if this fails then the polygon is invalid
	/// @returns true if valid
	bool Set(const b2Vec2* points, int32 count);

	/// Create a polygon from a given convex hull (see b2ComputeHull).
	/// @warning the hull must be valid or this will crash or have unexpected behavior
	void Set(const b2Hull& hull);

	/// Build vertices to represent an axis-aligned box centered on the local origin.
	/// @param hx the half-width.
	/// @param hy the half-height.
	void SetAsBox(float hx, float hy);

	/// Build vertices to represent an oriented box.
	/// @param hx the half-width.
	/// @param hy the half-height.
	/// @param center the center of the box in local coordinates.
	/// @param angle the rotation of the box in local coordinates.
	void SetAsBox(float hx, float hy, const b2Vec2& center, float angle);

	/// @see b2Shape::TestPoint
	bool TestPoint(const b2Transform& transform, const b2Vec2& p) const override;

	/// Implement b2Shape.
	/// @note because the polygon is solid, rays that start inside do not hit because the normal is
	/// not defined.
	bool RayCast(b2RayCastOutput* output, const b2RayCastInput& input,
					const b2Transform& transform, int32 childIndex) const override;

	/// @see b2Shape::ComputeAABB
	void ComputeAABB(b2AABB* aabb, const b2Transform& transform, int32 childIndex) const override;

	// @see b2Shape::ComputeDistance
	void ComputeDistance(const b2Transform& xf, const b2Vec2& p, float32* distance, b2Vec2* normal, int32 childIndex) const override;

	/// @see b2Shape::ComputeMass
	void ComputeMass(b2MassData* massData, float density) const override;
	
	// From LiquidFun library
	/// Get the vertex count.
	int32 GetVertexCount() const { return m_count; }

	// From LiquidFun library
	/// Get a vertex by index.
	const b2Vec2& GetVertex(int32 index) const;

	/// Validate convexity. This is a very time consuming operation.
	/// @returns true if valid
	bool Validate() const;

	// From LiquidFun library
#if LIQUIDFUN_EXTERNAL_LANGUAGE_API
public:
	/// Set centroid with direct floats.
	void SetCentroid(float32 x, float32 y);

	/// SetAsBox with direct floats for center.
	/// @see b2Shape::SetAsBox
	void SetAsBox(float32 hx,
		float32 hy,
		float32 centerX,
		float32 centerY,
		float32 angle);
#endif // LIQUIDFUN_EXTERNAL_LANGUAGE_API

	b2Vec2 m_centroid;
	b2Vec2 m_vertices[b2_maxPolygonVertices];
	b2Vec2 m_normals[b2_maxPolygonVertices];
	int32 m_count;
};

// From LiquidFun library
inline const b2Vec2& b2PolygonShape::GetVertex(int32 index) const
{
	b2Assert(0 <= index && index < m_count);
	return m_vertices[index];
}

#if LIQUIDFUN_EXTERNAL_LANGUAGE_API
inline void b2PolygonShape::SetCentroid(float32 x, float32 y)
{
	m_centroid.Set(x, y);
}

inline void b2PolygonShape::SetAsBox(float32 hx,
	float32 hy,
	float32 centerX,
	float32 centerY,
	float32 angle) {
	SetAsBox(hx, hy, b2Vec2(centerX, centerY), angle);
}
#endif // LIQUIDFUN_EXTERNAL_LANGUAGE_API

#endif
