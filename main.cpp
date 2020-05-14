////////////////////////////////////////////////////////////////////////////////
/*
 * Transform in Practice
 *  This tutorial assumes that you already understand vector math.
 *  Here we'll see how we can use the knowledge of vectors and matrices.
 *  In this tutorial we'd rather abstract from all the tiny mathematical
 *  details and simply use pre-made mathematics libraries. We will be using
 *  a pre-made mathematics library called GLM.
 *
 *  GLM stands for OpenGL Mathematics and is a header-only library, which
 *  means that we only have to include the proper header files and we're
 *  done; no linking and compiling necessary. GLM can be downloaded from
 *  their website
 *  https://glm.g-truc.net/0.9.8/index.html
 *  Copy the root directory of the header files into your includes folder and
 *  begin.
 *
 *  Most of GLM's functionality that we need can be found in 3 headers files
 *  that we'll include as follows:
 */
////////////////////////////////////////////////////////////////////////////////

#if 0
  #include <glm/glm.hpp>
  #include <glm/gtc/matrix_transform.hpp>
  #include <glm/gtc/type_ptr.hpp>
#endif

////////////////////////////////////////////////////////////////////////////////
/*
 * Let's see if we can put our transformation knowledge to good use by
 * translating a vector of (1, 0, 0) by (1, 1, 0) note that we define it as a
 * glm::vec4 with its homogenous coordinate set to 1.0:
 *
 * We first define a vector named vec using GLM's built-in vector class.
 * Next we define a mat4 and explicitly initialize it to the identity matrix
 * by initializing the matrix's diagonals to 1.0.
 *    NOTE: if we do not initialize the matrix to the identity matrix the
 *    matrix would be null matrix and all subsequent matrix operations would
 *    end up a null matrix as well.
 *
 * The next step is to create a transformation matrix by passing our identity
 * matrix to the glm::translate translate function, along with a translation
 * vector. This is done by multiplying the identity matrix with the
 * translation matrix.
 *
 * Then we multiply our vector by the transformation matrix and output the
 * result. If we still remember how matrix translation works then the
 * resulting vector should be ( 1 + 1, 0 + 1, 0 + 0 ) which is ( 2, 1, 0).
 * This snippet of code outputs 210 so the translation matrix did its job
 */
////////////////////////////////////////////////////////////////////////////////

#if 0
  glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
  glm::mat4 trans = glm::mat4(1.0f);
  trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
  vec = trans * vec;
#endif

////////////////////////////////////////////////////////////////////////////////
/*
 * Let's scale and rotate the container object from the previous project
 * (Texture).
 *
 * First we scale the container by 0.5 on each axis and then rotate the
 * container 90 degrees around the Z-axis. GLM expects its angles in radians
 * so we convert the degrees to radians using glm::radians. Note that the
 * textured rectangle is on the XY plane so we want to rotate around the
 * Z-axis. Keep in mind that the axis that we rotate around should be a unit
 * vector, so be sure to normalize the vector first if you're not rotating
 * around the X,Y, or Z axis. Because we pass the matrix to each of GLM's
 * functions, GLM automatically multiplies the matrices together, resulting
 * in a transformation matrix that combines all the transformations.
 */
////////////////////////////////////////////////////////////////////////////////
#if 0
  glm::mat4 trans = glm::mat4(1.0f);
  trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
  trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
#endif

////////////////////////////////////////////////////////////////////////////////
/*
 * Time to transport to the shaders. GLSL also has a mat4 type. So we'll
 * adapt the vertex shader to accept a mat4 uniform variable and multiply the
 * position vector by the matrix uniform
 *
 *  GLSL also has mat2 and mat3 types that allow for swizzling-like
 *  operations just like vectors. All the aforementioned math operarations
 *  (like scalar-matrix types. Wherever special matrix operations are used
 *  we'll be sure to explain what's happening.
 *
 *  We added the uniform and multiplied the position vector with the
 *  transformation matrix before passing it to gl_position. Our container
 *  should now be twice as small and rotated 90 degrees (tilted to the left).
 *  We still need to pass the transformation matrix to the shader though.
 *
 */
////////////////////////////////////////////////////////////////////////////////

#if 0
  #version 330 core
  layout (location = 0) in vec3 aPos;
  layout (location = 1) in vec2 aTexCoord;

  out vec2 TexCoord;

  uniform mat4 transform;

  void main()
  {
      gl_Position = transform * vec4(aPos, 1.0f);
      TexCoord = vec2(aTexCoord.x, aTexCoord.y);
  }
#endif

////////////////////////////////////////////////////////////////////////////////
/*
 * Passing transformation matrix to the shader
 *  We first query the location of the uniform variable and then sed the
 *  matrix data to the shaders using glUniform with Matrix4fv as its postfix.
 *  The first argument should be familiar by now which is the uniform's
 *  location. The second argument tells OpenGL how many matrices we'd like to
 *  send, which is 1. The third argument asks us if we want to transpose our
 *  matrix that is to swap the columns and rows. OpenGL developers often use
 *  an internal matrix layout called column-major ordering which is the
 *  default matrix layout in GLM so there is no need to transpose the
 *  matrices; we can keep it at GL_FALSE. The last parameter is the actual
 *  matrix data, but GLM stores their matrices' data in a way that doesn't
 *  always match OpenGL's expectations so we first convert the data with
 *  GLM's built-in function value_ptr.
 *
 *  We created a transformation matrix, declared a uniform in the vertex
 *  shader and sent the matrix to the shaders where we transform our vertex
 *  coordinates.
 */
////////////////////////////////////////////////////////////////////////////////

#if 0
  unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
#endif

////////////////////////////////////////////////////////////////////////////////
/*
 * Rotating and repositioning the container
 *  To rotate the container over time we have to update the transformation
 *  matrix in the render loop because it needs to update each frame. We use
 *  GLFW's time function to get an angle over time:
 *
 *  Keep in mind that in the previous case we could declare the
 *  transformation matrix anywhere, but now we have to create it every
 *  iteration to continuously update the rotation. This means we have to
 *  re-create the transformation matrix in each iteration of the render loop.
 *  Usually when rendering scenes we have several transformation matrices
 *  that are re-created with new values each frame.
 *
 *  Here we first rotate the container around the origin (0,0,0) and once
 *  it's rotated, we translate its rotated version to the bottom-right corner
 *  of the screen. Remember that the actual transformations first apply a
 *  rotation and then a translation. Understanding all these combinations of
 *  transformations and how they apply to objects is difficult to understand.
 *  Try and experiment with transformations like these and you'll quickly get
 *  a grasp of it.
 */
////////////////////////////////////////////////////////////////////////////////
#if 0
  glm::mat4 trans = glm::mat4(1.0f);
  trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
  trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // build and compile our shader zprogram
  // ------------------------------------
  Shader ourShader("4.1.texturevs.txt", "4.1.texturefs.txt");

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
          // positions          // colors           // texture coords
          0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
          0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
          -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
          -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
  };
  unsigned int indices[] = {
          0, 1, 3, // first triangle
          1, 2, 3  // second triangle
  };
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);


  // load and create a texture
  // -------------------------
  // bind textures one corresponding texture units
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
  unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);


  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // bind Texture
    glBindTexture(GL_TEXTURE_2D, texture);

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::translate(trans, glm::vec3(1.0f, -0.5f, 0.0f));
    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

    // render container
    ourShader.use();
    int uniformTime = glGetUniformLocation(ourShader.getProgram(), "time");
    unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    glUniform1f(uniformTime, glfwGetTime());


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}
