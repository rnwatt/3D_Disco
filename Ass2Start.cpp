/*
Rael Watt 2022 Assignment 2
Disco with loaded dalek and jellyfish object
Cubemap
Textures
*/


#ifdef _DEBUG
#pragma comment(lib, "glfw3D.lib")
#else
#pragma comment(lib, "glfw3.lib")
#endif
#pragma comment(lib, "opengl32.lib")
// Sources: https://github.com/VictorGordan/opengl-tutorials/tree/main/YoutubeOpenGL%2019%20-%20Cubemaps%20%26%20Skyboxes
/* Include the header to the GLFW wrapper class which
   also includes the OpenGL extension initialisation*/
// Disco floor texture copied from https://www.youtube.com/watch?v=blsUsykXqAI
#include "wrapper_glfw.h"
#include <iostream>
   // We'll use the STD stack class to make our stack or matrices
#include <stack>
/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

/* Include the stb image loader */
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "tiny_loader.h"

#include "cube_tex.h"
#include "sphere_tex.h"
#include "quad_tex.h"
#include "cylinder.h"


using namespace glm;
using namespace std;

/* Define buffer object indices */
GLuint quad_vbo, quad_normals, quad_colours, quad_tex_coords;
Cube aCube;
Sphere aSphere(true);
Quad aQuad(true);
Cylinder aCylinder;
GLuint skyboxVAO, skyboxVBO;
/* Define textureID*/
GLuint texID1, texID2;
GLfloat model_scale;

GLuint program;		/* Identifier for the shader prgoram */
GLuint vao;			/* Vertex array (Containor) object. This is the index of the VAO that will be the container for
                       our buffer objects */
GLuint cubeVAO, cubeVBO;

GLuint colourmode;	/* Index of a uniform to switch the colour mode in the vertex shader
                       I've included this to show you how to pass in an unsigned integer into
                       your vertex shader. */
GLuint emitmode;
GLuint attenuationmode;

                       /* Position and view globals */
GLfloat angle_x, angle_inc_x, x, scaler, z, y;
GLfloat angle_y, angle_inc_y, angle_z, angle_inc_z;
GLfloat cameraLX, cameraLY, cameraLZ;
GLfloat cameraPX, cameraPY, cameraPZ;
GLfloat rotation;
GLfloat rotAnim;

GLfloat light_x, light_y, light_z;

GLuint drawmode;			// Defines drawing mode of sphere as points, lines or filled polygons
GLuint numlats, numlongs;

TinyObjLoader dalek;			// This is an instance of our basic object loaded
TinyObjLoader jellyfish;
/* Uniforms*/
GLuint modelID, viewID, projectionID;
GLuint cubemapModelID, cubemapViewID, cubemapProjectionID, 
lightposID, normalmatrixID, colourmodeID, emitmodeID, attenuationmodeID;

// Define texture ID value (identifier for a specific texture)
GLuint textureID1, textureID2, textureID3, textureID4, textureID5;

GLfloat aspect_ratio;		/* Aspect ratio of the window defined in the reshape callback*/

// Mipmap mode variable
GLuint mipmap_min_mode = 0;

GLuint cubemapShader;
GLuint skyboxShader;


unsigned int cubemapTexture;

    float cubemapVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    
    float texcoord[] =
    {
        // Face 0
        0, 1.f, 0, 0, 1.f, 0,
        1.f, 0, 1.f, 1.f, 0, 1.f,

        // Face 1
        1.0, 0.f, 0.f, 0.f, 1.f, 1.f,
        0.f, 0.f, 0.f, 1.f, 1.f, 1.f,

        // Face 2
        1.f, 0.f, 0.f, 0.f, 1.f, 1.f,
        0.f, 0.f, 0.f, 1.f, 1.f, 1.f,

        // Face 3
        1.f, 0.f, 0.f, 0.f, 1.f, 1.f,
        0.f, 0.f, 0.f, 1.f, 1.f, 1.f,

        // Face 4
        0.f, 0.f, 1.f, 0.f, 1.f, 1.f,
        1.f, 1.f, 0.f, 1.f, 0.f, 0.f,

        // Face 5
        0.f, 1.f, 1.f, 1.f, 1.f, 0.f,
        1.f, 0.f, 0.f, 0.f, 0.f, 1.f
    };
    
    bool load_texture(char* filename, GLuint& texID, bool bGenMipmaps);
    //Texture for sphere
    bool load_texture(const char* filename, GLuint& texID, bool bGenMipmaps)
    {
        // Obtain an unused texture identifier. 
        glGenTextures(1, &texID);

        // local image parameters
        int width, height, nrChannels;

        /* load an image file using stb_image */
        unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

        // check for an error during the load process
        if (data)
        {
            // Note: this is not a full check of all pixel format types, just the most common two!
            int pixel_format = 0;
            if (nrChannels == 3)
                pixel_format = GL_RGB;
            else
                pixel_format = GL_RGBA;

            // Bind the texture ID before the call to create the texture.
                // texID[i] will now be the identifier for this specific texture
            glBindTexture(GL_TEXTURE_2D, texID);

            // Create the texture, passing in the pointer to the loaded image pixel data
            glTexImage2D(GL_TEXTURE_2D, 0, pixel_format, width, height, 0, pixel_format, GL_UNSIGNED_BYTE, data);

            // Generate Mip Maps
            if (bGenMipmaps)
            {
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                // If mipmaps are not used then ensure that the min filter is defined
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            }
        }
        else
        {
            printf("stb_image  loading error: filename=%s", filename);
            return false;
        }
        stbi_image_free(data);
        return true;
    }

unsigned int loadCubemap(vector<string> faces)
{
    //texture id
    unsigned int texID;
    //generate the texture
    glGenTextures(1, &texID);
    //bind the texture to a cubemap target, make it current
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    int width, height, nrChannels;
    //for each face of the cube
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            //get the texture with the face and move on to the next face
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //clamp to edge to avoid seams as the texture coordinates may not hit an exact face, for coordinates S,T,R
    //if tex coords are outside the range of 0.0 to 1.0 then texels at the edge of the texture are used to create a value that is sent to the shader
    // - OpenGL Programming Guide, Page 296, John Kessenich, Graham Sellers, Dave Shreiner
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texID;
}



void init(GLWrapper* glw)
{
    x = 0.05f;
    y = 0;
    z = 0;
    angle_x = angle_y = angle_z = 0;
    angle_inc_x = angle_inc_y = angle_inc_z = 0;
    model_scale = 1.f;
    aspect_ratio = 1.3333f;
    colourmode = 0;
    numlats = 60;		// Number of latitudes in our sphere
    numlongs = 60;		// Number of longitudes in our sphere
    light_x = 0; light_y = -1; light_z = 0;
    cameraLX = cameraLY = cameraLZ = 0;
    cameraPX = 0;
    cameraPY = 1;
    cameraPZ = 2;
    rotation = 0.0f;
    rotAnim = 0.0f;
    // Source: https://github.com/VictorGordan/opengl-tutorials/blob/main/YoutubeOpenGL%2019%20-%20Cubemaps%20%26%20Skyboxes/Main.cpp
    // Line 143, EBO removed
    //generate index for vertex array object for cubemap
    
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubemapVertices), &cubemapVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    /* Load and create our dalek object*/
    dalek.load_obj("..\\..\\obj\\newdalekparadigm1.obj");
    // Set all the colour vertices to blue
    // Note that this is a quick hack, it would be better to load and process the mtl file
    dalek.overrideColour(vec4(0.0f, 0.5, 1.0, 1.f));

    jellyfish.load_obj("..\\..\\obj\\jellyfish.obj");
    jellyfish.overrideColour(vec4(0.5f, 0, 1.0, 1.f));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
   
    aSphere.makeSphere(numlats, numlongs);
    aQuad.makeQuad();
    aCylinder.makeCylinder();

    try
    {
        //load shaders
        //cubemap to create a cube
       cubemapShader = glw->LoadShader("cubemap.vert", "cubemap.frag");
        //skybox for the area around the camera
       // skyboxShader = glw->LoadShader("skybox.vert", "skybox.frag");
        //for lighting
       program = glw->LoadShader("lab3start.vert", "lab3start.frag");
        
    }
    catch (exception& e)
    {
        cout << "Caught exception: " << e.what() << endl;
        cin.ignore();
        exit(0);
    }
    
    /* Define uniforms to send to vertex shader */
    modelID = glGetUniformLocation(program, "model");
    colourmodeID = glGetUniformLocation(program, "colourmode");
    viewID = glGetUniformLocation(program, "view");
    projectionID = glGetUniformLocation(program, "projection");
    emitmodeID = glGetUniformLocation(program, "emitmode");
    attenuationmodeID = glGetUniformLocation(program, "attenuationmode");
    lightposID = glGetUniformLocation(program, "lightpos");
    normalmatrixID = glGetUniformLocation(program, "normalmatrix");
    
    
    cubemapModelID =  glGetUniformLocation(cubemapShader, "model");
    cubemapViewID = glGetUniformLocation(cubemapShader, "view");
    cubemapProjectionID = glGetUniformLocation(cubemapShader, "projection");
    

    const char* filename1 = "TileFloor1.png";
    const char* filename2 = "TileFloor2.png";
    const char* filename3 = "TileFloor3.png";
    const char* filename4 = "Brick_Wall_019_basecolor.jpg";
    const char* filename5 = "black-glitter-background.png";
    
    vector<string> faces =
    {
        //cubemap textures
            "red-background.jpg", //right
            "yellow-background.jpg", // left
            "green-background.jpg", //top
            "black-background.jpg", //bottom
            "pink-background.jpg", // front
            "blue-background.jpg" //back
    };
    //call function to load cubemap with textures
    cubemapTexture = loadCubemap(faces);
    
    // This will flip the image so that the texture coordinates defined in
    // the sphere, match the image orientation as loaded by stb_image
    stbi_set_flip_vertically_on_load(true);
    if (!load_texture(filename1, textureID1, true))
    {
        cout << "Fatal error loading texture: " << filename1 << endl;
        exit(0);
    }

    // Load second texture image
    if (!load_texture(filename2, textureID2, false))
    {
        cout << "Fatal error loading texture: " << filename2 << endl;
        exit(0);
    }

    // Load third texture image
    if (!load_texture(filename3, textureID3, true))
    {
        cout << "Fatal error loading texture: " << filename3 << endl;
        exit(0);
    }
    if (!load_texture(filename4, textureID4, true))
    {
        cout << "Fatal error loading texture: " << filename4 << endl;
        exit(0);
    }
   if (!load_texture(filename5, textureID5, true))
    {
        cout << "Fatal error loading texture: " << filename5 << endl;
        exit(0);
    }

    // This is the location of the texture object (TEXTURE0), i.e. tex1 will be the name
    // of the sampler in the fragment shader
    int loc = glGetUniformLocation(program, "tex1");
    if (loc >= 0) glUniform1i(loc, 0);

    // SET Texture MAG_FILTER to linear which will blur the texture if we
    // zoom too close in
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void display()
{

    /* Define the background colour */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    /* Clear the colour and frame buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);

    /* Enable depth test  */
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_MULTISAMPLE);

    /* Make the compiled shader program current */
    glUseProgram(program);

    // Define the normal matrix
    mat3 normalmatrix;
  
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    mat4 projection = perspective(radians(45.0f), aspect_ratio, 0.1f, 100.0f);
 
    // Camera matrix
    mat4 view = lookAt(
        vec3(cameraPX, cameraPY, cameraPZ), // Camera is at (0,0,4), in World Space, use HJUK keys to move camera position
        vec3(cameraLX, cameraLY, cameraLZ), // and looks at the origin, added camera controls by using arrow keys to change looking direction
        vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Define the light position and transform by the view matrix
    vec4 lightpos = view * vec4(light_x, light_y, light_z, 1.0);
    // Send our projection and view uniforms to the currently bound shader
    glUniform1ui(colourmodeID, colourmode);
    glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);
    glUniform4fv(lightposID, 1, &lightpos[0]);
    glUniform1ui(attenuationmodeID, attenuationmode);
    // Define our model transformation in a stack and 
    // push the identity matrix onto the stack
    stack<mat4> model;
    model.push(mat4(1.0f));

    // Define our transformations that apply to all of the objects 
    // by modifying the matrix at the top of the stack
    model.top() = scale(model.top(), vec3(model_scale, model_scale, model_scale));//scale equally in all axis
    model.top() = rotate(model.top(), -radians(0.0f), vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
    model.top() = rotate(model.top(), -radians(0.0f), vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
    model.top() = rotate(model.top(), -radians(0.0f), vec3(0, 0, 1)); //rotating in clockwise direction around z-axis

    model.push(model.top());
    {

        //disco ball
        model.top() = translate(model.top(), vec3(-x - 0.5f, 2, 0));
        model.top() = scale(model.top(), vec3(0.4f, 0.4f, 0.4f));
        model.top() = rotate(model.top(), -radians(90.0f), vec3(1, 0, 0));
        model.top() = rotate(model.top(), -radians(angle_x), vec3(0, 0, 1)); //rotating in clockwise direction around x-axis
        
        glBindTexture(GL_TEXTURE_2D, textureID5);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


        // Send the model uniform to the currently bound shader,
        glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
        normalmatrix = transpose(inverse(mat3(view * model.top())));
        glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
        
        aSphere.drawSphere(drawmode);
    }
    model.pop();

    model.push(model.top());
    {

        model.top() = translate(model.top(), vec3(-x - 0.5f, 3, 0));
        model.top() = scale(model.top(), vec3(0.03f, 1.5f, 0.03f));
         //rotating in clockwise direction around z-axis
        // Send the model uniform to the currently bound shader,
        glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
        normalmatrix = transpose(inverse(mat3(view * model.top())));
        glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
        /* Draw our cylinder */
        aCylinder.drawCylinder(drawmode);
    }
    model.pop();

    model.push(model.top());
    {
        
        
        model.top() = translate(model.top(), vec3(-x - 1.5f, -1.75, -1));
        model.top() = rotate(model.top(), -radians(angle_y), vec3(0, 1, 0)); //rotating in clockwise direction around x-axis
        glBindTexture(GL_TEXTURE_2D, textureID3);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // Send our uniforms variables to the currently bound shader,
        glUniformMatrix4fv(modelID, 1, GL_FALSE, &model.top()[0][0]);
        normalmatrix = transpose(inverse(mat3(view * model.top())));
        glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
        /* Draw our dalek object */
        dalek.drawObject(drawmode);
    }
    model.pop();

    model.push(model.top());
    {


        model.top() = translate(model.top(), vec3(-x + 1.5f, -0.5, 0));
        model.top() = scale(model.top(), vec3(0.1f, 0.1f, 0.1f));
        glBindTexture(GL_TEXTURE_2D, textureID1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // Send our uniforms variables to the currently bound shader,
        glUniformMatrix4fv(modelID, 1, GL_FALSE, &model.top()[0][0]);
        normalmatrix = transpose(inverse(mat3(view * model.top())));
        glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
        /* Draw our jellyfish object */
        jellyfish.drawObject(drawmode);
    }
    model.pop();

    model.push(model.top());
    {
        //front

        model.top() = translate(model.top(), vec3(-x - 0.5f, 1, 0));
        model.top() = scale(model.top(), vec3(11.0f, 11.0f, 11.0f));
        glBindTexture(GL_TEXTURE_2D, textureID4);
        // Send our uniforms variables to the currently bound shader,
        glUniformMatrix4fv(modelID, 1, GL_FALSE, &model.top()[0][0]);
        normalmatrix = transpose(inverse(mat3(view * model.top())));
        glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
        /* Draw our quad object */
       aQuad.drawQuad(drawmode);
    }
    model.pop();
    model.push(model.top());
    {

        //back
        model.top() = translate(model.top(), vec3(-x - 0.5f, 1, 0));
        model.top() = scale(model.top(), vec3(11.0f, 11.0f, 11.0f));
        model.top() = rotate(model.top(), -radians(180.0f), vec3(0, 1, 0));
        glBindTexture(GL_TEXTURE_2D, textureID4);
        // Send our uniforms variables to the currently bound shader,
        glUniformMatrix4fv(modelID, 1, GL_FALSE, &model.top()[0][0]);
        normalmatrix = transpose(inverse(mat3(view * model.top())));
        glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
        /* Draw our quad object */
       aQuad.drawQuad(drawmode);
    }
    model.pop();

    model.push(model.top());
    {

        //floor
        model.top() = translate(model.top(), vec3(-x - 0.5f, 1, 0));
        model.top() = scale(model.top(), vec3(11.0f, 11.0f, 11.0f));
        model.top() = rotate(model.top(), -radians(90.0f), vec3(1, 0, 0));
        glBindTexture(GL_TEXTURE_2D, textureID1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // Send our uniforms variables to the currently bound shader,
        glUniformMatrix4fv(modelID, 1, GL_FALSE, &model.top()[0][0]);
        normalmatrix = transpose(inverse(mat3(view * model.top())));
        glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
        /* Draw our quad object */
        aQuad.drawQuad(drawmode);
    }
    model.pop();
    model.push(model.top());
    {

        //left
        model.top() = translate(model.top(), vec3(-x - 0.5f, 1, 0));
        model.top() = scale(model.top(), vec3(11.0f, 11.0f, 11.0f));
        model.top() = rotate(model.top(), radians(90.0f), vec3(0, 1, 0));
        glBindTexture(GL_TEXTURE_2D, textureID4);
        // Send our uniforms variables to the currently bound shader,
        glUniformMatrix4fv(modelID, 1, GL_FALSE, &model.top()[0][0]);
        normalmatrix = transpose(inverse(mat3(view * model.top())));
        glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
        /* Draw our quad object */
        aQuad.drawQuad(drawmode);
    }
    model.pop();
    model.push(model.top());
    {
        //roof

        model.top() = translate(model.top(), vec3(-x - 0.5f, 6.5, 0));
        model.top() = scale(model.top(), vec3(11.0f, 11.0f, 11.0f));
        model.top() = rotate(model.top(), -radians(90.0f), vec3(1, 0, 0));
        glBindTexture(GL_TEXTURE_2D, textureID1);
        // Send our uniforms variables to the currently bound shader,
        glUniformMatrix4fv(modelID, 1, GL_FALSE, &model.top()[0][0]);
        normalmatrix = transpose(inverse(mat3(view * model.top())));
        glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
        /* Draw our quad object */
       aQuad.drawQuad(drawmode);
    }
    model.pop();
    model.push(model.top());
    {
        //right

        model.top() = translate(model.top(), vec3(-x + 5.0f, 1, 0));
        model.top() = scale(model.top(), vec3(11.0f, 11.0f, 11.0f));
        model.top() = rotate(model.top(), radians(90.0f), vec3(0, 1, 0));
        glBindTexture(GL_TEXTURE_2D, textureID4);
        // Send our uniforms variables to the currently bound shader,
        glUniformMatrix4fv(modelID, 1, GL_FALSE, &model.top()[0][0]);
        normalmatrix = transpose(inverse(mat3(view * model.top())));
        glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
        /* Draw our quad object */
        aQuad.drawQuad(drawmode);
    }
    model.pop();



    //Source: https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp
    //change shader program to cubemap
    //make the cubemap drawn in the background of the other objects
    model.push(model.top());
    {
        model.top() = translate(model.top(), vec3(-x + 5.0f, 1, 0));
        glUseProgram(cubemapShader);
        //make the cubemap vao current
        glBindVertexArray(cubeVAO);
        //default texture
        glActiveTexture(GL_TEXTURE0);
        //texturise the texture target
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

        glUniformMatrix4fv(cubemapViewID, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(cubemapProjectionID, 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(cubemapModelID, 1, GL_FALSE, &(model.top()[0][0]));
        //draw the cubemap
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }model.pop();
    
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glDisableVertexAttribArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);

    rotation += rotAnim;

    /* Modify our animation variables */
    angle_x += angle_inc_x;
    angle_y += angle_inc_y;
    angle_z += angle_inc_z;

}

/* Called whenever the window is resized. The new window size is given, in pixels. */
static void reshape(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    aspect_ratio = ((float)w / 640.f * 4.f) / ((float)h / 480.f * 3.f);
}

/* change view angle, exit upon ESC */
static void keyCallback(GLFWwindow* window, int key, int s, int action, int mods)
{
    /* Enable this call if you want to disable key responses to a held down key*/
    //if (action != GLFW_PRESS) return;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == 'Q') angle_inc_x -= 0.05f;
    if (key == 'W') angle_inc_x += 0.05f;
    if (key == 'E') angle_inc_y -= 0.05f;
    if (key == 'R') angle_inc_y += 0.05f;
    if (key == 'A') model_scale -= 0.02f;
    if (key == 'S') model_scale += 0.02f;
    if (key == 'Z') x -= 0.05f;
    if (key == 'X') x += 0.05f;
    if (key == 'C') y -= 0.05f;
    if (key == 'V') y += 0.05f;
    if (key == 'B') z -= 0.05f;
    if (key == 'N') z += 0.05f;
    if (key == 'H') cameraPX -= 0.1f;
    if (key == 'U') cameraPY += 0.1f;
    if (key == 'J') cameraPY -= 0.1f;
    if (key == 'K') cameraPX += 0.1f;
    if (key == 'I') cameraPZ += 0.1f;
    if (key == 'L') cameraPZ -= 0.1f;

    if (key == 'M' && action != GLFW_PRESS)
    {
        colourmode = !colourmode;
    }

    /* Turn attenuation on and off */
    if (key == '.' && action != GLFW_PRESS)
    {
        attenuationmode = !attenuationmode;
    }

    /* Cycle between drawing vertices, mesh and filled polygons */
    if (key == ',' && action != GLFW_PRESS)
    {
        drawmode++;
        if (drawmode > 2) drawmode = 0;
    }

    //look right
    if (key == GLFW_KEY_RIGHT)
    {
        cameraLX += 0.1f;
    }
    //look up
    if (key == GLFW_KEY_UP)
    {
        cameraLY += 0.1f;
    }
    //look down
    if (key == GLFW_KEY_DOWN)
    {
        cameraLY -= 0.1f;
    }
    //look left
    if (key == GLFW_KEY_LEFT)
    {
        cameraLX -= 0.1f;
    }
    //look Z axis (+), useful for when you are positioned behind the model
    if (key == GLFW_KEY_PAGE_DOWN)
    {
        cameraLZ += 0.1f;
    }
    //Look Z axis (-)
    if (key == GLFW_KEY_PAGE_UP)
    {
        cameraLZ -= 0.1f;
    }

    //if (key == GLFW_KEY_1) rotAnim += 0.5f;

    //if (key == GLFW_KEY_2) rotAnim -= 0.5f;

    

}

/* Entry point of program */
int main(int argc, char* argv[])
{
    GLWrapper* glw = new GLWrapper(1024, 768, "Assignment 2");;

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD. Exiting." << endl;
        return -1;
    }

    glw->setRenderer(display);
    glw->setKeyCallback(keyCallback);
    glw->setReshapeCallback(reshape);

    init(glw);

    glw->eventLoop();

    delete(glw);
    return 0;
}