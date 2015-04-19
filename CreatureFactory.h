#pragma once

#include "Creature.h"
#include "CreatureGenome.h"

class CreatureFactory
{
public:
   CreatureFactory(void);
   ~CreatureFactory(void);

   static Creature * createCreatureFromGenome(CreatureGenome *genome, float x, float y, float z);

private:
   static void buildHelper(CreatureGenome *genome,
                           BodySegmentGene *parentGene,
                           BodySegment *parentSegment,
                           std::vector<BodySegment *> *expansionList,
                           std::vector<BodySegmentGene *> &visitedGenes,
                           std::vector<BodySegment *> &createdSegments);
};

