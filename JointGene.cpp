#include "stdafx.h"
#include "JointGene.h"


JointGene::JointGene(long parentId, long childId) : mParentId(parentId), mChildId(childId) {
}

long JointGene::getParentId() {
   return mParentId;
}

long JointGene::getChildId() {
   return mChildId;
}

const btVector3& JointGene::getParentXYZ() {
   return mPositionOnParent;
}

const btVector3& JointGene::getChildXYZ() {
   return mPositionOnChild;
}

const btVector3& JointGene::getLowerLimit() {
   return mAngularLowerLimit;
}

const btVector3& JointGene::getUpperLimit() {
   return mAngularUpperLimit;
}

void JointGene::setPositionOnParent(const btVector3 &position) {
   mPositionOnParent = position;
}

void JointGene::setPositionOnParent(float parentX, float parentY, float parentZ) {
   mPositionOnParent.setValue(parentX, parentY, parentZ);
}

void JointGene::setPositionOnChild(const btVector3 &position) {
   mPositionOnChild = position;
}

void JointGene::setPositionOnChild(float childX, float childY, float childZ) {
   mPositionOnChild.setValue(childX, childY, childZ);
}

void JointGene::setLowerLimit(float lowerX, float lowerY, float lowerZ) {
   mAngularLowerLimit.setValue(lowerX, lowerY, lowerZ);
}

void JointGene::setUpperLimit(float upperX, float upperY, float upperZ) {
   mAngularUpperLimit.setValue(upperX, upperY, upperZ);
}

JointGene * JointGene::createMutatedGene(float mutationRate) {
   JointGene *mutatedGene = new JointGene(mParentId, mChildId);
   mutatedGene->setPositionOnParent(createMutatedPositionVector(mPositionOnParent, mutationRate));
   mutatedGene->setPositionOnChild(createMutatedPositionVector(mPositionOnChild, mutationRate));
   mutatedGene->setLowerLimit(mAngularLowerLimit.x(), mAngularLowerLimit.y(), mAngularLowerLimit.z());
   mutatedGene->setUpperLimit(mAngularUpperLimit.x(), mAngularUpperLimit.y(), mAngularUpperLimit.z());
   //TODO: mutate the parent/child IDs
   return mutatedGene;
}

/**
 * Create a new position vector where each component is between -1, 1
 */
const btVector3 JointGene::createMutatedPositionVector(const btVector3 &position, float mutationRate) {
   static std::default_random_engine generator;

   std::normal_distribution<float> positionDistribution(0, mutationRate);

   float x = position.x() + positionDistribution(generator);
   //bound between -1, 1
   x = x < -1 ? -1 : x;
   x = x > 1 ? 1 : x;

   float y = position.y() + positionDistribution(generator);
   //bound between -1, 1
   y = y < -1 ? -1 : y;
   y = y > 1 ? 1 : y;

   float z = position.z() + positionDistribution(generator);
   //bound between -1, 1
   z = z < -1 ? -1 : z;
   z = z > 1 ? 1 : z;

   return btVector3(x, y, z);
}