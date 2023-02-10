
//------------------------------------------------------------------------
//
//  Name:   utils.h
//
//  Desc:   misc utility function and some constants
//			based of FEAR SDK & Mat Bucklands code
//     
//  Author: Brett Keir Jones 
//
//------------------------------------------------------------------------

#pragma once

// get a random number between the two given values
static float GetRandom(float min, float max)
{
	float randNum = (float)rand() / RAND_MAX;
	float num = min + (max - min) * randNum;
	return num;
}

//
//given two values, this will determine the smaller of the two values using the < operator
template<typename T>
inline T GetMin(T a, T b)					{ return ((a < b) ? a : b); }

//given two values this will return the larger of the two values using the < operator
template<typename T>
inline T GetMax(T a, T b)					{ return ((a < b) ? b : a); }

//given a value and two extents, this will ensure that min <= val <= max using the < operator
template<typename T>
inline T Clamp(T a, T min, T max)		{ return ((a < min) ? min : ((a < max) ? a : max)); }

