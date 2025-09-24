#include "raytracer.h"

Vec3 vec3(double x, double y, double z) {
    Vec3 v = {x, y, z};
    return v;
}

Vec3 vec3_add(Vec3 a, Vec3 b) {
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 vec3_sub(Vec3 a, Vec3 b) {
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3 vec3_mul(Vec3 v, double t) {
    return vec3(v.x * t, v.y * t, v.z * t);
}

Vec3 vec3_div(Vec3 v, double t) {
    return vec3_mul(v, 1.0 / t);
}

double vec3_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

double vec3_length(Vec3 v) {
    return sqrt(vec3_dot(v, v));
}

Vec3 vec3_normalize(Vec3 v) {
    return vec3_div(v, vec3_length(v));
}

Ray ray(Vec3 origin, Vec3 direction) {
    Ray r = {origin, direction};
    return r;
}

bool sphere_hit(Sphere sphere, Ray r, double t_min, double t_max, double* t) {
    Vec3 oc = vec3_sub(r.origin, sphere.center);
    double a = vec3_dot(r.direction, r.direction);
    double b = 2.0 * vec3_dot(oc, r.direction);
    double c = vec3_dot(oc, oc) - sphere.radius * sphere.radius;
    double discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0) return false;
    
    double sqrt_discriminant = sqrt(discriminant);
    double root = (-b - sqrt_discriminant) / (2.0 * a);
    
    if (root < t_min || root > t_max) {
        root = (-b + sqrt_discriminant) / (2.0 * a);
        if (root < t_min || root > t_max)
            return false;
    }
    
    *t = root;
    return true;
}

Vec3 ray_color(Ray r, Sphere* spheres, int sphere_count) {
    double closest_t = INFINITY;
    int hit_sphere = -1;
    
    for (int i = 0; i < sphere_count; i++) {
        double t;
        if (sphere_hit(spheres[i], r, 0.001, INFINITY, &t)) {
            if (t < closest_t) {
                closest_t = t;
                hit_sphere = i;
            }
        }
    }
    
    if (hit_sphere >= 0) {
        return spheres[hit_sphere].color;
    }
    
    Vec3 unit_direction = vec3_normalize(r.direction);
    double t = 0.5 * (unit_direction.y + 1.0);
    Vec3 white = vec3(1.0, 1.0, 1.0);
    Vec3 blue = vec3(0.5, 0.7, 1.0);
    return vec3_add(vec3_mul(white, 1.0 - t), vec3_mul(blue, t));
}