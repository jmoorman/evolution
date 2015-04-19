#include "stdafx.h"

#include <fstream>

#include "CreatureGenome.h"

CreatureGenome::CreatureGenome(std::vector<BodySegmentGene *> &bodyGenes,
                               std::vector<JointGene *> &jointGenes,
                               std::map<BodySegmentGene *, std::vector<JointGene *> > &adjacencyMap)
      : mBodyGenes(bodyGenes), mJointGenes(jointGenes), mAdjacencyMap(adjacencyMap) {
}

const std::vector<BodySegmentGene *>& CreatureGenome::getBodyGenes() {
   return mBodyGenes;
}

const std::vector<JointGene *>& CreatureGenome::getJointGenes() {
   return mJointGenes;
}

const std::vector<JointGene *> & CreatureGenome::getJointGenesForBodySegmentGene(BodySegmentGene *gene) {
   return mAdjacencyMap[gene];
}

CreatureGenome * CreatureGenome::createMutatedGenome(float mutationRate) {
   std::vector<BodySegmentGene *> mutatedBodyGenes;
   std::vector<JointGene *> mutatedJointGenes;
   std::map<BodySegmentGene *, std::vector<JointGene *> > mutatedAdjacencyMap;

   //mutate existing body segments
   for(BodySegmentGene *gene : mBodyGenes) {
      mutatedBodyGenes.push_back(gene->createMutatedGene(mutationRate));
   }

   //mutate existing joints
   for(JointGene *gene : mJointGenes) {
      JointGene *mutatedJointGene = gene->createMutatedGene(mutationRate);
      mutatedJointGenes.push_back(mutatedJointGene);
      //Only add the joint to the adjacency map if it joins valid body segments
      if(mutatedJointGene->getParentId() < mutatedBodyGenes.size() && mutatedJointGene->getChildId() < mutatedBodyGenes.size()) {
         mutatedAdjacencyMap[(mutatedBodyGenes[mutatedJointGene->getParentId()])].push_back(mutatedJointGene);
      }
   }

   return new CreatureGenome(mutatedBodyGenes, mutatedJointGenes, mutatedAdjacencyMap);
}