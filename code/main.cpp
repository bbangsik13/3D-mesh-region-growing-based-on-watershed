//--------------------------------------[ inculde ]--------------------------------------//
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "model_loading.h"
#include "edge_indices.h"

#include <iostream>
#include <windows.h>    //delay 위해
#include <chrono>       //시간 측정 위해

//--------------------------------------[ callback함수 선언부 ]--------------------------------------//
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//--------------------------------------[ window 세팅 상수 ]--------------------------------------//
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
unsigned int select_render=0;   
float pitch = 0.0f;
float yaw = 0.0f;
float roll = 0.0f;

int main()
{

//--------------------------------------[ GLFW 초기화 및 설정 ]--------------------------------------//
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 

//--------------------------------------[ GLFW 윈도우 설정 ]--------------------------------------//
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



//--------------------------------------[ GLAD 로딩 ]--------------------------------------//
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
//--------------------------------------[ Depth_test 설정 ]--------------------------------------//
    glEnable(GL_DEPTH_TEST);

//--------------------------------------[ Shader 설정 ]--------------------------------------//
    Shader ourShader("vertex.vs", "fragment.fs");

//--------------------------------------[ Vertex정보 ]--------------------------------------//
    //- - - - -{ 변수 선언 }- - - - -//
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> vertexIndices; 
    std::vector<unsigned int> normalIndices;
    std::vector<unsigned int> uvIndices;

    std::vector<glm::vec3> out_vertices;
    std::vector<glm::vec3> face_rgb;

    const int label_size = 7;
    int label[label_size] = { 0 };
    std::vector<glm::vec3> ver_vec_arr[label_size];
    std::vector<glm::vec3> color_vec_arr[label_size];
    
    
    label[0] = 139577;

    label[1] = 217578;

    label[2] = 8737;
    label[3] = 38797;

    label[4] = 210757;
    label[5] = 20343;
    label[6] = 28648;
    

    //- - - - -{ 처리 }- - - - -//
    bool res = loadOBJ("lowerjaw.obj", vertices, normals, vertexIndices, normalIndices);                          //모델 불러오기

    std::vector<std::vector<edge>> edges(vertices.size());//e1->f1,f2을 나타내는 edge의 2차원 벡터(edge을 포함하는 face의 인덱스)

    std::vector<unsigned int> faceIndices(normalIndices.size());//f1->fx,fy,fz을 나타내는 face의 1차원 벡터(인접면의 인덱스)

    std::chrono::system_clock::time_point start1 = std::chrono::system_clock::now();
    adjacentFace(vertexIndices,edges, faceIndices);                                                           //인접 face찾기
    std::chrono::duration<double> sec2 = std::chrono::system_clock::now() - start1;
    std::cout << "find adjacent face time cost (Sec) : " << sec2.count() << std::endl;

    std::vector<unsigned int> ws_label(vertexIndices.size() / 3);
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now(); 
    watershed_3D(normals, vertexIndices, normalIndices, label, label_size, ws_label,faceIndices);             //watershed거쳐서 구역에 따라 label저장 후
    std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
    std::cout << "Watershed time cost (Sec) : " << sec.count() << std::endl;


    std::cout << "vertex : " << vertices.size() << " face : " << vertexIndices.size()/3 << std::endl;
    rendering_vertex(vertices, vertexIndices, out_vertices, ws_label, face_rgb, ver_vec_arr, color_vec_arr);  //label별로 vertex 정보 저장
    std::chrono::duration<double> sec5 = std::chrono::system_clock::now() - start1;
    std::cout << "영역분할(adjacentFace + watershed + rendering)  (sec) : " << sec5.count() << std::endl;


//--------------------------------------[ Buffer 설정 ]--------------------------------------//

    GLuint VAO[label_size], VBO[label_size], ColorVBO[label_size];

    glGenVertexArrays(label_size, VAO);
    glGenBuffers(label_size, VBO);
    glGenBuffers(label_size, ColorVBO);

    //구역별로 나누어서 전달
    for (int i = 0; i < label_size; i++)
    {
        glBindVertexArray(VAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, ver_vec_arr[i].size() * sizeof(glm::vec3), &ver_vec_arr[i][0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(ColorVBO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, ColorVBO[i]);
        glBufferData(GL_ARRAY_BUFFER, color_vec_arr[i].size() * sizeof(glm::vec3), &color_vec_arr[i][0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(1);
    }
//--------------------------------------[ rander loop ]--------------------------------------//
 
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 위치, 회전, 스케일 조정
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::rotate(transform, roll, glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(0.01f, 0.01f, 0.01f));

        // get matrix's uniform location and set matrix
        ourShader.use();
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // render
        if (!(select_render&1))
        {
            glBindVertexArray(VAO[0]);
            glDrawArrays(GL_TRIANGLES, 0, ver_vec_arr[0].size());
        }
        if (!(select_render & 2))
        {
            glBindVertexArray(VAO[1]);
            glDrawArrays(GL_TRIANGLES, 0, ver_vec_arr[1].size());
        }
        if (!(select_render & 4))
        {
            glBindVertexArray(VAO[2]);
            glDrawArrays(GL_TRIANGLES, 0, ver_vec_arr[2].size());
        }
        if (!(select_render & 8))
        {
            glBindVertexArray(VAO[3]);
            glDrawArrays(GL_TRIANGLES, 0, ver_vec_arr[3].size());
        }
        if (!(select_render & 16))
        {
            glBindVertexArray(VAO[4]);
            glDrawArrays(GL_TRIANGLES, 0, ver_vec_arr[4].size());
        }
        if (!(select_render & 32))
        {
            glBindVertexArray(VAO[5]);
            glDrawArrays(GL_TRIANGLES, 0, ver_vec_arr[5].size());
        }
        if (!(select_render & 64))
        {
            glBindVertexArray(VAO[6]);
            glDrawArrays(GL_TRIANGLES, 0, ver_vec_arr[6].size());
        }


        //버퍼 사용
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(label_size, VAO);
    glDeleteBuffers(label_size, VBO);
    glDeleteBuffers(label_size, ColorVBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    return 0;
}


//key입력
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        roll += 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        roll -= 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        pitch += 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        pitch -= 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        yaw += 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        yaw -= 0.01f;
    }



    else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        select_render ^= 1;
        Sleep(150);
    }
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        select_render ^= 2;
        Sleep(150);
    }
    else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        select_render ^= 4;
        Sleep(150);
    }
    else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        select_render ^= 8;
        Sleep(150);
    }
    else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        select_render ^= 16;
        Sleep(150);
    }
    else if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        select_render ^= 32;
        Sleep(150);
    }
    else if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        select_render ^= 64;
        Sleep(150);
    }
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height){ glViewport(0, 0, width, height); }