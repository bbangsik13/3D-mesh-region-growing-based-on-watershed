# 3D-mesh-segmentation-using-watershed

1.edge_indices.cpp(edge_indices.h)는 mesh face의 인접한 3개의 face를 구하는 함수이다.
<br>
2.model_loading.cpp(model_loading.h)는 opengl tutorial 7을 변형하여 obj파일의 값을 vector로 저장하는 함수이다.
<br>
3.rendering_vertex.cpp는 labeling된 결과를 바탕으로 vertex의 color값을 설정해주는 함수이다.
<br>
4.watershed_3D.cpp는 seed(marker)를 받아와서 나머지 점들을 labeling(watershed, region growing)하는 함수이다.
<br>
5.main.cpp에서는 GL설정 및 라벨설정(seed☆)를 하고 key입력을 받아 원하는 부분만 렌더링하거나 회전을 시킬 수 있다.

자세한 내용 및 원리는 ppt를 참고
