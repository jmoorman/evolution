#pragma once

#include "BodySegmentGene.h"

//forward declaration
class BodySegmentGene;

//A joint is a connection between a specific point on a parent body node and the
//center of the top of it's child node
class JointGene
{
public:
	JointGene(long parentId, long childId);

   long getParentId();
   long getChildId();

   const btVector3& getParentXYZ();

   void setPositionOnParent(const btVector3 &position);
   void setPositionOnParent(float positionX, float positionY, float positionZ);

   const btVector3& getChildXYZ();

   void setPositionOnChild(const btVector3 &position);
   void setPositionOnChild(float positionX, float positionY, float positionZ);
   
   const btVector3& getLowerLimit();
   void setLowerLimit(float lowerX, float lowerY, float lowerZ);
   
   const btVector3& getUpperLimit();
   void setUpperLimit(float upperX, float upperY, float upperZ);

   /*
    * Generate a gene based off of this gene but with some variations.
    * The parent and/or child of the joint can be changed in addition to
    * its internal parameters
    */
   JointGene * createMutatedGene(float mutationRate);

private:
	//The two segments that are connected by this joint
   long mParentId;
   long mChildId;

   //The relative location on the parent segment where the joint will attach in the range [-1, 1]
   btVector3 mPositionOnParent;

   //The relative location on the child segment where the joint will attach
   btVector3 mPositionOnChild;

   //The lower angular rotation limit about each axis in radians
   btVector3 mAngularLowerLimit;
   
   //The upper angular rotation limit about each axis in radians
   btVector3 mAngularUpperLimit;

   const btVector3 createMutatedPositionVector(const btVector3 &position, float mutationRate);
};

