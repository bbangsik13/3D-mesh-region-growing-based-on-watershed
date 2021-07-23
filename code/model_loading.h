#ifndef __MODEL_LOADING_H__
#define __MODEL_LOADING_H__

#include <vector>
#include <stdio.h>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <algorithm>
#include <queue>

bool loadOBJ(const char* path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec3>& out_normals, 
	std::vector<unsigned int>& vertexIndices, std::vector<unsigned int>& normalIndices);
bool rendering_vertex(std::vector<glm::vec3>& vertices,	std::vector<unsigned int>& vertexIndices, std::vector<glm::vec3>& out_vertices, std::vector<unsigned int> ws_label, std::vector<glm::vec3>& face_rgb,
	std::vector<glm::vec3>* ver_vwc_arr, std::vector<glm::vec3>* color_vec_arr);
void watershed_3D(std::vector<glm::vec3>& normals, std::vector<unsigned int> vertexIndices, std::vector<unsigned int>& normalIndices, int* label, int label_size, std::vector<unsigned int>& ws_label, std::vector<unsigned int> faceIndices);

#endif
