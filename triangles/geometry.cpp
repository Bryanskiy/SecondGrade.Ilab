#include "geometry.h"

namespace {

bool is_double_valid(double x) {
    return (std::isnan(x) || !std::isfinite(x)) ? false : true;
}

bool is_doubles_equal(double lhs, double rhs) {
    return std::abs(lhs - rhs) < TOLERANCE;
}

}

/* ------------------------------------------------
                START POINT_METHODS
 -------------------------------------------------*/

point3D_t::point3D_t() : x(std::numeric_limits<double>::quiet_NaN()) ,
                         y(std::numeric_limits<double>::quiet_NaN()) ,
                         z(std::numeric_limits<double>::quiet_NaN()) {}

point3D_t::point3D_t(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

bool point3D_t::valid() const {
    bool flag = is_double_valid(x) &&
                is_double_valid(y) &&
                is_double_valid(z);

    return flag;
}

bool operator==(const point3D_t& lhs, const point3D_t& rhs) {
    bool flag = lhs.valid() && rhs.valid() &&
                is_doubles_equal(lhs.x, rhs.x) &&
                is_doubles_equal(lhs.y, rhs.y) &&
                is_doubles_equal(lhs.z, rhs.z);

    return flag;
}

std::istream& operator>>(std::istream& in, point3D_t& point) {
    in >> point.x >> point.y >> point.z;
    return in;
}

std::ostream& operator<<(std::ostream& out, const point3D_t& point) {
    out << '(' << point.x << ", " << point.y << ", " << point.z << ')';
    return out;
}

double distance(const point3D_t& rhs, const point3D_t& lhs) {
    return vector3D_t(lhs, rhs).len();
}

/* ------------------------------------------------
                END POINT_METHODS
 -------------------------------------------------*/




/* ------------------------------------------------
                START VECTOR_METHODS
 -------------------------------------------------*/
vector3D_t::vector3D_t() : x(std::numeric_limits<double>::quiet_NaN()) ,
                           y(std::numeric_limits<double>::quiet_NaN()) ,
                           z(std::numeric_limits<double>::quiet_NaN()) {}

vector3D_t::vector3D_t(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

vector3D_t::vector3D_t(const point3D_t& r_vector) : x(r_vector.x), y(r_vector.y), z(r_vector.z) {}

vector3D_t::vector3D_t(const point3D_t& lhs, const point3D_t& rhs) : x(rhs.x - lhs.x),
                                                                     y(rhs.y - lhs.y),
                                                                     z(rhs.z - lhs.z) {}

vector3D_t& vector3D_t::operator+=(const vector3D_t& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
}

const vector3D_t vector3D_t::operator-() const {
    vector3D_t tmp = *this;
    tmp.x = -x;
    tmp.y = -y;
    tmp.z = -z;

    return tmp;
}

vector3D_t& vector3D_t::operator-=(const vector3D_t& rhs) {
    return *this += (-rhs);
}

vector3D_t& vector3D_t::operator*=(double lambda) {
    x *= lambda;
    y *= lambda;
    z *= lambda;
}

bool vector3D_t::valid() const {
    point3D_t tmp(x, y, z);
    return tmp.valid();
}

double vector3D_t::len() const{
    return std::sqrt(dot(*this, *this));
}

bool vector3D_t::is_zero() const{
    bool flag = this->valid() &&
                is_doubles_equal(x, 0.0) &&
                is_doubles_equal(y, 0.0) &&
                is_doubles_equal(z, 0.0);

    return flag;
}

bool operator==(const vector3D_t& lhs, const vector3D_t& rhs) {
    return point3D_t(lhs.x, lhs.y, lhs.z) == point3D_t(rhs.x, rhs.y, rhs.z);
}

const vector3D_t operator+(const vector3D_t& lhs, const vector3D_t& rhs) {
    vector3D_t tmp = lhs;
    return tmp += rhs;
}

const vector3D_t operator-(const vector3D_t& lhs, const vector3D_t& rhs) {
    vector3D_t tmp = lhs;
    return tmp -= rhs;
}

const vector3D_t operator*(double lambda, const vector3D_t& rhs)  {
    vector3D_t tmp = rhs;
    return tmp *= lambda;
}

const vector3D_t operator*(const vector3D_t& lhs, double lambda) {
    return lambda * lhs;
}

double dot(const vector3D_t& lhs, const vector3D_t& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

vector3D_t cross(const vector3D_t& lhs, const vector3D_t& rhs) {
    double x = lhs.y * rhs.z - lhs.z * rhs.y;
    double y = lhs.z * rhs.x - lhs.x * rhs.z;
    double z = lhs.x * rhs.y - lhs.y * rhs.x;
    return vector3D_t(x, y, z);
}

bool is_parallel(const vector3D_t& lhs, const vector3D_t& rhs) {
    bool flag = lhs.valid() && rhs.valid() && is_doubles_equal(lhs.x * rhs.y, lhs.y * rhs.x) &&
                                              is_doubles_equal(lhs.y * rhs.z, lhs.z * rhs.y);

    return flag;
}
/* ------------------------------------------------
                END VECTOR_METHODS
 -------------------------------------------------*/



/* ------------------------------------------------
                START LINE_METHODS
 -------------------------------------------------*/
line_t::line_t() : start(), direction() {}
line_t::line_t(const point3D_t& lhs, const point3D_t& rhs) : start(lhs.x, lhs.y, lhs.z),
                                                             direction(lhs, rhs) {}

bool line_t::include(const point3D_t &point, double& t) const {
    if(!point.valid()) {
        return false;
    }

    if(!is_doubles_equal(direction.x, 0.0)) {
        t = (point.x - start.x) / direction.x;
    }

    else if(!is_doubles_equal(direction.y, 0.0)) {
        t = (point.y - start.y) / direction.y;
    }

    else if(!is_doubles_equal(direction.x, 0.0)) {
        t = (point.z - start.z) / direction.z;
    }

    else {
        return false;
    }

    bool flag =  is_doubles_equal(point.x, start.x + direction.x * t) &&
                 is_doubles_equal(point.y, start.y + direction.y * t) &&
                 is_doubles_equal(point.z, start.z + direction.z * t);


    return flag;
}

bool line_t::include(const point3D_t &point) const {
    double tmp;
    return include(point, tmp);
}

bool line_t::valid() const {
    return (start.valid() && direction.valid());
}

bool is_parallel(const line_t& lhs, const line_t& rhs) {
    return is_parallel(lhs.direction, rhs.direction);
}

/* ------------------------------------------------
                END LINE_METHODS
 -------------------------------------------------*/



/* ------------------------------------------------
                START SEGMENT_METHODS
 -------------------------------------------------*/
segment_t::segment_t() : start(), end() {}
segment_t::segment_t(const point3D_t& lhs, const point3D_t& rhs) : start(lhs), end(rhs) {}

bool segment_t::include(const point3D_t &point) const {
    line_t segment_line(start, end);
    double t = std::numeric_limits<double>::quiet_NaN();
    bool flag = segment_line.include(point, t);
    flag = flag && t >= 0 && t <= 1;
    return flag;
}
/* ------------------------------------------------
                END SEGMENT_METHODS
 -------------------------------------------------*/




/* ------------------------------------------------
                START TRIANGLE_METHODS
 -------------------------------------------------*/
triangle_t::triangle_t() : A(), B(), C() {}
triangle_t::triangle_t(const point3D_t& A_, const point3D_t& B_, const point3D_t& C_) : A(A_), B(B_), C(C_) {}

bool triangle_t::valid() {
    if(!A.valid() || !B.valid() || !C.valid()) return false;

    double AB = distance(A, B);
    double BC = distance(B, C);
    double AC = distance(C, A);

    if(AC + BC < AB) return false;
    if(AB + BC < AC) return false;
    if(AB + AC < BC) return false;

    return true;
}

std::istream& operator>>(std::istream& in, triangle_t& triangle) {
    in >> triangle.A >> triangle.B >> triangle.C;
    return in;
}

std::ostream& operator<<(std::ostream& out, const triangle_t& triangle) {
    out << '[' << triangle.A << ' ' << triangle.B << ' ' << triangle.C << ']';
    return out;
}
/* ------------------------------------------------
                END TRIANGLE_METHODS
 -------------------------------------------------*/



/* ------------------------------------------------
                START PLANE_METHODS
 -------------------------------------------------*/
plane_t::plane_t(double a_, double b_, double c_, double d_) : a(a_), b(b_), c(c_), d(d_) {}

//todo: chek if 3 point on a 1 line
//compute a, b, c, d using equation:
//  |  x - x1    y - y1    z - z1 |
//  | x2 - x1   y2 - y1   z2 - z1 |  ==  0
//  | x3 - x1   y3 - y1   z3 - z1 |
plane_t::plane_t(const point3D_t& x1, const point3D_t& x2, const point3D_t& x3) {
    a = (x2.y - x1.y) * (x3.z - x1.z) - (x2.z - x1.z) * (x3.y - x1.y);
    b = -((x2.x - x1.x) * (x3.z - x1.z) - (x2.z - x1.z) * (x3.x - x1.x));
    c = ((x2.x - x1.x) * (x3.y - x1.y) - (x2.y - x1.y) * (x3.x - x1.x));
    d = -(x1.x * a + x1.y * b + x1.z * c);
}

vector3D_t plane_t::normal() const{
    return vector3D_t(a, b, c);
}

bool operator==(const plane_t& lhs, const plane_t& rhs) {
    return is_parallel(lhs.normal(), rhs.normal()) && (is_doubles_equal(lhs.a * rhs.d, lhs.d * rhs.a));
}

bool is_parallel(const plane_t& lhs, const plane_t& rhs) {
    vector3D_t n_lhs = lhs.normal();
    vector3D_t n_rhs = rhs.normal();

    return is_parallel(n_lhs, n_rhs);
}
/* ------------------------------------------------
                END PLANE_METHODS
 -------------------------------------------------*/




/* ------------------------------------------------
                START INTERSECTION_METHODS
 ------------------------------------------------*/
//this function use equation:
//          ->       ->         ->  ->
// L = (a * n1 + b * n2) + t * [n1, n2] , where
//            ->   ->
//      s2 * (n1 * n2) - s1 * n2^2
// a = ----------------------------
//       ->   ->
//      (n1 * n2) ^ 2 - n1^2 * n2^2
//
//            ->   ->
//      s1 * (n1 * n2) - s2 * n2^2
// b = ----------------------------
//       ->   ->
//      (n1 * n2) ^ 2 - n1^2 * n2^2
line_t intersection_of_2planes(const plane_t& lhs, const plane_t& rhs) {
    vector3D_t lhs_normal = lhs.normal();
    vector3D_t rhs_normal = rhs.normal();
    vector3D_t crs = cross(lhs_normal, rhs_normal);
    if(is_doubles_equal(crs.len(), 0.0)) {
        return line_t();
    }

    line_t ret;
    ret.direction = crs;
    double s1, s2, a, b;
    s1 = lhs.d;
    s2 = rhs.d;

    double n1n2dot = dot(lhs_normal, rhs_normal);
    double n1sqr = dot(lhs_normal, lhs_normal);
    double n2sqr = dot(rhs_normal, rhs_normal);

    double denominator = -std::pow(n1n2dot, 2) + n1sqr * n2sqr;
    a = (s2 * n1n2dot - s1 * n2sqr) / denominator;
    b = (s1 * n1n2dot - s2 * n1sqr) / denominator;

    ret.start = a * lhs_normal + b * rhs_normal;
    return ret;
}

point3D_t intersection_of_2lines(const line_t& lhs, const line_t& rhs) {
    double t = std::numeric_limits<double>::quiet_NaN();
    double x, y, z;
    point3D_t ret;
    if(!is_doubles_equal(rhs.direction.x, lhs.direction.y)) {
        t = (lhs.start.x - rhs.start.x) / (rhs.direction.x - lhs.direction.x);
    }

    else if(!is_doubles_equal(rhs.direction.y, lhs.direction.y)) {
        t = (lhs.start.y - rhs.start.y) / (rhs.direction.y - lhs.direction.y);
    }

    else if(!is_doubles_equal(rhs.direction.z, lhs.direction.z)) {
        t = (lhs.start.z - rhs.start.z) / (rhs.direction.z - lhs.direction.z);
    }

    // parallel
    else {
        return ret;
    }

    ret.x = lhs.start.x + t * lhs.direction.x;
    ret.y = lhs.start.y + t * lhs.direction.y;
    ret.z = lhs.start.z + t * lhs.direction.z;

    bool flag = (ret.x == rhs.start.x + t * rhs.direction.x) &&
                (ret.y == rhs.start.y + t * rhs.direction.y) &&
                (ret.z == rhs.start.z + t * rhs.direction.z);

    return (flag == true) ? ret : point3D_t();
};

point3D_t intersection_of_2segments(const segment_t& lhs, const segment_t& rhs) {
    line_t lhs_line_segment(lhs.start, lhs.end);
    point3D_t tmp = intersection_line_segment(lhs_line_segment, rhs);
    return rhs.include(tmp) ? tmp : point3D_t();
}

point3D_t intersection_line_segment(const line_t& line, const segment_t& segment) {
    line_t segment_line(segment.start, segment.end);
    if(is_parallel(line, segment_line)) {
        return point3D_t();
    }

    else {
        point3D_t destination;
        point3D_t intersection = intersection_of_2lines(line, segment_line);
        return segment.include(intersection) ? intersection : point3D_t();
    }
}

point3D_t intersection_line_triangle_2D(const line_t& line, const triangle_t& triangle) {
    point3D_t tmp;
    tmp = intersection_line_segment(line, segment_t(triangle.A, triangle.B));
    if(tmp.valid()) {
        return tmp;
    }

    tmp = intersection_line_segment(line, segment_t(triangle.C, triangle.B));
    return tmp;
}


point3D_t intersection_of_2triangles_2D(const triangle_t& lhs, const triangle_t& rhs) {
    line_t lhs_AB(lhs.A, rhs.B);
    line_t lhs_BC(lhs.B, lhs.C);
    point3D_t tmp = intersection_line_triangle_2D(lhs_AB, rhs);
    if(tmp.valid()) {
        return tmp;
    }
    return intersection_line_triangle_2D(lhs_BC, rhs);
}

bool intersection_of_2triangles_3D(const triangle_t& lhs, const triangle_t& rhs) {
    plane_t lhs_plane(lhs.A, lhs.B, lhs.C);
    plane_t rhs_plane(rhs.A, rhs.B, rhs.C);
    line_t plane_intersection = intersection_of_2planes(lhs_plane, rhs_plane);

    if(!plane_intersection.valid()) {
        return (lhs_plane == rhs_plane) ? false : intersection_of_2triangles_2D(lhs, rhs).valid();
    }

    point3D_t lhs_line_intersection = intersection_line_triangle_2D(plane_intersection, lhs);
    point3D_t rhs_line_intersection = intersection_line_triangle_2D(plane_intersection, rhs);

    return (lhs_line_intersection.valid() && rhs_line_intersection.valid()) ? true : false;
}
/* ------------------------------------------------
                END INTERSECTION_METHODS
 ------------------------------------------------*/
