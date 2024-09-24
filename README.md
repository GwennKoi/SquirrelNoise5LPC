# LPC Port of SquirrelNoise5 
## Squirrel's Raw Noise utilities (version 5)

This code is made available under the Creative Commons attribution 3.0 license (CC-BY-3.0 US): Attribution in source code comments (even closed-source/commercial code) is sufficient. License summary and text available at: https://creativecommons.org/licenses/by/3.0/us/

These noise functions were written by Squirrel Eiserloh as a cheap and simple substitute for the [sometimes awful] bit-noise sample code functions commonly found on the web, many of which are hugely biased or terribly patterned, e.g. having bits which are on (or off) 75% or even 100% of the time (or are excessively overkill/slow for our needs, such as MD5 or SHA).

Note: This is work in progress; not all functions have been tested. Use at your own risk.

The following functions are all based on a simple bit-noise hash function which returns an integer containing 32 reasonably-well-scrambled bits, based on a given (signed) integer input parameter (position/index) and [optional] seed.  Kind of like looking up a value in an infinitely large [non-existent] table of previously rolled random numbers.

These functions are deterministic and random-access / order-independent (i.e. state-free), so they are particularly well-suited for use in smoothed/fractal/simplex/Perlin noise functions and out-of-order (or on-demand) procedural content generation (i.e. that mountain village is the same whether you generated it first or last, ahead of time or just now).

The N-dimensional variations simply hash their multidimensional coordinates down to a single 32-bit index and then proceed as usual, so
while results are not unique they should (hopefully) not seem locally predictable or repetitive.
