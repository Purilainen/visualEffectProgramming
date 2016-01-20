#ifndef MYMATERIALS_H
#define MYMATERIALS_H

#include <graphics/Shader.h>


struct SharedShaderValues
{
	float totalTime = 0.0f;
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
	const SharedShaderValues* m_shaderShaderValues;
	GLint m_id;
	
};














#endif