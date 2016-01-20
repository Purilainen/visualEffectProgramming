#include "scene.h"
#include <core/log.h>

class QuadScene : public Scene
{
public:
	QuadScene()
	{

		LOG("QuadScene construct");
		checkOpenGL();

		m_count = 0.0f;
		m_hShaderProgram = 0;

		const char* g_strVSProgram =
			"attribute vec4 g_vVertex;											 \n"
			"attribute vec4 g_vColor;											 \n"
			"varying   vec4 g_vVSColor;                                          \n"
			"attribute vec2 g_vTexCoord;										 \n"
			"varying vec2 g_vvTexCoord;											 \n"
			"void main()														 \n"
			"{																	 \n"
			"    gl_Position  = vec4( g_vVertex.x, g_vVertex.y,                  \n"
			"                         g_vVertex.z, g_vVertex.w );                \n"
			"    g_vVSColor = g_vColor;                                          \n"
			"	g_vvTexCoord = g_vTexCoord;										 \n"
			"}																	 \n";


		const char* g_strFSProgram =
			"#ifdef GL_FRAGMENT_PRECISION_HIGH									 \n"
			"   precision highp float;											 \n"
			"#else																 \n"
			"   precision mediump float;										 \n"
			"#endif																 \n"
			"varying vec2 g_vTexCoord;											 \n"
			"varying   vec4 g_vVSColor;                                          \n"
			"																	 \n"
			"void main()														 \n"
			"{																	 \n"
			"    gl_FragColor = g_vVSColor;                                      \n"
			"}																	 \n";

		//pixel shader
		const char* g_strPSProgram =
			"#ifdef GL_FRAGMENT_PRECISION_HIGH									 \n"
			"   precision highp float;											 \n"
			"#else																 \n"
			"   precision mediump float;										 \n"
			"#endif																 \n"
			"varying vec2 g_vvTexCoord;											 \n"
			"																	 \n"
			"																	 \n"
			"																	 \n"
			"void main()														 \n"
			"{																	 \n"
			"gl_FragColor = vec4((g_vvTexCoord.x + g_vvTexCoord.y) / 2);	 \n"
			"																	 \n"
			"}																	 \n";
		//test shader
		const char* g_strTSProgram =
			"#ifdef GL_FRAGMENT_PRECISION_HIGH									 \n"
			"   precision highp float;											 \n"
			"#else																 \n"
			"   precision mediump float;										 \n"
			"#endif																 \n"
			"varying vec2 g_vvTexCoord;											 \n"
			"uniform float time;																	 \n"
			"																	 \n"
			"																	 \n"
			"void main()														 \n"
			"{																	 \n"
			"																	 \n"
			"																	 \n"
			"																	 \n"
			"float pi = 3.1415;													 \n"
			"float wave = sin(4*2*pi*((g_vvTexCoord.y+g_vvTexCoord.x)/(time)));							 \n"
			"																	 \n"
				"																 \n"
			"																	 \n"
			"																	 \n"
			"gl_FragColor = vec4(vec3(wave),1);	 \n"
			"																	 \n"
			"}																	 \n";

		FRM_SHADER_ATTRIBUTE attributes[3] = {
			{ "g_vVertex", 0 },
			{ "g_vColor", 1 },
			{ "g_vTexCoord", 2 }
		};

		FrmCompileShaderProgram(g_strVSProgram, g_strTSProgram, &m_hShaderProgram, attributes, sizeof(attributes) / sizeof(FRM_SHADER_ATTRIBUTE));

		checkOpenGL();
	}


	virtual ~QuadScene()
	{
		glDeleteProgram(m_hShaderProgram);
		LOG("QuadScene destructor");
	}


	virtual void update(graphics::ESContext* esContext, float deltaTime)
	{
		m_count += deltaTime;

		if (m_count > 2.0f)
			m_count = 0.0f;
	}


	virtual void render(graphics::ESContext* esContext)
	{
		checkOpenGL();

		// Set the viewport
		glViewport(0, 0, esContext->width, esContext->height);
		checkOpenGL();

		float aspectRatio = float(esContext->width) / float(esContext->height);
		float fSize = 1.0f;
		float VertexPositions[] =
		{
			-fSize, +fSize*aspectRatio, 0.0f, 1.0f,
			+fSize, +fSize*aspectRatio, 0.0f, 1.0f,
			-fSize, -fSize*aspectRatio, 0.0f, 1.0f,
			+fSize, -fSize*aspectRatio, 0.0f, 1.0f,
		};

		float VertexColors[] = { 
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f,
		};

		float TextCoords[] =
		{
			0.0, 0.0,
			1.0, 0.0,
			0.0, 1.0,
			1.0, 1.0,
		};

		
		

		
		// Clear the backbuffer and depth-buffer
		glClearColor(0.0f, m_count, m_count, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		checkOpenGL();

		// Set the shader program and the texture
		glUseProgram(m_hShaderProgram);
		checkOpenGL();

		GLfloat time = m_count;
		glUniform1f(glGetUniformLocation(m_hShaderProgram, "time"), time);
		// Draw the colored triangle
		glVertexAttribPointer(0, 4, GL_FLOAT, 0, 0, VertexPositions);
		checkOpenGL();
		glEnableVertexAttribArray(0);
		checkOpenGL();

		glVertexAttribPointer(1, 4, GL_FLOAT, 0, 0, VertexColors);
		checkOpenGL();
		glEnableVertexAttribArray(1);
		checkOpenGL();

		glVertexAttribPointer(2, 2, GL_FLOAT, 0, 0, TextCoords);
		checkOpenGL();
		glEnableVertexAttribArray(2);
		checkOpenGL();

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		checkOpenGL();

		glDisableVertexAttribArray(0);
		checkOpenGL();
		glDisableVertexAttribArray(1);
		checkOpenGL();
		glDisableVertexAttribArray(2);
		checkOpenGL();
	}
private:
	float m_count;

	GLuint       m_hShaderProgram;

};


