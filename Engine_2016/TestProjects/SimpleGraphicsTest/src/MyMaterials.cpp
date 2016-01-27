#include "MyMaterials.h"


GlobalShaderUniforms::GlobalShaderUniforms(graphics::Shader* shader, 
	const SharedShaderValues* shaderShaderValues) 
	: ShaderUniforms(shader),
	m_sharedShaderValues(shaderShaderValues)
{
};

void GlobalShaderUniforms::getUniformLocations(graphics::Shader* shader)
{
	// Get uniform locations
	m_id = glGetUniformLocation(shader->getProgram(), "totalTime");
	m_id2 = glGetUniformLocation(shader->getProgram(), "g_matModelViewProj");
};

void GlobalShaderUniforms::bind(graphics::Shader* shader)
{
	shader->bind();
	if (m_sharedShaderValues)
	{
		glUniform1f(m_id, m_sharedShaderValues->totalTime);

		if (m_id2 >= 0)
		{
			glUniformMatrix4fv(m_id2, 1, GL_FALSE, &m_sharedShaderValues->matModelViewProj[0][0]);
		}
	}
};

GlobalShaderUniforms::~GlobalShaderUniforms()
{};