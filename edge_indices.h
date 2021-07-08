#ifndef __EDGE_INDICES_H__
#define __EDGE_INDICES_H__

#include <vector>
#include <stdio.h>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <algorithm>
#include <queue>

class edge {
public:
	unsigned int v1;		//인접 face 찾을 때 필요
	unsigned int v2;
	int f1;
	int f2;
	edge();
};

void adjacentFace(std::vector<unsigned int>& vertexIndices, std::vector<std::vector<edge>>& edges, std::vector<unsigned int>& faceIndices);
#endif