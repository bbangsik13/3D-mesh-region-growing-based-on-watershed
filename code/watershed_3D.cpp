#include "model_loading.h"

class face {
public:
	unsigned int v1;		//���� face ã�� �� �ʿ�
	unsigned int v2;
	unsigned int v3;
	glm::vec3 nf;			//���� face���� angle ���� �� �ʿ�
	float angle;			//�켱������ ���
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



	//-------------------------------[ �ʱ� �󺧸� ]-------------------------------//
	unsigned int index = 0;
	for (int i = 0; i < label_size; i++) {
		index = 3 * (label[i] - 1); //index�� 0���� �����ϸ� stride�� 3

		//ws_label[(label[i] - 1)] = i+1;
		nf = glm::normalize(normals[normalIndices[index] - 1] + normals[normalIndices[index + 1] - 1] + normals[normalIndices[index + 2] - 1]); //�� normal�� ���к��� ���� �� normalization 
		temp_face.set_face(vertexIndices[index], vertexIndices[index + 1], vertexIndices[index + 2], nf, (label[i] - 1), -1.0f,i+1);
		pq.push(temp_face);

	}


	//-------------------------------[ watershad Ȯ�� ]-------------------------------//
	//	1. queue���� face�ϳ� �̰�
	//	2. ���� face ã�� ����
	//	3. �������ͳ����� �� ��� �� ����
	//	4. 1 ���� �ݺ�
	//--------------------------------------------------------------------------------//
	face pop_face;
	float angle;
	unsigned int faceIndex[3];

	while (!pq.empty()) {
		pop_face = pq.top();		//1�� �̰�
		pq.pop();
		
		if (ws_label[pop_face.findex] == 0) ws_label[pop_face.findex] = pop_face.marker;

		faceIndex[0] = faceIndices[3 * pop_face.findex];
		faceIndex[1] = faceIndices[3 * pop_face.findex + 1];
		faceIndex[2] = faceIndices[3 * pop_face.findex + 2];
		
			//- - - - - -{ ���� face ã�� }- - - - - -//
		for (int i = 0; i < 3; i++) {

			if (ws_label[faceIndex[i]] == 0) {//labeling���� ���� face
				index = 3 * faceIndex[i];
				nf = glm::normalize(normals[normalIndices[index] - 1] + normals[normalIndices[index + 1] - 1] + normals[normalIndices[index + 2] - 1]);
				//���� normal�� ���к��� ���� �� normalization, face normal�� ���� 

				//ws_label[faceIndex[i]] = ws_label[pop_face.findex];
				//			������ face�� face index			��pop_face�� face index

				angle = glm::acos(glm::dot(nf, pop_face.nf));

				temp_face.set_face(vertexIndices[index], vertexIndices[index + 1], vertexIndices[index + 2], nf, faceIndex[i], angle, pop_face.marker);
				pq.push(temp_face);
			}
		}
			
	}
}
