#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <math.h>
#include <stdbool.h>

typedef struct {
    double x, y, z;
} Vec3;

typedef struct {
    Vec3 origin;
    Vec3 direction;
} Ray;

typedef struct {
    Vec3 center;
    double radius;
    Vec3 color;
} Sphere;

typedef struct {
    Vec3 position;
    Vec3 at;
    Vec3 up;
    double fov;
} Camera;

Vec3 vec3(double x, double y, double z);
Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_mul(Vec3 v, double t);
Vec3 vec3_div(Vec3 v, double t);
double vec3_dot(Vec3 a, Vec3 b);
Vec3 vec3_normalize(Vec3 v);
double vec3_length(Vec3 v);

Ray ray(Vec3 origin, Vec3 direction);
bool sphere_hit(Sphere sphere, Ray r, double t_min, double t_max, double* t);
Vec3 ray_color(Ray r, Sphere* spheres, int sphere_count);

#endif