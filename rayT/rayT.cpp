#include <iostream>
#include <vector>
#include <memory>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <float.h>
#include <cstring>

#include "vec3.h"

using namespace std;

#define WIN_WIDTH 400
#define WIN_HEIGHT 400
#define FOV 45
#define MAX_DEPTH 5
#define BIAS 0.001
vec3 backgroundrgb = {0.2,0.2,0.2};
float fov_angle = tan(M_PI * 0.5 * FOV / 180);
float aspectRatio = WIN_WIDTH/WIN_HEIGHT;

////////////////////////////////////////////////////////////////////////////////
/////// Light Class START //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Light{
public:
  vec3 location;
  vec3 intensity;
  Light(vec3 l, vec3 i) : location(l), intensity(i){}
};

////////////////////////////////////////////////////////////////////////////////
/////// Light Class END ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/////// Ray Class START ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Ray{
public:
  vec3 origin;
  vec3 direction;
  float t;

  Ray(vec3 o, vec3 d) : origin(o), direction(d), t(FLT_MAX) {};
};
////////////////////////////////////////////////////////////////////////////////
/////// Ray Class END //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/////// Sphere Class START /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class Sphere{
public:
  vec3 center;
  float radius;
  vec3 diffuse_rgb;
  vec3 specular_rgb;
  float q;  // specular exp
  float kt; // reflection coef
  float kr; // refraction coef
  float refraction; // index of refraction

  Sphere(vec3 c, float r, vec3 drgb, vec3 srgb, float qq, float krr, float ktt, float rr) :
  center(c),radius(r),diffuse_rgb(drgb),specular_rgb(srgb),q(qq),kt(ktt),kr(krr),refraction(rr) {}

  bool intersect(Ray &ray){
    vec3 oc = ray.origin - center;
    float a = ray.direction.dot(ray.direction);
    float b = 2*ray.direction.dot(oc);
    float c = oc.dot(oc) - radius*radius;
    float disc = b*b - 4*a*c;

    if (disc < 0) return false;
    if (disc == 0) ray.t = -0.5*b/a;
    else if(disc > 0){
      float disc2;
      if (b>0) {disc2 = sqrt(disc);}
      else {disc2 = -sqrt(disc);}
      float t0 = -0.5*(b+disc2)/a;
      float t1 = c / (-0.5 * (b+disc2));
      float min_t = ( ((t0 < t1) && (t0 > 0)) || t1 < 0) ? t0 :t1 ;
      if (min_t < 0)  return false;
      ray.t = min_t;
    }
    return true;
  }
};

////////////////////////////////////////////////////////////////////////////////
/////// Sphere Class END ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/////// Plane Class START //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Plane{
public:
  vec3 center;
  vec3 direction;
  vec3 diffuse_rgb;
  vec3 specular_rgb;
  float q;  // specular exp
  float kt; // reflection coef
  float kr; // refraction coef
  float refraction; // index of refraction

  Plane(vec3 c, vec3 n, vec3 drgb, vec3 srgb, float qq, float krr, float ktt, float rr) :
  center(c),direction(n),diffuse_rgb(drgb),specular_rgb(srgb),q(qq),kt(ktt),kr(krr),refraction(rr) {}

  bool intersect(Ray &ray, float &t){
    float max = direction.dot(ray.direction);
    if (max > 1e-7){
      t = (center - ray.origin).dot(direction) / max;
      return (t >= 0);
    }
    return false;
  }

};


////////////////////////////////////////////////////////////////////////////////
/////// Plane Class END ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// vectors to store sphere and lightsources
vector <Sphere> vec_spheres;
vector <Light> vec_lights;
Plane *plane;


////////////////////////////////////////////////////////////////////////////////
/////// Helper functions ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
Sphere *intersectSphere(Ray &ray){
  float min_t = FLT_MAX;
  Sphere *target = nullptr;
  for (int i =0; i < vec_spheres.size(); ++i){
    if (vec_spheres[i].intersect(ray) && ray.t < min_t){
      target = &vec_spheres[i];
      min_t = ray.t;
    }
  }
  if (min_t < FLT_MAX){ ray.t = min_t;  }
  return target;
}

vec3 Refraction_func(const vec3 &direct, const vec3 &norm, const float &refraction_index)
{
    float c1 = direct.dot(norm);
    float n1 = 1, n2 = refraction_index;
    vec3 normal = norm;
    if (c1<0) {c1=-c1;} else {swap(n1,n2); normal= -norm;}
    float n =n1/n2;
    float c2 =1-n*n*(1-c1*c1);
    return c2 < 0 ? vec3(0,0,0) : direct*n+normal*(n*c1-sqrtf(c2));
}

float Fresnel_func( const vec3 &direct, const vec3 &norm, const float &refraction_index){
  float output, a,b;
  float cosf = max(-1.f, min(1.f, direct.dot(norm)));

  if (cosf > 0) {a =refraction_index; b = 1;}
  else{a = 1; b =refraction_index;}

  //Snell's law
  float sint = a/b*sqrtf(max(0.f,1-cosf*cosf));
  //internal reflection
  if (sint >= 1) {return 1; }
  else {
    float cost = sqrtf(max(0.f,1-sint*sint));
    cosf = fabsf(cosf);
    float x = ((b*cosf)-(a*cost)) / ((b*cosf) + (a*cost));
    float y = ((a*cosf)-(b*cost)) / ((a*cosf) + (b*cost));
    output = (x*x+y*y)/2;
    return output;
  }
}
// check if the plane intersects with sphere
int check_plane(Ray &ray){
  float t = FLT_MAX;
  if(plane->intersect(ray,t)){
    if (t < ray.t){
      ray.t = t;
      return 1;
    }
  }
  return 0;
}

// phong lighting equation for spheres and planes
vec3 Phong(Ray &ray, int depth){
  //flag == 1 do phong for plane |||| flag == 0 do phong for sphere//
  if (depth < 1) return backgroundrgb;

  vec3 diff_rgb =  backgroundrgb.scale(0.5);
  vec3 spec_rgb = backgroundrgb.scale(0.5);
  vec3 refl_rgb = {0.0,0.0,0.0};
  vec3 rgb_ret = {0.0,0.0,0.0};
  vec3 offset,reflection_vec;
  int flag = 0;

  if(plane){
    flag = check_plane(ray);
  }
  Sphere *sph = intersectSphere(ray);
  if (sph && (flag == 0)){
    vec3 intersection = ray.origin + ray.direction*ray.t;
    vec3 normal = (intersection - sph->center).normalize();
    if (sph->kt > 0 || sph->kr >0){
      float fres = Fresnel_func(ray.direction,normal,sph->refraction);
      vec3 refraction_vec = Refraction_func(ray.direction,normal,sph->refraction).normalize();

      if (ray.direction.dot(normal) >0) offset = normal * BIAS;
      else offset = -normal * BIAS;

      Ray refraction_ray(intersection+offset,refraction_vec);
      vec3 refraction_rgb = Phong(refraction_ray, depth-1);

      reflection_vec = ray.direction - (normal.scale(2)*ray.direction.dot(normal));
      Ray reflection_ray(intersection-offset,reflection_vec);
      vec3 reflection_rgb = Phong(reflection_ray,depth-1);

      rgb_ret = refraction_rgb*sph->kt *(1-fres) + reflection_rgb*sph->kr*fres;
    }
    for(int i = 0; i<vec_lights.size(); ++i){
      vec3 lightv = (intersection - vec_lights[i].location).normalize();
      vec3 lightInt = vec_lights[i].intensity;
      //change
      Ray shad_ray(intersection + normal * BIAS, -lightv);
      Sphere *shadows = intersectSphere(shad_ray);

      if(!shadows){
        diff_rgb = diff_rgb + lightInt * -lightv.dot(normal);
        refl_rgb = lightv - (normal*2*lightv.dot(normal));
        spec_rgb = spec_rgb + lightInt * pow(max(0.f,refl_rgb.dot(-ray.direction)),sph->q);
      }
    }
    return rgb_ret +diff_rgb*sph->diffuse_rgb  + spec_rgb*sph->specular_rgb;
  }
  if (flag == 1){
    vec3 intersection = ray.origin + ray.direction*ray.t;
    vec3 normal = -plane->direction;

    if(plane->kt >0 || plane-> kr >0){
    float fres = Fresnel_func(ray.direction,normal,plane->refraction);
    vec3 refraction_vec = (Refraction_func(ray.direction,normal,plane->refraction)).normalize();

    if (ray.direction.dot(normal) >0) offset = normal * BIAS;
    else offset = -normal * BIAS;

    Ray refraction_ray(intersection+offset,refraction_vec);
    vec3 refraction_rgb = Phong(refraction_ray,depth-1);


    reflection_vec = ray.direction - (normal*ray.direction.dot(normal)).scale(2);
    Ray reflection_ray(intersection-offset,reflection_vec);
    vec3 reflection_rgb = Phong(reflection_ray,depth-1);
    rgb_ret = refraction_rgb*plane->kt * (1-fres)+reflection_rgb*plane->kr*fres;
  }
    for(int i = 0; i<vec_lights.size(); ++i){
      vec3 lightv = (intersection - vec_lights[i].location).normalize();
      vec3 lightInt = vec_lights[i].intensity;
      Ray shad_ray(intersection + normal * BIAS, -lightv);
      Sphere *shadows = intersectSphere(shad_ray);
      if(!shadows){
        diff_rgb = diff_rgb + lightInt * normal.dot(-lightv);
        refl_rgb = lightv - (normal*2*lightv.dot(normal));
        spec_rgb = spec_rgb + lightInt * pow(max(0.f,refl_rgb.dot(-ray.direction)),plane->q);
      }
    }
    return rgb_ret +diff_rgb*plane->diffuse_rgb +spec_rgb*plane->specular_rgb;
  }
  return backgroundrgb;
}

////////////////////////////////////////////////////////////////////////////////
///// Opengl window ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void render(){
  for(int y = 0; y < int(WIN_HEIGHT); y++){
    for(int x = 0; x < int(WIN_WIDTH); x++){
      float xx = fov_angle* aspectRatio * (2*((x+0.5)/float(WIN_WIDTH)) -1);
      float yy = fov_angle * (1-2*(y+0.5)/float(WIN_HEIGHT));
      Ray input_ray = Ray(vec3(0,0,0),vec3(xx,yy,-1));
      vec3 rgb_out = Phong(input_ray, MAX_DEPTH);
      glColor3f(rgb_out.x,rgb_out.y,rgb_out.z);
      glEnable(GL_POINT_SMOOTH);
      glPointSize(1);
      glBegin(GL_POINTS);
        glVertex2i(x,y);
      glEnd();
    }
  }
  glutSwapBuffers();
}

void init1(){
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluOrtho2D( 0.0, WIN_WIDTH, WIN_HEIGHT,0.0 );
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT);
  // arguements for sphere
  // vec3 center, float radius, vec3 diffuse, vec3 specular, float q, float kt, float kr, float refaction
  vec_lights.push_back(Light(vec3(50,100,100),vec3(1,1,1)));
  vec_spheres.push_back(Sphere(vec3(-8,3,-30),3,vec3(0,0,1),vec3(1,1,1), 1, 0, 0, 0));
  vec_spheres.push_back(Sphere(vec3(-1,3,-30),3,vec3(0,0,1),vec3(1,1,1),  25, 0, 0,   0));
  vec_spheres.push_back(Sphere(vec3(6,3,-30),3,vec3(0,0,1),vec3(1,1,1),  100, 0, 0,   0));
  plane = new Plane(vec3(0,-2,0),vec3(0,-1,0),vec3(0.8,0.8,0.8),vec3(1,1,1),1,1,0,2.5);
}

void init2(){
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluOrtho2D( 0.0, WIN_WIDTH, WIN_HEIGHT,0.0 );
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT);

  vec_lights.push_back(Light(vec3(50,100,100),vec3(1,1,1)));
  vec_spheres.push_back(Sphere(vec3(-5,4,-30),4,vec3(0,0,1),vec3(1,1,1), 100, 1, 0.1, 0.0001));
  vec_spheres.push_back(Sphere(vec3(6,5,-30),5,vec3(0,0,1),vec3(1,1,1),  100, 0.7, 0.1,   0.0001));
  vec_spheres.push_back(Sphere(vec3(-1,16,-60),3,vec3(0,0,1),vec3(1,1,1),  100, 0.3, 0.1,   0.0001));
  plane = new Plane(vec3(0,-2,0),vec3(0,-1,0),vec3(0.8,0.8,0.8),vec3(1,1,1),1,1,0,2.5);
}

void init3(){
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluOrtho2D( 0.0, WIN_WIDTH, WIN_HEIGHT,0.0 );
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT);

  vec_lights.push_back(Light(vec3(50,100,100),vec3(1,1,1)));
  vec_spheres.push_back(Sphere(vec3(-7,4,-30),4,vec3(0,0,1),vec3(1,1,1), 100, 0.5, 0.9, 0.7));
  vec_spheres.push_back(Sphere(vec3(0.5,9,-60),9,vec3(0.0,0.5,0.8),vec3(1,1,1),  100, 0.5, 0.9,   0.5));
  vec_spheres.push_back(Sphere(vec3(25,13,-100),13,vec3(0.4,0.5,0.7),vec3(1,1,1),100,0.1,0.2,0.2));
  plane = new Plane(vec3(0,-2,0),vec3(0,-1,0),vec3(0.8,0.8,0.8),vec3(1,1,1),1,1,0,2.5);
}

void init4(){
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluOrtho2D( 0.0, WIN_WIDTH, WIN_HEIGHT,0.0 );
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT);
  vec_lights.push_back(Light(vec3(50,100,100),vec3(1,1,1)));
  //eyeball
  vec_spheres.push_back(Sphere(vec3(-20,13,-100),13,vec3(0.8,0.8,0.8),vec3(1,1,1), 100, 0, 0.0, 0.0));
  vec_spheres.push_back(Sphere(vec3(-18,14,-95),9,vec3(0.0,0.0,0.0),vec3(1,1,1), 100, 1, 0.1, 0.0001));
  vec_spheres.push_back(Sphere(vec3(-15,24,-110),5,vec3(0.0,0.0,0.0),vec3(1,1,1), 100, 0,0,0));
  vec_spheres.push_back(Sphere(vec3(-23,26,-110),5,vec3(0.0,0.0,0.0),vec3(1,1,1), 100, 0,0,0));
  vec_spheres.push_back(Sphere(vec3(-30,24,-110),5,vec3(0.0,0.0,0.0),vec3(1,1,1), 100, 0,0,0));

  //bird face
  vec_spheres.push_back(Sphere(vec3(25,13,-100),13,vec3(0.4,0.5,0.7),vec3(1,1,1), 100, 0.1, 0.2, 0.2));
  vec_spheres.push_back(Sphere(vec3(30,15,-90),3,vec3(0.0,0.0,0.0),vec3(1,1,1), 100, 0.1, 0.2, 0.2));
  vec_spheres.push_back(Sphere(vec3(18,15,-91),3,vec3(0.0,0.0,0.0),vec3(1,1,1), 100, 0.1, 0.2, 0.2));

  plane = new Plane(vec3(0,-2,0),vec3(0,-1,0),vec3(0.8,0.8,0.8),vec3(1,1,1),1,1,0,2.5);

}
int main(int argc, char **argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowPosition(80, 80);
  glutInitWindowSize(WIN_WIDTH,WIN_HEIGHT);
  glutCreateWindow("Ray Test");
  if (strcmp(argv[1], "1") == 0)	init1();
  else if (strcmp(argv[1], "2") == 0)	init2();
  else if (strcmp(argv[1], "3") == 0)	init3();
  else if (strcmp(argv[1], "4") == 0)	init4();
  else {
    cout << "Scene inputted out of bounds. please use as arguement one of [1,2,3,4]" << endl;
    cout << "./rayT <1,2,3,4>" << endl;
  }
  glutDisplayFunc(render);
  glutIdleFunc(render);
  glutMainLoop();
  return 0;
}
