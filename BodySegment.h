#pragma once

#include "JointInfo.h"
#include "BodySegmentGene.h"

class BodySegment
{
public:
   BodySegment(BodySegmentGene *gene, float scale=1.f);
   BodySegment(float width, float height, float depth, float density);

	~BodySegment(void);

   BodySegmentGene *getGene();
   float getWidth();
   float getHeight();
   float getDepth();

   void setPosition(btVector3 &position);

   void addToWorld(btDynamicsWorld *world);

   void joinTo(BodySegment &parent, const JointInfo &info);

   void print();
  
   btGeneric6DofConstraint * getJoint();

private:
   //The gene that was used to construct this segment
   BodySegmentGene *mGene;

   //The physics object that represents the body segment in the world
	btRigidBody *mPhysicsObject;

	//The collision shape of the segment representing its physical extents 
	btCollisionShape *mShape;

   //The joint between this segment and it's parent segment
   btGeneric6DofConstraint *mJoint;

   //The dimensions of the body segment
   float mWidth;
   float mHeight;
   float mDepth;

   //The density of this body segment which will be used to calculate mass
   float mDensity;

   btRigidBody * getPhysicsObject();

};

