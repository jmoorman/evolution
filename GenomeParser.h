#pragma once

#include "BodySegmentGene.h"
#include "CreatureGenome.h"
#include "JointGene.h"

class GenomeParser
{
public:
   static const int MAX_LINE_LENGTH = 120;
   static const char *SEGMENTS_STRING;
   static const char *JOINTS_STRING;
   static const char *JOINT_FORMAT_STRING;
   static const char *SEGMENT_FORMAT_STRING;

   static CreatureGenome *parseGenome(const char *genomeFile);

private:
   static BodySegmentGene *parseBodySegmentGene(const char *bodySegmentLine);
   static JointGene *parseJointGene(const char *jointLine);

};

