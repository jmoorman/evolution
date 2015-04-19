#pragma once

#include "BodySegment.h"

class CreatureGenome; //forward declaration to avoid circular dependency

class Creature
{
public:
	Creature(CreatureGenome *genome, std::vector<BodySegment *> &bodySegments);
	~Creature(void);

   void addToWorld(btDynamicsWorld *world);
   std::vector<BodySegment *> & getBodySegments();

private:
   CreatureGenome *mGenome;
   std::vector<BodySegment *> mBodySegments;
   BodySegment *mHead;
};

