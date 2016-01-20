#include "MyMaterials.h"


GlobalShaderUniforms::GlobalShaderUniforms(graphics::Shader* shader, 
	const SharedShaderValues* shaderShaderValues) : ShaderUniforms(shader),
	m_shaderShaderValues(shaderShaderValues)
{
};

void GlobalShaderUniforms::getUniformLocations(graphics::Shader* shader)
{
	m_id = glGetUniformLocation(shader->getProgram(), "totalTime");
};

void GlobalShaderUniforms::bind(graphics::Shader* shader)
{
	shader->bind();
	if (m_shaderShaderValues)
	{
		glUniform1f(m_id, m_shaderShaderValues->totalTime);
	}
};

GlobalShaderUniforms::~GlobalShaderUniforms()
{};