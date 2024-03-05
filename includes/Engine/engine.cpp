/*
    nombre: Motor gráfico 3D (Implementación)
    autores: Bennett Harnisch y David Emmanuel Santana Romero (adaptación)
*/

#include "engine.h"
vec3 :: vec3 (float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}
vec3::~vec3 () {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 0;
}
tri3 :: tri3(vec3 p1, vec3 p2, vec3 p3, const float red, const float green, const float blue, float alpha) {
    this->p[0] = p1;
    this->p[1] = p2;
    this->p[2] = p3;
    this->color_A[0] = red;
    this->color_A[1] = green;
    this->color_A[2] = blue;
    this->color_A[3] = alpha;
    this->color_B[0] = red;
    this->color_B[1] = green;
    this->color_B[2] = blue;
    this->color_B[3] = alpha;
    this->color_C[0] = red;
    this->color_C[1] = green;
    this->color_C[2] = blue;
    this->color_C[3] = alpha;
}
tri3 :: tri3(vec3 p1, vec3 p2, vec3 p3, const float color1 [4], const float color2 [4], const float color3 [4]) {
    this->p[0] = p1;
    this->p[1] = p2;
    this->p[2] = p3;
    this->color_A[0] = color1[0];
    this->color_A[1] = color1[1];
    this->color_A[2] = color1[2];
    this->color_A[3] = color1[3];
    this->color_B[0] = color2[0];
    this->color_B[1] = color2[1];
    this->color_B[2] = color2[2];
    this->color_B[3] = color2[3];
    this->color_C[0] = color3[0];
    this->color_C[1] = color3[1];
    this->color_C[2] = color3[2];
    this->color_C[3] = color3[3];
}
tri3::~tri3() {
    this->color_A[0] = 0;
    this->color_A[1] = 0;
    this->color_A[2] = 0;
    this->color_A[3] = 0;
    this->color_B[0] = 0;
    this->color_B[1] = 0;
    this->color_B[2] = 0;
    this->color_B[3] = 0;
    this->color_C[0] = 0;
    this->color_C[1] = 0;
    this->color_C[2] = 0;
    this->color_C[3] = 0;
}
bool mesh :: LoadObj(string Filename) {
    ifstream file(Filename);
    if (!file.is_open())
        return false;

    vector<vec3> vertices;

    while (!file.eof())
    {
        char line[128];
        file.getline(line, 128);

        std :: stringstream stream;
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

mat4::~mat4() {
    m[0][0] = 0;
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = 0;
    m[1][0] = 0;
    m[1][1] = 0;
    m[1][2] = 0;
    m[1][3] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 0;
    m[2][3] = 0;
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 0;
}

vec3 LinearAlgebra :: MatxVec (mat4 &m, vec3 &i) {
	vec3 v;
	v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
	v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
	v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
	v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
	return v;
}

vec3 LinearAlgebra :: AddVec (vec3 &v1, vec3 &v2) {
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}
vec3 LinearAlgebra :: SubVec (vec3 &v1, vec3 &v2) {
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}
vec3 LinearAlgebra :: VecxScalar (vec3 &v1, float k) {
	return { v1.x * k, v1.y * k, v1.z * k };
}
vec3 LinearAlgebra :: VecdScalar (vec3 &v1, float k) {
	return { v1.x / k, v1.y / k, v1.z / k };
}
float LinearAlgebra :: Dot (vec3 &v1, vec3 &v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
float LinearAlgebra :: Magnitude (vec3 &v) {
	return sqrtf(Dot(v, v));
}
vec3 LinearAlgebra :: Norm (vec3 &v) {
	float l = Magnitude(v);
	return { v.x / l, v.y / l, v.z / l };
}
vec3 LinearAlgebra :: Cross (vec3 &v1, vec3 &v2) {
	vec3 v3;
	v3.x = v1.y * v2.z - v1.z * v2.y;
	v3.y = v1.z * v2.x - v1.x * v2.z;
	v3.z = v1.x * v2.y - v1.y * v2.x;
	return v3;
}
vec3 LinearAlgebra :: VectorIntersectPlane (vec3 &plane_p, vec3 &plane_n, vec3 &lineStart, vec3 &lineEnd) {
	plane_n = Norm(plane_n);
	float plane_d = -Dot(plane_n, plane_p);
	float ad = Dot(lineStart, plane_n);
	float bd = Dot(lineEnd, plane_n);
	float t = (-plane_d - ad) / (bd - ad);
	vec3 lineStartToEnd = SubVec(lineEnd, lineStart);
	vec3 lineToIntersect = VecxScalar(lineStartToEnd, t);
	return AddVec(lineStart, lineToIntersect);
}

float LinearAlgebra :: dist (vec3 &plane_p, vec3 &plane_n, vec3 &p) {
    vec3 n = Norm(p);
    return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - Dot(plane_n, plane_p));
};

int LinearAlgebra :: ClipTriangleAgainstPlane (vec3 plane_p, vec3 plane_n, tri3 &in_tri, tri3 &out_tri1, tri3 &out_tri2) {
	plane_n = Norm(plane_n);

	
	vec3* inside_points[3];  int nInsidePointCount = 0;
	vec3* outside_points[3]; int nOutsidePointCount = 0;

	float d0 = dist(plane_p, plane_n, in_tri.p[0]);
	float d1 = dist(plane_p, plane_n, in_tri.p[1]);
	float d2 = dist(plane_p, plane_n, in_tri.p[2]);

	if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[0]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.p[0]; }
	if (d1 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[1]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.p[1]; }
	if (d2 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[2]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.p[2]; }

	if (nInsidePointCount == 0)
	{
		return 0;
	}

	if (nInsidePointCount == 3)
	{
		out_tri1 = in_tri;

		return 1;
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2)
	{
		out_tri1.color_A[0] = in_tri.color_A[0];
        out_tri1.color_B[0] = in_tri.color_B[0];
        out_tri1.color_C[0] = in_tri.color_C[0];
        out_tri1.color_A[1] = in_tri.color_A[1];
        out_tri1.color_B[1] = in_tri.color_B[1];
        out_tri1.color_C[1] = in_tri.color_C[1];
        out_tri1.color_A[2] = in_tri.color_A[2];
        out_tri1.color_B[2] = in_tri.color_B[2];
        out_tri1.color_C[2] = in_tri.color_C[2];
        out_tri1.color_A[3] = in_tri.color_A[3];
        out_tri1.color_B[3] = in_tri.color_B[3];
        out_tri1.color_C[3] = in_tri.color_C[3];

		out_tri1.p[0] = *inside_points[0];

		out_tri1.p[1] = VectorIntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
		out_tri1.p[2] = VectorIntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1]);

		return 1;
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{

		out_tri1.color_A[0] = in_tri.color_A[0];
        out_tri1.color_B[0] = in_tri.color_B[0];
        out_tri1.color_C[0] = in_tri.color_C[0];
        out_tri1.color_A[1] = in_tri.color_A[1];
        out_tri1.color_B[1] = in_tri.color_B[1];
        out_tri1.color_C[1] = in_tri.color_C[1];
        out_tri1.color_A[2] = in_tri.color_A[2];
        out_tri1.color_B[2] = in_tri.color_B[2];
        out_tri1.color_C[2] = in_tri.color_C[2];
        out_tri1.color_A[3] = in_tri.color_A[3];
        out_tri1.color_B[3] = in_tri.color_B[3];
        out_tri1.color_C[3] = in_tri.color_C[3];

		out_tri2.color_A[0] = in_tri.color_A[0];
        out_tri2.color_B[0] = in_tri.color_B[0];
        out_tri2.color_C[0] = in_tri.color_C[0];
        out_tri2.color_A[1] = in_tri.color_A[1];
        out_tri2.color_B[1] = in_tri.color_B[1];
        out_tri2.color_C[1] = in_tri.color_C[1];
        out_tri2.color_A[2] = in_tri.color_A[2];
        out_tri2.color_B[2] = in_tri.color_B[2];
        out_tri2.color_C[2] = in_tri.color_C[2];
        out_tri2.color_A[3] = in_tri.color_A[3];
        out_tri2.color_B[3] = in_tri.color_B[3];
        out_tri2.color_C[3] = in_tri.color_C[3];

		out_tri1.p[0] = *inside_points[0];
		out_tri1.p[1] = *inside_points[1];
		out_tri1.p[2] = VectorIntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);

		out_tri2.p[0] = *inside_points[1];
		out_tri2.p[1] = out_tri1.p[2];
		out_tri2.p[2] = VectorIntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0]);

		return 2;
	}
    return 0;
}

mat4 LinearAlgebra :: IdentityMatrix () {
	mat4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}
mat4 LinearAlgebra :: XRotationMatrix (float ThetaRad) {
	mat4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = cosf(ThetaRad);
	matrix.m[1][2] = sinf(ThetaRad);
	matrix.m[2][1] = -sinf(ThetaRad);
	matrix.m[2][2] = cosf(ThetaRad);
	matrix.m[3][3] = 1.0f;
	return matrix;
}
mat4 LinearAlgebra :: YRotationMatrix (float ThetaRad) {
	mat4 matrix;
	matrix.m[0][0] = cosf(ThetaRad);
	matrix.m[0][2] = sinf(ThetaRad);
	matrix.m[2][0] = -sinf(ThetaRad);
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = cosf(ThetaRad);
	matrix.m[3][3] = 1.0f;
	return matrix;
}
mat4 LinearAlgebra :: ZRotationMatrix (float ThetaRad) {
	mat4 matrix;
	matrix.m[0][0] = cosf(ThetaRad);
	matrix.m[0][1] = sinf(ThetaRad);
	matrix.m[1][0] = -sinf(ThetaRad);
	matrix.m[1][1] = cosf(ThetaRad);
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}
mat4 LinearAlgebra :: TranslationMatrix (float x, float y, float z) {
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
mat4 LinearAlgebra :: ProjectionMatrix (float FOVdeg, float Aspect, float CameraDistance, float RenderDistance) {
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
mat4 LinearAlgebra :: MatrixMultiply (mat4 &m1, mat4 &m2) {
	mat4 matrix;
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
	return matrix;
}
mat4 LinearAlgebra :: PointingMatrix (vec3 &pos, vec3 &target, vec3 &up) {
	
	vec3 newForward = SubVec(target, pos);
	newForward = Norm(newForward);

	
	vec3 a = VecxScalar(newForward, Dot(up, newForward));
	vec3 newUp = SubVec(up, a);
	newUp = Norm(newUp);

	
	vec3 newRight = Cross(newUp, newForward);

	
	mat4 matrix;
	matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
	return matrix;

}
mat4 LinearAlgebra :: MatrixQuickInverse (mat4 &m) {
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