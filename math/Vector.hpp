#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

#include"Global.hpp"

template<size_t N, typename T>
struct Vector{
    T data[N];
    inline Vector(){
        for (size_t i = 0; i < N; ++i){
            data[i] = T();
        }
    }
    inline Vector(const T& rhs){
        for (size_t i = 0; i < N; ++i){
            data[i] = rhs;
        }
    }
    inline Vector(const T* ptr){
        for (size_t i = 0; i < N; ++i){
            data[i] = ptr[i];
        }
    }
    inline Vector(const Vector<N,T>& v ){
        for (size_t i = 0; i < N; ++i){
            data[i] = v.data[i];
        }
    }
    inline Vector(Vector<N,T>&&v ){
        for (size_t i = 0; i < N; ++i){
            data[i] = v.data[i];
        }
    }
    inline Vector<N,T>& operator=(const Vector<N,T>& v){
        for (size_t i = 0; i < N; ++i){
            data[i] = v.data[i];
        }
    }
    inline Vector<N,T>& operator=(Vector<N,T>&& v){
        for (size_t i = 0; i < N; ++i){
            data[i] = v.data[i];
        }
    }
    inline Vector(const std::initializer_list<T> & v ){
        assert(N >= v.size());
        size_t i = 0;
        for (auto it = v.begin(); it != v.end(); ++it, ++i){
            data[i] = *it;
        }
    }
    inline T& operator[](const size_t& i) const{
        assert(i < N);
        return data[i];
    }
    inline T& operator[](size_t&& i) const{
        assert(i < N);
        return data[i];
    }
};

template<typename T>
struct Vector<2,T>{
    union{
        struct{
            T x, y;
        };
        struct{
            T u, v;
        };
        struct{
            T s, t;
        };
        struct{
            T left, right;
        };
        T data[2];
    };
    inline Vector()  = default;
    inline Vector(const T &rhs) : x(rhs), y(rhs){};
    inline Vector(const T &a, const &b) : x(a), y(b){};
    inline Vector(const Vector<2, T> &v) : x(v.x), y(v.y){};
    inline Vector(const std::initializer_list<T>& v){
        assert(2 >= v.size());
        size_t i = 0;
        for (auto it = v.begin(); it != v.end(); ++it, ++i){
            data[i] = *it;
        }
    }
    inline T& operator[](const size_t& i) const{
        assert(i < 2);
        return data[i];
    }
    inline T& operator[](size_t&& i ) const {
        assert(i < 2);
        return data[i];
    }
};

template<typename T>
struct Vector<3,T>{
    union{
        struct{
            T x, y,z;
        };
        struct{
            T r, g, b;
        };
        T data[3];
    };
    inline Vector()  = default;
    inline Vector(const T &rhs) : x(rhs), y(rhs), z(rhs){};
    inline Vector(const T &a, const T &b, const T &c) : x(a), y(b), z(c){};
    inline Vector(const Vector<3, T> &v) : x(v.x), y(v.y), z(v.z){};
    inline Vector(const std::initializer_list<T>& v){
        assert(3 >= v.size());
        size_t i = 0;
        for (auto it = v.begin(); it != v.end(); ++it, ++i)
        {
            data[i] = *it;
        }
    }
    inline T& operator[](const size_t& i) const{
        assert(i < 3);
        return data[i];
    }
    inline T& operator[](size_t&& i ) const {
        assert(i < 3);
        return data[i];
    }
};

template<typename T>
struct Vector<4,T>{
    union{
        struct{
            T x, y,z, w;
        };
        struct{
            T r, g, b, alpha;
        };
        T data[4];
    };
    inline Vector() = default;
    inline Vector(const T &rhs) : x(rhs), y(rhs), z(rhs), w(rhs){};
    inline Vector(const Vector<4, T> &v) : x(v.x), y(v.y), z(v.z), w(v.w){};
    inline Vector(const std::initializer_list<T>& v){
        assert(4 >= v.size());
        size_t i = 0;
        for (auto it = v.begin(); it != v.end(); ++it, ++i){
            data[i] = *it;
        }
    }
    inline T& operator[](const size_t& i) const{
        assert(i < 4);
        return data[i];
    }
    inline T& operator[](size_t&& i ) const {
        assert(i < 4);
        return data[i];
    }
};

template<size_t N, typename T>
inline Vector<N,T>& operator+(const Vector<N,T>& v){
    return v;
}

//-v
template<size_t N, typename T>
inline Vector<N,T>& operator-(const Vector<N,T>&v){
    Vector<N,T> ret(v);
    for (size_t i = 0; i < N;++i){
        ret.data[i] *= -1;
    }
    return ret;
}

// a+b
template<size_t N, typename T>
inline Vector<N,T> operator+(const Vector<N,T>& a, const Vector<N,T>& b){
    Vector<N, T> ret(a);
    for (size_t i = 0; i < N;++i){
        ret.data[i] += b.data[i];
    }
    return ret;
}
// a+ Tb
template<size_t N, typename T>
inline Vector<N,T> operator+(const Vector<N,T>& a, const T& b){
    Vector<N, T> ret(a);
    for (size_t i = 0; i < N;++i){
        ret.data[i] += b;
    }
    return ret;
}


// a-b
template<size_t N, typename T>
inline Vector<N,T> operator-(const Vector<N,T>& a, const Vector<N,T>& b){
    Vector<N, T> ret(a);
    for (size_t i = 0; i < N;++i){
        ret.data[i] -= b.data[i];
    }
    return ret;
}

// a- Tb
template<size_t N, typename T>
inline Vector<N,T> operator-(const Vector<N,T>& a, const T& b){
    Vector<N, T> ret(a);
    for (size_t i = 0; i < N;++i){
        ret.data[i] -= b;
    }
    return ret;
}

// a * Tb
template<size_t N, typename T>
inline Vector<N,T> operator*(const Vector<N,T>& a, const T& b){
    Vector<N, T> ret(a);
    for (size_t i = 0; i < N;++i){
        ret.data[i] *= b;
    }
    return ret;
}

// a * b
template<size_t N, typename T>
inline Vector<N,T> operator*(const Vector<N,T>& a, const Vector<N,T>& b){
    Vector<N, T> ret(a);
    for (size_t i = 0; i < N;++i){
        ret.data[i] *= b.data[i];
    }
    return ret;
}


// a / Tb
template<size_t N, typename T>
inline Vector<N,T> operator/(const Vector<N,T>&v, const T& divisor){
    Vector<N, T> ret(v);
    for (size_t i = 0; i<N; ++i){
        ret.data[i] /= divisor;
    }
    return ret;
}

// a+=Tb
template<size_t N, typename T>
inline Vector<N,T> operator+=(Vector<N,T>& v, const T& b){
    for (size_t i = 0; i < N; ++i){
        v.data[i] += b;
    }
    return v;
}

// a+=b
template<size_t N, typename T>
inline Vector<N,T> operator+=(Vector<N,T>& v, const Vector<N,T>&  b){
    for (size_t i = 0; i < N; ++i){
        v.data[i] += b.data[i];
    }
    return v;
}
//a-= Tb
template<size_t N, typename T>
inline Vector<N,T> operator-=(Vector<N,T>& v, const T& b){
    for (size_t i = 0; i < N; ++i){
        v.data[i] -= b;
    }
    return v;
}
// a-=b
template<size_t N, typename T>
inline Vector<N,T> operator-=(Vector<N,T>& v, const Vector<N,T>& b){
    for (size_t i = 0; i < N; ++i){
        v.data[i] -= b.data[i];
    }
    return v;
}

// a*=b

template<size_t N, typename T>
inline Vector<N,T> operator*=(Vector<N,T>& v, const Vector<N,T>& b){
    for (size_t i = 0; i < N; ++i){
        v.data[i] *= b.data[i];
    }
    return v;
}

//a*= Tb
template<size_t N, typename T>
inline Vector<N,T> operator*=(Vector<N,T>& v, const T& b){
    for (size_t i = 0; i < N; ++i){
        v.data[i] *= b;
    }
    return v;
}

// a /=Tb
template<size_t N, typename T>
inline Vector<N,T> operator/=(Vector<N,T>& v, const T& b){
    for (size_t i = 0; i < N; ++i){
        v.data[i] /= b;
    }
    return v;
}

//求模长
template<size_t N,typename T>
inline float len(const Vector<N,T>& v){
    float ans = 0;
    for (size_t i = 0; i < N; ++i){
        ans += v.data[i] * v.data[i];
    }
    return std::sqrt(ans);
}
//单位化
template<size_t N, typename T>
inline Vector<N,T> normalize(const Vector<N,T>& v){
    return v / len(v);
}
//点乘
template<size_t N, typename T>
inline float dot(const Vector<N,T>&a, const Vector<N,T>&b){
    float ans = 0;
    for (size_t i = 0; i < N; ++i){
        ans += a.data[i] * b.data[i];
    }
    return ans;
} 

//叉乘
template<typename T>
inline Vector<3,T> cross(const Vector<3,T>& a, const Vector<3,T>& b){
    return Vector<3, T>{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

template<typename T>
inline Vector<4,T> toVec4(const Vector<3,T>& a, const T& b = 1){
    return {a.x, a.y, a.z, b};
}

using Vector3f = Vector<3, float>;
using Vector2f = Vector<2, float>;
using Vector4f = Vector<4, float>;

#endif