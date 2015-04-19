#include "stdafx.h"

#include "GLDebugDrawer.h"

#include "ApplicationManager.h"
#include "BodySegment.h"
#include "CreatureFactory.h"
#include "GenomeParser.h"
#include "TextRenderer.h"

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2     1.57079632679489661923
#endif

#ifndef M_PI_4
#define M_PI_4     0.785398163397448309616
#endif

ApplicationManager::ApplicationManager(unsigned int width, unsigned int height)
      : mWidth(width), mHeight(height), mRunning(false), mWindow(NULL) {
}

ApplicationManager::~ApplicationManager() {
}

int ApplicationManager::run() {
   if (!init()) {
      return -1;
   }
   mRunning = true;
   std::thread updateThread(&ApplicationManager::update, this);
   render();
   
   updateThread.join();
   cleanup();

   return 0;
}

bool ApplicationManager::init() {
   srand(time(NULL));
   if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return false;
	}
	atexit(SDL_Quit);

   if (TTF_Init() != 0) {
      std::cout << "TTF_Init Error: " << TTF_GetError() << std::endl;
      return false;
   }

   mWindow = SDL_CreateWindow("Evolution", 300, 100, mWidth, mHeight, SDL_WINDOW_OPENGL);
	if (mWindow == NULL){
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return false;
	}
   
	mContext = SDL_GL_CreateContext(mWindow);

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	mCollisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	mDispatcher = new	btCollisionDispatcher(mCollisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	mOverlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	mSolver = new btSequentialImpulseConstraintSolver();

	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mOverlappingPairCache, mSolver, mCollisionConfiguration);
	
	GLDebugDrawer *debugDrawer = new GLDebugDrawer();	
   debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	mDynamicsWorld->setDebugDrawer(debugDrawer);
	mDynamicsWorld->setGravity(btVector3(0,-10,0));

   /*********************
    * SET UP DEMO SCENE *
    *********************/
	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(50.)));
	//make sure to re-use collision shapes among rigid bodies whenever possible!

	mCollisionShapes.push_back(groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0,-50,0));

	{
		btScalar mass(0.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		btVector3 localInertia(0,0,0);
		
		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		mDynamicsWorld->addRigidBody(body);
	}

   GenomeParser parser;
   CreatureGenome *genome = parser.parseGenome("sample_genome.txt");
   Creature *creature;
   
   /*
   creature = factory.buildCreatureFromGenome(genome, 0, 10, 0);
   creature->addToWorld(mDynamicsWorld);
   creatures.push_back(creature);
   */
   
   int i = 0; int j = 0;
   for(int i = -2; i < 2; i++) {
      for(int j = -2; j < 2; j++) { 
         creature = CreatureFactory::createCreatureFromGenome(genome->createMutatedGenome(0.3), i * 6, 5, j * 6);
         creature->addToWorld(mDynamicsWorld);
         creatures.push_back(creature);
      }
   }
   
   mCamera = new FreeLookCamera(btVector3(0, 5, 25), -10, -90);
   return true;
}

void ApplicationManager::update() {
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::microseconds;

	auto previous_time = high_resolution_clock::now();
	while(mRunning) {
		auto current_time = high_resolution_clock::now();
		auto dt = duration_cast<microseconds>(current_time - previous_time).count();
      previous_time = current_time;

		mDynamicsWorld->stepSimulation(dt / 1000000.f);
	}
}

void ApplicationManager::render() {
   TextRenderer textRenderer;
   textRenderer.setColor(255, 0, 0);
   textRenderer.setFont(TextRenderer::kOpenSans, 20);

   using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::microseconds;
   SDL_Event event;
	auto previous_time = high_resolution_clock::now();
   long long dt = 0;
   int count = 0;
   int fps = 0;
   while(mRunning) {
      auto current_time = high_resolution_clock::now();
		dt += duration_cast<microseconds>(current_time - previous_time).count();
      count++;
      if(dt > 1000000) {
         fps = count * 1000000.f / dt;
         dt -= 1000000;
         count = 0;
      }
      previous_time = current_time;

      while (SDL_PollEvent(&event)) {
         onEvent(event);
      }

      //Render 3D world
	   glMatrixMode(GL_PROJECTION);
	   glLoadIdentity();
	   gluPerspective(60, mWidth/(float)mHeight, 0.1, 200.0);

	   glMatrixMode(GL_MODELVIEW);
	   glLoadIdentity();
	   glViewport(0, 0, mWidth, mHeight);

	   glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

      const btVector3 &cameraPosition = mCamera->getCameraPosition();
      const btVector3 &cameraOrientation = mCamera->getCameraOrientation();
      const btVector3 &cameraUpVector = mCamera->getCameraUpVector();
      gluLookAt(cameraPosition.x(), cameraPosition.y(), cameraPosition.z(),
            cameraPosition.x() + cameraOrientation.x(),
            cameraPosition.y() + cameraOrientation.y(),
            cameraPosition.z() + cameraOrientation.z(),
            cameraUpVector.x(), cameraUpVector.y(), cameraUpVector.z());
		mDynamicsWorld->debugDrawWorld();

      //Render 2D overlay
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(0, mWidth, mHeight, 0, -1, 1);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glClear(GL_DEPTH_BUFFER_BIT);

      textRenderer.renderText(std::string("Render FPS: ").append(std::to_string(fps)));

		SDL_GL_SwapWindow(mWindow);
   }
}
void ApplicationManager::cleanup() {
   //cleanup in the reverse order of creation/initialization
   for(Creature *creature : creatures) {
      for(BodySegment *segment : creature->getBodySegments()) {
         if(segment->getJoint() != NULL) {
            mDynamicsWorld->removeConstraint(segment->getJoint());
            delete(segment->getJoint());
         }
      }
   }
	//remove the rigidbodies from the dynamics world and delete them
   btCollisionObjectArray &objects = mDynamicsWorld->getCollisionObjectArray();
   for (int i = 0; i < objects.size(); i++) {
		btCollisionObject* object = objects[i];
		btRigidBody* body = btRigidBody::upcast(object);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		mDynamicsWorld->removeCollisionObject(object);
		delete object;
	}

   //delete collision shapes
	for (int i = 0; i < mCollisionShapes.size(); i++) {
		btCollisionShape *shape = mCollisionShapes[i];
		mCollisionShapes[i] = NULL;
		delete shape;
	}

   delete mCamera;
	delete mDynamicsWorld;
	delete mSolver;
	delete mOverlappingPairCache;
	delete mDispatcher;
	delete mCollisionConfiguration;
		
	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	SDL_GL_DeleteContext(mContext); 
	SDL_DestroyWindow(mWindow);
   SDL_Quit();
}

void ApplicationManager::onEvent(SDL_Event &event) {
   switch(event.type) {
      case SDL_KEYDOWN:
         switch(event.key.keysym.sym) {
            case SDLK_w:
               mCamera->moveForward(1.f);
               break;
            case SDLK_a:
               mCamera->moveLeft(1.f);
               break;
            case SDLK_s:
               mCamera->moveBackward(1.f);
               break;
            case SDLK_d:
               mCamera->moveRight(1.f);
               break;
            default:
               mRunning = false;
               break;
         }
         break;
      case SDL_MOUSEMOTION:
         if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            mCamera->rotate(-event.motion.yrel / 2.f, event.motion.xrel / 2.f);
         }
      default: break;
   }
}
