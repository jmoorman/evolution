#include "stdafx.h"
#include "BodySegmentGene.h"


BodySegmentGene::BodySegmentGene(float baseWidth, float baseHeight, float baseDepth, float scaleFactor, float density, int expansionLimit)
   : mBaseWidth(baseWidth), mBaseHeight(baseHeight), mBaseDepth(baseDepth), mScaleFactor(scaleFactor), mDensity(density), mExpansionLimit(expansionLimit)
{
}


BodySegmentGene::~BodySegmentGene(void)
{
}

BodySegmentGene * BodySegmentGene::createMutatedGene(float mutationRate) {
   static std::default_random_engine generator;

   //floor at 0.1 to prevent negative or miniscule dimensions
   std::normal_distribution<float> widthDistribution(mBaseWidth, mBaseWidth * mutationRate);
   float baseWidth = widthDistribution(generator);
   baseWidth = baseWidth < 0.1 ? 0.1 : baseWidth;

   //floor at 0.1 to prevent negative or miniscule dimensions
   std::normal_distribution<float> heightDistribution(mBaseHeight, mBaseHeight * mutationRate);
   float baseHeight = heightDistribution(generator);
   baseHeight = baseHeight < 0.1 ? 0.1 : baseHeight;

   //floor at 0.1 to prevent negative or miniscule dimensions
   std::normal_distribution<float> depthDistribution(mBaseDepth, mBaseDepth * mutationRate);
   float baseDepth = depthDistribution(generator);
   baseDepth = baseDepth < 0.1 ? 0.1 : baseDepth;
   
   std::normal_distribution<float> scaleFactorDistribution(mScaleFactor, mScaleFactor * mutationRate);
   float scaleFactor = scaleFactorDistribution(generator);

   //floor at 0.1 to prevent negative or miniscule density
   std::normal_distribution<float> densityDistribution(mDensity, mDensity * mutationRate);
   float density = densityDistribution(generator);
   density = density < 0.1 ? 0.1 : density;

   //expansion limit has a chance to increase or decrease by 1
   int expansionLimit = mExpansionLimit;
   if (rand() % 100 < (mutationRate * 100)) {
      expansionLimit += (rand() % 3) - 1; //possible values: -1, 0, 1
   }
   expansionLimit = expansionLimit < 0 ? 0 : expansionLimit;

   //std::cout << "mutated gene: (" << baseWidth << ", " << baseHeight << ", " << baseDepth << ")" << std::endl; 
   return new BodySegmentGene(baseWidth, baseHeight, baseDepth, scaleFactor, density, expansionLimit);
}

