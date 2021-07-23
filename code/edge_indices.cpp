#include "edge_indices.h"

edge::edge() { v1 = 0, v2 = 0, f1 = -1, f2 = -1; };


bool operator==(edge t, edge u) {//std::find ���� ������ ������
	return (t.v1 == u.v1) && (t.v2 == u.v2);
}


void adjacentFace(std::vector<unsigned int>& vertexIndices, std::vector<std::vector<edge>>& edges, std::vector<unsigned int>& faceIndices) {
	unsigned int vertexIndex[3];
	std::vector<edge>::iterator it;//edges�� v1�� �ش��ϴ� 1���� �迭�� iterator
	int index;//iterator�� �� �� �ִ� v1������ index
	edge temp_edge1;//v1 v2
	edge temp_edge2;//v1 v3
	edge temp_edge3;//v2 v3

	//-------------------------------------edges����--------------------------------------------------------------------------------------//

	for (int i = 0; i < vertexIndices.size()/3; i++) {//face������ŭ �ݺ�		
		vertexIndex[0] = vertexIndices[3 * i];//i��° face�� v1
		vertexIndex[1] = vertexIndices[3 * i + 1];//i��° face�� v2
		vertexIndex[2] = vertexIndices[3 * i + 2];//i��° face�� v3
		std::sort(vertexIndex, vertexIndex + 3);//v1<v2<v3�� �ǵ���=>�ߺ� ����� ���� ���� �� �ִ�
	
		temp_edge1.v1 = vertexIndex[0];//edge��ü�� vertex�� ����
		temp_edge1.v2 = vertexIndex[1];
		it = std::find(edges[vertexIndex[0]].begin(), edges[vertexIndex[0]].end(), temp_edge1);//v1 v2�� ���� ���� edge��ü�� edges[v1]���Ϳ� �ִ���

		if (it != edges[vertexIndex[0]].end()) {//�̹� edge�� ����
			index = std::distance(edges[vertexIndex[0]].begin(), it);//edges[v1] vector�� index
			edges[vertexIndex[0]][index].f2=i;//edge�� ������ 2��° ���� index
		}
		else {//���ο� edge
			temp_edge1.f1=i;//face index �߰�
			edges[vertexIndex[0]].push_back(temp_edge1);//edge[v1]�� push
		}
		//v1 v3�� ���� �ݺ�
		temp_edge2.v1 = vertexIndex[0];
		temp_edge2.v2 = vertexIndex[2];
		it = std::find(edges[vertexIndex[0]].begin(), edges[vertexIndex[0]].end(), temp_edge2);//v1 v2

		if (it != edges[vertexIndex[0]].end()) {//�̹� edge�� ����
			index = std::distance(edges[vertexIndex[0]].begin(), it);
			edges[vertexIndex[0]][index].f2 = i;
		}
		else {
			temp_edge2.f1 = i;//face index �߰�
			edges[vertexIndex[0]].push_back(temp_edge2);
		}
		//v2 v3�� ���� �ݺ�
		temp_edge3.v1 = vertexIndex[1];
		temp_edge3.v2 = vertexIndex[2];
		it = std::find(edges[vertexIndex[1]].begin(), edges[vertexIndex[1]].end(), temp_edge3);//v1 v2

		if (it != edges[vertexIndex[1]].end()) {//�̹� edge�� ����
			index = std::distance(edges[vertexIndex[1]].begin(), it);
			edges[vertexIndex[1]][index].f2 = i;
		}
		else {
			temp_edge3.f1 = i;//face index �߰�
			edges[vertexIndex[1]].push_back(temp_edge3);
		}
	}

	//------------------------------------------------adjacent face ã��-------------------------------------------------------------//
	//3���� edge�� ���ؼ� ���� 1���� �� 3���� ������
	for (int i = 0; i < vertexIndices.size() / 3; i++) {//face ������ŭ �ݺ�
		vertexIndex[0] = vertexIndices[3 * i];//i��° face�� v1
		vertexIndex[1] = vertexIndices[3 * i + 1];//i��° face�� v2
		vertexIndex[2] = vertexIndices[3 * i + 2];//i��° face�� v3
		std::sort(vertexIndex, vertexIndex + 3);//v1<v2<v3 =>edges�� ����

		temp_edge1.v1 = vertexIndex[0];
		temp_edge1.v2 = vertexIndex[1];//v1 v2�� �ش��ϴ� edge

		it = std::find(edges[vertexIndex[0]].begin(), edges[vertexIndex[0]].end(), temp_edge1);//v1 v2�� ������ edge�˻�	
		index = std::distance(edges[vertexIndex[0]].begin(), it);
		if (edges[vertexIndex[0]][index].f1 == i) faceIndices[3 * i] = edges[vertexIndex[0]][index].f2;//���� face���� �ٸ� face���� �������� index
		else faceIndices[3 * i] = edges[vertexIndex[0]][index].f1;
		

		temp_edge2.v1 = vertexIndex[0];
		temp_edge2.v2 = vertexIndex[2];
		it = std::find(edges[vertexIndex[0]].begin(), edges[vertexIndex[0]].end(), temp_edge2);//v1 v3�� ���ؼ� �ݺ�	
		index = std::distance(edges[vertexIndex[0]].begin(), it);
		if (edges[vertexIndex[0]][index].f1 == i) faceIndices[3 * i + 1] = edges[vertexIndex[0]][index].f2;
		else faceIndices[3 * i + 1] = edges[vertexIndex[0]][index].f1;

		temp_edge3.v1 = vertexIndex[1];
		temp_edge3.v2 = vertexIndex[2];
		it = std::find(edges[vertexIndex[1]].begin(), edges[vertexIndex[1]].end(), temp_edge3);//v2 v3�� ���ؼ� �ݺ�
		index = std::distance(edges[vertexIndex[1]].begin(), it);
		if (edges[vertexIndex[1]][index].f1 == i) faceIndices[3 * i + 2] = edges[vertexIndex[1]][index].f2;
		else faceIndices[3 * i + 2] = edges[vertexIndex[1]][index].f1;
	}
}
