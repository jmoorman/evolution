#pragma once

#include "stdafx.h"

/*
 * A free-look camera has a position and orientation. Rotation is allowed along
 * the pitch and yaw axis only. (aka you can't "do a barrel roll").
 */
class FreeLookCamera
{
public:
   static const float kMaxPitch;
   static const float kMinPitch;

   //pitch and yaw are given in degrees
   FreeLookCamera(btVector3 &position, float pitch, float yaw);
   ~FreeLookCamera(void);

   void rotate(float pitch, float yaw);
   void moveForward(float distance);
   void moveBackward(float distance);
   void moveLeft(float distance);
   void moveRight(float distance);
   void moveUp(float distance);
   void moveDown(float distance);

   const btVector3& getCameraPosition();
   const btVector3& getCameraOrientation();
   const btVector3& getCameraUpVector();

private:
   //position
   btVector3 mCameraPosition;
   //orientation
   btVector3 mCameraOrientation;
   //up vector
   btVector3 mCameraUpVector;

   //It's easier to track state in pitch/yaw and then convert to a vector
   float mPitch;
   float mYaw;

   //clamp the pitch value between the min/max
   void clampPitch();
   void calculateOrientation();
};

