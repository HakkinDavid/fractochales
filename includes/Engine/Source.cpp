/*
    nombre: Motor gráfico 3D
    autores: Bennett Harnisch y David Emmanuel Santana Romero (adaptación)
*/

#include "../SFML/Graphics.hpp"
#include "../SFML/System.hpp"
#include "../SFML/Audio.hpp"
#include "../SFML/Window.hpp"
#include <sstream>
#include <fstream>
#include <list>
#include <cmath>
using namespace std;

//Declare WindowSettings
class RenderSettings
{
    public:
	bool Watermark;
	int x_res;
	int y_res;
	int depth;
	float FOV;
	float x_sens;
	float y_sens;
};

//Declare 3D Vector Format
class vec3
{
    public:
	float x = 0, y = 0, z = 0, w = 1;
    vec3 (float x = 0, float y = 0, float z = 0, float w = 1) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
};

//Declare 3D Polygon Format
class tri3
{
    public:
	vec3 p[3];
	int color;
    tri3(vec3 p1 = vec3(), vec3 p2 = vec3(), vec3 p3 = vec3(), int color = 0) {
        this->p[0] = p1;
        this->p[1] = p2;
        this->p[2] = p3;
        this->color = color;
    }
};

//Declare 3D Mesh Format
class mesh
{
    public:
	vector<tri3> polys;

	bool LoadObj(string Filename)
	{
		ifstream file(Filename);
		if (!file.is_open())
			return false;

		vector<vec3> vertices;

		while (!file.eof())
		{
			char line[128];
			file.getline(line, 128);

			std::stringstream stream;
			stream << line;

			char junk;
			if (line[0] == 'v')
			{
				vec3 v;
				stream >> junk >> v.x >> v.y >> v.z;
				vertices.push_back(v);
			}
			if (line[0] == 'f')
			{
				int f[3];
				stream >> junk >> f[0] >> f[1] >> f[2];
				polys.push_back({ vertices[f[0] - 1], vertices[f[1] - 1], vertices[f[2] - 1] });
			}

		}

		return true;
	}

};

//Declare 4x4 Matrix Format
class mat4
{
    public:
	float m[4][4] = { 0 };
};

//Function for Multiplying Vectors by Matrix
vec3 MatxVec(mat4 &m, vec3 &i)
{
	vec3 v;
	v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
	v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
	v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
	v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
	return v;
}

//Functions for Manipulating Vectors
vec3 AddVec(vec3 &v1, vec3 &v2)
{
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}
vec3 SubVec(vec3 &v1, vec3 &v2)
{
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}
vec3 VecxScalar(vec3 &v1, float k)
{
	return { v1.x * k, v1.y * k, v1.z * k };
}
vec3 VecdScalar(vec3 &v1, float k)
{
	return { v1.x / k, v1.y / k, v1.z / k };
}
float Dot(vec3 &v1, vec3 &v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
float Magnitude(vec3 &v)
{
	return sqrtf(Dot(v, v));
}
vec3 Norm(vec3 &v)
{
	float l = Magnitude(v);
	return { v.x / l, v.y / l, v.z / l };
}
vec3 Cross(vec3 &v1, vec3 &v2)
{
	vec3 v3;
	v3.x = v1.y * v2.z - v1.z * v2.y;
	v3.y = v1.z * v2.x - v1.x * v2.z;
	v3.z = v1.x * v2.y - v1.y * v2.x;
	return v3;
}
vec3 VectorIntersectPlane(vec3 &plane_p, vec3 &plane_n, vec3 &lineStart, vec3 &lineEnd)
{
	plane_n = Norm(plane_n);
	float plane_d = -Dot(plane_n, plane_p);
	float ad = Dot(lineStart, plane_n);
	float bd = Dot(lineEnd, plane_n);
	float t = (-plane_d - ad) / (bd - ad);
	vec3 lineStartToEnd = SubVec(lineEnd, lineStart);
	vec3 lineToIntersect = VecxScalar(lineStartToEnd, t);
	return AddVec(lineStart, lineToIntersect);
}

int ClipTriangleAgainstPlane(vec3 plane_p, vec3 plane_n, tri3 &in_tri, tri3 &out_tri1, tri3 &out_tri2)
{
	plane_n = Norm(plane_n); //Make sure plane normal is unit vector

	auto dist = [&](vec3 &p)
	{
		vec3 n = Norm(p);
		return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - Dot(plane_n, plane_p)); //Return signed distance from point to plane
	};

	//Designate points as in plane or out of plane based on whether signed distance is positive or negative
	vec3* inside_points[3];  int nInsidePointCount = 0;
	vec3* outside_points[3]; int nOutsidePointCount = 0;

	float d0 = dist(in_tri.p[0]);
	float d1 = dist(in_tri.p[1]);
	float d2 = dist(in_tri.p[2]);

	if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[0]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.p[0]; }
	if (d1 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[1]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.p[1]; }
	if (d2 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[2]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.p[2]; }

	if (nInsidePointCount == 0)
	{
		return 0; //Triangle is completely outside of plane, destroy it
	}

	if (nInsidePointCount == 3)
	{
		out_tri1 = in_tri;

		return 1; //All three points are valid, return the original triangle
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2)
	{
		out_tri1.color = in_tri.color;

		out_tri1.p[0] = *inside_points[0];

		out_tri1.p[1] = VectorIntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
		out_tri1.p[2] = VectorIntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1]);

		return 1; //One point is valid, clip the triangle down to a new triangle and return the new triangle
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{

		out_tri1.color = in_tri.color;

		out_tri2.color = in_tri.color;

		out_tri1.p[0] = *inside_points[0];
		out_tri1.p[1] = *inside_points[1];
		out_tri1.p[2] = VectorIntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);

		out_tri2.p[0] = *inside_points[1];
		out_tri2.p[1] = out_tri1.p[2];
		out_tri2.p[2] = VectorIntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0]);

		return 2; //Two points are valid, split valid area into two triangles and return those
	}
    return 0;
}

//Functions for Manipulating Matrices
mat4 IdentityMatrix()
{
	mat4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}
mat4 XRotationMatrix(float ThetaRad)
{
	mat4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = cosf(ThetaRad);
	matrix.m[1][2] = sinf(ThetaRad);
	matrix.m[2][1] = -sinf(ThetaRad);
	matrix.m[2][2] = cosf(ThetaRad);
	matrix.m[3][3] = 1.0f;
	return matrix;
}
mat4 YRotationMatrix(float ThetaRad)
{
	mat4 matrix;
	matrix.m[0][0] = cosf(ThetaRad);
	matrix.m[0][2] = sinf(ThetaRad);
	matrix.m[2][0] = -sinf(ThetaRad);
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = cosf(ThetaRad);
	matrix.m[3][3] = 1.0f;
	return matrix;
}
mat4 ZRotationMatrix(float ThetaRad)
{
	mat4 matrix;
	matrix.m[0][0] = cosf(ThetaRad);
	matrix.m[0][1] = sinf(ThetaRad);
	matrix.m[1][0] = -sinf(ThetaRad);
	matrix.m[1][1] = cosf(ThetaRad);
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}
mat4 TranslationMatrix(float x, float y, float z)
{
	mat4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	matrix.m[3][0] = x;
	matrix.m[3][1] = y;
	matrix.m[3][2] = z;
	return matrix;
}
mat4 ProjectionMatrix(float FOVdeg, float Aspect, float CameraDistance, float RenderDistance)
{
	float FOVrad = 1.0f / tanf(FOVdeg * 0.5f / 180.0f * 3.14159f);
	mat4 matrix;
	matrix.m[0][0] = Aspect * FOVrad;
	matrix.m[1][1] = FOVrad;
	matrix.m[2][2] = RenderDistance / (RenderDistance - CameraDistance);
	matrix.m[3][2] = (-RenderDistance * CameraDistance) / (RenderDistance - CameraDistance);
	matrix.m[2][3] = 1.0f;
	matrix.m[3][3] = 0.0f;
	return matrix;
}
mat4 MatrixMultiply(mat4 &m1, mat4 &m2)
{
	mat4 matrix;
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
	return matrix;
}
mat4 PointingMatrix(vec3 &pos, vec3 &target, vec3 &up)
{
	//Calculate new forward direction
	vec3 newForward = SubVec(target, pos);
	newForward = Norm(newForward);

	//Calculate new Up direction
	vec3 a = VecxScalar(newForward, Dot(up, newForward));
	vec3 newUp = SubVec(up, a);
	newUp = Norm(newUp);

	//New Right direction is easy, its just cross product
	vec3 newRight = Cross(newUp, newForward);

	//Construct Dimensioning and Translation Matrix	
	mat4 matrix;
	matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
	return matrix;

}
mat4 MatrixQuickInverse(mat4 &m) //Only for Rotation/Translation Matrices
{
	mat4 matrix;
	matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
	matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
	matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
	matrix.m[3][3] = 1.0f;
	return matrix;
}