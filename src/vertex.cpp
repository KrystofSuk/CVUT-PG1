#include "vertex.h"

Vertex::Vertex()
{

}

Vertex::Vertex(const Vec3& _position) : position(_position)
{

}

Vertex::Vertex(const Vec3& _position, const Vec3& _normal) : position(_position), normal(_normal)
{

}

Vec3 Vertex::RecalculateNormal(const Vertex& _vertex1, const Vertex& _vertex2)
{
	normal = Vec3::Cross(_vertex1.position - position, _vertex2.position - position).normalized();
	return normal;
}

