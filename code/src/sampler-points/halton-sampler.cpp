// Copyright (c) 2012 Leonhard Gruenschloss (leonhard@gruenschloss.org)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "halton/halton_enum.h"
#include "halton/halton_sampler.h"
#include "sampler-points.h"

#include <fstream>
#include <cassert>
#include <cstdlib>

using namespace std;

// Random number generator wrapper for drand48.
struct Drand48
{
    template <typename Integer>
    Integer operator()(const Integer n) const
    {
        return static_cast<Integer>(drand48() * n);
    }
};

template<int Dimension, typename T>
std::vector<T> PointSampler<Dimension, T>::halton_samples(int N, T* bBox, int ucomponent, int vcomponent, bool faure){

    std::vector<T> result;
    // Initialize the sampler, either with Faure permutations or randomized digit permutations.
    Halton_sampler halton_sampler;
    if (faure)
    {
        halton_sampler.init_faure();
    }
    else
    {
        Drand48 rng;
        halton_sampler.init_random(rng);
    }

    for(int i = 0; i < N; i++){
        const float sx = halton_sampler.sample(ucomponent, i);
        const float sy = halton_sampler.sample(vcomponent, i);
        result.push_back(sx);
        result.push_back(sy);
    }

    return result;
}

template std::vector<double> PointSampler<2, double>::halton_samples(int N, double *bBox, int ucomponent, int vcomponent, bool faure);

//int main(int, char**)
//{
//    test("faure-halton.txt", 1024, false);
//    //test("faure.dat", 640, 480, true);
//    //srand48(5784); // Make reproducible.
//    //test("random.dat", 2048, 1152, false);
//    return 0;
//}

