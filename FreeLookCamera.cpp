#include "stdafx.h"
#include "FreeLookCamera.h"

const float FreeLookCamera::kMaxPitch = 60;
const float FreeLookCamera::kMinPitch = -60;

FreeLookCamera::FreeLookCamera(btVector3 &position, float pitch, float yaw)
   : mCameraPosition(position), mPitch(pitch), mYaw(yaw), mCameraUpVector(0, 1, 0)
{
   clampPitch();
   calculateOrientation();
}


FreeLookCamera::~FreeLookCamera(void)
{
}

const btVector3& FreeLookCamera::getCameraPosition() {
   return mCameraPosition;
}

const btVector3& FreeLookCamera::getCameraOrientation() {
   return mCameraOrientation;
}

const btVector3& FreeLookCamera::getCameraUpVector() {
   return mCameraUpVector;
}


void FreeLookCamera::rotate(float pitch, float yaw) {
   mPitch += pitch;
   mYaw += yaw;
   clampPitch();
   calculateOrientation();
}

void FreeLookCamera::moveForward(float distance) {
   mCameraPosition += mCameraOrientation.normalize() * distance;
}

void FreeLookCamera::moveBackward(float distance) {
   mCameraPosition -= mCameraOrientation.normalize() * distance;
}

void FreeLookCamera::moveLeft(float distance) {
   mCameraPosition -= mCameraOrientation.cross(mCameraUpVector).normalize() * distance;
}

void FreeLookCamera::moveRight(float distance) {
   mCameraPosition += mCameraOrientation.cross(mCameraUpVector).normalize() * distance;
}

void FreeLookCamera::moveUp(float distance) {
   mCameraPosition += mCameraUpVector.normalize() * distance;
}

void FreeLookCamera::moveDown(float distance) {
   mCameraPosition -= mCameraUpVector.normalize() * distance;
}

void FreeLookCamera::clampPitch() {
   mPitch = mPitch < kMinPitch ? kMinPitch : mPitch;
   mPitch = mPitch > kMaxPitch ? kMaxPitch : mPitch;
}

void FreeLookCamera::calculateOrientation() {
   float pitchRadians = mPitch * M_PI / 180.f;
   float yawRadians = mYaw * M_PI / 180.f;
   mCameraOrientation.setX(cosf(pitchRadians) * cosf(yawRadians));
   mCameraOrientation.setY(sinf(pitchRadians));
   mCameraOrientation.setZ(cosf(pitchRadians) * cosf((M_PI / 2) - yawRadians)); 
}
