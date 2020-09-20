#include "geometry.h"

/* ------------------------------------------------
                START POINT_METHODS
 -------------------------------------------------*/

point3D_t::point3D_t() : x(std::numeric_limits<double>::quiet_NaN()) ,
                         y(std::numeric_limits<double>::quiet_NaN()) ,
                         z(std::numeric_limits<double>::quiet_NaN()) {}

point3D_t::point3D_t(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

bool point3D_t::valid() const {
    if (std::isnan(x) || !std::isfinite(x)) return false;
    if (std::isnan(y) || !std::isfinite(y)) return false;
    if (std::isnan(z) || !std::isfinite(z)) return false;

    return true;
}

bool operator==(const point3D_t& lhs, const point3D_t& rhs) {
    if(std::abs(lhs.x - rhs.x) >= TOLERANCE) return false;
    if(std::abs(lhs.y - rhs.y) >= TOLERANCE) return false;
    if(std::abs(lhs.z - rhs.z) >= TOLERANCE) return false;

    return true;
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
    return sqrt(pow((lhs.x - rhs.x), 2) +
                pow((lhs.y - rhs.y), 2) +
                pow((lhs.z - rhs.z), 2));
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

vector3D_t::vector3D_t(const point3D_t& lhs, const point3D_t& rhs) {
    x = rhs.x - lhs.x;
    y = rhs.y - lhs.y;
    z = rhs.z - lhs.z;
}

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
    vector3D_t tmp = *this;
    return tmp += (-rhs);
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
    return distance(point3D_t(0.0, 0.0, 0.0), point3D_t(x, y, z));
}

bool vector3D_t::is_zero() const{
    if(std::abs(x) > TOLERANCE) return false;
    if(std::abs(y) > TOLERANCE) return false;
    if(std::abs(z) > TOLERANCE) return false;

    return true;
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
    if((std::abs(lhs.x * rhs.y - lhs.y * rhs.x) < TOLERANCE) &&
       (std::abs(lhs.y * rhs.z - lhs.z * rhs.y) < TOLERANCE)) {
        return true;
    }

    return false;
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

bool line_t::include(const point3D_t &point) const {
    if(!point.valid()) {
        return false;
    }

    double t = 1.0;
    if(std::abs(direction.x) > TOLERANCE) {
        t = (point.x - start.x) / direction.x;
    }

    else if(std::abs(direction.y) > TOLERANCE) {
        t = (point.y - start.y) / direction.y;
    }

    else if(std::abs(direction.z) > TOLERANCE) {
        t = (point.z - start.z) / direction.z;
    }

    else {
        return false;
    }

    bool flag = 1 && (std::abs(point.x - start.x + direction.x * t) < TOLERANCE) &&
                     (std::abs(point.y - start.y + direction.y * t) < TOLERANCE) &&
                     (std::abs(point.z - start.z + direction.z * t) < TOLERANCE);

    return flag;
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
    if(std::abs(crs.len()) < TOLERANCE) {
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

double intersection_of_2lines(const line_t& lhs, const line_t& rhs, point3D_t& destination) {
    double t = std::numeric_limits<double>::quiet_NaN();
    double x, y, z;
    if(std::abs(rhs.direction.x - lhs.direction.x) > TOLERANCE) {
        t = (lhs.start.x - rhs.start.x) / (rhs.direction.x - lhs.direction.x);
    }

    else if(std::abs(rhs.direction.y - lhs.direction.y) > TOLERANCE) {
        t = (lhs.start.y - rhs.start.y) / (rhs.direction.y - lhs.direction.y);
    }

    else if(std::abs(rhs.direction.z - lhs.direction.z) > TOLERANCE) {
        t = (lhs.start.z - rhs.start.z) / (rhs.direction.z - lhs.direction.z);
    }

    else {
        return t;
    }

    destination.x = lhs.start.x + t * lhs.direction.x;
    destination.y = lhs.start.y + t * lhs.direction.y;
    destination.z = lhs.start.z + t * lhs.direction.z;

    return t;
};

point3D_t intersection_line_segment(const line_t& line, const segment_t& segment) {
    line_t segment_line(segment.start, segment.end);
    if(is_parallel(line, segment_line)) {
        return (line.include(segment.start)) ? segment.start : point3D_t();
    }

    else {
        point3D_t destination;
        double t = intersection_of_2lines(line, segment_line, destination);
        return (t < 0.0 || t > 1.0) ? point3D_t() : destination;
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

bool intersection_of_2triangles_3D(const triangle_t& lhs, const triangle_t& rhs) {
    plane_t lhs_plane(lhs.A, lhs.B, lhs.C);
    plane_t rhs_plane(rhs.A, rhs.B, rhs.C);
    line_t plane_intersection = intersection_of_2planes(lhs_plane, rhs_plane);

    /* if(!line_t.valid()) {

    } */

    point3D_t lhs_line_intersection = intersection_line_triangle_2D(plane_intersection, lhs);
    point3D_t rhs_line_intersection = intersection_line_triangle_2D(plane_intersection, rhs);

    return (lhs_line_intersection.valid() && rhs_line_intersection.valid()) ? true : false;
}
/* ------------------------------------------------
                END INTERSECTION_METHODS
 ------------------------------------------------*/
