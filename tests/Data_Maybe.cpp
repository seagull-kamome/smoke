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
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//

#include <string>
#include <functional>

#include "smoke/Data/Maybe.h"
#include "smoke/Data/Function.h"

#include <CppUTest/CommandLineTestRunner.h>

using namespace smoke::Data::Maybe;
using namespace smoke::Data::Function;

TEST_GROUP(Data_Maybe) { };
TEST(Data_Maybe, Maybe) {
    CHECK(nothing<int>().withDefault(1) == 1);
    CHECK(just<int>(2).withDefault(1) == 2);

    CHECK(nothing<std::string>().withDefault("abc") == "abc");
    CHECK(just<std::string>("abc").withDefault("def") == "abc");

    CHECK(nothing<std::string>().maybe(std::string("abc"),
      [](std::string const& x) { return x + "def"; } ) == "abc");

    CHECK(nothing<int>().maybe(1, [](int const& x) { return x + 1; }) == 1);

    CHECK(just<std::string>("abc").fmap<std::string>([](std::string const& x) { return x + "def"; }) == just<std::string>("abcdef"));
}