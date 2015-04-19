#pragma once

#include <vector>

#include "BodySegmentGene.h"
#include "Creature.h"

//A representation of a creature's genome from which we can construct an arbitrary number of
//identical creatures. The genome can also be mutated to generate new creatures.
class CreatureGenome
{
public:
   CreatureGenome(std::vector<BodySegmentGene *> &bodyGenes,
                  std::vector<JointGene *> &jointGenes,
                  std::map<BodySegmentGene *, std::vector<JointGene *> > &adjacencyMap);

   const std::vector<BodySegmentGene *> & getBodyGenes();
   const std::vector<JointGene *> & getJointGenes();

   const std::vector<JointGene *> & getJointGenesForBodySegmentGene(BodySegmentGene *gene);

	//Construct a new genome from this genome with random mutations
	CreatureGenome * createMutatedGenome(float mutationRate);

private:
   //The list of all body segments in the genome. Not all segments in the list are
	//guaranteed to be expressed (i.e. part of the directed graph).
	std::vector<BodySegmentGene *> mBodyGenes;
   
   //The list of joints between body segments
   std::vector<JointGene *> mJointGenes;

   //The adjacency list representing the connections between body segments genes
   std::map<BodySegmentGene *, std::vector<JointGene *> > mAdjacencyMap;
};

