#include "scene.h"
#include "teapot.h"
#include <graphics/Mesh.h>
#include <graphics/Shader.h>
#include "MyMaterials.h"

class BlinnPhongScene : public Scene
{
public:
    BlinnPhongScene()
    {
        FRM_SHADER_ATTRIBUTE attributes[2] =
        {
            { "g_vPositionOS", graphics::ATTRIB_POSITION },
            { "g_vNormalOS", graphics::ATTRIB_NORMAL }
        };

        int numAttributes = sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE);

        //Load shader
        m_shader = new graphics::Shader("assets/blinn-phong.vertexshader",
            "assets/blinn-phong.fragmentshader", attributes, numAttributes);

        //m_smUniforms = new SimpleMaterialUniforms(m_shader, &m_sharedValues);
        m_smUniforms = new SimpleMaterialUniforms(m_shader, &m_sharedValues);
        checkOpenGL();

        m_smUniforms2 = new SimpleMaterialUniforms(m_shader, &m_sharedValues2);
        checkOpenGL();
        //Create mesh1
        m_smUniforms->vAmbient = slmath::vec4(0.5f, 0.2f, 1.0f, 1.0f);
        m_smUniforms->vDiffuse = slmath::vec4(0.5, 0.2f, 1.0f, 1.0f);
        m_smUniforms->vSpecular = slmath::vec4(1.0f, 1.0f, 1.0f, 5.0f);
        m_material = m_smUniforms;

        m_mesh = createTeapotMesh();
        
        m_mesh2 = graphics::Mesh::loadFromOBJ("assets/Sphere.obj");
        m_mesh3 = graphics::Mesh::loadFromOBJ("assets/Torus.obj");
       
    }

    virtual ~BlinnPhongScene()
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
            new graphics::VertexArrayImpl<slmath::vec3>
            (graphics::ATTRIB_POSITION, (slmath::vec3*)TeapotData::positions,
            TeapotData::numVertices),

            new graphics::VertexArrayImpl<slmath::vec3>
            (graphics::ATTRIB_NORMAL, (slmath::vec3*)TeapotData::normals,
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
        
        m_matModel = slmath::rotationX((-3.1415 * 0.5f) *m_totalTime / 2); // 90 degrees around X-axis
        m_matModel = slmath::rotationY(m_totalTime * 5) * m_matModel; //Rotate according to total time
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
        glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        checkOpenGL();

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
        checkOpenGL();
    }

private:
    core::Ref<graphics::Mesh> m_mesh, m_mesh2, m_mesh3;
    core::Ref<graphics::Shader> m_shader;
    SharedShaderValues m_sharedValues, m_sharedValues2;
    core::Ref<graphics::ShaderUniforms> m_material, m_material2;
    core::Ref<SimpleMaterialUniforms> m_smUniforms, m_smUniforms2;
    float m_totalTime, m_totalTime2;
    slmath::mat4 m_matProjection, m_matProjection2;
    slmath::mat4 m_matView, m_matView2;
    slmath::mat4 m_matModel, m_matModel2;

    
};