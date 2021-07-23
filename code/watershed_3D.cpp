#include "model_loading.h"

class face {
public:
	unsigned int v1;		//인접 face 찾을 때 필요
	unsigned int v2;
	unsigned int v3;
	glm::vec3 nf;			//인접 face와의 angle 구할 때 필요
	float angle;			//우선순위로 사용
	int findex;
	int marker;
	face() { v1 = 0, v2 = 0, v3 = 0, nf = { 0.0f, 0.0f, 0.0f }, angle = 0.0f, findex = 0,  marker = 0; }

	void set_face(unsigned int v1, unsigned int v2, unsigned int v3, glm::vec3 nf, int findex, float angle, int marker)
	{
		this->v1 = v1;
		this->v2 = v2;
		this->v3 = v3;
		this->nf = nf;
		this->findex = findex;
		this->angle = angle;
		this->marker = marker;
	}
};
bool operator<(face t, face u) {
	return t.angle > u.angle;
}


void watershed_3D(
	std::vector<glm::vec3>& normals,
	std::vector<unsigned int>	vertexIndices,
	std::vector<unsigned int>& normalIndices,
	int* label,
	int label_size,
	std::vector<unsigned int>& ws_label,
	std::vector<unsigned int> faceIndices)
{
	std::priority_queue<face> pq;


	face temp_face;
	glm::vec3 nf;



	//-------------------------------[ 초기 라벨링 ]-------------------------------//
	unsigned int index = 0;
	for (int i = 0; i < label_size; i++) {
		index = 3 * (label[i] - 1); //index는 0부터 시작하며 stride가 3

		//ws_label[(label[i] - 1)] = i+1;
		nf = glm::normalize(normals[normalIndices[index] - 1] + normals[normalIndices[index + 1] - 1] + normals[normalIndices[index + 2] - 1]); //점 normal을 성분별로 더한 후 normalization 
		temp_face.set_face(vertexIndices[index], vertexIndices[index + 1], vertexIndices[index + 2], nf, (label[i] - 1), -1.0f,i+1);
		pq.push(temp_face);

	}


	//-------------------------------[ watershad 확장 ]-------------------------------//
	//	1. queue에서 face하나 뽑고
	//	2. 인접 face 찾은 다음
	//	3. 법선벡터끼리의 각 계산 후 넣음
	//	4. 1 부터 반복
	//--------------------------------------------------------------------------------//
	face pop_face;
	float angle;
	unsigned int faceIndex[3];

	while (!pq.empty()) {
		pop_face = pq.top();		//1개 뽑고
		pq.pop();
		
		if (ws_label[pop_face.findex] == 0) ws_label[pop_face.findex] = pop_face.marker;

		faceIndex[0] = faceIndices[3 * pop_face.findex];
		faceIndex[1] = faceIndices[3 * pop_face.findex + 1];
		faceIndex[2] = faceIndices[3 * pop_face.findex + 2];
		
			//- - - - - -{ 인접 face 찾기 }- - - - - -//
		for (int i = 0; i < 3; i++) {

			if (ws_label[faceIndex[i]] == 0) {//labeling되지 않은 face
				index = 3 * faceIndex[i];
				nf = glm::normalize(normals[normalIndices[index] - 1] + normals[normalIndices[index + 1] - 1] + normals[normalIndices[index + 2] - 1]);
				//ㄴ점 normal을 성분별로 더한 후 normalization, face normal을 구함 

				//ws_label[faceIndex[i]] = ws_label[pop_face.findex];
				//			ㄴ인접 face의 face index			ㄴpop_face의 face index

				angle = glm::acos(glm::dot(nf, pop_face.nf));

				temp_face.set_face(vertexIndices[index], vertexIndices[index + 1], vertexIndices[index + 2], nf, faceIndex[i], angle, pop_face.marker);
				pq.push(temp_face);
			}
		}
			
	}
}
