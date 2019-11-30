#pragma once

#include <array>
#include <vector>
#include <set>
#include <algorithm>
#include <optional>
#include <numeric>
#include <cmath>
#include <math.h>

template<class T>
constexpr T epsilon = std::numeric_limits<T>::epsilon();

struct fill_t {};
static fill_t fillv;

template<class T, size_t D>
class vector;

template<class T, size_t D>
class point;

template<class T, size_t D>
class line_like;

template<class T, size_t D>
class line;

template<class T, size_t D>
class segment;

template<class T, size_t D>
class ray;

template<class T>
class polygon;

template<class T, size_t D>
class vector
{
    friend class point<T, D>;
private:
    std::array<T, D> _coords;
public:
    vector() = default;
    vector(fill_t, const T& v) { std::fill(_coords.begin(), _coords.end(), v); }
    vector(const std::array<T, D>& coords) : _coords(coords) {}
    vector(const vector& other) = default;
    vector(vector&& other) = default;
    explicit vector(const point<T, D>& other);
    explicit vector(point<T, D>&& other);
public:
    vector& operator=(const vector& other) = default;
    vector& operator=(vector && other) = default;
public:
    vector& operator+=(const vector& other)
    {
        for (size_t i = 0; i < D; i++)
        {
            _coords[i] += other._coords[i];
        }
        
        return *this;
    }
    vector& operator-=(const vector& other)
    {
        for (size_t i = 0; i < D; i++)
        {
            _coords[i] -= other._coords[i];
        }

        return *this;
    }
    vector& operator*=(const T& v)
    {
        for (size_t i = 0; i < D; i++)
        {
            _coords[i] *= v;
        }

        return *this;
    }
    vector& operator/=(const T& v)
    {
        for (size_t i = 0; i < D; i++)
        {
            _coords[i] /= v;
        }

        return *this;
    }

    vector operator+(const vector& other) const
    {
        vector result{ *this };
        result += other;
        return result;
    }
    vector operator-(const vector& other) const
    {
        vector result{ *this };
        result -= other;
        return result;
    }
    vector operator*(const T& v) const
    {
        vector result{ *this };
        result *= v;
        return result;
    }
    vector operator/(const T& v) const
    {
        vector result{ *this };
        result /= v;
        return result;
    }

    vector operator-() const
    {
        vector result{ *this };
        for (size_t i = 0; i < D; i++)
        {
            result._coords[i] = -result._coords[i];
        }
        return result;
    }
public:
    T dot_product(const vector& other) const
    {
        return std::inner_product(begin(), end(), other.begin(), T(0));
    }
    T dot(const vector& other) const { return dot_product(other); }
    T length() const
    {
        return std::sqrt(dot_product(*this));
    }
    T magnitude() const
    {
        return length();
    }
    vector& normalize()
    {
        *this /= length();
        return *this;
    }
    vector normalized()
    {
        return *this / length();
    }
public:
    auto begin() { return _coords.begin(); }
    auto begin() const { return _coords.begin(); }

    auto end() { return _coords.end(); }
    auto end() const { return _coords.end(); }

    auto data() { return _coords.data(); }
    auto data() const { return _coords.data(); }

    auto size() const { return _coords.size(); }

    auto operator[](size_t i) { return _coords[i]; }
    auto operator[](size_t i) const { return _coords[i]; }
public:
    bool operator==(const vector& other)
    {
        return std::equal(begin(), end(), other.begin(), [](const auto& c1, const auto& c2) {return std::abs(c1 - c2) < epsilon<T>; });
    }
    bool operator!=(const vector& other)
    {
        return !std::equal(begin(), end(), other.begin(), [](const auto& c1, const auto& c2) {return std::abs(c1 - c2) < epsilon<T>; });
    }
};

template<class T, size_t D>
T dot(const vector<T, D>& v1, const vector<T, D>& v2)
{
    return v1.dot(v2);
}

template<class T, size_t D>
T dot_product(const vector<T, D>& v1, const vector<T, D>& v2)
{
    return v1.dot(v2);
}

template<class T, size_t D>
vector<T, D> operator*(const T& v, const vector<T,D>& other)
{
    vector<T, D> result{ other };
    result *= v;
    return result;
}

template<class T, size_t D>
class point
{
    friend class vector<T, D>;
private:
    std::array<T, D> _coords;
public:
    point() = default;
    point(fill_t, const T& v) { std::fill(_coords.begin(), _coords.end(), v); }
    point(const std::array<T, D>& coords) : _coords(coords) {}
    point(const point& other) = default;
    point(point&& other) = default;
    explicit point(const vector<T, D>& other) : _coords(other._coords) {}
    explicit point(vector<T, D>&& other) : _coords(std::move(other._coords)) {}
public:
    point& operator=(const point& other) = default;
    point& operator=(point&& other) = default;
public:
    point& operator+=(const vector<T,D>& other)
    {
        for (size_t i = 0; i < D; i++)
        {
            _coords[i] += other._coords[i];
        }

        return *this;
    }
    point& operator-=(const vector<T, D>& other)
    {
        for (size_t i = 0; i < D; i++)
        {
            _coords[i] -= other._coords[i];
        }

        return *this;
    }

    point operator+(const vector<T, D>& other) const
    {
        point result{ *this };
        result += other;
        return result;
    }
    point operator-(const vector<T, D>& other) const
    {
        point result{ *this };
        result -= other;
        return result;
    }
    
    vector<T, D> operator-(const point& other) const
    {
        vector<T, D> result{ *this };
        result -= vector<T, D>(other);
        return result;
    }
public:
    auto begin() { return _coords.begin(); }
    auto begin() const { return _coords.begin(); }

    auto end() { return _coords.end(); }
    auto end() const { return _coords.end(); }

    auto data() { return _coords.data(); }
    auto data() const { return _coords.data(); }

    auto size() const { return _coords.size(); }

    auto operator[](size_t i) { return _coords[i]; }
    auto operator[](size_t i) const { return _coords[i]; }
public:
    bool operator==(const point& other)
    {
        return std::equal(begin(), end(), other.begin(), [](const auto& c1, const auto& c2) {return std::abs(c1 - c2) < epsilon<T>; });
    }
    bool operator!=(const point& other)
    {
        return !std::equal(begin(), end(), other.begin(), [](const auto& c1, const auto& c2) {return std::abs(c1 - c2) < epsilon<T>; });
    }
};

template<class T, size_t D>
vector<T, D>::vector(const point<T, D>& other) :
    _coords(other._coords)
{
}

template<class T, size_t D>
vector<T, D>::vector(point<T, D>&& other) :
    _coords(std::move(other._coords))
{
}

template<class T, size_t D>
point<T, D> operator+(const vector<T, D>& v, const point<T, D>& p)
{
    return p + v;
}

template<class T, size_t D>
class line_like
{
protected:
    std::array<point<T, D>, 2> _ps;
public:
    line_like() = default;
    line_like(const point<T, D> & p1, const point<T, D> & p2) : _ps({ p1,p2 }) {}
    line_like(const line_like& other) = default;
    line_like(line_like&& other) = default;
public:
    line_like& operator=(const line_like& other) = default;
    line_like& operator=(line_like&& other) = default;
public:
    auto begin() { return _ps.begin(); }
    auto begin() const { return _ps.begin(); }

    auto end() { return _ps.end(); }
    auto end() const { return _ps.end(); }

    auto data() { return _ps.data(); }
    auto data() const { return _ps.data(); }

    auto size() const { return _ps.size(); }

    auto operator[](size_t i) { return _ps[i]; }
    auto operator[](size_t i) const { return _ps[i]; }

    auto p1() { return _ps[0]; }
    auto p1() const { return _ps[0]; }
    auto p2() { return _ps[1]; }
    auto p2() const { return _ps[1]; }
public:
    vector<T, D> creating_vector() const
    {
        return _ps[1] - _ps[0];
    }
    vector<T, D> unit_creating_vector() const
    {
        return creating_vector().normalized();
    }
public:
    virtual bool algebraically_inside(const T& coefficent) const = 0;
public:
    virtual bool operator==(const line_like& other) { return std::equal(_ps.begin(), _ps.end(), other._ps.begin()); }
    virtual bool operator!=(const line_like& other) { return !std::equal(_ps.begin(), _ps.end(), other._ps.begin()); }
};

template<class T, size_t D>
class line : public line_like<T, D>
{
public:
    line() = default;
    line(const point<T, D>& p1, const point<T, D>& p2) : line_like<T, D>(p1,p2) {}
    line(const line& other) = default;
    line(line&& other) = default;
    explicit line(const line_like<T, D>& other) : line_like<T, D>(other) {}
    explicit line(line_like<T, D>&& other) : line_like<T, D>(other) {}
public:
    line& operator=(const line& other) = default;
    line& operator=(line&& other) = default;
public:
    line& operator+=(const vector<T, D> & other)
    {
        _ps[0] += other;
        _ps[1] += other;

        return *this;
    }
    line& operator-=(const vector<T, D> & other)
    {
        _ps[0] -= other;
        _ps[1] -= other;

        return *this;
    }

    line operator+(const vector<T, D> & other) const
    {
        line result{ *this };
        result += other;
        return result;
    }
    line operator-(const vector<T, D> & other) const
    {
        line result{ *this };
        result -= other;
        return result;
    }
public:
    virtual bool algebraically_inside(const T& coefficent) const override
    {
        return true;
    }
};

template<class T, size_t D>
line<T, D> operator+(const vector<T, D>& v, const line<T, D>& p)
{
    return p + v;
}

template<class T, size_t D>
class segment : public line_like<T, D>
{
public:
    segment() = default;
    segment(const point<T, D>& p1, const point<T, D>& p2) : line_like<T, D>(p1, p2) {}
    segment(const segment& other) = default;
    segment(segment&& other) = default;
    explicit segment(const line_like<T, D>& other) : line_like<T, D>(other) {}
    explicit segment(line_like<T, D>&& other) : line_like<T, D>(other) {}
public:
    segment& operator=(const segment& other) = default;
    segment& operator=(segment&& other) = default;
public:
    segment& operator+=(const vector<T, D>& other)
    {
        _ps[0] += other;
        _ps[1] += other;

        return *this;
    }
    segment& operator-=(const vector<T, D>& other)
    {
        _ps[0] -= other;
        _ps[1] -= other;

        return *this;
    }

    segment operator+(const vector<T, D>& other) const
    {
        segment result{ *this };
        result += other;
        return result;
    }
    segment operator-(const vector<T, D>& other) const
    {
        segment result{ *this };
        result -= other;
        return result;
    }
public:
    virtual bool algebraically_inside(const T& coefficent) const override
    {
        return coefficent >= 0 && coefficent <= 1;
    }
};

template<class T, size_t D>
segment<T, D> operator+(const vector<T, D>& v, const segment<T, D>& p)
{
    return p + v;
}

template<class T, size_t D>
class ray : public line_like<T, D>
{
public:
    ray() = default;
    ray(const point<T, D>& p1, const point<T, D>& p2) : line_like<T, D>(p1, p2) {}
    ray(const ray& other) = default;
    ray(ray&& other) = default;
    explicit ray(const line_like<T, D>& other) : line_like<T, D>(other) {}
    explicit ray(line_like<T, D>&& other) : line_like<T, D>(other) {}
public:
    ray& operator=(const ray& other) = default;
    ray& operator=(ray&& other) = default;
public:
    ray& operator+=(const vector<T, D>& other)
    {
        _ps[0] += other;
        _ps[1] += other;

        return *this;
    }
    ray& operator-=(const vector<T, D>& other)
    {
        _ps[0] -= other;
        _ps[1] -= other;

        return *this;
    }

    ray operator+(const vector<T, D>& other) const
    {
        ray result{ *this };
        result += other;
        return result;
    }
    ray operator-(const vector<T, D>& other) const
    {
        ray result{ *this };
        result -= other;
        return result;
    }
public:
	void rotate_clockwisely(const float& angle)
	{
		auto x = _ps[1][0];
		auto y = _ps[1][1];
		_ps[1] =point<T, 2>({ x*cos(angle) - y * sin(-angle), x*sin(angle) + y * cos(-angle) });
	}
    virtual bool algebraically_inside(const T& coefficent) const override
    {
        return coefficent >= 0;
    }
};

template<class T, size_t D>
ray<T, D> operator+(const vector<T, D>& v, const ray<T, D>& p)
{
    return p + v;
}

template<class T>
class polygon : public std::vector<point<T, 2>>
{
public:
    polygon() = default;
    polygon(const std::vector<point<T, 2>>& vertices) : std::vector<point<T, 2>>(vertices) {}
    polygon(const polygon& other) = default;
    polygon(polygon&& other) = default;
public:
    polygon& operator=(const polygon& other) = default;
    polygon& operator=(polygon&& other) = default;
};