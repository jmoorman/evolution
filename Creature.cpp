#include "stdafx.h"
#include "Creature.h"


Creature::Creature(CreatureGenome *genome, std::vector<BodySegment *> &bodySegments)
   : mGenome(genome), mBodySegments(bodySegments)
{
}


Creature::~Creature(void)
{
}

std::vector<BodySegment *> & Creature::getBodySegments() {
   return mBodySegments;
}

void Creature::addToWorld(btDynamicsWorld *world) {
   for (BodySegment *segment : mBodySegments) {
      segment->addToWorld(world);
   }
}
