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
bool mesh :: LoadObj(std::string Filename) {
    std::ifstream file(Filename);
    if (!file.is_open())
        return false;

    std::vector<vec3> vertices;

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

void Engine :: drawMesh (std::vector<tri3> & mesh, std::vector<tri3> & raster_pipeline, RenderSettings & window_settings, mat4 & world_bounds, vec3 & camera_position, mat4 & camera_view_matrix, mat4 & screen_projection_matrix, vec3 & projection_offset) {
    for (tri3 &tri : mesh) {
        tri3 projected_triangles, transformed, viewable_triangles;

        transformed.p[0] = LinearAlgebra::MatxVec(world_bounds, tri.p[0]);
        transformed.p[1] = LinearAlgebra::MatxVec(world_bounds, tri.p[1]);
        transformed.p[2] = LinearAlgebra::MatxVec(world_bounds, tri.p[2]);

        // normalizar el vector
        vec3 normal, line1, line2;

        line1 = LinearAlgebra::SubVec(transformed.p[1], transformed.p[0]);
        line2 = LinearAlgebra::SubVec(transformed.p[2], transformed.p[0]);
        normal = LinearAlgebra::Cross(line1, line2);
        normal = LinearAlgebra::Norm(normal);

        vec3 vision_line = LinearAlgebra::SubVec(transformed.p[0], camera_position);
        // verificar que la línea de visión no sea perpendicular (que se mire de lado)
        if (LinearAlgebra::Dot(normal, vision_line) != 0.0f) {
            // introducir en el marco de la cámara
            viewable_triangles.p[0] = LinearAlgebra::MatxVec(camera_view_matrix, transformed.p[0]);
            viewable_triangles.p[1] = LinearAlgebra::MatxVec(camera_view_matrix, transformed.p[1]);
            viewable_triangles.p[2] = LinearAlgebra::MatxVec(camera_view_matrix, transformed.p[2]);

            // cortar triángulos si el plano de visión está muy próximo
            int n_clipped_triangles = 0;
            tri3 clipped[2];
            n_clipped_triangles = LinearAlgebra::ClipTriangleAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, viewable_triangles, clipped[0], clipped[1]);

            // proyectar los triángulos cortados
            for (int n = 0; n < n_clipped_triangles; n++) {

                // proyección 3D en 2D
                projected_triangles.p[0] = LinearAlgebra::MatxVec(screen_projection_matrix, clipped[n].p[0]);
                projected_triangles.p[1] = LinearAlgebra::MatxVec(screen_projection_matrix, clipped[n].p[1]);
                projected_triangles.p[2] = LinearAlgebra::MatxVec(screen_projection_matrix, clipped[n].p[2]);

                projected_triangles.p[0] = LinearAlgebra::VecdScalar(projected_triangles.p[0], projected_triangles.p[0].w);
                projected_triangles.p[1] = LinearAlgebra::VecdScalar(projected_triangles.p[1], projected_triangles.p[1].w);
                projected_triangles.p[2] = LinearAlgebra::VecdScalar(projected_triangles.p[2], projected_triangles.p[2].w);

                projected_triangles.p[0].x *= -1.0f;
                projected_triangles.p[1].x *= -1.0f;
                projected_triangles.p[2].x *= -1.0f;
                projected_triangles.p[0].y *= -1.0f;
                projected_triangles.p[1].y *= -1.0f;
                projected_triangles.p[2].y *= -1.0f;

                // escalar triángulos al tamaño de la pantalla
                projected_triangles.p[0] = LinearAlgebra::AddVec(projected_triangles.p[0], projection_offset);
                projected_triangles.p[1] = LinearAlgebra::AddVec(projected_triangles.p[1], projection_offset);
                projected_triangles.p[2] = LinearAlgebra::AddVec(projected_triangles.p[2], projection_offset);

                projected_triangles.p[0].x *= 0.5f * window_settings.x_res;
                projected_triangles.p[0].y *= 0.5f * window_settings.y_res;
                projected_triangles.p[1].x *= 0.5f * window_settings.x_res;
                projected_triangles.p[1].y *= 0.5f * window_settings.y_res;
                projected_triangles.p[2].x *= 0.5f * window_settings.x_res;
                projected_triangles.p[2].y *= 0.5f * window_settings.y_res;

                projected_triangles.color_A[0] = tri.color_A[0];
                projected_triangles.color_B[0] = tri.color_B[0];
                projected_triangles.color_C[0] = tri.color_C[0];
                projected_triangles.color_A[1] = tri.color_A[1];
                projected_triangles.color_B[1] = tri.color_B[1];
                projected_triangles.color_C[1] = tri.color_C[1];
                projected_triangles.color_A[2] = tri.color_A[2];
                projected_triangles.color_B[2] = tri.color_B[2];
                projected_triangles.color_C[2] = tri.color_C[2];
                projected_triangles.color_A[3] = tri.color_A[3];
                projected_triangles.color_B[3] = tri.color_B[3];
                projected_triangles.color_C[3] = tri.color_C[3];

                // enviar a rasterizado
                raster_pipeline.push_back(projected_triangles);
            }
        }
    }
}

void Engine :: rasterVector (std::vector<tri3> & raster_pipeline, sf::RenderTarget * window, RenderSettings & window_settings) {
    for (const tri3 &triToRaster : raster_pipeline) {
        tri3 clipped[2];
        std::list<tri3> listTriangles;

        listTriangles.push_back(triToRaster);
        int nNewTriangles = 1;

        for (int p = 0; p < 4; p++) {
            int nTrisToAdd = 0;
            while (nNewTriangles > 0) {
                tri3 test = listTriangles.front();
                listTriangles.pop_front();
                nNewTriangles--;

                switch (p) {
                    case 0:	nTrisToAdd = LinearAlgebra::ClipTriangleAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                    case 1:	nTrisToAdd = LinearAlgebra::ClipTriangleAgainstPlane({ 0.0f, float(window_settings.y_res) - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                    case 2:	nTrisToAdd = LinearAlgebra::ClipTriangleAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                    case 3:	nTrisToAdd = LinearAlgebra::ClipTriangleAgainstPlane({ float(window_settings.x_res) - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                }

                for (int w = 0; w < nTrisToAdd; w++)
                    listTriangles.push_back(clipped[w]);
            }
            nNewTriangles = listTriangles.size();
        }

        for (auto &Final : listTriangles) {
            // dibujar los triángulos finales en la pantalla
            sf::VertexArray poly(sf::Triangles, 3);

            poly[0].position = sf::Vector2f(Final.p[0].x, Final.p[0].y);
            poly[1].position = sf::Vector2f(Final.p[1].x, Final.p[1].y);
            poly[2].position = sf::Vector2f(Final.p[2].x, Final.p[2].y);
            poly[0].color = sf::Color(Final.color_A[0], Final.color_A[1], Final.color_A[2], Final.color_A[3]);
            poly[1].color = sf::Color(Final.color_B[0], Final.color_B[1], Final.color_B[2], Final.color_B[3]);
            poly[2].color = sf::Color(Final.color_C[0], Final.color_C[1], Final.color_C[2], Final.color_C[3]);

            window->draw(poly);
        }
    }
}

void Engine :: calculateMotion (vec3 & forward_direction, vec3 & look_direction, vec3 & up_direction, vec3 & right_direction, vec3 & fly_up_direction, sf::Time & cycle_time_diff) {
    forward_direction = LinearAlgebra::VecxScalar(look_direction, 20.0f*cycle_time_diff.asSeconds());
    up_direction = { 0,0,1 };
    right_direction = LinearAlgebra::Cross(look_direction, up_direction);
    fly_up_direction = LinearAlgebra::Cross(right_direction, look_direction);
    right_direction = LinearAlgebra::VecxScalar(right_direction, 20.0f*cycle_time_diff.asSeconds());
    fly_up_direction = LinearAlgebra::VecxScalar(fly_up_direction, 20.0f*cycle_time_diff.asSeconds());
}

void Engine :: calculateCameraView (mat4 & y_rotation_matrix, float & y_rotation, mat4 & x_rotation_matrix, float & x_rotation, mat4 & z_rotation_matrix, float & z_rotation, mat4 & movement_matrix, mat4 & world_bounds, vec3 & crosshair, mat4 & camera_x_rotation_matrix, float & camera_x_rotation, mat4 & camera_y_rotation_matrix, float & camera_y_rotation, vec3 & look_direction, vec3 & camera_position, mat4 & camera_position_matrix, vec3 & up_direction, mat4 & camera_view_matrix) {
    y_rotation_matrix = LinearAlgebra::ZRotationMatrix(y_rotation);
    x_rotation_matrix = LinearAlgebra::YRotationMatrix(x_rotation);
    z_rotation_matrix = LinearAlgebra::XRotationMatrix(z_rotation);
    movement_matrix = LinearAlgebra::TranslationMatrix(5.0f, 0.0f, 0.0f);
    world_bounds = LinearAlgebra::MatrixMultiply(x_rotation_matrix, y_rotation_matrix);
    world_bounds = LinearAlgebra::MatrixMultiply(world_bounds, z_rotation_matrix);
    world_bounds = LinearAlgebra::MatrixMultiply(world_bounds, movement_matrix);
    crosshair = { 1,0,0 };
    camera_x_rotation_matrix = LinearAlgebra::ZRotationMatrix(-camera_x_rotation);
    camera_y_rotation_matrix = LinearAlgebra::YRotationMatrix(camera_y_rotation);
    camera_x_rotation_matrix = LinearAlgebra::MatrixMultiply(camera_y_rotation_matrix, camera_x_rotation_matrix);
    look_direction = LinearAlgebra::MatxVec(camera_x_rotation_matrix, crosshair);
    crosshair = LinearAlgebra::AddVec(camera_position, look_direction);
    camera_position_matrix = LinearAlgebra::PointingMatrix(camera_position, crosshair, up_direction);
    camera_view_matrix = LinearAlgebra::MatrixQuickInverse(camera_position_matrix);
}