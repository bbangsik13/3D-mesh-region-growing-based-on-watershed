#include "edge_indices.h"

edge::edge() { v1 = 0, v2 = 0, f1 = -1, f2 = -1; };


bool operator==(edge t, edge u) {//std::find 위해 연산자 재정의
	return (t.v1 == u.v1) && (t.v2 == u.v2);
}


void adjacentFace(std::vector<unsigned int>& vertexIndices, std::vector<std::vector<edge>>& edges, std::vector<unsigned int>& faceIndices) {
	unsigned int vertexIndex[3];
	std::vector<edge>::iterator it;//edges의 v1에 해당하는 1차원 배열의 iterator
	int index;//iterator로 알 수 있는 v1벡터의 index
	edge temp_edge1;//v1 v2
	edge temp_edge2;//v1 v3
	edge temp_edge3;//v2 v3

	//-------------------------------------edges정의--------------------------------------------------------------------------------------//

	for (int i = 0; i < vertexIndices.size()/3; i++) {//face개수만큼 반복		
		vertexIndex[0] = vertexIndices[3 * i];//i번째 face의 v1
		vertexIndex[1] = vertexIndices[3 * i + 1];//i번째 face의 v2
		vertexIndex[2] = vertexIndices[3 * i + 2];//i번째 face의 v3
		std::sort(vertexIndex, vertexIndex + 3);//v1<v2<v3이 되도록=>중복 경우의 수를 줄일 수 있다
	
		temp_edge1.v1 = vertexIndex[0];//edge객체에 vertex값 저장
		temp_edge1.v2 = vertexIndex[1];
		it = std::find(edges[vertexIndex[0]].begin(), edges[vertexIndex[0]].end(), temp_edge1);//v1 v2의 값을 가진 edge객체가 edges[v1]벡터에 있는지

		if (it != edges[vertexIndex[0]].end()) {//이미 edge가 정의
			index = std::distance(edges[vertexIndex[0]].begin(), it);//edges[v1] vector의 index
			edges[vertexIndex[0]][index].f2=i;//edge를 가지는 2번째 면의 index
		}
		else {//새로운 edge
			temp_edge1.f1=i;//face index 추가
			edges[vertexIndex[0]].push_back(temp_edge1);//edge[v1]에 push
		}
		//v1 v3에 대해 반복
		temp_edge2.v1 = vertexIndex[0];
		temp_edge2.v2 = vertexIndex[2];
		it = std::find(edges[vertexIndex[0]].begin(), edges[vertexIndex[0]].end(), temp_edge2);//v1 v2

		if (it != edges[vertexIndex[0]].end()) {//이미 edge가 정의
			index = std::distance(edges[vertexIndex[0]].begin(), it);
			edges[vertexIndex[0]][index].f2 = i;
		}
		else {
			temp_edge2.f1 = i;//face index 추가
			edges[vertexIndex[0]].push_back(temp_edge2);
		}
		//v2 v3에 대해 반복
		temp_edge3.v1 = vertexIndex[1];
		temp_edge3.v2 = vertexIndex[2];
		it = std::find(edges[vertexIndex[1]].begin(), edges[vertexIndex[1]].end(), temp_edge3);//v1 v2

		if (it != edges[vertexIndex[1]].end()) {//이미 edge가 정의
			index = std::distance(edges[vertexIndex[1]].begin(), it);
			edges[vertexIndex[1]][index].f2 = i;
		}
		else {
			temp_edge3.f1 = i;//face index 추가
			edges[vertexIndex[1]].push_back(temp_edge3);
		}
	}

	//------------------------------------------------adjacent face 찾기-------------------------------------------------------------//
	//3개의 edge에 대해서 각각 1개씩 총 3개의 인접면
	for (int i = 0; i < vertexIndices.size() / 3; i++) {//face 개수만큼 반복
		vertexIndex[0] = vertexIndices[3 * i];//i번째 face의 v1
		vertexIndex[1] = vertexIndices[3 * i + 1];//i번째 face의 v2
		vertexIndex[2] = vertexIndices[3 * i + 2];//i번째 face의 v3
		std::sort(vertexIndex, vertexIndex + 3);//v1<v2<v3 =>edges에 접근

		temp_edge1.v1 = vertexIndex[0];
		temp_edge1.v2 = vertexIndex[1];//v1 v2에 해당하는 edge

		it = std::find(edges[vertexIndex[0]].begin(), edges[vertexIndex[0]].end(), temp_edge1);//v1 v2를 가지는 edge검색	
		index = std::distance(edges[vertexIndex[0]].begin(), it);
		if (edges[vertexIndex[0]][index].f1 == i) faceIndices[3 * i] = edges[vertexIndex[0]][index].f2;//현재 face값과 다른 face값이 인접면의 index
		else faceIndices[3 * i] = edges[vertexIndex[0]][index].f1;
		

		temp_edge2.v1 = vertexIndex[0];
		temp_edge2.v2 = vertexIndex[2];
		it = std::find(edges[vertexIndex[0]].begin(), edges[vertexIndex[0]].end(), temp_edge2);//v1 v3에 대해서 반복	
		index = std::distance(edges[vertexIndex[0]].begin(), it);
		if (edges[vertexIndex[0]][index].f1 == i) faceIndices[3 * i + 1] = edges[vertexIndex[0]][index].f2;
		else faceIndices[3 * i + 1] = edges[vertexIndex[0]][index].f1;

		temp_edge3.v1 = vertexIndex[1];
		temp_edge3.v2 = vertexIndex[2];
		it = std::find(edges[vertexIndex[1]].begin(), edges[vertexIndex[1]].end(), temp_edge3);//v2 v3에 대해서 반복
		index = std::distance(edges[vertexIndex[1]].begin(), it);
		if (edges[vertexIndex[1]][index].f1 == i) faceIndices[3 * i + 2] = edges[vertexIndex[1]][index].f2;
		else faceIndices[3 * i + 2] = edges[vertexIndex[1]][index].f1;
	}
}
