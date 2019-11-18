#pragma once
#include <vector>
#include <array>
#include <cmath>
#include <iostream>

namespace lingeo3D{

	const float flt_tolerance = 0.00001;                 // tolerance that will be used in calculation of coordinates (e.g. for comparing the points)
	const float inter_area_width = 100.0;                    // defines the width (counted in flt_tolerances) of INTER_AREA that used to avoid some corner cases
	enum areas_t {LEFT_SIDE, INTER_SIDE, RIGHT_SIDE}; // there are 3 areas that are defined by the line cut of plane: left, right and inter area. The last one represents the same line with defined thickness (inter_are_width)
	enum degen_t {NORMAL_DEG, LINE_DEG, POINT_DEG, INVALID_DEG};

	template<typename T>
	struct point_t {      // (x , y, z)
		T x_, y_, z_;
		explicit point_t(T x = NAN, T y = NAN, T z = NAN);
		point_t(const point_t<T> &point);
		
		void print() const;		// prints on screen: "(x ; y)"

		bool valid() const;

		point_t<T> vector_prod(point_t<T> const &pnt) const;

		T scalar_prod(point_t<T> const &pnt) const;

		T distance(point_t<T> const &pnt = point_t<T>{0.0f, 0.0f, 0.0f}) const;
		
		bool operator==(point_t<T> const &pnt) const{
			return (std::abs(x_ - pnt.x_) < flt_tolerance && std::abs(y_ - pnt.y_) < flt_tolerance && std::abs(z_ - pnt.z_) < flt_tolerance) ? true : false;
		}
		bool operator!=(point_t<T> const &pnt) const{
			return !operator==(pnt);
		}

		point_t<T>& operator+=(point_t const &another){
			x_ += another.x_; y_ += another.y_; z_ += another.z_;
			return *this;
		}

		point_t<T>& operator-=(point_t const &another){
			x_ -= another.x_; y_ -= another.y_; z_ -= another.z_;
			return *this;
		}

		point_t<T> operator-(point_t<T> const &another) const{
			point_t<T> ret(x_ - another.x_, y_ - another.y_, z_ - another.z_);
			return ret;
		}

		point_t<T> operator+(point_t const &another) const{
			point_t<T> ret(x_ + another.x_, y_ + another.y_, z_ + another.z_);
			return ret;	
		}
		point_t<T> operator*(float num) const{
			point_t<T> ret(x_ * num, y_ * num, z_ * num);
			return ret;	
		}

		point_t<T> normalize(){
			T module = distance(point_t<T>{0.0, 0.0, 0.0});
			x_ /= module;
			y_ /= module;
			z_ /= module;
			return *this;
		}


		virtual ~point_t(){};

//		point_t& operator=(const point_t &pnt);
	};
template<typename T>
	class line_t{        // {r : r = root + dir * t , t is real} 
	
		point_t<T> root_, dir_;
	
	public:
	
		line_t();                                     // default line x = y
	
		line_t(struct point_t<T> a1, struct point_t<T> a2); // constructs the line passing through points a1 and a2
		
		bool valid() const;
		
		void print() const;								           // prints on screen: "ax + by + c = 0"
		
		bool perpendicular(line_t<T> const &another) const;

		T angle(point_t<T> const &pnt1, point_t<T> const &pnt2) const;

		point_t<T> get_root() const{
			return root_;

		}
		point_t<T> get_dir() const{
			return dir_;
		}
		virtual ~line_t(){};
	};
template<typename T>
	struct plane_t {     // ax + by + cz + d = 0
	private:

		T a_, b_, c_, d_;

	public:

		explicit plane_t(T a = 0.0, T b = 0.0, T c = 1.0, T d = 0.0);

		plane_t(point_t<T> p1, point_t<T> p2, point_t<T> p3);

		plane_t(line_t<T> line, point_t<T> pnt);

		point_t<T> get_outer_point();
		point_t<T> intersection(line_t<T> const &line) const;
		enum areas_t get_side_area(point_t<T> const &point) const;         // tells in what area the point is lying
		bool separates(point_t<T> const &pnt1, point_t<T> const &pnt2) const; // checks if the plane separates two points		
		point_t<T> get_normal() const;

		void print() const;

		bool valid() const;

		virtual ~plane_t(){};
	};

template<typename T>
	struct polygon_t{ // (x1, y1, z1) (x2, y2, z2) ... (xn, yn, zn)

		std::vector<point_t<T>> vertices;

		polygon_t();                                  // empty polygon
		explicit polygon_t(std::vector<point_t<T>> vertices);     // constructing by vector<point_t> array

		T square() const;                               // returns the square of polygon
		void print() const;                                 // prints: "Polygon is nangle: (x1, y1) (x2, y2) ... (xn, yn)"
		
		line_t<T> get_side(int index) const;                   // return the line holding the polygon side (n side is a line constructed with nth and (n + 1)th vertex of polygon)

		bool valid() const;
		bool is_divided_by_side_plane(polygon_t<T> const & another) const;
		bool intersect(polygon_t<T> const & another) const;                                      // checks if polygon intersects with "another" polygon
		bool holding(point_t<T> const & vert) const;											  // tells if this polygon is holding the vert as one of it's vertices
		void add(point_t<T> const &vert);														  // adds the vert to the tail of vertices if its not in vertices already
		plane_t<T> get_plane();
		virtual ~polygon_t(){};
	};







//******************DEFINITIONS OF TEMPLATE METHODS***********************





	const float pi_ = 3.141593;


//*********STRUCT point_t BEGIN**********

template<typename T>
point_t<T>::point_t(T x, T y, T z): x_(x), y_(y), z_(z){

}
template<typename T>
point_t<T>::point_t(const point_t<T> &point): x_(point.x_), y_(point.y_), z_(point.z_){

}


template<typename T>
void point_t<T>::print() const{
	std::cout << x_ << " " << y_ << " " << z_ << std::endl;
}

template<typename T>
T point_t<T>::distance(point_t<T> const &pnt) const{
	T dx = x_ - pnt.x_;
	T dy = y_ - pnt.y_;
	T dz = z_ - pnt.z_;

	return std::sqrt(dx * dx + dy * dy + dz * dz);
}

template<typename T>
T point_t<T>::scalar_prod(point_t<T> const &pnt) const{
	return x_*pnt.x_ + y_*pnt.y_ + z_*pnt.z_;
}

template<typename T>
point_t<T> point_t<T>::vector_prod(point_t<T> const &pnt) const{
	return point_t<T>{y_ * pnt.z_ - pnt.y_ * z_, -x_ * pnt.z_ + pnt.x_ * z_, x_ * pnt.y_ - pnt.x_ * y_};
}

template<typename T>
bool point_t<T>::valid() const{
	return (x_ != x_ || y_ != y_ || z_ != z_) ? false : true;
}


//*********STRUCT point_t END************

//*********STRUCT line_t BEGIN***********

template<typename T>
line_t<T>::line_t(): root_{0.0, 0.0, 0.0}, dir_{1.0, 1.0, 1.0}{

}

template<typename T>
line_t<T>::line_t(struct point_t<T> a1, struct point_t<T> a2): root_{a1}, dir_{a2 - a1}{

}


template<typename T>
bool line_t<T>::perpendicular(line_t<T> const &another) const{
	T scalar_product = dir_.x_ * another.dir_.x_ + dir_.y_ * another.dir_.y_ + dir_.z_ * another.dir_.z_;
	return (std::abs(scalar_product) < flt_tolerance) ? true : false;
}


template<typename T>
bool line_t<T>::valid() const{
	return root_.valid() && dir_.valid() && dir_ != point_t<T>{0.0, 0.0};
}
template<typename T>
void line_t<T>::print() const{
	std::cout << "Root is: "; root_.print();
	std::cout << "; Dir is: "; dir_.print();
	std::cout << std::endl;
}

template<typename T>
T line_t<T>::angle(point_t<T> const &pnt1, point_t<T> const &pnt2) const{
	plane_t<T> pln1{*this, pnt1};
	plane_t<T> pln2{*this, pnt2};
	if(!pln1.valid() || !pln2.valid())
		return NAN;
	point_t<T> norm1 = pln1.get_normal();
	point_t<T> norm2 = pln2.get_normal();
	point_t<T> vect_prod = norm1.vector_prod(norm2);
	int sign = ((((vect_prod.x_ > 0) == (dir_.x_ > 0)) && std::abs(dir_.x_) > flt_tolerance) ||
				(((vect_prod.y_ > 0) == (dir_.y_ > 0)) && std::abs(dir_.y_) > flt_tolerance) ||
				(((vect_prod.z_ > 0) == (dir_.z_ > 0)) && std::abs(dir_.z_) > flt_tolerance)   ) ? 1 : -1;

	T mod1 = norm1.distance(point_t<T>{0.0, 0.0, 0.0});
	T mod2 = norm2.distance(point_t<T>{0.0, 0.0, 0.0});
	T scalar = norm1.scalar_prod(norm2);
	T angle = std::acos(scalar/(mod1 * mod2));
	if(sign < 0)
		angle = 2 * pi_ - angle;
	return angle;
}


//*********STRUCT line_t END*************

//*********STRUCT plane_t BEGIN**********

template<typename T>
plane_t<T>::plane_t(T a, T b, T c, T d): a_(a), b_(b), c_(c), d_(d){

}
template<typename T>
plane_t<T>::plane_t(point_t<T> p1, point_t<T> p2, point_t<T> p3){
	point_t<T> v1 = p2 - p1;
	point_t<T> v2 = p3 - p1;
	a_ = v1.y_ * v2.z_ - v2.y_ * v1.z_;
	b_ = -(v1.x_ * v2.z_ - v2.x_ * v1.z_);
	c_ = v1.x_ * v2.y_ - v2.x_ * v1.y_;
	d_ =-(a_ * p1.x_ + b_ * p1.y_ + c_ * p1.z_);
}
template<typename T>
plane_t<T>::plane_t(line_t<T> line, point_t<T> pnt): plane_t<T>(pnt, line.get_root(), line.get_dir() + line.get_root()){

}


template<typename T>
point_t<T> plane_t<T>::intersection(line_t<T> const &line) const{
	line_t<T> normal(point_t<T>{0.0, 0.0, 0.0}, point_t<T>{a_, b_, c_});
	if(line.perpendicular(normal)){
		return point_t<T>{};
	}

	point_t<T> root = line.get_root();
	point_t<T> dir = line.get_dir();

	T param = -(root.x_ * a_ + root.y_ * b_ + root.z_ * c_ + d_)/(dir.x_ * a_ + dir.y_ * b_ + dir.z_ * c_);

	point_t<T> ret(root + dir * param);
	return ret;

}

template<typename T>
point_t<T> plane_t<T>::get_normal() const{
	return point_t<T>{a_, b_, c_};
}

template<typename T>
enum areas_t plane_t<T>::get_side_area(point_t<T> const &point) const{
	T side_offset = a_ * point.x_ + b_ * point.y_ + c_ * point.z_ + d_;

	if(side_offset > 0.0 + flt_tolerance * inter_area_width)
		return LEFT_SIDE;
	
	if(side_offset > 0.0 - flt_tolerance * inter_area_width)
		return INTER_SIDE;
	
	return 
	RIGHT_SIDE;

}


template<typename T>
bool plane_t<T>::separates(point_t<T> const &pnt1, point_t<T> const &pnt2) const{
	enum areas_t side1 = get_side_area(pnt1);
	enum areas_t side2 = get_side_area(pnt2);
	if(side1 == INTER_SIDE || side2 == INTER_SIDE)
		return false;
	return (side1 == side2) ? false : true;
}

template<typename T>
bool plane_t<T>::valid() const{
	point_t<T> norm = get_normal();
	return (!norm.valid() || d_ != d_) ? false : true;
}

template<typename T>
void plane_t<T>::print() const{
	const char* str_1 = (b_ > 0.0) ? "+ " : "- ";
	const char* str_2 = (c_ > 0.0) ? "+ " : "- ";
	const char* str_3 = (d_ > 0.0) ? "+ " : "- ";

	std::cout << a_ << "x " << str_1 << std::abs(b_) << "y " << str_2 << std::abs(c_) << "z " << str_3 << std::abs(d_) << " = 0" << std::endl;

}


//*********STRUCT plane_t END************

//*********STRUCT polygon_t BEGIN********

template<typename T>
lingeo3D::polygon_t<T>::polygon_t(){

}

template<typename T>
polygon_t<T>::polygon_t(std::vector<point_t<T>> vertices){
	this -> vertices = vertices;

}     // constructing by vector<point_t> array

template<typename T>
T polygon_t<T>::square() const{
	//TODO
	return 0.0;
}

template<typename T>
void polygon_t<T>::print() const{
	for(int i = 0; i < vertices.size(); i++)
		vertices[i].print();
	std::cout << std::endl;
}

template<typename T>
line_t<T> polygon_t<T>::get_side(int index) const{
	while(index < 0)
		index += vertices.size();

	line_t<T> ret(vertices[index % vertices.size()], vertices[(index + 1) % vertices.size()]);
	
	return ret;
}

template<typename T>
bool polygon_t<T>::valid() const{
	if(vertices.size() < 3)
		return false;
	
	for(int i = 0; i < vertices.size(); i++)
		if(!vertices[i].valid())
			return false;

	return true;
}

template<typename T>
bool polygon_t<T>::is_divided_by_side_plane(polygon_t<T> const & another) const{
	for(int i = 0; i < vertices.size(); i++){
		line_t<T> line = get_side(i);
		point_t<T> pnt = vertices[(i + 2) % vertices.size()];
		T max_angle = 0.0;
		T min_angle = pi_ * 2.0;
		for(int j = 0; j < another.vertices.size(); j++){
			T angle = line.angle(pnt, another.vertices[j]);
			if(angle != angle)
				return false;
			if(angle < min_angle) min_angle = angle;
			if(angle > max_angle) max_angle = angle;
		}
		if(max_angle - min_angle < pi_ && min_angle != 0.0 && max_angle != pi_* 2){
			return true;
		}
	}
	return false;	
}

template<typename T>
bool polygon_t<T>::intersect(polygon_t<T> const & another) const{
	if(!valid() || !another.valid()){
		return false;
	}

	return (is_divided_by_side_plane(another) || another.is_divided_by_side_plane(*this)) ? false : true;

}

template<typename T>
bool polygon_t<T>::holding(point_t<T> const & vert) const{
	for(int i = 0; i < vertices.size(); i++)
		if(vert == vertices[i])
			return true;
	return false;	
}
template<typename T>
plane_t<T> polygon_t<T>::get_plane(){
	return plane_t<T>{vertices[0], vertices[1], vertices[2]};
}

template<typename T>
void polygon_t<T>::add(point_t<T> const &vert){
	vertices.insert(vertices.end(), vert);
}

//*********STRUCT polygon_t END**********



};