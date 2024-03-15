/*
    nombre: Motor gráfico 3D (Encabezado)
    autores: Bennett Harnisch y David Emmanuel Santana Romero (adaptación)
*/
#ifndef ENGINE_H
#define ENGINE_H
#include "../SFML/Graphics.hpp"
#include "../SFML/System.hpp"
#include "../SFML/Audio.hpp"
#include "../SFML/Window.hpp"
#include <sstream>
#include <fstream>
#include <list>
#include <cmath>

#include "engine.h"

//Declare WindowSettings
class RenderSettings {
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
class vec3 {
    public:
	float x = 0, y = 0, z = 0, w = 1;
    vec3 (float x = 0, float y = 0, float z = 0, float w = 1);
    ~vec3 ();
};

//Declare 3D Polygon Format
class tri3 {
    public:
	vec3 p[3];
	float color_A[4];
    float color_B[4];
    float color_C[4];
    float camera_distance;
    tri3(vec3 p1 = vec3(), vec3 p2 = vec3(), vec3 p3 = vec3(), const float red = 255.f, const float green = 255.f, const float blue = 255.f, float alpha = 255.f);
    tri3(vec3 p1, vec3 p2, vec3 p3, const float color1 [4], const float color2 [4], const float color3 [4]);
    ~tri3();
};

//Declare 3D Mesh Format
class mesh {
    public:
	std::vector<tri3> polys;

	bool LoadObj(std::string obj_filename);

};

//Declare 4x4 Matrix Format
class mat4 {
    public:
	float m[4][4] = { 0 };
    ~mat4();
};

namespace LinearAlgebra {
    vec3 MatxVec(mat4 &m, vec3 &i);
    vec3 AddVec(vec3 &v1, vec3 &v2);
    vec3 SubVec(vec3 &v1, vec3 &v2);
    vec3 VecxScalar(vec3 &v1, float k);
    vec3 VecdScalar(vec3 &v1, float k);
    float Dot(vec3 &v1, vec3 &v2);
    float Magnitude(vec3 &v);
    vec3 Norm(vec3 &v);
    vec3 Cross(vec3 &v1, vec3 &v2);
    vec3 VectorIntersectPlane(vec3 &plane_p, vec3 &plane_n, vec3 &lineStart, vec3 &lineEnd);
    float dist (vec3 &plane_p, vec3 &plane_n, vec3 &p);
    float distance (vec3 & p1, vec3 & p2);
    int ClipTriangleAgainstPlane(vec3 plane_p, vec3 plane_n, tri3 &in_tri, tri3 &out_tri1, tri3 &out_tri2);
    mat4 IdentityMatrix();
    mat4 XRotationMatrix(float ThetaRad);
    mat4 YRotationMatrix(float ThetaRad);
    mat4 ZRotationMatrix(float ThetaRad);
    mat4 TranslationMatrix(float x, float y, float z);
    mat4 ProjectionMatrix(float FOVdeg, float Aspect, float CameraDistance, float RenderDistance);
    mat4 MatrixMultiply(mat4 &m1, mat4 &m2);
    mat4 PointingMatrix(vec3 &pos, vec3 &target, vec3 &up);
    mat4 MatrixQuickInverse(mat4 &m);
}

namespace Engine {
    void drawPrism (std::vector<tri3> & output, const vec3 start, const vec3 end, const float thickness, const float color[3]);
    vec3 * createPrismVertices (const vec3 & start, const vec3 & end, const float & thickness);
    void appendPrism (std::vector<tri3> & output, const vec3 * vertex, const float color [3]);
    void drawPrism (std::vector<tri3> & output, const vec3 start, const vec3 end, const float thickness, const float color[3], std::wstringstream & obj_stream, int & vertices_offset);
    void drawMesh (std::vector<tri3> & mesh, std::vector<tri3> & raster_pipeline, RenderSettings & window_settings, mat4 & world_bounds, vec3 & camera_position, mat4 & camera_view_matrix, mat4 & screen_projection_matrix, vec3 & projection_offset);
    void rasterVector (std::vector<tri3> & raster_pipeline, RenderSettings & window_settings, std::vector<tri3> & render_triangles);
    void renderTriangles (std::vector<tri3> & render_triangles, sf::RenderTarget * window);
    void calculateMotion (vec3 & forward_direction, vec3 & look_direction, vec3 & up_direction, vec3 & right_direction, vec3 & fly_up_direction, sf::Time & cycle_time_diff);
    void calculateCameraView (mat4 & y_rotation_matrix, float & y_rotation, mat4 & x_rotation_matrix, float & x_rotation, mat4 & z_rotation_matrix, float & z_rotation, mat4 & movement_matrix, mat4 & world_bounds, vec3 & crosshair, mat4 & camera_x_rotation_matrix, float & camera_x_rotation, mat4 & camera_y_rotation_matrix, float & camera_y_rotation, vec3 & look_direction, vec3 & camera_position, mat4 & camera_position_matrix, vec3 & up_direction, mat4 & camera_view_matrix);
    bool compareDrawOrder (tri3 &t1, tri3 &t2);
}

#endif