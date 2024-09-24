// noise.h
// Procedural noise generator
// Kryssa@SWmud, September 2024
// Ported from "SquirrelNoise5" by Squirrel Eiserloh

#ifndef _NOISE_H
#define _NOISE_H

////////////////////////////////////////////////////////////////////////////
// SquirrelNoise5 - Squirrel's Raw Noise utilities (version 5)
//
// This code is made available under the Creative Commons attribution 3.0
//  license (CC-BY-3.0 US): Attribution in source code comments (even
//  closed-source/commercial code) is sufficient. License summary and text
//  available at: https://creativecommons.org/licenses/by/3.0/us/
//
// These noise functions were written by Squirrel Eiserloh as a cheap and
//  simple substitute for the [sometimes awful] bit-noise sample code
//  functions commonly found on the web, many of which are hugely biased or
//  terribly patterned, e.g. having bits which are on (or off) 75% or even
//	100% of the time (or are excessively overkill/slow for our needs, such
//  as MD5 or SHA).
//
// Note: This is work in progress; not all functions have been tested. Use
//  at your own risk.
//
// The following functions are all based on a simple bit-noise hash
//  function which returns an integer containing 32 reasonably-well-scrambled
//  bits, based on a given (signed) integer input parameter (position/index)
//  and [optional] seed.  Kind of like looking up a value in an infinitely
//  large [non-existent] table of previously rolled random numbers.
//
// These functions are deterministic and random-access / order-independent
//  (i.e. state-free), so they are particularly well-suited for use in
//  smoothed/fractal/simplex/Perlin noise functions and out-of-order
//  (or on-demand) procedural content generation (i.e. that mountain village
//  is the same whether you generated it first or last, ahead of time or just
//  now).
//
// The N-dimensional variations simply hash their multidimensional
//  coordinates down to a single 32-bit index and then proceed as usual, so
//  while results are not unique they should (hopefully) not seem locally
//  predictable or repetitive.
//
////////////////////////////////////////////////////////////////////////////

#define INT_32_UNSIGNED_MAX     0xFFFFFFFF
#define INT_32_SIGNED_MAX       0x7FFFFFFF

//--------------------------------------------------------------------------
// Raw pseudorandom noise functions (random-access / deterministic).  Basis
//  of all other noise.
//
int Get1dNoise( int index, int seed );
int Get2dNoise( int posX, int posY, int seed );
int Get3dNoise( int posX, int posY, int posZ, int seed );
int Get4dNoise( int posX, int posY, int posZ, int posT, int seed );

//--------------------------------------------------------------------------
// Same functions, mapped to floats in [0,1] for convenience.
//
float Get1dNoiseZeroToOne( int index, int seed );
float Get2dNoiseZeroToOne( int posX, int posY, int seed );
float Get3dNoiseZeroToOne( int posX, int posY, int posZ, int seed );
float Get4dNoiseZeroToOne( int posX, int posY, int posZ, int posT, int seed );

//--------------------------------------------------------------------------
// Same functions, mapped to floats in [-1,1] for convenience.
//
float Get1dNoiseNegOneToOne( int index, int seed );
float Get2dNoiseNegOneToOne( int posX, int posY, int seed );
float Get3dNoiseNegOneToOne( int posX, int posY, int posZ, int seed );
float Get4dNoiseNegOneToOne( int posX, int posY, int posZ, int posT, int seed );


////////////////////////////////////////////////////////////////////////////
// Function definitions below
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
// Fast hash of an int32 into a different (unrecognizable) 32.
//
// Returns an integer containing 32 reasonably-well-scrambled bits, based on
//  the hash of a given (signed) integer input parameter (position/index) and
//  [optional] seed.  Kind of like looking up a value in an infinitely large
//  table of previously generated random numbers.
//
// I call this particular approach SquirrelNoise5 (5th iteration of my 1D raw
//  noise function).
//
// Many thanks to Peter Schmidt-Nielsen whose outstanding analysis helped
//  identify a weakness in the SquirrelNoise3 code I originally used in my
//  GDC 2017 talk, "Noise-based RNG". Version 5 avoids a noise repetition
//  found in version 3 at extremely high position values caused by a lack of
//  influence by some of the high input bits onto some of the low output bits.
//
// The revised SquirrelNoise5 function ensures all input bits affect all
//  output bits, and to (for me) a statistically acceptable degree.  I believe
//  the worst-case here is in the amount of influence input position bit #30
//  has on output noise bit #0 (49.99%, vs. 50% ideal).
//
private int SquirrelNoise5(int positionX, int seed )
{
	int SQ5_BIT_NOISE1 = 0xd2a80a3f;	// 11010010101010000000101000111111
	int SQ5_BIT_NOISE2 = 0xa884f197;	// 10101000100001001111000110010111
	int SQ5_BIT_NOISE3 = 0x6C736F4B;  // 01101100011100110110111101001011
	int SQ5_BIT_NOISE4 = 0xB79F3ABB;	// 10110111100111110011101010111011
	int SQ5_BIT_NOISE5 = 0x1b56c4f5;	// 00011011010101101100010011110101

	int mangledBits = positionX;

  seed = sanitize_seed(seed || 0);

	mangledBits = (mangledBits * SQ5_BIT_NOISE1)      & INT_32_UNSIGNED_MAX;
	mangledBits = (mangledBits + seed)                & INT_32_UNSIGNED_MAX;
	mangledBits = (mangledBits ^ (mangledBits >> 9))  & INT_32_UNSIGNED_MAX;
	mangledBits = (mangledBits + SQ5_BIT_NOISE2)      & INT_32_UNSIGNED_MAX;
	mangledBits = (mangledBits ^ (mangledBits >> 11)) & INT_32_UNSIGNED_MAX;
	mangledBits = (mangledBits * SQ5_BIT_NOISE3)      & INT_32_UNSIGNED_MAX;
	mangledBits = (mangledBits ^ (mangledBits >> 13)) & INT_32_UNSIGNED_MAX;
	mangledBits = (mangledBits + SQ5_BIT_NOISE4)      & INT_32_UNSIGNED_MAX;
	mangledBits = (mangledBits ^ (mangledBits >> 15)) & INT_32_UNSIGNED_MAX;
	mangledBits = (mangledBits * SQ5_BIT_NOISE5)      & INT_32_UNSIGNED_MAX;
	mangledBits = (mangledBits ^ (mangledBits >> 17)) & INT_32_UNSIGNED_MAX;

	return mangledBits;
}

//--------------------------------------------------------------------------
private int sanitize_seed(int seed) {
  return abs(seed) & INT_32_UNSIGNED_MAX;
}

//--------------------------------------------------------------------------
private int fake_uint32_overflow(int number) {
  return number & INT_32_UNSIGNED_MAX;
}

//--------------------------------------------------------------------------
private int fake_int32_overflow(int number) {
  if(number <= INT_32_SIGNED_MAX)
    return number;
  return (number % INT_32_SIGNED_MAX) - INT_32_SIGNED_MAX - 2;
}


//--------------------------------------------------------------------------
int Get1dNoise( int positionX, int seed )
{
	return SquirrelNoise5( positionX, seed );
}

//--------------------------------------------------------------------------
int Get2dNoise( int posX, int posY, int seed )
{
	int PRIME_NUMBER = 198491317; // Large prime number with non-boring bits
	return SquirrelNoise5( posX
    + fake_uint32_overflow(PRIME_NUMBER * posY)
    , seed
  );
}

//--------------------------------------------------------------------------
int Get3dNoise( int posX, int posY, int posZ, int seed )
{
	int PRIME1 = 198491317; // Large prime number with non-boring bits
	int PRIME2 = 6542989;   // Large prime number with distinct, non-boring bits
	return SquirrelNoise5( posX
    + fake_uint32_overflow(PRIME1 * posY)
    + fake_uint32_overflow(PRIME2 * posZ)
    , seed
  );
}

//--------------------------------------------------------------------------
int Get4dNoise( int posX, int posY, int posZ, int posT, int seed )
{
	int PRIME1 = 198491317; // Large prime number with non-boring bits
	int PRIME2 = 6542989;   // Large prime number with distinct, non-boring bits
	int PRIME3 = 357239;    // Large prime number with distinct, non-boring bits
	return SquirrelNoise5( posX
    + fake_uint32_overflow(PRIME1 * posY)
    + fake_uint32_overflow(PRIME2 * posZ)
    + fake_uint32_overflow(PRIME3 * posT)
    , seed
  );
}

//--------------------------------------------------------------------------
float Get1dNoiseZeroToOne( int index, int seed )
{
	return ( 1.0 * Get1dNoise( index, seed ) ) / INT_32_UNSIGNED_MAX;
}

//--------------------------------------------------------------------------
float Get2dNoiseZeroToOne( int posX, int posY, int seed )
{
	return ( 1.0 * Get2dNoise( posX, posY, seed ) ) / INT_32_UNSIGNED_MAX;
}

//--------------------------------------------------------------------------
float Get3dNoiseZeroToOne( int posX, int posY, int posZ, int seed )
{
  int result = Get3dNoise( posX, posY, posZ, seed );
	return ( 1.0 * result ) / INT_32_UNSIGNED_MAX;
}

//--------------------------------------------------------------------------
float Get4dNoiseZeroToOne( int posX, int posY, int posZ, int posT, int seed )
{
	int result = Get4dNoise( posX, posY, posZ, posT, seed );
	return ( 1.0 * result ) / INT_32_UNSIGNED_MAX;
}


//--------------------------------------------------------------------------
float Get1dNoiseNegOneToOne( int index, int seed )
{
	int result = Get1dNoise( index, seed );
	return ( 1.0 * (result - INT_32_UNSIGNED_MAX) ) / INT_32_SIGNED_MAX;
}


//--------------------------------------------------------------------------
float Get2dNoiseNegOneToOne( int posX, int posY, int seed )
{
	int result = Get2dNoise( posX, posY, seed );
	return ( 1.0 * (result - INT_32_UNSIGNED_MAX) ) / INT_32_SIGNED_MAX;
}

//--------------------------------------------------------------------------
float Get3dNoiseNegOneToOne( int posX, int posY, int posZ, int seed )
{
	int result = Get3dNoise( posX, posY, posZ, seed );
	return ( 1.0 * (result - INT_32_UNSIGNED_MAX) ) / INT_32_SIGNED_MAX;
}


//--------------------------------------------------------------------------
float Get4dNoiseNegOneToOne( int posX, int posY, int posZ, int posT, int seed )
{
	int result = Get4dNoise( posX, posY, posZ, posT, seed );
	return ( 1.0 * (result - INT_32_UNSIGNED_MAX) ) / INT_32_SIGNED_MAX;
}

#endif
