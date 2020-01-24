#ifndef vec3_h
#define vec3_h

#include <cmath>

using namespace std;

class vec3{
public:
		float  x;
    float  y;
    float  z;

  vec3( float x, float y, float z ) : x(x), y(y), z(z) {}
	vec3(void): x(0),y(0),z(0){}

	// non modifying arthimetic operators

    vec3 operator - () const  // unary minus operator
	{ return vec3( -x, -y, -z ); }

    vec3 operator + ( const vec3& vec ) const
	{ return vec3( x + vec.x, y + vec.y, z + vec.z ); }

    vec3 operator - ( const vec3& vec ) const
	{ return vec3( x - vec.x, y - vec.y, z - vec.z ); }

    vec3 operator * ( const float s ) const
	{ return vec3( s*x, s*y, s*z ); }

    vec3 operator * ( const vec3& vec ) const
	{ return vec3( x*vec.x, y*vec.y, z*vec.z ); }

	float dot(const vec3& vec) const
	{ return float(x*vec.x + y*vec.y + z*vec.z); }

	inline float dot(const vec3& v1,const vec3& v2) {
		return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
	}

	float norm() const
	{ return float(sqrt(dot(*this)));	}

	float norm2() const
	{ return float(dot(*this));	} //norm 2 just equals to the dot this

	vec3 normalize()
	{
		return vec3((*this)*(1/norm()));
	}

	vec3 scale (float s){
		vec3 v;
		v.x = x*s;
		v.y = y*s;
		v.z = z*s;
		return v;
	}

};

#endif
