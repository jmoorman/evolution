#include "stdafx.h"
#include "BodySegment.h"
#include "BodySegmentGene.h"

/**
 * Create a body segment from a given gene.
 */
BodySegment::BodySegment(BodySegmentGene *gene, float scale)
      : mGene(gene), mJoint(NULL) {
   mWidth = gene->getBaseWidth() * scale;
   mHeight = gene->getBaseHeight() * scale;
   mDepth = gene->getBaseDepth() * scale;
   mDensity = gene->getDensity();
   mShape = new btBoxShape(btVector3(btScalar(mWidth / 2),btScalar(mHeight / 2),btScalar(mDepth / 2)));
	btTransform transform;
   transform.setIdentity();
	btScalar mass(mWidth * mHeight * mDepth * mDensity);
   btVector3 localInertia(0, 0, 0);
   mShape->calculateLocalInertia(mass, localInertia);
   //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
   btDefaultMotionState* motionState = new btDefaultMotionState(transform);
   btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, mShape, localInertia);
   mPhysicsObject = new btRigidBody(rbInfo);
}

/**
 * Create a body segment at the origin.
 */
BodySegment::BodySegment(float width, float height, float depth, float density) 
      : mWidth(width), mHeight(height), mDepth(depth), mDensity(density), mJoint(NULL) {
   mShape = new btBoxShape(btVector3(btScalar(width / 2),btScalar(height / 2),btScalar(depth / 2)));
	btTransform transform;
   transform.setIdentity();
	btScalar mass(width * height * depth * mDensity);
   btVector3 localInertia(0, 0, 0);
   mShape->calculateLocalInertia(mass, localInertia);
   //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
   btDefaultMotionState* motionState = new btDefaultMotionState(transform);
   btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, mShape, localInertia);
   mPhysicsObject = new btRigidBody(rbInfo);
}

BodySegment::~BodySegment(void)
{
}

BodySegmentGene * BodySegment::getGene() {
   return mGene;
}

float BodySegment::getWidth() {
   return mWidth;
}

float BodySegment::getHeight() {
   return mHeight;
}

float BodySegment::getDepth() {
   return mDepth;
}

void BodySegment::setPosition(btVector3 &position) {
   btTransform newPosition;
   newPosition.setIdentity();
   newPosition.setOrigin(position);
   mPhysicsObject->setWorldTransform(newPosition);
}

btRigidBody * BodySegment::getPhysicsObject() {
   return mPhysicsObject;
}

void BodySegment::addToWorld(btDynamicsWorld *world) {
   if (mPhysicsObject != NULL) {
      world->addRigidBody(mPhysicsObject);
   }
   if (mJoint != NULL) {
      world->addConstraint(mJoint, true);
   }
}

void BodySegment::joinTo(BodySegment &parent, const JointInfo &info) {

   btRigidBody &rbA = *parent.getPhysicsObject();
   btVector3 positionOnA(info.positionOnA.x() * parent.getWidth() / 2,
                         info.positionOnA.y() * parent.getHeight() / 2,
                         info.positionOnA.z() * parent.getDepth() / 2);
   btTransform rbAFrame;
   rbAFrame.setIdentity();
   rbAFrame.setOrigin(positionOnA);
   btVector3 positionOnB(info.positionOnB.x() * mWidth / 2,
                         info.positionOnB.y() * mHeight / 2,
                         info.positionOnB.z() * mDepth / 2);
   btTransform rbBFrame;
   rbBFrame.setIdentity();
   rbBFrame.setOrigin(positionOnB);

   //Calculate where to position this segment in relation to its parent
   btTransform newPosition;
   newPosition.setIdentity();
   newPosition.mult(newPosition, rbBFrame.inverse());
   newPosition.mult(newPosition, rbAFrame);
   newPosition.mult(newPosition, rbA.getWorldTransform());
   mPhysicsObject->setWorldTransform(newPosition);

   mJoint = new btGeneric6DofConstraint(rbA, *mPhysicsObject, rbAFrame, rbBFrame, false);
   mJoint->setLinearLowerLimit(btVector3(0, 0, 0));
   mJoint->setLinearUpperLimit(btVector3(0, 0, 0));
   mJoint->setAngularLowerLimit(info.lowerLimits);
   mJoint->setAngularUpperLimit(info.upperLimits);
   for(int i = 0; i < 3; i++) {
      //mJoint->getRotationalLimitMotor(i)->m_enableMotor = true;
      //mJoint->getRotationalLimitMotor(i)->m_maxMotorForce = 1;
      //mJoint->getRotationalLimitMotor(i)->m_targetVelocity = 1;
   }
}

void BodySegment::print() {
	if (mPhysicsObject != NULL && mPhysicsObject->getMotionState()) {
	   btTransform trans;
	   mPhysicsObject->getMotionState()->getWorldTransform(trans);
	   printf("world pos = %f,%f,%f\n",float(trans.getOrigin().getX()),float(trans.getOrigin().getY()),float(trans.getOrigin().getZ()));
	}
}

btGeneric6DofConstraint * BodySegment::getJoint() {
   return mJoint;
}
