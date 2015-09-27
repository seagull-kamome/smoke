// -*- mode: c++; indent-tabs-mode: nil; c-basic-offset:4; flycheck-clang-language-standard: "c++14" -*-


#pragma once

#include <utility>

namespace smoke { namespace Data { namespace Proxy {


template <typename T> struct Proxy;
template <typename T> inline Proxy<T> const& proxy() { return *(Proxy<T> const*)nullptr; }


template <typename T, typename TagType>
struct Tagged {
    T value_;

    inline Tagged(T const& x) : value_(x) {}
    inline Tagged(T&& x) : value_(std::move(x)) {}
    inline Tagged(Tagged<T, TagType> const& x) : value_(x.value_) {}
    inline Tagged(Tagged<T, TagType>&& x) : value_(std::move(x.value_)) {}

    template <typename A>
    inline Tagged<A, TagType> rewrite(Tagged<T, TagType> const& x) const {
        return Tagged<A, TagType>(x.value_);
    }

    template <typename A>
    inline Tagged<A, TagType> rewrite(Tagged<T, TagType>&& x) const {
        return Tagged<A, TagType>(std::move(x.value_));
    }


    template <typename A>
    inline Tagged<T, A> retag(Tagged<T, TagType> const& x) const {
        return Tagged<T, A>(x.value_);
    }

    template <typename A>
    inline Tagged<T, A> retag(Tagged<T, TagType>&& x) const {
        return Tagged<T, A>(std::move(x.value_));
    }
};

}}}
