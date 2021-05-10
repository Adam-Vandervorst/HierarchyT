#ifndef VECTORIZE_H
#define VECTORIZE_H

#include <limits>
#include <cmath>
#include <functional>
#include <numeric>
#include <iostream>


template<int N, class T=double>
class Vector : public std::array<T, N> {
public:
    static Vector constant(T c) {
        Vector<N, T> v;
        v.fill(c);
        return v;
    }

    static Vector one_hot(unsigned int k) {
        auto v = Vector<N, T>::constant(0);
        v[k] = (T)1;
        return v;
    }

    constexpr auto map(std::function<T (T)> f) const {
        Vector<N, T> v;
        std::transform(this->begin(), this->end(), v.begin(), f);
        return v;
    }

    constexpr auto partial(std::function<T (T, T)> op) const {
        return [*this, op](Vector<N, T> other) constexpr {
            Vector<N, T> v;
            std::transform(this->begin(), this->end(), other.begin(), v.begin(), op);
            return v;
        };
    }

    template<class V = T>
    constexpr auto reduce(std::function<V (V, T)> f = std::plus{}, V initial = V()) const {
        return std::accumulate(this->begin(), this->end(), initial, f);
    }

    T norm() {
        return std::sqrt(*this | *this);
    }

    Vector normalized() {
        return *this/norm();
    }

    auto bounds() {
        auto limits = std::make_pair(std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity());
        using LP = decltype(limits);
        return reduce<LP>([](LP bs, T v) -> LP {return {std::min(bs.first, v), std::max(bs.second, v)};}, limits);
    }

    auto clip(std::pair<T, T> bounds) {
        return map([bounds](T v){return std::max(bounds.first, std::min(bounds.second, v));});
    }
};

template<int N_, class T_>
std::ostream& operator << (std::ostream& os, const Vector<N_, T_> v) {
    for (unsigned int i = 0; i < N_; ++i) {
        os << v[i];
        if (i != N_ - 1) os << " ";
    }
    return os;
};

#define ops(O) \
template<int N, class A, class B> constexpr auto operator O (Vector<N, A> v, Vector<N, B> w) \
{return v.partial([](A x, B y){return x O y;})(w);} \
template<int N, class T> constexpr auto operator O (Vector<N, T> v, T s) \
{return v.map([s](T x){return x O s;});} \
template<int N, class T> constexpr auto operator O (T s, Vector<N, T> v) \
{return v.map([s](T x){return s O x;});}

ops(+)
ops(-)
ops(*)
ops(/)
ops(<)
ops(>)

template<int N, class T>
constexpr auto operator | (Vector<N, T> v, Vector<N, T> w) {
    return (v*w).reduce();
}

template<int N, class T>
constexpr auto operator ^ (Vector<N, T> v, Vector<N, T> w) {
    return std::acos((v | w)/(v.norm()*w.norm()));
}

#endif
