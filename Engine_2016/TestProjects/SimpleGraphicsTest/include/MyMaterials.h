#ifndef MYMATERIALS_H
#define MYMATERIALS_H

#include <graphics/Shader.h>
#include <graphics/Texture.h>


struct SharedShaderValues
{
	float totalTime = 0.0f;
	slmath::mat4 matModelViewProj; //Model view projection matrix. Transform position vertices to clip space.
    slmath::mat4 matView; //View matrix (inverse of camera world matrix)
    slmath::mat4 matModel; //Model matrix (object world matrix)
    slmath::mat4 matModelView; //Model view matrix. Used to transform position vertices to camera space.
    slmath::mat4 matNormal; //Model view matrix. Used to transform normal/binormal/tangent vertices to clip space.
    slmath::mat4 matProj; //Projison matrix of the camera
    slmath::vec3 lightPos; //World position of point light
    slmath::vec3 camPos; //World position of camera
};

class GlobalShaderUniforms : public graphics::ShaderUniforms
{
public:
	GlobalShaderUniforms(graphics::Shader* shader,
		const SharedShaderValues* sharedShaderValues = 0);


	~GlobalShaderUniforms();
	virtual void getUniformLocations(graphics::Shader* shader);
	virtual void bind(graphics::Shader* shader);

private:
	const SharedShaderValues* m_sharedShaderValues;
    GLint m_ids[10];
	
};


class SimpleMaterialUniforms : public graphics::ShaderUniforms
{
public:
    slmath::vec4 vAmbient;
    slmath::vec4 vDiffuse;
    slmath::vec4 vSpecular;

public:
    SimpleMaterialUniforms(graphics::Shader* shader,
        const SharedShaderValues* sharedValues = 0);
      
    virtual ~SimpleMaterialUniforms();

    virtual void getUniformLocations(graphics::Shader* shader);
    virtual void bind(graphics::Shader* shader);   

private:
    core::Ref<GlobalShaderUniforms> m_globalShaderUniforms;
    GLint m_materialAmbientLoc;
    GLint m_materialDiffuseLoc;
    GLint m_materialSpecularLoc;
};

class SimpleMaterialWithTextureUniforms : public SimpleMaterialUniforms
{
public:
    core::Ref<graphics::Texture> diffuseMap;

    SimpleMaterialWithTextureUniforms(graphics::Shader* shader, SharedShaderValues* sharedValues)
        : SimpleMaterialUniforms(shader, sharedValues)
    {
    }

    virtual ~SimpleMaterialWithTextureUniforms()
    {
    }

    virtual void getUniformLocations(graphics::Shader* shader)
    {
        SimpleMaterialUniforms::getUniformLocations(shader);

        m_diffuseMapLoc = glGetUniformLocation(shader->getProgram(), "s_diffuseMap");
    }

    virtual void bind(graphics::Shader* shader)
    {
        SimpleMaterialUniforms::bind(shader);

        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap->getTextureId());

        glUniform1i(m_diffuseMapLoc, 0);
    }

private:
    GLint m_diffuseMapLoc;

};

class SimpleMaterialWithTextureUniformsCube : public SimpleMaterialWithTextureUniforms
{
public:
    core::Ref<graphics::TextureCube> cubeMap;

    SimpleMaterialWithTextureUniformsCube(graphics::Shader* shader, SharedShaderValues* sharedValues)
        : SimpleMaterialWithTextureUniforms(shader, sharedValues)
    {
    }

    virtual ~SimpleMaterialWithTextureUniformsCube()
    {}

    virtual void getUniformLocations(graphics::Shader* shader)
    {
        SimpleMaterialWithTextureUniforms::getUniformLocations(shader);

        m_cubeMapLoc = glGetUniformLocation(shader->getProgram(),"s_cubeMap");
    }

    virtual void bind(graphics::Shader* shader)
    {
        SimpleMaterialWithTextureUniforms::bind(shader);

        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->getTextureId());

        glUniform1i(m_cubeMapLoc, 1);
    }

private:
    GLint m_cubeMapLoc;
};









#endif