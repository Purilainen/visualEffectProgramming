#include "scene.h"
#include "teapot.h"
#include <graphics/Mesh.h>
#include <graphics/Shader.h>
#include "MyMaterials.h"
#include "graphics/Image.h"
#include "graphics/Texture.h"

class CubeReflectionScene : public Scene
{
public:
    CubeReflectionScene()
    {
        FRM_SHADER_ATTRIBUTE attributes[3] =
        {
            { "g_vPositionOS", graphics::ATTRIB_POSITION },
            { "g_vNormalOS", graphics::ATTRIB_NORMAL },
            { "g_texCoordsOS", graphics::ATTRIB_UV }
        };

        int numAttributes = sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE);

        //Load shader
        m_shader = new graphics::Shader("assets/blinn-phong-textured-cube.vertexshader",
            "assets/blinn-phong-textured-cube.fragmentshader", attributes, numAttributes);

        //m_smUniforms = new SimpleMaterialUniforms(m_shader, &m_sharedValues);
        m_smUniforms = new SimpleMaterialWithTextureUniformsCube(m_shader, &m_sharedValues);
        checkOpenGL();

        //Create mesh1
        m_smUniforms->vAmbient = slmath::vec4(0.8f, 0.8f, 0.8f, 1.0f);
        m_smUniforms->vDiffuse = slmath::vec4(0.8f, 0.8f, 0.8f, 1.0f);
        m_smUniforms->vSpecular = slmath::vec4(0.2f, 0.2f, 0.2f, 5.0f);
        m_material = m_smUniforms;

        

        m_mesh = createTeapotMesh();

        //Cube
        std::string name = "BedroomCubeMap";
        core::Ref<graphics::Image> cubeImageRefs[6];
        /*
        cubeImageRefs[0] = graphics::Image::loadFromTGA("assets/" + name + "_RT.tga");
        cubeImageRefs[1] = graphics::Image::loadFromTGA("assets/" + name + "_LF.tga");
        cubeImageRefs[2] = graphics::Image::loadFromTGA("assets/" + name + "_DN.tga");
        cubeImageRefs[3] = graphics::Image::loadFromTGA("assets/" + name + "_UP.tga");
        cubeImageRefs[4] = graphics::Image::loadFromTGA("assets/" + name + "_FR.tga");
        cubeImageRefs[5] = graphics::Image::loadFromTGA("assets/" + name + "_BK.tga");
        */

        cubeImageRefs[0] = graphics::Image::loadFromTGA("assets/BedroomCubeMap_RT.tga");
        cubeImageRefs[1] = graphics::Image::loadFromTGA("assets/BedroomCubeMap_LF.tga");
        cubeImageRefs[2] = graphics::Image::loadFromTGA("assets/BedroomCubeMap_DN.tga");
        cubeImageRefs[3] = graphics::Image::loadFromTGA("assets/BedroomCubeMap_UP.tga");
        cubeImageRefs[4] = graphics::Image::loadFromTGA("assets/BedroomCubeMap_FR.tga");
        cubeImageRefs[5] = graphics::Image::loadFromTGA("assets/BedroomCubeMap_BK.tga");

        graphics::Image* cubeImages[6];
        for (int i = 0; i < 6; ++i)
        {
            cubeImages[i] = cubeImageRefs[i].ptr();
        }

        core::Ref<graphics::TextureCube> cubeMap = new graphics::TextureCube();
        cubeMap->setData(cubeImages);
        m_smUniforms->cubeMap = cubeMap;

        //Texture
        

        graphics::Image* img = graphics::Image::loadFromTGA("assets/MetalPlateDiffuse.tga");
        //graphics::Texture2D* tex = new graphics::Texture2D();

        core::Ref<graphics::Texture2D> tex = new graphics::Texture2D();
        tex->setData(img);
        m_smUniforms->diffuseMap = tex;
    }

    virtual ~CubeReflectionScene()
    {
        
    }
    ///////////////////////////////////////////////////////////
    graphics::Mesh* createTeapotMesh()
    {
        //Creating index buffer
        graphics::IndexBuffer* indexBuf = new graphics::IndexBuffer
            (TeapotData::indices, TeapotData::numIndices);

        //Creating vertex arrays
        graphics::VertexArray* vertexArr[] =
        {
            //Positions
            new graphics::VertexArrayImpl<slmath::vec3>
            (graphics::ATTRIB_POSITION, (slmath::vec3*)TeapotData::positions,
            TeapotData::numVertices),
            //Normals
            new graphics::VertexArrayImpl<slmath::vec3>
            (graphics::ATTRIB_NORMAL, (slmath::vec3*)TeapotData::normals,
            TeapotData::numVertices),
            //Texcoords
            new graphics::VertexArrayImpl<slmath::vec3>
            (graphics::ATTRIB_UV, (slmath::vec3*)TeapotData::texCoords,
            TeapotData::numVertices)
        };

        //Creating vertex buffer from arrays
        graphics::VertexBuffer* vertexBuf =
            new graphics::VertexBuffer(&vertexArr[0],
            sizeof(vertexArr) / sizeof(vertexArr[0]));

        //Create mesh from indices and vertices
        return new graphics::Mesh(indexBuf, vertexBuf);
    }

    /////////////////////////////////////////////////////////////////
    virtual void update(graphics::ESContext* esContext, float deltaTime)
    {
        m_totalTime += deltaTime;

        m_sharedValues.totalTime += deltaTime;
        if (m_sharedValues.totalTime > 2.0f)
        {
            m_sharedValues.totalTime = 0.0;
        }
        //Camera perspective matrix = Field of view, aspect ratio 
        //near plane distance and far plane distance
        float fAspect = (float)esContext->width / (float)esContext->height;
        m_matProjection = slmath::perspectiveFovRH(
            slmath::radians(45.0f),
            fAspect,
            5.0f,
            1000.0f);



        //Look at view matrix
        m_matView = slmath::lookAtRH(
            slmath::vec3(0.0f, 70.0f, 70.0f),
            slmath::vec3(0.0f, 15.0f, 0.0f),
            slmath::vec3(0.0f, 1.0f, 0.0f));

        //Calculate needed stuff for m_sharedValues
        m_sharedValues.matModel = m_matModel;
        m_sharedValues.matView = m_matView;
        m_sharedValues.matProj = m_matProjection;

        //Update teapot model matrix

        m_matModel = slmath::rotationX((-3.1415 * 0.5f)); // 90 degrees around X-axis
        m_matModel = slmath::rotationY(m_totalTime) * m_matModel; //Rotate according to total time
        m_matModel = slmath::translation(slmath::vec3(0.0f, 0.0f, 0.0f)) * m_matModel; //Translate
        //m_matModel = slmath::scaling(10.0f);


        slmath::mat4 matModelView = m_matView * m_matModel;
        slmath::mat4 matModelViewProj = m_matProjection * matModelView;
        slmath::mat4 matNormal = slmath::transpose(slmath::inverse(matModelView));

        m_sharedValues.matModelView = matModelView;
        m_sharedValues.matNormal = matNormal;
        m_sharedValues.matModelViewProj = matModelViewProj;

        m_sharedValues.lightPos = slmath::vec3(10.0, 50.0f, 70.0f);
        m_sharedValues.camPos = slmath::vec3(0.0, 70.0f, 70.0f);

    }

    virtual void render(graphics::ESContext* esContext)
    {
        checkOpenGL();
        //set viewport
        glViewport(0, 0, esContext->width, esContext->height);
        //clear back and depth buffers
        glClearColor(0.4f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        checkOpenGL();

        glEnable(GL_TEXTURE_2D);
        //initialize GL state.
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        glDepthFunc(GL_LEQUAL);
        checkOpenGL();

        m_material->bind();
        checkOpenGL();
        m_mesh->render();
        //m_mesh2->render();
        //m_mesh3->render();

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_TEXTURE_2D);
        checkOpenGL();
    }

private:
    core::Ref<graphics::Mesh> m_mesh;
    core::Ref<graphics::Shader> m_shader;
    SharedShaderValues m_sharedValues;
    core::Ref<graphics::ShaderUniforms> m_material;
    core::Ref<SimpleMaterialWithTextureUniformsCube> m_smUniforms;
    float m_totalTime;
    slmath::mat4 m_matProjection;
    slmath::mat4 m_matView;
    slmath::mat4 m_matModel;


};