//
//  Author:
//    seagull seagull.kamome@gmail.com
//
//  Copyright (c) 2015, seagull
//
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in
//       the documentation and/or other materials provided with the distribution.
//     * Neither the name of the [ORGANIZATION] nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#pragma once
#include <functional>
#include <type_traits>
#include <new>
#include <utility>
#include <ostream>

#include "smoke/Data/Functor.h"


namespace smoke {
    namespace Data{
        namespace Maybe {
            
            template <typename T>
            class Maybe
            {
            public:
                using element_type = T;
            
            protected:
                bool isJust_ = false;
                std::aligned_storage<sizeof(T), std::alignment_of<T>::value> storage;
             
            public:
                inline Maybe() {}
                inline Maybe(T const& x) { new(&storage) T(x); isJust_ = true; }
                inline Maybe(T&& x) { new(&storage) T(std::move(x)); isJust_ = true; }
                inline Maybe(Maybe<T> const& x) {
                    isJust_ = false;
                    if (x.isJust()) {
                        new(&storage) T(*(T*)&x.storage);
                        isJust_ = true;
                    }
                }
                inline Maybe(Maybe<T>&& x) {
                    isJust_ = false;
                    if (x.isJust()) {
                        new(&storage) T(std::move(*(T*)&x.storage));
                        x.isJust_ = false;
                        isJust_ = true;
                    }
                }

                inline bool isNothing() const { return !isJust_; }
                inline bool isJust() const { return isJust_; }

                inline T const& unsafeFromJust() const { return *(T*)&storage; }

                T withDefault(T const& x) { return isNothing()? x : unsafeFromJust(); }

                template <typename B, typename F>
                B maybe(B const& x, F f) const {
                    return isNothing()? x : f(unsafeFromJust());
                }


                // instance Functor
                template <typename B>
                Maybe<B> fmap(std::function<B (T const&)> const& f) {
                    return isNothing()? Maybe<B>() : Maybe<B>(f(unsafeFromJust()));
                }


                template <typename B>
                Maybe<B> bind(std::function<Maybe<B>(T)> const& x) {
                    
                }
            };

            template <typename T> Maybe<T> nothing() { return Maybe<T>(); }
            template <typename T> Maybe<T> just(T const& x) { return Maybe<T>(x); }
            template <typename T> Maybe<T> just(T&& x) { return Maybe<T>(std::move(x)); }

            // Monad
            template <typename A>
            Maybe<A> join(Maybe<Maybe<A>> const& x) {
                return x.withDefault(Maybe<A>::nothing());
            }


        }
    }
}

template <typename A>
std::ostream& operator<<(std::ostream&ost, smoke::Data::Maybe::Maybe<A> const& x)
{
    if (x.isJust()) ost << "Just " << x.unsafeFromJust() << std::endl;
    else ost << "Nothing";

    return ost;
}
      
template <typename A>
bool operator==(smoke::Data::Maybe::Maybe<A> const& x, smoke::Data::Maybe::Maybe<A> const& y)
{
    return x.maybe(y.isNothing(),
            [&](A const& xx) { return y.maybe(false, [&](A const& yy) { return xx == yy; }); } );
}
