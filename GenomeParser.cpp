#include "stdafx.h"

#include "GenomeParser.h"

enum ParseState {
   NONE,
   SEGMENTS,
   JOINTS
};

const char *GenomeParser::JOINTS_STRING = "joints";
const char *GenomeParser::SEGMENTS_STRING = "segments";

//parentId, childId, parentX, parentY, parentZ, childX, childY, childZ, lowerX, lowerY, lowerZ, upperX, upperY, upperZ
const char *GenomeParser::JOINT_FORMAT_STRING = "%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f";
//#width, height, depth, density, scale, expansions
const char *GenomeParser::SEGMENT_FORMAT_STRING = "%f,%f,%f,%f,%f,%d";

CreatureGenome * GenomeParser::parseGenome(const char *genomeFile) {
   std::vector<BodySegmentGene *> bodySegmentGenes;
   std::vector<JointGene *> jointGenes;
   std::map<BodySegmentGene *, std::vector<JointGene *> > adjacencyMap;

   ParseState state = NONE;
   char line[MAX_LINE_LENGTH];
   std::ifstream inFile(genomeFile);
   while (inFile.good()) {
      inFile.getline(line, MAX_LINE_LENGTH);
      if(strcmp(line, SEGMENTS_STRING) == 0) {
         state = SEGMENTS;
      } else if (strcmp(line, JOINTS_STRING) == 0) {
         state = JOINTS;
      } else if (line[0] >= '0' && line[0] <= '9') {
         if(state == SEGMENTS) {
            BodySegmentGene *bodyGene = parseBodySegmentGene(line);
            bodySegmentGenes.push_back(bodyGene);
         } else if (state == JOINTS) {
            JointGene *jointGene = parseJointGene(line);
            jointGenes.push_back(jointGene);
            //Only add the joint to the adjacency map if it joins valid body segments
            if(jointGene->getParentId() < bodySegmentGenes.size() && jointGene->getChildId() < bodySegmentGenes.size()) {
               adjacencyMap[(bodySegmentGenes[jointGene->getParentId()])].push_back(jointGene);
            }
         }
      }
   }
   return new CreatureGenome(bodySegmentGenes, jointGenes, adjacencyMap);
}

BodySegmentGene * GenomeParser::parseBodySegmentGene(const char *bodySegmentLine) {
   std::cout << bodySegmentLine << std::endl;
   float width, height, depth, density, scale;
   int expansions;
   sscanf(bodySegmentLine, SEGMENT_FORMAT_STRING,&width, &height, &depth, &density, &scale, &expansions);
   return new BodySegmentGene(width, height, depth, density, scale, expansions);
}

JointGene * GenomeParser::parseJointGene(const char *jointLine) {
   std::cout << jointLine << std::endl;
   long parentId, childId;
   float parentX, parentY, parentZ;
   float childX, childY, childZ;
   float lowerX, lowerY, lowerZ;
   float upperX, upperY, upperZ;
   sscanf(jointLine, JOINT_FORMAT_STRING, &parentId, &childId,
         &parentX, &parentY, &parentZ,
         &childX, &childY, &childZ,
         &lowerX, &lowerY, &lowerZ,
         &upperX, &upperY, &upperZ);
   JointGene *jointGene = new JointGene(parentId, childId);
   jointGene->setPositionOnParent(parentX, parentY, parentZ);
   jointGene->setPositionOnChild(childX, childY, childZ);
   jointGene->setLowerLimit(lowerX, lowerY, lowerZ);
   jointGene->setUpperLimit(upperX, upperY, upperZ);
   return jointGene;
}