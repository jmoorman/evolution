#pragma once

#include "Creature.h"
#include "FreeLookCamera.h"

class ApplicationManager
{
public:
   ApplicationManager(unsigned int width, unsigned int height);
   ~ApplicationManager(void);
   int run();

private:
   unsigned int mWidth;
   unsigned int mHeight;
   bool mRunning;
   SDL_Window *mWindow;
   SDL_GLContext mContext;
   FreeLookCamera *mCamera;
   
   // Bullet Physics infrastructure
   //collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration *mCollisionConfiguration;
   //use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher *mDispatcher;
   //btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface *mOverlappingPairCache;
   //the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver *mSolver;
   btDiscreteDynamicsWorld *mDynamicsWorld;
   //keep track of the shapes, we release memory at exit.
   btAlignedObjectArray<btCollisionShape*> mCollisionShapes;

   //temp
   //BodySegment *segment;
   //BodySegment *segment2;
   std::vector<Creature *> creatures;

   bool init();
   void update();
   void render();
   void cleanup();

   void onEvent(SDL_Event &event);
};

