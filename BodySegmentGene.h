#pragma once

#include <vector>

#include "JointGene.h"

//forward declaration
class JointGene;

class BodySegmentGene
{
public:
   BodySegmentGene(float baseWidth, float baseHeight, float baseDepth, float scaleFactor, float density, int expansionLimit);
	~BodySegmentGene(void);

   float getBaseWidth() { return mBaseWidth; }
   float getBaseHeight() { return mBaseHeight; }
   float getBaseDepth() { return mBaseDepth; }
   float getScaleFactor() { return mScaleFactor; }
   float getDensity() { return mDensity; }
   int getExpansionLimit() { return mExpansionLimit; }
   
   BodySegmentGene * createMutatedGene(float mutationRate);

private:

	//The dimensions of the segment without respect to scaling
	float mBaseWidth;
	float mBaseHeight;
	float mBaseDepth;

	//Every body segment connected to this segment is scaled in the range [0, 1]
	float mScaleFactor;

   //The density of the body segment
   float mDensity;

	//The maximum number of times this gene's subtree can be repeated within itself
	int mExpansionLimit;
};

