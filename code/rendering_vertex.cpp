
#include <vector>
#include <stdio.h>

#include <glm/glm.hpp>

bool rendering_vertex(	std::vector<glm::vec3>& vertices,
						std::vector<unsigned int>& vertexIndices,
						std::vector<glm::vec3>& out_vertices,
						std::vector<unsigned int> ws_label,
						std::vector<glm::vec3>& face_rgb,
						std::vector<glm::vec3>* ver_vec_arr,
						std::vector<glm::vec3>* color_vec_arr
					 )
{

	glm::vec3 vertex;
	glm::vec3 color;

    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];

        // Get the attributes thanks to the index
        vertex = vertices[vertexIndex - 1];

        switch (ws_label[i / 3])
        {
        case 1:
            //face_rgb.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
            ver_vec_arr[0].push_back(vertex);
            color_vec_arr[0].push_back(glm::vec3(0.909f, 0.564f, 0.612f));//ÇÎ
            break;
        case 2:
            //face_rgb.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
            ver_vec_arr[1].push_back(vertex);
            color_vec_arr[1].push_back(glm::vec3(1.0f, 0.0f, 0.0f));//»¡
            break;
        case 3:
            //face_rgb.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
            ver_vec_arr[2].push_back(vertex);
            color_vec_arr[2].push_back(glm::vec3(0.0f, 1.0f, 0.0f));//ÃÊ
            break;
        case 4:
            //face_rgb.push_back(glm::vec3(0.0f, 1.0f, 1.0f));
            ver_vec_arr[3].push_back(vertex);
            color_vec_arr[3].push_back(glm::vec3(0.0f, 0.0f, 1.0f));//ÆÄ
            break;
        case 5:
            //face_rgb.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
            ver_vec_arr[4].push_back(vertex);
            color_vec_arr[4].push_back(glm::vec3(1.0f, 0.0f, 0.0f));//R
            break;
        case 6:
            //face_rgb.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
            ver_vec_arr[5].push_back(vertex);
            color_vec_arr[5].push_back(glm::vec3(0.0f, 1.0f, 0.0f));//G
            break;
        case 7:
            //face_rgb.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
            ver_vec_arr[6].push_back(vertex);
            color_vec_arr[6].push_back(glm::vec3(0.0f, 0.0f, 1.0f));//B
            break;

        default:
            printf("\nerror!!! out of case!\n");
            //face_rgb.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            break;
            //out_vertices.push_back(vertex);
        }
    }
	return 1;
}