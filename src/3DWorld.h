// 3D World - OpenGL CS184 Computer Graphics Project
// by Frank Gennari
// 3/10/02

#ifndef _3DWORLD_H_
#define _3DWORLD_H_

#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gl_includes.h"

 // STL include
#include <vector>
#include <deque>
#include <algorithm>
#include <set>
#include <map>
#include <assert.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <memory>
using std::vector;
using std::deque;
using std::set;
using std::map;
using std::swap;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;
using std::min;
using std::max;

#ifndef PI
#define PI 3.141592654
#endif

int      const CAMERA_ID        = -1;
int      const NO_SOURCE        = -2;
unsigned const INIT_CCELL_SIZE  = 16;
float    const LARGE_OBJ_RAD    = 0.01;
float    const TOLERANCE        = 1.0E-12;
float    const ABSOLUTE_ZERO    = -273; // in degrees C
float    const MAX_SPLASH_DEPTH = 0.1;
float    const WATER_INDEX_REFRACT = 1.333;
float    const WATER_COL_ATTEN  = 0.6;

float    const MESH_BOT_QUAD_DZ = 0.050; // measured from zbottom
float    const MIN_WATER_DZ     = 0.045;
float    const MESH_LOWEST_DZ   = 0.040;

unsigned const TICKS_PER_SECOND = 40;
float    const SMALL_NUMBER     = 0.001;
float    const MIN_POLY_THICK   = 0.001;
unsigned const MAX_CHARS        = 256;

float    const LIGHT_ROT_AMT    = 0.05;
float    const WIND_ADJUST      = 0.2;

float    const DEF_TIMESTEP     = 0.007;
int      const TIMESCALE        = 1; // (integer) larger makes surface movement faster
int      const MAX_I_TIMESCALE  = 8; // (integer) larger makes surface movement slower (max inverse of TIMESCALE)
float    const GRAVITY          = 300.0;

float    const CLOUD_CEILING0   = 1.5;
int      const LITNING_TIME     = 50; // in ticks
float    const MEL_EVAP_RATIO   = 0.75;
float    const LITNING_LINEAR_I = 1.0;

float    const STICK_THRESHOLD  = 1.0;
float    const COLL_DAMAGE      = 0.75;
float    const WATER_DAMAGE     = 5.0;
float    const WATER_FRAME_DAM  = 1.0;
float    const TEMP_INCREMENT   = 10.0;
float    const RAIN_MIN_TEMP    = 2.0; // degrees C
float    const SNOW_MAX_TEMP    = -2.0;
float    const WATER_MAX_TEMP   = 100.0; // boiling point
float    const DEF_TEMPERATURE  = 20.0;

float    const SNOW_ACC         = 10.0;
float    const W_FREEZE_POINT   = -0.1;
float    const WATER_DENSITY    = 1.0;
float    const SPLASH_BASE_SZ   = 0.01;
float    const DEF_AMBIENT      = 0.5;
float    const DEF_DIFFUSE      = 0.9;
float    const RAIN_TAIL_MIN_V  = -1.0;

float    const WATER_ALPHA      = 0.75;
float    const ICE_ALPHA        = 0.88;
float    const SPLASH_ALPHA     = 1.0;
float    const SNOW_ALPHA       = 1.0;

int      const N_RAND_GAUSS     = 10;
int      const N_RAND_DIST      = 10000;
int      const N_STAR_POINTS    = 5;
int      const N_CYL_SIDES      = 32;
int      const N_SPHERE_DIV     = 32;
int      const N_COLL_POLY_PTS  = 4;
int      const MAX_SPLASH_DROP  = 100;
int      const SMILEY_NCHUNKS   = 25;
int      const NUM_CHUNK_BLOCKS = 4;

float    const GROUND_SPEED     = 0.0175;
float    const SIDESTEP_SPEED   = 0.8;
float    const BACKWARD_SPEED   = 0.85;
float    const DEF_NEAR_CLIP    = 0.01;
float    const DEF_FAR_CLIP     = 100.0;
float    const FAR_DISTANCE     = 100.0;
float    const PERSP_ANGLE      = 60.0;
float    const ZOOM_FACTOR      = 5.0;
float    const UNIV_NCLIP_SCALE = 0.02;

float    const MIN_SHADOW_ALPHA = 0.5;
float    const GET_OCC_EXPAND   = 0.02;
float    const DEF_Z_BIAS       = 0.0005;

unsigned const CELL_Z_DIVS      = 8;
unsigned const SMALL_NDIV       = 8;
int const FAST_VISIBILITY_CALC  = 1;

float const TWO_PI          = 2.0*PI;
float const PI_TWO          = PI/2.0;
float const PI_INV          = 1.0/PI;
float const SQRT2           = sqrt(2.0);
float const SQRT3           = sqrt(3.0);
float const TWO_SQRT2       = 2.0*SQRT2;
float const SQRTOFTWOINV    = 1.0/SQRT2;
float const TO_DEG          = 180.0/PI;
float const TO_RADIANS      = PI/180.0;
float const SQRT_ZOOMF      = sqrt(ZOOM_FACTOR);
float const SQRT_ZOOMF_INV  = 1.0/SQRT_ZOOMF;

unsigned const MAX_SHADER_LIGHTS = 8;

unsigned const quad_to_tris_ixs [6] = {0,1,2, 0,2,3};
unsigned const cube_dim_table[2][3] = {{1, 2, 0}, {2, 0, 1}};


#define CLIP_TO_01(x)  max( 0.0f, min(1.0f, (x)))
#define CLIP_TO_pm1(x) max(-1.0f, min(1.0f, (x)))

#define BITSHIFT_CEIL(num, bs) (((num-1) >> bs) + 1)

#define UNROLL_2X(expr) {{unsigned const i_(0); expr} {unsigned const i_(1); expr}}
#define UNROLL_3X(expr) {UNROLL_2X(expr) {unsigned const i_(2); expr}}
#define UNROLL_4X(expr) {UNROLL_3X(expr) {unsigned const i_(3); expr}}

enum {CAM_FILT_DAMAGE=0, CAM_FILT_FOG, CAM_FILT_BURN, CAMERA_FILT_BKG, CAM_FILT_UWATER, CAM_FILT_TELEPORT, CAM_FILT_FROZEN, CAM_FILT_END};
enum {FG_PROJECTION=0, FG_MODELVIEW};


template<typename T> struct point2d { // size = 8

	T x, y;

	point2d() {}
	point2d(T x_, T y_) : x(x_), y(y_) {}
	point2d(point2d const &a, point2d const &b) : x(a.x - b.x), y(a.y - b.y) {}
	T mag_sq() const {return (x*x + y*y);}
	T mag()    const {return sqrt(mag_sq());}
	T cp_mag(point2d const &p) const {return (x*p.y - y*p.x);}
	void operator+=(point2d const &p) {x += p.x; y += p.y;}
	void operator-=(point2d const &p) {x -= p.x; y -= p.y;}
	void operator*=(point2d const &p) {x *= p.x; y *= p.y;} // component multiply
	void operator+=(T const &v)      {x += v; y += v;}
	void operator-=(T const &v)      {x -= v; y -= v;}
	void operator*=(T m)             {x *= m; y *= m;}
	point2d operator+(point2d const &p) const {return point2d((x+p.x), (y+p.y));}
	point2d operator-(point2d const &p) const {return point2d((x-p.x), (y-p.y));}
	point2d operator+(T const &v)       const {return point2d((x+v), (y+v));}
	point2d operator-(T const &v)       const {return point2d((x-v), (y-v));}
	point2d operator*(T      const val) const {return point2d(x*val, y*val);}
	point2d operator*(point2d const &p) const {return point2d(x*p.x, y*p.y);} // component multiply
	point2d operator-()                 const {return point2d(-x, -y, -z);}

	const T &operator[](unsigned i) const {
		switch(i) {
			case 0: return x;
			case 1: return y;
			default: assert(0);
		}
		return x; // never gets here
	}
	T &operator[](unsigned i) {
		switch(i) {
			case 0: return x;
			case 1: return y;
			default: assert(0);
		}
		return x; // never gets here
	}
	void negate() {x = -x; y = -y;}
	void normalize() {
		T const d(mag());
		if (d >= TOLERANCE) {x /= d; y /= d;}
	}
	point2d get_norm() const {
		T const vmag(mag());
		return ((vmag < TOLERANCE) ? *this : point2d(x/vmag, y/vmag));
	}
};

typedef point2d<float> vector2d;


template<typename T> struct pointT { // size = 12 (float), 24(double)

	T x, y, z;

	pointT() {}
	//pointT(T v) : x(v), y(v), z(v) {} // unsafe?
	pointT(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}
	pointT(pointT const &p1, pointT const &p2) : x(p1.x-p2.x), y(p1.y-p2.y), z(p1.z-p2.z) {} // take the difference (vector)
	template<typename S> pointT(S const &p) : x(p.x), y(p.y), z(p.z) {}

	std::string str() const {std::ostringstream oss; oss << x << ", " << y << ", " << z; return oss.str();}
	std::string raw_str() const {std::ostringstream oss; oss << x << " " << y << " " << z; return oss.str();}
	template<typename S> void operator=(pointT<S> const &p) {x = p.x; y = p.y; z = p.z;}
	bool operator==(const pointT &p) const {return (p.x == x && p.y == y && p.z == z);}
	bool operator!=(const pointT &p) const {return !operator==(p);}

	const T &operator[](unsigned i) const {
		switch(i) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: assert(0);
		}
		return x; // never gets here
	}
	T &operator[](unsigned i) {
		switch(i) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: assert(0);
		}
		return x; // never gets here
	}
	void assign(T x_, T y_, T z_)    {x = x_; y = y_; z = z_;}
	void operator+=(pointT const &p) {x += p.x; y += p.y; z += p.z;}
	void operator-=(pointT const &p) {x -= p.x; y -= p.y; z -= p.z;}
	void operator*=(pointT const &p) {x *= p.x; y *= p.y; z *= p.z;} // component multiply
	void operator+=(T const &v)      {x += v; y += v; z += v;}
	void operator-=(T const &v)      {x -= v; y -= v; z -= v;}
	void operator*=(T m)             {x *= m; y *= m; z *= m;}

	void operator/=(T d) {
		assert(d != 0.0);
		T const m(1.0/d);
		x *= m; y *= m; z *= m;
	}
	void normalize() {
		T const d(mag());
		if (d >= TOLERANCE) {
			T const m(1.0/d);
			x *= m; y *= m; z *= m;
		}
	}
	bool normalize_test() {
		T const d(mag());
		if (d < TOLERANCE) return 0;
		T const m(1.0/d);
		x *= m; y *= m; z *= m;
		return 1;
	}
	void negate() {x = -x; y = -y; z = -z;}

	void invert() {
		if (x == 0.0) {x = TOLERANCE;}
		if (y == 0.0) {y = TOLERANCE;}
		if (z == 0.0) {z = TOLERANCE;}
		x = 1.0/x; y = 1.0/y; z = 1.0/z;
	}
	pointT get_norm() const {
		T const vmag(mag());
		return ((vmag < TOLERANCE) ? *this : pointT(x/vmag, y/vmag, z/vmag));
	}
	void set_max_mag(T vmax) {
		T const vmag(mag());
		if (vmag > TOLERANCE && vmag > vmax) operator*=(vmax/vmag);
	}
	pointT operator+(pointT const &p)  const {return pointT((x+p.x), (y+p.y), (z+p.z));}
	pointT operator-(pointT const &p)  const {return pointT((x-p.x), (y-p.y), (z-p.z));}
	pointT operator+(T const &v)       const {return pointT((x+v), (y+v), (z+v));}
	pointT operator-(T const &v)       const {return pointT((x-v), (y-v), (z-v));}
	pointT operator*(T      const val) const {return pointT(x*val, y*val, z*val);}
	pointT operator*(pointT const &p)  const {return pointT(x*p.x, y*p.y, z*p.z);} // component multiply
	pointT operator-()                 const {return pointT(-x, -y, -z);}
	
	pointT operator/(T      const val) const {
		assert(val != 0.0);
		T const val_inv(1.0/val);
		return pointT(x*val_inv, y*val_inv, z*val_inv);
	}
	float  dot  (pointT const &v) const {return (x*v.x + y*v.y + z*v.z);}
	pointT cross(pointT const &v) const {return pointT((y*v.z - z*v.y), (z*v.x - x*v.z), (x*v.y - y*v.x));}
	T mag_sq()    const {return (x*x + y*y + z*z);}
	T mag()       const {return sqrt(mag_sq());}
	T xy_mag_sq() const {return (x*x + y*y);}
	T xy_mag()    const {return sqrt(xy_mag_sq());}
	T get_min_val()   const {return min(x, min(y, z));}
	T get_max_val()   const {return max(x, max(y, z));}
	bool is_nonzero() const {return (x != 0.0 || y != 0.0 || z != 0.0);}

	bool operator<(pointT const &p) const { // needed for maps and stuff
		if (z > p.z) return 1; if (z < p.z) return 0; // greater than operation?
		if (y < p.y) return 1; if (y > p.y) return 0;
		return (x < p.x);
	}
};

// premultiply a pointT by a scalar
template<typename S, typename T> pointT<T> inline operator*(S const v, pointT<T> const &p) {return pointT<T>(v*p.x, v*p.y, v*p.z);}


typedef pointT<float>  point;
typedef pointT<float>  vector3d;
typedef pointT<double> point_d;
typedef pointT<double> vector3d_d;
typedef point_d        upos_point_type;


// constants
point    const all_zeros(0, 0, 0);
vector3d const plus_x(1, 0, 0);
vector3d const plus_y(0, 1, 0);
vector3d const plus_z(0, 0, 1);
vector3d const zero_vector(0, 0, 0);
vector3d const all_ones(1, 1, 1);


struct vector4d : public vector3d { // size = 16
	float w;

	vector4d() {}
	vector4d(float x_, float y_, float z_, float w_) : vector3d(x_, y_, z_), w(w_) {}
	vector4d(vector3d const &v, float w_) : vector3d(v), w(w_) {}
	void assign(float x_, float y_, float z_, float w_)    {x = x_; y = y_; z = z_; w = w_;}
	std::string str() const {std::ostringstream oss; oss << x << ", " << y << ", " << z << ", " << w; return oss.str();}
	vector4d operator+ (vector4d const &p) const {return vector4d((x+p.x), (y+p.y), (z+p.z), (w+p.w));}
	vector4d operator- (vector4d const &p) const {return vector4d((x-p.x), (y-p.y), (z-p.z), (w-p.w));}
	void     operator+=(vector4d const &p) {x += p.x; y += p.y; z += p.z; w += p.w;}
	void     operator-=(vector4d const &p) {x -= p.x; y -= p.y; z -= p.z; w -= p.w;}
	vector4d operator-()                   const {return vector4d(-x, -y, -z, -w);}
	bool     operator==(vector4d const &v) const {return (v.x == x && v.y == y && v.z == z && v.w == w);}
	bool     operator!=(vector4d const &v) const {return !operator==(v);}
	bool     operator< (vector4d const &p) const {return ((w == p.w) ? vector3d::operator<(p) : (w < p.w));}
};


struct sphere_t {

	point pos;
	float radius;
	
	sphere_t(point const &p=all_zeros, float r=0.0) : pos(p), radius(r) {}
	bool operator==(sphere_t const &s) const {return (pos == s.pos && radius == s.radius);}
	bool operator!=(sphere_t const &s) const {return (pos != s.pos || radius != s.radius);}
	point const &get_pos() const {return pos;}
	float get_radius()     const {return radius;}
	float get_volume()     const {return (4.0/3.0)*PI*radius*radius*radius;}
	float get_surf_area()  const {return 4.0*PI*radius*radius;}
	bool contains_point(point const &p) const;
};


struct cube_t { // size = 24

	float d[3][2]; // {x,y,z},{min,max}

	cube_t() {}
	
	cube_t(float x1, float x2, float y1, float y2, float z1, float z2) {
		d[0][0] = x1; d[0][1] = x2;
		d[1][0] = y1; d[1][1] = y2;
		d[2][0] = z1; d[2][1] = z2;
	}
	cube_t(point const &p1, point const &p2) {
		UNROLL_3X(d[i_][0] = min(p1[i_], p2[i_]); d[i_][1] = max(p1[i_], p2[i_]);)
	}
	cube_t(point const &pt) {set_from_point(pt);}
	cube_t(point const *const pts, unsigned npts) {set_from_points(pts, npts);}
	void set_to_zeros() {set_from_point(all_zeros);}
	void copy_from(cube_t const &c) {
		UNROLL_3X(d[i_][0] = c.d[i_][0];)
		UNROLL_3X(d[i_][1] = c.d[i_][1];)
	}
	void set_from_point(point const &pt) {UNROLL_3X(d[i_][0] = d[i_][1] = pt[i_];)}
	void set_from_sphere(point const &pt, float radius) {
		UNROLL_3X(d[i_][0] = pt[i_]-radius; d[i_][1] = pt[i_]+radius;)
	}
	void set_from_sphere(sphere_t const &s) {set_from_sphere(s.pos, s.radius);}
	bool operator==(cube_t const &c) const {
		UNROLL_3X(if (d[i_][0] != c.d[i_][0]) return 0;)
		UNROLL_3X(if (d[i_][1] != c.d[i_][1]) return 0;)
		return 1;
	}
	float x1() const {return d[0][0];}
	float x2() const {return d[0][1];}
	float y1() const {return d[1][0];}
	float y2() const {return d[1][1];}
	float z1() const {return d[2][0];}
	float z2() const {return d[2][1];}
	float &x1() {return d[0][0];}
	float &x2() {return d[0][1];}
	float &y1() {return d[1][0];}
	float &y2() {return d[1][1];}
	float &z1() {return d[2][0];}
	float &z2() {return d[2][1];}
	bool operator!=(cube_t const &c) const {return !operator==(c);}
	cube_t operator+ (vector3d const &p) const {cube_t c(*this); c += p; return c;}
	cube_t operator- (vector3d const &p) const {cube_t c(*this); c -= p; return c;}
	cube_t operator* (float scale      ) const {cube_t c(*this); c *= scale; return c;}
	void   operator+=(vector3d const &p) {translate( p);}
	void   operator-=(vector3d const &p) {translate(-p);}
	void   operator*=(float scale      ) {UNROLL_3X(d[i_][0] *= scale; d[i_][1] *= scale;)}

	void translate(point const &p) {
		UNROLL_3X(d[i_][0] += p[i_]; d[i_][1] += p[i_];)
	}
	void set_from_points(point const *const pts, unsigned npts);
	std::string str() const;
	std::string raw_str() const;
	bool is_near_zero_area() const;
	bool is_all_zeros() const {return (d[0][0] == 0 && d[0][1] == 0 && d[1][0] == 0 && d[1][1] == 0 && d[2][0] == 0 && d[2][1] == 0);}

	void union_with_pt(point const &pt) {
		UNROLL_3X(d[i_][0] = min(d[i_][0], pt[i_]); d[i_][1] = max(d[i_][1], pt[i_]);)
	}
	void assign_or_union_with_pt(point const &pt) {
		if (is_zero_area()) {set_from_point(pt);} else {union_with_pt(pt);} // Note: won't work if pt == (0,0,0)
	}
	void assign_or_union_with_sphere(point const &pt, float radius) {
		if (is_zero_area()) {set_from_sphere(pt, radius);} else {union_with_sphere(pt, radius);} // Note: won't work if pt == (0,0,0)
	}
	void union_with_sphere(point const &pt, float radius) {
		UNROLL_3X(d[i_][0] = min(d[i_][0], pt[i_]-radius); d[i_][1] = max(d[i_][1], pt[i_]+radius);)
	}
	void union_with_sphere(sphere_t const &s) {union_with_sphere(s.pos, s.radius);}
	void union_with_cube(cube_t const &c) {
		UNROLL_3X(d[i_][0] = min(d[i_][0], c.d[i_][0]); d[i_][1] = max(d[i_][1], c.d[i_][1]);)
	}
	void assign_or_union_with_cube(cube_t const &c) {
		if (c.is_zero_area()) return;
		if (is_zero_area()) {copy_from(c);} else {union_with_cube(c);}
	}
	void intersect_with_cube(cube_t const &c) { // Note: cube and *this must overlap
		UNROLL_3X(d[i_][0] = max(d[i_][0], c.d[i_][0]); d[i_][1] = min(d[i_][1], c.d[i_][1]);)
	}
	void normalize() {
		UNROLL_3X(if (d[i_][1] < d[i_][0]) swap(d[i_][0], d[i_][1]);)
	}
	bool is_zero_area() const {
		UNROLL_3X(if (d[i_][0] == d[i_][1]) return 1;)
		return 0;
	}
	bool is_normalized() const {
		UNROLL_3X(if (d[i_][0] > d[i_][1]) return 0;)
		return 1;
	}
	bool is_strictly_normalized() const {
		UNROLL_3X(if (d[i_][0] >= d[i_][1]) return 0;)
		return 1;
	}
	bool intersects(const cube_t &cube) const {
		UNROLL_3X(if (cube.d[i_][1] < d[i_][0] || cube.d[i_][0] > d[i_][1]) return 0;)
		return 1;
	}
	bool intersects_xy(const cube_t &cube) const {
		UNROLL_2X(if (cube.d[i_][1] < d[i_][0] || cube.d[i_][0] > d[i_][1]) return 0;)
		return 1;
	}
	bool intersects(const cube_t &cube, float toler) const { // Note: toler > 0 makes adjacent cubes *not* intersect
		UNROLL_3X(if (cube.d[i_][1] < (d[i_][0] + toler) || cube.d[i_][0] > (d[i_][1] - toler)) return 0;)
		return 1;
	}
	bool contains_cube(const cube_t &cube) const {
		UNROLL_3X(if (cube.d[i_][0] < d[i_][0] || cube.d[i_][1] > d[i_][1]) return 0;)
		return 1;
	}
	bool contains_cube_xy(const cube_t &cube) const {
		UNROLL_2X(if (cube.d[i_][0] < d[i_][0] || cube.d[i_][1] > d[i_][1]) return 0;)
		return 1;
	}
	bool contains_pt(point const &pt) const {
		UNROLL_3X(if (pt[i_] < d[i_][0] || pt[i_] > d[i_][1]) return 0;)
		return 1;
	}
	bool contains_pt_xy(point const &pt) const {
		return (pt.x > d[0][0] && pt.x < d[0][1] && pt.y > d[1][0] && pt.y < d[1][1]);
	}
	bool contains_pt_xy_inc_low_edge(point const &pt) const {
		return (pt.x >= d[0][0] && pt.x < d[0][1] && pt.y >= d[1][0] && pt.y < d[1][1]);
	}
	bool quick_intersect_test(const cube_t &cube) const {
		UNROLL_3X(if (cube.d[i_][0] >= d[i_][1] || cube.d[i_][1] <= d[i_][0]) return 0;)
		return 1;
	}
	void clamp_pt(point &pt) const {
		UNROLL_3X(pt[i_] = min(d[i_][1], max(d[i_][0], pt[i_]));)
	}
	float get_volume() const {
		return fabs(d[0][1] - d[0][0])*fabs(d[1][1] - d[1][0])*fabs(d[2][1] - d[2][0]);
	}
	float get_area() const {
		return 2.0*(fabs(d[0][1] - d[0][0])*fabs(d[1][1] - d[1][0]) +
			        fabs(d[1][1] - d[1][0])*fabs(d[2][1] - d[2][0]) +
			        fabs(d[2][1] - d[2][0])*fabs(d[0][1] - d[0][0]));
	}
	float max_len() const {
		return max((d[0][1] - d[0][0]), max((d[1][1] - d[1][0]), (d[2][1] - d[2][0])));
	}
	float min_len() const {
		return min((d[0][1] - d[0][0]), min((d[1][1] - d[1][0]), (d[2][1] - d[2][0])));
	}
	float second_largest_len() const {
		return min(max((d[0][1] - d[0][0]), (d[1][1] - d[1][0])),
			   min(max((d[1][1] - d[1][0]), (d[2][1] - d[2][0])),
			       max((d[2][1] - d[2][0]), (d[0][1] - d[0][0]))));
	}
	point get_cube_center() const {
		return point(0.5*(d[0][0]+d[0][1]), 0.5*(d[1][0]+d[1][1]), 0.5*(d[2][0]+d[2][1]));
	}
	float get_bsphere_radius() const {
		return 0.5*sqrt((d[0][1]-d[0][0])*(d[0][1]-d[0][0]) + (d[1][1]-d[1][0])*(d[1][1]-d[1][0]) + (d[2][1]-d[2][0])*(d[2][1]-d[2][0]));
	}
	float get_xy_bsphere_radius() const {
		return 0.5*sqrt((d[0][1]-d[0][0])*(d[0][1]-d[0][0]) + (d[1][1]-d[1][0])*(d[1][1]-d[1][0]));
	}
	sphere_t get_bsphere() const {return sphere_t(get_cube_center(), get_bsphere_radius());}
	sphere_t get_bcylin () const {return sphere_t(get_cube_center(), get_xy_bsphere_radius());}
	point get_llc() const {return point(d[0][0], d[1][0], d[2][0]);}
	point get_urc() const {return point(d[0][1], d[1][1], d[2][1]);}
	vector3d get_size() const {return vector3d((d[0][1]-d[0][0]), (d[1][1]-d[1][0]), (d[2][1]-d[2][0]));}
	float get_dx() const {return (d[0][1]-d[0][0]);}
	float get_dy() const {return (d[1][1]-d[1][0]);}
	float get_dz() const {return (d[2][1]-d[2][0]);}
	void expand_by(float val) {UNROLL_3X(d[i_][0] -= val; d[i_][1] += val;)}
	void expand_by(float x, float y, float z) {expand_by(vector3d(x, y, z));}
	void expand_by(vector3d const &val) {UNROLL_3X(d[i_][0] -= val[i_]; d[i_][1] += val[i_];)}
	void expand_by_xy(float val) {UNROLL_2X(d[i_][0] -= val; d[i_][1] += val;)}
	unsigned get_split_dim(float &max_sz, float &sval, unsigned skip_dims) const;
	bool cube_intersection(const cube_t &cube, cube_t &res) const;
	float get_overlap_volume(const cube_t &cube) const;
	vector3d closest_side_dir(point const &pos) const;
	
	point closest_pt(point const &pos) const {
		point pt;
		UNROLL_3X(pt[i_] = max(d[i_][0], min(d[i_][1], pos[i_]));)
		return pt;
	}
	float get_max_extent() const { // from (0,0,0)
		float mextent(0.0);
		UNROLL_3X(mextent = max(mextent, max(-d[i_][0], d[i_][1]));)
		return mextent;
	}
	int closest_face(point const &pos) const;
	bool cube_merge(cube_t const &cube);
	void get_points(point pts[8]) const;
};

cube_t const all_zeros_cube(0,0,0,0,0,0);


template<typename T> cube_t get_polygon_bbox(vector<T> const &p) {

	if (p.empty()) return all_zeros_cube;
	cube_t bbox(p.front().v, p.front().v);
	for (unsigned i = 1; i < p.size(); ++i) {bbox.union_with_pt(p[i].v);}
	return bbox;
}


vector3d get_poly_norm(point const *const points, bool normalize=1);

struct tquad_t { // size = 52

	point pts[4];
	unsigned npts;
	
	tquad_t(unsigned npts_=0) : npts(npts_) {}
	bool is_valid() const;
	void update_bcube(cube_t &c) const;
	cube_t get_bcube() const;
	vector3d get_norm(bool normalize=1) const {return get_poly_norm(pts, normalize);}
	point const &operator[](unsigned i) const {return pts[i];}
	point       &operator[](unsigned i)       {return pts[i];}
};


template<typename T, typename S> inline float p2p_dist(const pointT<T> &pt1, const pointT<S> &pt2);

struct line_3dw {

	point p1, p2;

	line_3dw() : p1(all_zeros), p2(all_zeros) {}
	line_3dw(point const &p1_, point const &p2_) : p1(p1_), p2(p2_) {assert(p1 != p2);}
	vector3d get_norm_dir_vect() const {return (p2 - p1).get_norm();}
	float get_length() const {return p2p_dist(p1, p2);}
	void translate(point const &p) {p1 += p; p2 += p;}
};


struct vector_point_norm {

	vector<point>    p;
	vector<vector3d> n;
};


struct pos_dir_up { // defines a view frustum

	point pos;
	vector3d dir, upv, upv_, cp;
	float angle, tterm, sterm, x_sterm, behind_sphere_mult, near_, far_;
	double A; // aspect ratio x/y
	bool valid;

	pos_dir_up(void) : valid(0) {}
	pos_dir_up(point const &p, vector3d const &d, vector3d const &u, float angle_, float n, float f, float a=0.0, bool no_zoom=0);
	void orthogonalize_up_dir();
	bool point_visible_test(point const &pos_) const;
	bool sphere_visible_test(point const &pos_, float radius) const;
	bool sphere_visible_test_no_inside_test(point const &pos_, float radius) const;
	bool sphere_completely_visible_test(point const &pos_, float radius) const {return sphere_visible_test(pos_, -radius);}
	template<unsigned N> bool pt_set_visible(point const *const pts) const;
	bool cube_visible(cube_t const &cube) const;
	bool projected_cube_visible(cube_t const &cube, point const &proj_pt) const;
	bool sphere_and_cube_visible_test(point const &pos_, float radius, cube_t const &cube) const;
	void draw_frustum() const;
	void translate(vector3d const &tv) {pos += tv;}
	void scale(float s) {pos *= s; near_ *= s; far_ *= s;}
	void rotate(vector3d const &axis, float angle);
	void apply_z_mirror(float zval);
};


struct cylinder_3dw : public line_3dw { // size = 32

	float r1, r2;

	cylinder_3dw() : r1(0.0), r2(0.0) {}
	cylinder_3dw(point const &p1_, point const &p2_, float r1_, float r2_) : line_3dw(p1_, p2_), r1(r1_), r2(r2_) {}
	void calc_bcube(cube_t &bcube) const;
	float get_volume() const {return PI*(r1*r1 + r1*r2 + r2*r2)*get_length()/3.0;}
	float get_surface_area() const;
	point get_center() const {return 0.5*(p1 + p2);}
	float get_avg_radius() const {return 0.5*(r1 + r2);}
	float get_bounding_radius() const;
};


struct colorRGB { // size = 12

	float R, G, B;
	colorRGB() {}
	colorRGB(float r, float g, float b) : R(r), G(g), B(b) {}
	void assign(float r, float g, float b) {R = r; G = g; B = b;}
	void set_to_val(float val) {R = G = B = val;}
	bool operator==(const colorRGB &c) const {return (c.R == R && c.G == G && c.B == B);}
	bool operator!=(const colorRGB &c) const {return !operator==(c);}

	const float &operator[](unsigned i) const {
		switch(i) {
			case 0: return R;
			case 1: return G;
			case 2: return B;
			default: assert(0);
		}
		return R; // never gets here
	}
	float &operator[](unsigned i) {
		switch(i) {
			case 0: return R;
			case 1: return G;
			case 2: return B;
			default: assert(0);
		}
		return R; // never gets here
	}
	bool operator<(const colorRGB &c) const { // greater than operation?
		if (R < c.R) return 1;
		if (R > c.R) return 0;
		if (G < c.G) return 1;
		if (G > c.G) return 0;
		return (B < c.B);
	}
	colorRGB operator+ (colorRGB const &c) const {return colorRGB(R+c.R, G+c.G, B+c.B);}
	void     operator+=(colorRGB const &c)       {R += c.R; G += c.G; B += c.B;}
	colorRGB operator*(float val) const {return colorRGB(R*val, G*val, B*val);}
	void operator*=(float val) {R *= val; G *= val; B *= val;}
	colorRGB modulate_with(colorRGB const &c) const {return colorRGB(R*c.R, G*c.G, B*c.B);}

	void set_valid_color() {
		R = CLIP_TO_01(R);
		G = CLIP_TO_01(G);
		B = CLIP_TO_01(B);
	}
	void from_normal(vector3d const &n) { // for normal maps
		R = 0.5*(n.x + 1.0);
		G = 0.5*(n.y + 1.0);
		B = 0.5*(n.z + 1.0);
	}
	void to_normal(vector3d &n) const { // for normal maps
		n.x = 2.0*R - 1.0;
		n.y = 2.0*G - 1.0;
		n.z = 2.0*B - 1.0;
	}
	void normalize_to_max_comp() {
		float const max_comp(get_max_component());
		if (max_comp > TOLERANCE) {R /= max_comp; G /= max_comp; B /= max_comp;}
	}
	std::string str() const {std::ostringstream oss; oss << "R: " << R << ", G: " << G << ", B: " << B; return oss.str();}
	std::string raw_str() const {std::ostringstream oss; oss << R << " " << G << " " << B; return oss.str();}
	float get_luminance() const {return (R + G + B)/3.0;}
	float get_max_component() const {return max(R, max(G, B));}
	void set_for_cur_shader() const;
};


struct colorRGBA : public colorRGB { // size = 16

	union {float A; float alpha;}; // A and alpha are both valid components

	colorRGBA() {}
	colorRGBA(float r, float g, float b, float a=1.0) : colorRGB(r, g, b), A(a) {}
	colorRGBA(colorRGB const &color, float a=1.0) : colorRGB(color), A(a) {}
	void assign(float r, float g, float b, float a=1.0) {R = r; G = g; B = b; A = a;}
	bool operator==(const colorRGBA &c) const {return (c.R == R && c.G == G && c.B == B && c.A == A);}
	bool operator!=(const colorRGBA &c) const {return !operator==(c);}

	const float &operator[](unsigned i) const {
		switch(i) {
			case 0: return R;
			case 1: return G;
			case 2: return B;
			case 3: return A;
			default: assert(0);
		}
		return R; // never gets here
	}
	float &operator[](unsigned i) {
		switch(i) {
			case 0: return R;
			case 1: return G;
			case 2: return B;
			case 3: return A;
			default: assert(0);
		}
		return R; // never gets here
	}
	bool operator<(const colorRGBA &c) const { // greater than operation?
		if (A > c.A) return 1; // note: alpha less than so that low alpha colors are last
		if (A < c.A) return 0;
		return colorRGB::operator<(c);
	}
	colorRGBA operator* (float val) const             {return colorRGBA(R*val, G*val, B*val, A);}
	colorRGBA operator/ (float val) const             {return colorRGBA(R/val, G/val, B/val, A);}
	colorRGBA operator+ (colorRGBA const &c) const    {return colorRGBA(R+c.R, G+c.G, B+c.B, A+c.A);}
	void      operator+=(colorRGBA const &c)          {R += c.R; G += c.G; B += c.B; A += c.A;}
	colorRGBA modulate_with(colorRGBA const &c) const {return colorRGBA(R*c.R, G*c.G, B*c.B, A*c.A);}

	void set_valid_color() {
		colorRGB::set_valid_color();
		A = CLIP_TO_01(A);
	}
	void normalize_to_alpha_1() {
		if (A == 1.0) return;
		R *= A; G *= A; B *= A;
		A = 1.0;
	}
	bool within_thresh_of_rgb(float thresh, colorRGBA const &c) const { // no alpha check
		return ((fabs(R-c.R) + fabs(G-c.G) + fabs(B-c.B)) < thresh);
	}
	bool within_thresh_of_rgba(float thresh, colorRGBA const &c) const { // no alpha check
		return ((fabs(R-c.R) + fabs(G-c.G) + fabs(B-c.B) + fabs(A-c.A)) < thresh);
	}
	bool is_valid() const {return (R >= 0 && G >= 0 && B >= 0 && A >= 0 && R <= 1 && G <= 1 && B <= 1 && A <= 1);}
	std::string str() const {std::ostringstream oss; oss << "R: " << R << ", G: " << G << ", B: " << B << ", A: " << A; return oss.str();}
	std::string raw_str() const {std::ostringstream oss; oss << R << " " << G << " " << B << " " << A; return oss.str();}
	void set_for_cur_shader() const;
};


struct tex_range_t {

	float x1, y1, x2, y2;
	bool clip_quad, swap_xy;

	tex_range_t() : x1(0.0), y1(0.0), x2(1.0), y2(1.0), clip_quad(0), swap_xy(0) {}
	tex_range_t(float x1_, float y1_, float x2_, float y2_, bool clip_quad_=0, bool swap_xy_=0) : x1(x1_), y1(y1_), x2(x2_), y2(y2_), clip_quad(clip_quad_), swap_xy(swap_xy_) {}

	static tex_range_t from_atlas(unsigned xv, unsigned yv, unsigned nx, unsigned ny) {
		assert(nx > 0 && ny > 0 && xv < nx && yv < ny);
		return tex_range_t(xv/float(nx), yv/float(ny), (xv+1)/float(nx), (yv+1)/float(ny));
	}
};


struct vert_norm { // size = 24
	point v;
	vector3d n;
	vert_norm() {}
	vert_norm(point const &v_, vector3d const &n_) : v(v_), n(n_) {}
	void assign(point const &v_, vector3d const &n_) {v = v_; n = n_;}
	bool operator< (vert_norm const &p) const {return ((v == p.v) ? (n < p.n) : (v < p.v));}
	bool operator==(vert_norm const &p) const {return (v == p.v && n == p.n);}
	static void set_vbo_arrays(bool set_state=1, void const *vbo_ptr_offset=NULL);
	static void set_vbo_arrays_shadow(bool include_tcs);
	static void unset_attrs() {}
};


struct norm_comp { // size = 4
	char n[3];
	char pad; // unused padding
	norm_comp() : pad(0) {}
	norm_comp(vector3d const &n_) : pad(0) {set_norm(n_);}
	void set_norm(norm_comp const &n_) {UNROLL_3X(n[i_] = n_.n[i_];)}
	void set_norm(vector3d const &n_) {UNROLL_3X(n[i_] = char(max(-128, min(127, int(127.0*n_[i_]))));)}
	void set_norm_no_clamp(vector3d const &n_) {UNROLL_3X(n[i_] = int(127.0*n_[i_]);)}
	vector3d get_norm() const {return vector3d(n[0]/127.0, n[1]/127.0, n[2]/127.0);}
};


// unused
struct norm_xy { // size = 8
	float x, y; // z can be calculated in the shader as sqrt(1 - x*x - y*y), as long as z >= 0.0
	norm_xy() {}
	norm_xy(vector3d const &n) {set_norm(n);}
	void set_norm(vector3d const &n) {assert(n.z >= 0.0); x = n.x; y = n.y;} 
	void ensure_normalized_and_set(vector3d const &n) {assert(n.z >= 0.0); float const mag(n.mag()); x = n.x/mag; y = n.y/mag;}
	vector3d get_norm() const {return vector3d(x, y, sqrt(1.0 - x*x - y*y));}
};


struct vert_wrap_t { // size = 12; so we can put the vertex first
	point v;
	vert_wrap_t() {}
	vert_wrap_t(point const &v_) : v(v_) {}
	static void set_vbo_arrays(bool set_state=1, void const *vbo_ptr_offset=NULL);
	static void unset_attrs() {}
};


struct vert_tc_t : public vert_wrap_t { // size = 20
	float t[2];
	vert_tc_t() {}
	vert_tc_t(point const &v_, float ts, float tt) : vert_wrap_t(v_) {t[0] = ts; t[1] = tt;}
	static void set_vbo_arrays(bool set_state=1, void const *vbo_ptr_offset=NULL);
};


struct vert_norm_comp : public vert_wrap_t, public norm_comp { // size = 16
	vert_norm_comp() {}
	vert_norm_comp(vert_norm const &vn) : vert_wrap_t(vn.v), norm_comp(vn.n) {}
	vert_norm_comp(point const &v_, vector3d  const &n_) : vert_wrap_t(v_), norm_comp(n_) {}
	vert_norm_comp(point const &v_, norm_comp const &n_) : vert_wrap_t(v_), norm_comp(n_) {}
	static void set_vbo_arrays(bool set_state=1, void const *vbo_ptr_offset=NULL);
};


struct vert_norm_comp_tc : public vert_norm_comp { // size = 24
	float t[2];
	typedef vert_norm_comp_tc non_color_class;
	vert_norm_comp_tc() {}
	vert_norm_comp_tc(point const &v_, vector3d const &n_, float ts, float tt) : vert_norm_comp(v_, n_) {t[0] = ts; t[1] = tt;}
	vert_norm_comp_tc(point const &v_, vector3d const &n_, float const tc[2] ) : vert_norm_comp(v_, n_) {t[0] = tc[0]; t[1] = tc[1];}
	static void set_vbo_arrays(bool set_state=1, void const *vbo_ptr_offset=NULL);
};


struct vert_norm_comp_tc_comp : public vert_norm_comp { // size = 20
	short t[2]; // could even use char
	vert_norm_comp_tc_comp() {}
	vert_norm_comp_tc_comp(point const &v_, vector3d const &n_, float ts, float tt) : vert_norm_comp(v_, n_) {t[0] = 32767*ts; t[1] = 32767*tt;}
	static void set_vbo_arrays(bool set_state=1, void const *vbo_ptr_offset=NULL);
};


struct vert_norm_tc : public vert_norm { // size = 32
	float t[2];
	typedef vert_norm_tc non_color_class;
	vert_norm_tc() {}
	vert_norm_tc(point const &v_, vector3d const &n_, float ts, float tt) : vert_norm(v_, n_) {t[0] = ts;    t[1] = tt;   }
	vert_norm_tc(point const &v_, vector3d const &n_, float const t_[2])  : vert_norm(v_, n_) {t[0] = t_[0]; t[1] = t_[1];}
	vert_norm_tc(vert_norm const &vn) : vert_norm(vn) {t[0] = t[1] = 0.0;} // tc not set here (required for model3d/voxel code)
	void assign(point const &v_, vector3d const &n_, float ts, float tt) {v = v_; n = n_; t[0] = ts; t[1] = tt;}

	bool operator<(vert_norm_tc const &p) const {
		if (v < p.v) return 1;
		if (p.v < v) return 0;
		if (n < p.n) return 1;
		if (p.n < n) return 0;
		if (t[0] < p.t[0]) return 1;
		if (p.t[0] < t[0]) return 0;
		return (t[1] < p.t[1]);
	}
	bool operator==(vert_norm_tc const &p) const {return (v == p.v && n == p.n && t[0] == p.t[0] && t[1] == p.t[1]);}
	static void set_vbo_arrays(bool set_state=1, void const *vbo_ptr_offset=NULL);
	static void set_vbo_arrays_shadow(bool include_tcs);
};


struct vert_norm_tc_tan : public vert_norm_tc { // size = 48
	vector4d tangent;

	vert_norm_tc_tan() {}
	vert_norm_tc_tan(vert_norm_tc const &vntc, vector4d const &tangent_=vector4d(0,0,0,0))
		: vert_norm_tc(vntc), tangent(tangent_) {}
	vert_norm_tc_tan(point const &v_, vector3d const &n_, float ts, float tt, vector4d const &tangent_=vector4d(0,0,0,0))
		: vert_norm_tc(v_, n_, ts, tt), tangent(tangent_) {}

	bool operator<(vert_norm_tc_tan const &p) const {
		if (v < p.v) return 1;
		if (p.v < v) return 0;
		if (n < p.n) return 1;
		if (p.n < n) return 0;
		if (t[0] < p.t[0]) return 1;
		if (p.t[0] < t[0]) return 0;
		if (t[1] < p.t[1]) return 1;
		if (p.t[1] < t[1]) return 0;
		return (tangent < p.tangent);
	}
	//bool operator==(vert_norm_tc_tan const &p) const {}
	static void set_vbo_arrays(bool set_state=1, void const *vbo_ptr_offset=NULL);
	static void set_vbo_arrays_shadow(bool include_tcs);
	static void unset_attrs();
};


struct color_wrapper { // size = 4, can be used in a union
	unsigned char c[4]; // Note: c[3] (alpha component) is not used in all cases

	color_wrapper() {}
	color_wrapper(colorRGBA const &c_) {set_c4(c_);}
	color_wrapper(colorRGB  const &c_) {set_c3(c_);}
	template<typename T> void set_c3(T const &c_) {UNROLL_3X(c[i_] = (unsigned char)(255.0*CLIP_TO_01(c_[i_]));) c[3] = 255;}
	void set_c4(colorRGBA const &c_) {UNROLL_4X(c[i_]  = (unsigned char)(255.0*CLIP_TO_01(c_[i_]));)}
	void add_c4(colorRGBA const &c_) {UNROLL_4X(c[i_] += (unsigned char)(255.0*CLIP_TO_01(c_[i_]));)}
	void copy_color(color_wrapper const &cw) {c[0] = cw.c[0]; c[1] = cw.c[1]; c[2] = cw.c[2]; c[3] = cw.c[3];}
	colorRGB  get_c3() const {return colorRGB(c[0]/255.0, c[1]/255.0, c[2]/255.0);}
	colorRGBA get_c4() const {return colorRGBA(get_c3(), c[3]/255.0);}
	static bool is_compressed() {return 1;}
};

struct color_wrapper_ctor : public color_wrapper { // size = 4
	color_wrapper_ctor() {}
	color_wrapper_ctor(colorRGB  const &color) {set_c3(color);}
	color_wrapper_ctor(colorRGBA const &color) {set_c4(color);}
};


struct color_wrapper_float { // size = 16
	colorRGBA c; // Note: c[3] (alpha component) is not used in all cases

	template<typename T> void set_c3(T const &c_) {c = c_; c.A = 1.0;}
	void set_c4(colorRGBA const &c_) {c = c_;}
	colorRGB  get_c3() const {return colorRGB(c.R, c.G, c.B);}
	colorRGBA get_c4() const {return c;}
	static bool is_compressed() {return 0;}
};


struct vert_color : public color_wrapper { // size = 16
	point v;
	typedef point non_color_class;

	vert_color() {}
	vert_color(point const &v_, color_wrapper const &cw) : v(v_), color_wrapper(cw) {}
	vert_color(point const &v_, colorRGBA const &c_)     : v(v_) {set_c4(c_);}
	vert_color(point const &v_, unsigned char const *c_) : v(v_) {c[0]=c_[0]; c[1]=c_[1]; c[2]=c_[2]; c[3]=c_[3];}
	static void set_vbo_arrays(bool set_state=1, void const *vbo_ptr_offset=NULL);
	static void unset_attrs() {}
};


struct vert_norm_color : public vert_norm, public color_wrapper { // size = 28
	typedef vert_norm non_color_class;
	vert_norm_color() {}
	vert_norm_color(vert_norm const &vn, color_wrapper const &cw) : vert_norm(vn), color_wrapper(cw) {}
	vert_norm_color(vert_norm const &vn, colorRGBA const &c_) : vert_norm(vn) {set_c4(c_);}
	vert_norm_color(point const &v_, vector3d const &n_, colorRGBA const     &c_) : vert_norm(v_, n_) {set_c4(c_);}
	vert_norm_color(point const &v_, vector3d const &n_, unsigned char const *c_) : vert_norm(v_, n_) {c[0]=c_[0]; c[1]=c_[1]; c[2]=c_[2]; c[3]=c_[3];}
	void assign(point const &v_, vector3d const &n_, color_wrapper const &cw) {v = v_; n = n_; color_wrapper::operator=(cw);}
	static void set_vbo_arrays(bool set_state=1, void const *vbo_ptr_offset=NULL);
};


struct vert_norm_comp_color : public vert_norm_comp, public color_wrapper { // size = 20
	typedef vert_norm_comp non_color_class; // non-compressed type
	vert_norm_comp_color() {}
	vert_norm_comp_color(vert_norm const &vn, color_wrapper const &cw) : vert_norm_comp(vn), color_wrapper(cw) {}
	vert_norm_comp_color(vert_norm_comp const &vn, color_wrapper const &cw) : vert_norm_comp(vn), color_wrapper(cw) {}
	vert_norm_comp_color(point const &v_, vector3d const &n_, colorRGBA const &c_) : vert_norm_comp(v_, n_) {set_c4(c_);}
	vert_norm_comp_color(point const &v_, vector3d const &n_, color_wrapper const &cw) : vert_norm_comp(v_, n_), color_wrapper(cw) {}
	void assign(point const &v_, vector3d const &n_, unsigned char const *const c_) {
		v = v_; set_norm(n_); c[0] = c_[0]; c[1] = c_[1]; c[2] = c_[2]; c[3] = 255;
	}
	void assign(point const &v_, char const *const n_, unsigned char const *const c_) {
		v = v_; n[0] = n_[0]; n[1] = n_[1]; n[2] = n_[2]; c[0] = c_[0]; c[1] = c_[1]; c[2] = c_[2]; c[3] = 255;
	}
	static void set_vbo_arrays(bool set_state=1, void const *vbo_ptr_offset=NULL);
};


struct vert_norm_tc_color : public vert_norm_tc, public color_wrapper { // size = 36
	typedef vert_norm_tc non_color_class;
	vert_norm_tc_color() {}
	vert_norm_tc_color(point const &v_, vector3d const &n_, float ts, float tt, colorRGB const &c_)
		: vert_norm_tc(v_, n_, ts, tt) {set_c3(c_);}
	vert_norm_tc_color(point const &v_, vector3d const &n_, float ts, float tt, colorRGBA const &c_)
		: vert_norm_tc(v_, n_, ts, tt) {set_c4(c_);}
	vert_norm_tc_color(point const &v_, vector3d const &n_, float ts, float tt, unsigned char const *const c_, bool has_alpha=0)
		: vert_norm_tc(v_, n_, ts, tt) {c[0] = c_[0]; c[1] = c_[1]; c[2] = c_[2]; c[3] = (has_alpha ? c_[3] : 255);}
	vert_norm_tc_color(vert_norm_tc const &vntc, color_wrapper const &cw) : vert_norm_tc(vntc), color_wrapper(cw) {}
	void assign(point const &v_, vector3d const &n_, float ts, float tt, unsigned char const *const c_, bool has_alpha=0) {
		v = v_; n = n_; t[0] = ts; t[1] = tt; c[0] = c_[0]; c[1] = c_[1]; c[2] = c_[2]; c[3] = (has_alpha ? c_[3] : 255);
	}
	static void set_vbo_arrays(bool set_state=1, void const *vbo_ptr_offset=NULL);
};


struct vert_tc_color : public vert_tc_t, public color_wrapper { // size = 24
	typedef vert_tc_t non_color_class;
	vert_tc_color() {}
	vert_tc_color(point const &v_, float ts, float tt, colorRGBA const &c_) : vert_tc_t(v_, ts, tt) {set_c4(c_);}
	vert_tc_color(point const &v_, float ts, float tt, unsigned char const c_[4]) : vert_tc_t(v_, ts, tt) {UNROLL_4X(c[i_] = c_[i_];)}
	static void set_vbo_arrays(bool set_state=1, void const *vbo_ptr_offset=NULL);
};


struct vert_norm_comp_tc_color : public vert_norm_comp_tc, public color_wrapper { // size = 28
	typedef vert_norm_tc non_color_class;
	vert_norm_comp_tc_color() {}
	vert_norm_comp_tc_color(vert_norm_comp_tc const &vntc, color_wrapper const &cw) : vert_norm_comp_tc(vntc), color_wrapper(cw) {}
	void assign(point const &v_, vector3d const &n_, float ts, float tt, unsigned char const *const c_) {
		v = v_; set_norm(n_); t[0] = ts; t[1] = tt; c[0] = c_[0]; c[1] = c_[1]; c[2] = c_[2]; c[3] = 255;
	}
	static void set_vbo_arrays(bool set_state=1, void const *vbo_ptr_offset=NULL);
};


struct vert_norm_comp_tc_comp_color : public vert_norm_comp_tc_comp, public color_wrapper { // size = 24
	typedef vert_norm_tc non_color_class;
	vert_norm_comp_tc_comp_color() {}
	vert_norm_comp_tc_comp_color(vert_norm_comp_tc_comp const &vntc, color_wrapper const &cw) : vert_norm_comp_tc_comp(vntc), color_wrapper(cw) {}
	void assign(point const &v_, vector3d const &n_, float ts, float tt, unsigned char const *const c_) {
		v = v_; set_norm(n_); t[0] = 32767*ts; t[1] = 32767*tt; c[0] = c_[0]; c[1] = c_[1]; c[2] = c_[2]; c[3] = 255;
	}
	static void set_vbo_arrays(bool set_state=1, void const *vbo_ptr_offset=NULL);
};


struct vert_norm_color_tangent : public vert_norm_color {
	vector3d t;

	vert_norm_color_tangent() {}
	vert_norm_color_tangent(point const &v_, vector3d const &n_, colorRGBA const     &c_, vector3d const &t_) : vert_norm_color(v_, n_, c_), t(t_) {}
	vert_norm_color_tangent(point const &v_, vector3d const &n_, unsigned char const *c_, vector3d const &t_) : vert_norm_color(v_, n_, c_), t(t_) {}
};


struct texgen_params_t { // size = 32
	float st[2][4];
	texgen_params_t() {UNROLL_4X(st[0][i_] = st[1][i_] = 0.0;)} // zero initialized
};

// Note: could probably use norm_comp, since used for cubes, cylinder ends, and polygons, but won't save much
struct vert_norm_texp : public vert_norm, public texgen_params_t { // size = 76
	vert_norm_texp() {}
	vert_norm_texp(vert_norm const &vn, texgen_params_t const &tp) : vert_norm(vn), texgen_params_t(tp) {}
	vert_norm_texp(point const &v_, vector3d const &n_, texgen_params_t const &tp) : vert_norm(v_, n_), texgen_params_t(tp) {}
	vert_norm_texp(float x, float y, float z, vector3d const &n_, texgen_params_t const &tp) : vert_norm(point(x, y, z), n_), texgen_params_t(tp) {}
	static void set_vbo_arrays(bool set_state=1, void const *vbo_ptr_offset=NULL);
	static void unset_attrs();
};


template<typename T> uint32_t jenkins_one_at_a_time_hash(const T* key, size_t length) { // T is an unsigned integer type

	size_t i = 0;
	uint32_t hash = 0;

	while (i != length) {
		hash += key[i++];
		hash += hash << 10;
		hash ^= hash >> 6;
	}
	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;
	return hash;
}

template<typename T> struct hash_by_bytes { // should work with all packed vertex types
	uint32_t operator()(T const &v) const {return jenkins_one_at_a_time_hash((const uint8_t*)&v, sizeof(T));} // slower but better quality hash
	//uint32_t operator()(T const &v) const {return jenkins_one_at_a_time_hash((const uint32_t*)&v, sizeof(T)>>2);} // faster but lower quality hash
};


bool bind_temp_vbo_from_verts(void const *const verts, unsigned count, unsigned vert_size, void const *&vbo_ptr_offset);
void unbind_temp_vbo();

template< typename T> void set_ptr_state(T const *const verts, unsigned count, unsigned start_ix=0) {
	void const *ptr_offset = NULL;
	if (verts && !bind_temp_vbo_from_verts(verts+start_ix, count, sizeof(T), ptr_offset)) {ptr_offset = verts + start_ix;}
	T::set_vbo_arrays(1, ptr_offset);
}

template <typename T> void unset_ptr_state(T const *const verts) {
	T::unset_attrs();
	if (verts) {unbind_temp_vbo();}
}

template <typename T> void draw_verts(T const *const verts, unsigned count, int gl_type, unsigned start_ix=0) {
	assert(count > 0);
	set_ptr_state(verts, count, start_ix);
	glDrawArrays(gl_type, start_ix, count);
	unset_ptr_state(verts);
}

template <typename T> void draw_verts(vector<T> const &verts, int gl_type, unsigned start_ix=0) {
	if (!verts.empty()) {draw_verts(&verts.front(), verts.size(), gl_type, start_ix);}
}

template <typename T> void draw_and_clear_verts(vector<T> &verts, int gl_type) {
	draw_verts(verts, gl_type);
	verts.resize(0); // clear()?
}

void draw_quads_as_tris(unsigned num_quad_verts, unsigned start_quad_vert=0, unsigned num_instances=1);
bool bind_quads_as_tris_ivbo(unsigned num_quad_verts);
void convert_quad_ixs_to_tri_ixs(vector<unsigned> const &qixs, vector<unsigned> &tixs);

template <typename T> void draw_quad_verts_as_tris(T const *const verts, unsigned count, unsigned start_ix=0, unsigned num_instances=1) {
	assert(count > 0);
	set_ptr_state(verts, count, start_ix);
	draw_quads_as_tris(count, start_ix, num_instances);
	unset_ptr_state(verts);
}

template <typename T> void draw_quad_verts_as_tris(vector<T> const &verts) {
	if (!verts.empty()) {draw_quad_verts_as_tris(&verts.front(), verts.size());}
}

template <typename T> void draw_quad_verts_as_tris_and_clear(vector<T> &verts) {
	draw_quad_verts_as_tris(verts); verts.clear();
}

extern bool use_core_context;
template<typename T> void draw_vect_quads(T const &verts) {
	if (use_core_context) {draw_quad_verts_as_tris(verts);} else {draw_verts(verts, GL_QUADS);}
}


template <typename T> void translate_verts(vector<T> &verts, vector3d const &xlate) {
	for (vector<T>::iterator i = verts.begin(); i != verts.end(); ++i) {i->v += xlate;}
}

template <typename T> void scale_verts(vector<T> &verts, vector3d const &scale) {
	for (vector<T>::iterator i = verts.begin(); i != verts.end(); ++i) {i->v *= scale;}
}

template <typename T> void tri_strip_push(vector<T> &v) {
	assert(v.size() >= 3);
	v.push_back(v[v.size()-2]);
	v.push_back(v[v.size()-2]);
}


template<typename T> struct triangle_t {

	T pts[3];

	triangle_t() {}
	triangle_t(T const &p1, T const &p2, T const &p3) {pts[0] = p1; pts[1] = p2; pts[2] = p3;}
	triangle_t(T const *const p) {pts[0] = p[0]; pts[1] = p[1]; pts[2] = p[2];}
	vector3d get_normal(bool normalize=1) const {return get_poly_norm(pts, normalize);}
	void operator+=(T const &p) {pts[0] += p; pts[1] += p; pts[2] += p;}

	cube_t get_bbox(vector<T> const &p) const { // Note: only works on some types of triangles
		cube_t bbox(pts[0], pts[0]);
		bbox.union_with_pt(pts[1]);
		bbox.union_with_pt(pts[2]);
		return bbox;
	}
};

typedef triangle_t<point>        triangle;
typedef triangle_t<point_d>      triangle_d;
typedef triangle_t<vert_tc_t>    triangle_vtc; // unused
typedef triangle_t<vert_norm_tc> triangle_vntc;


struct ray3d { // size = 40

	point pts[2];
	colorRGBA color;
	
	ray3d(point const &pt0, point const &pt1, colorRGBA const &c) : color(c) {
		pts[0] = pt0;
		pts[1] = pt1;
	}
	ray3d() {}
};


class line_tquad_draw_t;

struct beam3d : public ray3d { // size = 52

	int distant, shooter;
	float intensity;

	beam3d(int dist, int shoot, point const &pt0, point const &pt1, colorRGBA const &c, float int_=1.0)
		: ray3d(pt0, pt1, c), distant(dist), shooter(shoot), intensity(int_) {}
	beam3d() {}
	void draw(line_tquad_draw_t &drawer) const;
};


class line3d { // size = 28

public:
	float width;
	vector<point> points;
	colorRGBA color;

	line3d() : width(0.0) {}
	void draw_lines() const;
	bool empty() const {return points.empty();}
};


struct lightning { // size = 40

	int time, enabled;
	point start, end;
	vector<line3d> path;
	typedef unsigned long long cell_ix_t;
	set<cell_ix_t> cells_seen;

	lightning() : time(0), enabled(-1) {}
	void gen();
	void gen_recur(point const &start, float strength, int xpos, int ypos, int zpos, float zval, int l_frame_counter);
	void draw() const;
	
	cell_ix_t get_cell_ix(unsigned const x, unsigned const y, unsigned const z) const {
		return (x + (cell_ix_t(y) << 16) + (cell_ix_t(z) << 32)); // x, y, z < 2^16
	}
};


colorRGBA const DEF_TEX_COLOR(0.0, 0.0, 0.0, 0.0); // black with alpha of 0.0


class texture_t { // size >= 116

public:
	char type, format, use_mipmaps, defer_load_type;
	bool wrap, mirror, invert_y, do_compress, has_binary_alpha, is_16_bit_gray, no_avg_color_alpha_fill, invert_alpha, normal_map;
	int width, height, ncolors, bump_tid, alpha_tid;
	float anisotropy, mipmap_alpha_weight;
	std::string name;

protected:
	unsigned char *data, *orig_data, *colored_data, *mm_data;
	unsigned tid;
	colorRGBA color;
	vector<unsigned> mm_offsets;

	enum {DEFER_TYPE_NONE=0, DEFER_TYPE_DDS, NUM_DEFER_TYPE};

	void maybe_swap_rb(unsigned char *ptr) const;

public:
	texture_t() : type(0), format(0), use_mipmaps(0), defer_load_type(DEFER_TYPE_NONE), wrap(0), mirror(0), invert_y(0), do_compress(0), has_binary_alpha(0),
		is_16_bit_gray(0), no_avg_color_alpha_fill(0), invert_alpha(0), normal_map(0), width(0), height(0), ncolors(0), bump_tid(-1), alpha_tid(-1),
		anisotropy(1.0), mipmap_alpha_weight(1.0), data(0), orig_data(0), colored_data(0), mm_data(0), tid(0), color(DEF_TEX_COLOR) {}

	texture_t(char t, char f, int w, int h, int wrap_mir, int nc, char um, std::string const &n, bool inv=0, bool do_comp=1, float a=1.0, float maw=1.0, bool nm=0)
		: type(t), format(f), use_mipmaps(um), defer_load_type(DEFER_TYPE_NONE), wrap(wrap_mir != 0), mirror(wrap_mir == 2), invert_y(inv), do_compress(do_comp),
		has_binary_alpha(0), is_16_bit_gray(0), no_avg_color_alpha_fill(0), invert_alpha(0), normal_map(nm), width(w), height(h), ncolors(nc), bump_tid(-1),
		alpha_tid(-1), anisotropy(a), mipmap_alpha_weight(maw), name(n), data(0), orig_data(0), colored_data(0), mm_data(0), tid(0), color(DEF_TEX_COLOR) {}
	bool is_inverted_y_type() const {return (defer_load_type == DEFER_TYPE_DDS);}
	void init();
	void do_gl_init(bool free_after_upload=0);
	GLenum calc_internal_format() const;
	GLenum calc_format() const;
	GLenum get_data_format() const {return (is_16_bit_gray ? GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE);}
	void calc_color();
	void copy_alpha_from_texture(texture_t const &at, bool alpha_in_red_comp);
	void merge_in_alpha_channel(texture_t const &at);
	void build_mipmaps();
	void create_custom_mipmaps();
	unsigned char const *get_mipmap_data(unsigned level) const;
	void set_to_color(colorRGBA const &c);
	void maybe_assign_normal_map_tid(int nm_tid) {if (nm_tid >= 0 && bump_tid < 0) {bump_tid = nm_tid;}}
	void alloc();
	void bind_gl() const;
	void free_mm_data();
	void free_client_mem();
	void free_data() {gl_delete(); free_client_mem();}
	void gl_delete();
	void load(int index, bool allow_diff_width_height=0, bool allow_two_byte_grayscale=0, bool ignore_word_alignment=0);
	void load_raw_bmp(int index, bool allow_diff_width_height, bool allow_two_byte_grayscale);
	void load_targa(int index, bool allow_diff_width_height);
	void load_jpeg(int index, bool allow_diff_width_height);
	void load_png(int index, bool allow_diff_width_height, bool allow_two_byte_grayscale);
	void load_tiff(int index, bool allow_diff_width_height, bool allow_two_byte_grayscale);
	void load_dds(int index);
	void auto_insert_alpha_channel(int index);
	void fill_transparent_with_avg_color();
	void do_invert_y();
	void fix_word_alignment();
	void add_alpha_channel();
	void resize(int new_w, int new_h);
	bool try_compact_to_lum();
	void make_normal_map();
	void gen_rand_texture(unsigned char val, unsigned char a_add=0, unsigned a_rand=256);
	void load_from_gl();
	void deferred_load_and_bind();
	void update_texture_data(int x1, int y1, int x2, int y2);
	int write_to_jpg(std::string const &fn) const;
	int write_to_bmp(std::string const &fn) const;
	int write_to_png(std::string const &fn) const;
	unsigned get_texel_ix(float u, float v) const;
	// assumes width and height are a power of 2
	unsigned get_texel_ix_fast_pow2(float u, float v) const {return (width*(int(height*v) & (height-1)) + (int(width*u) & (width-1)));}
	colorRGBA get_texel(unsigned ix) const;
	colorRGBA get_texel(float u, float v) const {return get_texel(get_texel_ix(u, v));}
	float get_component(float u, float v, int comp) const;
	float get_component_grayscale_pow2(float u, float v) const {return data[get_texel_ix_fast_pow2(u, v)]/255.0;}
	void check_init(bool free_after_upload=0) {if (tid == 0) do_gl_init(free_after_upload);}
	unsigned num_pixels() const {return unsigned(width*height);}
	unsigned num_bytes()  const {return ncolors*num_pixels();}
	unsigned bytes_per_channel() const {return (is_16_bit_gray ? 2U : 1U);}
	void set_color_alpha_to_one() {color.alpha = 1.0;} // to make has_alpha() return 0
	bool has_alpha()    const {return (color.alpha < 1.0 || alpha_tid >= 0);}
	bool is_bound()     const {return (tid > 0);}
	bool is_allocated() const {return (data != nullptr);}
	bool defer_load()   const {return (defer_load_type != DEFER_TYPE_NONE);}
	bool is_loaded()    const {return (is_allocated() || defer_load());}
	colorRGBA get_avg_color() const {return color;}
	unsigned char *get_data() {assert(data); return data;}
	unsigned char const *get_data() const {assert(data); return data;}
	void write_pixel_16_bits(unsigned ix, float val);
};


struct camera_filter {

	int tid;
	unsigned time, init_time; // in ticks
	bool fades;
	colorRGBA color;

	camera_filter() : tid(-1), time(0), init_time(0), fades(0) {}
	camera_filter(colorRGBA const &c, unsigned t, int tid_, bool fades_) : tid(tid_), time(t), init_time(t), fades(fades_), color(c) {}
	void draw(bool apply_texture=1);
};


struct portal {

	point pts[4]; // quads only, for now
	vector3d normal; // for back face determination

	portal() : normal(zero_vector) {}
	static void pre_draw(vector<vert_wrap_t> &verts);
	static void post_draw(vector<vert_wrap_t> &verts);
	void draw(vector<vert_wrap_t> &verts) const;
	point get_center_pt() const {return (pts[0] + pts[1] + pts[2] + pts[3])*0.25;}
	bool is_visible(int reflection_pass) const;
};


struct fire_elem_t {

	float hp, fuel, burn_amt;

	fire_elem_t() : hp(0.0), fuel(0.0), burn_amt(0.0) {}
	bool burn(float val);
	void next_frame(float burn_rate, float consume_rate, float die_rate=1.0);
	static float get_burn_rate();
};


extern float gauss_rand_arr[];

class rgen_core_t {
protected:
	// this is a good random number generator written by Stephen E. Derenzo
	template<typename T> inline void randome_int(T &ranptr) {
		if ((rseed1 = 40014*(rseed1%53668) - 12211*(rseed1/53668)) < 0) rseed1 += 2147483563;
		if ((rseed2 = 40692*(rseed2%52774) - 3791 *(rseed2/52774)) < 0) rseed2 += 2147483399;
		if ((ranptr = rseed1 - rseed2) < 1) ranptr += 2147483562;
	}

public:
	long rseed1, rseed2;

	rgen_core_t() {set_state(1,1);}
	void set_state(long rs1, long rs2) {rseed1 = rs1; rseed2 = rs2;}
	double randd();
};

class rgen_pregen_t : public rgen_core_t {

	std::shared_ptr<vector<double>> pregen_rand_reals;
	unsigned cur_pos;

public:
	rgen_pregen_t() : cur_pos(0) {}
	void pregen_floats(unsigned num);
	double randd();
};

template<typename base> class rand_gen_template_t : public base {

public:
	int rand() {
		int rand_num;
		randome_int(rand_num);
		return rand_num;
	}
	int rand_fast() { // faster but lower quality, only uses rseed1
		rseed1 *= 16807;
		return rseed1;
	}
	float rand_float_fast() { // faster but lower quality, only uses rseed1 (uniform 0 to 1)
		union {float fres; unsigned int ires;};
		rseed1 *= 16807;
		ires = ((((unsigned long)rseed1)>>9) | 0x3f800000);
		return fres - 1.0f;
	}
	int rand_seed_mix() {
		int val1(rand()); swap(rseed1, rseed2); return (val1 + rand()); // more random
		//return (rseed1 ^ (rseed2 >> 8)); // faster (should call rand2_mix() after)
	}
	void rand_mix() {rand(); swap(rseed1, rseed2);}
	float rand_float() {return 0.000001*(rand()%1000000);} // uniform 0 to 1
	float signed_rand_float() {return 2.0*float(randd()) - 1.0;}
	bool rand_bool() {return ((rand()&1) != 0);}
	float rand_uniform(float val1, float val2) {assert(val1 <= val2); return val1 + (val2 - val1)*float(randd());}
	unsigned rand_uniform_uint(unsigned min_val, unsigned max_val) {assert(min_val <= max_val); return (min_val + (rand() % (max_val - min_val + 1)));}
	int rand_int(int start, int end) {return (rand()%(end - start + 1) + start);} // used for trees; start and end should be positive
	float rgauss() {return gauss_rand_arr[rand()%N_RAND_DIST];} // mean = 0.0, std_dev = 1.0
	float rand_gaussian(float mean, float std_dev) {return mean + std_dev*rgauss();}
	bool rand_probability(float prob) {return (prob >= 1.0 || (prob > 0.0 && rand_float() < prob));}
	vector3d rand_vector(float scale=1.0);
	vector3d signed_rand_vector(float scale=1.0);
	vector3d signed_rand_vector_xy(float scale=1.0);
	vector3d signed_rand_vector_norm(float scale=1.0);
	vector3d signed_rand_vector_spherical(float scale=1.0);
	vector3d signed_rand_vector_spherical_noloop(float scale=1.0);
	point gen_rand_cube_point(cube_t const &c);
};

typedef rand_gen_template_t<rgen_core_t> rand_gen_t;
typedef rand_gen_template_t<rgen_pregen_t> rand_gen_pregen_t;


class shader_t;
class vpc_shader_t;


class volume_part_cloud {

public:
	typedef vert_norm_comp vert_type_t;

protected:
	static vector<vert_type_t> unscaled_points[2];
	vector<vert_type_t> points;

public:
	static colorRGBA gen_color(rand_gen_t &rgen);
	static void calc_unscaled_points(bool simplified);
	vector<vert_type_t> const &get_points() const {return points;}
	void gen_pts(vector3d const &size, point const &pos=all_zeros, bool simplified=0);
	void gen_pts(float radius, point const &pos=all_zeros, bool simplified=0) {gen_pts(vector3d(radius, radius, radius), pos, simplified);}
	static void shader_setup(vpc_shader_t &s, unsigned noise_ncomp, bool ridged=1, float alpha_bias=-0.4, float dist_bias=0.0,
		unsigned num_octaves=5, bool enable_lighting=0, bool use_cloud_mode=0, bool irregular_shape=0);
	void draw_quads(bool depth_map_already_disabled=0) const;
	bool enabled() const {return !points.empty();}
	void clear() {points.clear();}
};


struct water_params_t {

	float alpha, mud, bright, reflect, green, wave_amp;
	water_params_t() {set_def_water();}
	void set_def_water();
	void set_def_lava();
};


struct text_string_t {

	std::string str;
	point pos;
	float size;
	colorRGBA color;

	text_string_t() {}
	text_string_t(std::string const &s, point const &p, float sz, colorRGBA const &c) : str(s), pos(p), size(sz), color(c) {}
};

struct text_drawer_t {

	vector<text_string_t> strs;
	void draw() const;
};


// colors
colorRGBA const RED      (1.0,  0.0,  0.0,  1.0);
colorRGBA const GREEN    (0.0,  1.0,  0.0,  1.0);
colorRGBA const BLUE     (0.0,  0.0,  1.0,  1.0);
colorRGBA const BLACK    (0.0,  0.0,  0.0,  1.0);
colorRGBA const WHITE    (1.0,  1.0,  1.0,  1.0);
colorRGBA const CYAN     (0.0,  1.0,  1.0,  1.0);
colorRGBA const MAGENTA  (1.0,  0.0,  1.0,  1.0);
colorRGBA const YELLOW   (1.0,  1.0,  0.0,  1.0);
colorRGBA const PURPLE   (0.5,  0.0,  0.6,  1.0);
colorRGBA const TRANS    (1.0,  1.0,  1.0,  0.4);
colorRGBA const ALPHA0   (1.0,  1.0,  1.0,  0.0);
colorRGBA const RGBA0    (0.0,  0.0,  0.0,  0.0);
colorRGBA const ALPHA0_5 (1.0,  1.0,  1.0,  0.5);
colorRGBA const BROWN    (0.6,  0.25, 0.1,  1.0);
colorRGBA const DK_BROWN (0.3,  0.15, 0.08, 1.0);
colorRGBA const LT_BROWN (0.6,  0.4,  0.2,  1.0);
colorRGBA const LT_RED   (1.0,  0.58, 0.58, 1.0);
colorRGBA const DK_RED   (0.7,  0.0,  0.0,  1.0);
colorRGBA const LT_GREEN (0.58, 1.0,  0.58, 1.0);
colorRGBA const DK_GREEN (0.0,  0.7,  0.0,  1.0);
colorRGBA const OLIVE    (0.3,  0.4,  0.2,  1.0);
colorRGBA const LT_BLUE  (0.58, 0.58, 1.0,  1.0);
colorRGBA const DK_BLUE  (0.0,  0.0,  0.7,  1.0);
colorRGBA const ORANGE   (1.0,  0.5,  0.0,  1.0);
colorRGBA const PINK     (1.0,  0.5,  0.5,  1.0);
colorRGBA const GRAY     (0.5,  0.5,  0.5,  1.0);
colorRGBA const LT_GRAY  (0.75, 0.75, 0.75, 1.0);
colorRGBA const DK_GRAY  (0.25, 0.25, 0.25, 1.0);
colorRGBA const GRAY_BLACK(0.1, 0.1,  0.1,  1.0);
colorRGBA const BKGRAY   (0.05, 0.05, 0.05, 1.0);
colorRGBA const WATER    (0.35, 0.35, 1.0,  1.0);
colorRGBA const SUN_C    (1.0,  0.8,  0.5,  1.0);
colorRGBA const SUN_LT_C (1.0,  1.0,  0.85, 1.0);
colorRGBA const MOON_C   (0.9,  0.8,  0.7,  1.0);
colorRGBA const LEAF_C   (0.0,  0.6,  0.0,  1.0);
colorRGBA const LITN_C   (0.8,  0.9,  1.0,  1.0);
colorRGBA const WATER_C  (0.4,  0.4,  1.0,  WATER_ALPHA);
colorRGBA const ICE_C    (0.65, 0.65, 1.0,  ICE_ALPHA);
colorRGBA const SPLASH_C (0.7,  0.7,  1.0,  SPLASH_ALPHA);
colorRGBA const BLOOD_C  (0.4,  0.0,  0.0,  1.0);
colorRGBA const CLOUD_C  (0.9,  0.9,  0.9,  1.0);
colorRGBA const TREE_C   (0.7,  0.4,  0.2,  1.0);
colorRGBA const PTREE_C  (0.8,  0.6,  0.5,  1.0);
colorRGBA const MUD_C    (0.3,  0.18, 0.09, 1.0);
colorRGBA const MUD_S_C  (0.7,  0.35, 0.25, 1.0);
colorRGBA const GOLD     (0.7,  0.45, 0.05, 1.0);
colorRGBA const BRASS_C  (0.7,  0.65, 0.25, 1.0);
colorRGBA const BRONZE_C (0.52, 0.23, 0.17, 1.0);
colorRGBA const COPPER_C (0.7,  0.3,  0.05,  1.0);
colorRGBA const MED_GREEN(0.2,  0.7,  0.2,  1.0);
colorRGBA const SNOW_COLOR(1.0, 1.0,  1.4,  1.0); // slightly bluish (saturated)
colorRGBA const LAVA_COLOR(1.0, 0.15, 0.05, 1.0); // red-orange
colorRGBA const FREEZE_COLOR(0.3, 0.3, 1.0, 1.0);
colorRGBA const BACKGROUND_DAY(0.2, 0.3, 0.8, 1.0);
colorRGBA const BACKGROUND_NIGHT(BLACK);


void register_timing_value(const char *str, int delta_time);
void toggle_timing_profiler();
void timing_profiler_stats();

// macros
#define GET_TIME_MS()    glutGet(GLUT_ELAPSED_TIME)
#define RESET_TIME       int const timer1(GET_TIME_MS());
#define GET_DELTA_TIME   (GET_TIME_MS() - timer1)
#define PRINT_TIME(str) {register_timing_value(str, GET_DELTA_TIME);}
#define PRINT_TIME_STR(str) {PRINT_TIME((str).c_str());}
#define PRINT_TIME_ONSCREEN(str) {std::ostringstream oss; oss << str << " time = " << GET_DELTA_TIME; print_debug_text(oss);}
#define TXT(x) #x"=" << x << " "

#if 0 // still only ms accuracy in windows
#include <time.h>
#define GET_TIME_CLOCK()  clock()
#define RESET_TIME2       clock_t const timer1(GET_TIME_CLOCK());
#define GET_DELTA_TIME2   double(GET_TIME_CLOCK() - timer1)/double(CLOCKS_PER_SEC)
#define PRINT_TIME2(str) {cout << str << " time = " << GET_DELTA_TIME << endl;}
#endif

class timer_t {
	std::string name;
	int timer1;
public:
	timer_t(char const *const name_) : name(name_), timer1(GET_TIME_MS()) {}
	timer_t(std::string const &name_) : name(name_), timer1(GET_TIME_MS()) {}
	~timer_t() {end();}
	void end() {if (!name.empty()) {register_timing_value(name.c_str(), GET_DELTA_TIME); name.clear();}}
};


// world modes
enum {WMODE_GROUND=0, WMODE_UNIVERSE, WMODE_INF_TERRAIN, NUM_WMODE};

// movement directions
enum {MOVE_FRONT, MOVE_BACK, MOVE_LEFT, MOVE_RIGHT, MOVE_STOP};

// gameplay powerups
enum {PU_NONE=-1, PU_DAMAGE, PU_REGEN, PU_SHIELD, PU_SPEED, PU_FLIGHT, PU_INVISIBILITY, NUM_POWERUPS};

// object definitions
enum {RAIN = 0, SNOW, HAIL, LEAF, BALL, S_BALL, SMILEY, BLOOD, CHARRED, CHUNK,
	SFPART, ROCKET, LANDMINE, SEEK_D, STAR5, PLASMA, GRENADE, CGRENADE, SHRAPNEL, SHELLC,
	PROJC, DROPLET, WDROPLET, SAND, DIRT, ROCK, FRAGMENT, PARTICLE, HEALTH, SHIELD,
	POWERUP, WEAPON, AMMO, WA_PACK, CAMERA, PRECIP, BLAST_RADIUS, PROJECTILE, BEAM, IMPACT,
	PLASMA_LT_D, LASER, DROWNED, BURNED, FIRE, FELL, FROZEN, SUFFOCATED, CRUSHED, GASSED,
	WAYPOINT, SMOKE, DYNAM_PART, SKULL, GRASS, TELEFRAG, SAWBLADE, MAT_SPHERE, COLLISION, RAPT_PROJ,
	FREEZE_BOMB, XLOCATOR, XLOCATOR_DEATH, JUMP_PAD, TELEPORTER, NUM_TOT_OBJS};

inline bool is_rocket_type(int type) {return (type == ROCKET || type == SEEK_D || type == RAPT_PROJ || type == FREEZE_BOMB);}

// textures
enum {GROUND_TEX = 0, DARK_ROCK_TEX, WATER_TEX, STUCCO_TEX, CLOUD_TEX, BRICK2_TEX, MOON_TEX, EARTH_TEX, MARBLE_TEX, SNOW_TEX,
	LEAF_TEX, WOOD_TEX, SAND_TEX, DIRT_TEX, CAMOFLAGE_TEX, HEDGE_TEX, BRICK_TEX, MANHOLE_TEX, PALM_FROND_TEX, SMOKE_TEX,
	PLASMA_TEX, GEN_TEX, LANDSCAPE_TEX, TREE_END_TEX, TREE_HEMI_TEX, SHINGLE_TEX, PANELING_TEX, CBLOCK_TEX, MJ_LEAF_TEX, LIVE_OAK_TEX,
	LEAF2_TEX, LEAF3_TEX, PLANT1_TEX, PLANT2_TEX, PLANT3_TEX, PLANT4_TEX, FENCE_TEX, SKULL_TEX, RADIATION_TEX, YUCK_TEX,
	SAW_TEX, SAW_B_TEX, BLUR_TEX, SBLUR_TEX, PINE_TEX, NOISE_TEX, WOOD2_TEX, HBBRICK_TEX, PARTB_TEX, PLASTER_TEX,
	TILE_TEX, LOGO_TEX, DISINT_TEX, BLUR_TEX_INV, HSTRIPE_TEX, VSTRIPE_TEX, BCUBE_TEX, EXPLOSION_TEX, SHIP_HULL_TEX, BCUBE2_TEX,
	BCUBE_T_TEX, ROCK_SPHERE_TEX, PAPAYA_TEX, COFFEE_TEX, SMILEY_SKULL_TEX, ICE_TEX, ROCK_TEX, BLACK_TEX, WHITE_TEX, FIRE_TEX,
	CLOUD_RAW_TEX, SNOWFLAKE_TEX, BLUR_CENT_TEX, GRADIENT_TEX, GRASS_BLADE_TEX, WIND_TEX, MOSSY_ROCK_TEX, BARK1_TEX, BARK2_TEX, BARK2_NORMAL_TEX,
	BARK3_TEX, BARK4_TEX, WATER_NORMAL_TEX, OCEAN_WATER_NORMAL_TEX, WATER_CAUSTIC_TEX, PS_NOISE_TEX, NOISE_GEN_TEX, NOISE_GEN_MIPMAP_TEX, SPARSE_NOISE_TEX, PLAYER_BBB_TEX,
	PINE_TREE_TEX, FLARE1_TEX, FLARE2_TEX, FLARE3_TEX, FLARE4_TEX, FLARE5_TEX, FOAM_TEX, BULLET_D_TEX, BULLET_A_TEX, BULLET_N_TEX,
	ROCK_NORMAL_TEX, RAINDROP_TEX, SPACESHIP1_TEX, SPACESHIP2_TEX, BLOOD_SPLAT_TEX, LICHEN_TEX, PALM_BARK_TEX, DAISY_TEX, LAVA_TEX, SMOKE_PUFF_TEX,
	BARK5_TEX, BARK6_TEX, RIPPLE_MAP_TEX, STARBURST_TEX, ROCK1_NORMAL_TEX, ROCK2_NORMAL_TEX, ROCK3_NORMAL_TEX, DIRT_NORMAL_TEX, FLAT_NMAP_TEX, RED_TEX,
	HAZARD_TEX, BLDG_WINDOW_TEX, NUM_PREDEF_TEXTURES
};

// lighting files/types
enum {LIGHTING_SKY=0, LIGHTING_GLOBAL, LIGHTING_LOCAL, LIGHTING_COBJ_ACCUM, LIGHTING_DYNAMIC /*must be last*/, NUM_LIGHTING_TYPES};

// heightmap generation modes
enum {MGEN_SINE=0, MGEN_SIMPLEX, MGEN_PERLIN, MGEN_SIMPLEX_GPU, MGEN_DWARP_GPU, MGEN_END};


// shadow mask bits
#define MESH_SHADOW      0x02
#define SHADOWED_ALL     0xCF

// light sources
enum {LIGHT_SUN = 0, LIGHT_MOON, NUM_LIGHT_SRC};

#define SUN_SHADOW  0x01
#define MOON_SHADOW 0x02

#define EF_Z1  0x01
#define EF_Z2  0x02
#define EF_Y1  0x04
#define EF_Y2  0x08
#define EF_X1  0x10
#define EF_X2  0x20
#define EF_ALL 0x3F

unsigned char const EFLAGS[3][2] = {{EF_X1, EF_X2}, {EF_Y1, EF_Y2}, {EF_Z1, EF_Z2}};

// forward references
struct base_mat_t;
class  sd_sphere_d;
struct dwobject;
class  obj_group;
struct star;
struct cobj_params;
class  coll_obj;
class  coll_obj_group;
class  shape3d;
struct lightning;
struct color_tid_vol;
class  vert_coll_detector;
struct cobj_query_callback;
struct user_waypt_t;
class  voxel_model;


#endif // _3DWORLD_H_

