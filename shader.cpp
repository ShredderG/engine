#define CONVERT_TO_STRING(A) #A

struct GM_shader
{
	uint program, program2, rbo, fbo, texture;
	const static int SIZE = 2048;

	PFNGLCREATEPROGRAMPROC				gl_CreateProgram;
	PFNGLDELETEPROGRAMPROC				gl_DeleteProgram;
	PFNGLUSEPROGRAMPROC					gl_UseProgram;
	PFNGLATTACHSHADERPROC				gl_AttachShader;
	PFNGLDETACHSHADERPROC				gl_DetachShader;
	PFNGLLINKPROGRAMPROC				gl_LinkProgram;
	PFNGLGETPROGRAMIVPROC				gl_GetProgramiv;
	PFNGLGETSHADERINFOLOGPROC			gl_GetShaderInfoLog;
	PFNGLGETUNIFORMLOCATIONPROC			gl_GetUniformLocation;
	PFNGLUNIFORM1IPROC					gl_Uniform1i;
	PFNGLUNIFORM2IPROC					gl_Uniform2i;
	PFNGLUNIFORM3IPROC					gl_Uniform3i;
	PFNGLUNIFORM4IPROC					gl_Uniform4i;
	PFNGLUNIFORM1FPROC					gl_Uniform1f;
	PFNGLUNIFORM2FPROC					gl_Uniform2f;
	PFNGLUNIFORM3FPROC					gl_Uniform3f;
	PFNGLUNIFORM4FPROC					gl_Uniform4f;
	PFNGLUNIFORMMATRIX4FVPROC			gl_UniformMatrix4fv;
	PFNGLGETATTRIBLOCATIONPROC			gl_GetAttribLocation;
	PFNGLVERTEXATTRIB1FPROC				gl_VertexAttrib1f;
	PFNGLVERTEXATTRIB1FVPROC			gl_VertexAttrib1fv;
	PFNGLVERTEXATTRIB2FVPROC			gl_VertexAttrib2fv;
	PFNGLVERTEXATTRIB3FVPROC			gl_VertexAttrib3fv;
	PFNGLVERTEXATTRIB4FVPROC			gl_VertexAttrib4fv;
	PFNGLENABLEVERTEXATTRIBARRAYPROC	gl_EnableVertexAttribArray;
	PFNGLBINDATTRIBLOCATIONPROC			gl_BindAttribLocation;
	PFNGLACTIVETEXTUREPROC				gl_ActiveTexture;
	PFNGLCREATESHADERPROC				gl_CreateShader;
	PFNGLDELETESHADERPROC				gl_DeleteShader;
	PFNGLSHADERSOURCEPROC				gl_ShaderSource;
	PFNGLCOMPILESHADERPROC				gl_CompileShader;
	PFNGLGETSHADERIVPROC				gl_GetShaderiv;
	PFNGLGENFRAMEBUFFERSPROC			gl_GenFramebuffers;
	PFNGLBINDFRAMEBUFFERPROC			gl_BindFramebuffer;
	PFNGLFRAMEBUFFERTEXTURE2DPROC		gl_FramebufferTexture2D;
	PFNGLCHECKFRAMEBUFFERSTATUSPROC		gl_CheckFramebufferStatus;

	GM_shader()
	{
	}

	void setUp()
	{
		gl_CreateProgram			= (PFNGLCREATEPROGRAMPROC)           wglGetProcAddress("glCreateProgram");
		gl_DeleteProgram			= (PFNGLDELETEPROGRAMPROC)           wglGetProcAddress("glDeleteProgram");
		gl_UseProgram				= (PFNGLUSEPROGRAMPROC)              wglGetProcAddress("glUseProgram");
		gl_AttachShader				= (PFNGLATTACHSHADERPROC)            wglGetProcAddress("glAttachShader");
		gl_DetachShader				= (PFNGLDETACHSHADERPROC)            wglGetProcAddress("glDetachShader");
		gl_LinkProgram				= (PFNGLLINKPROGRAMPROC)             wglGetProcAddress("glLinkProgram");
		gl_GetProgramiv				= (PFNGLGETPROGRAMIVPROC)            wglGetProcAddress("glGetProgramiv");
		gl_GetShaderInfoLog			= (PFNGLGETSHADERINFOLOGPROC)        wglGetProcAddress("glGetShaderInfoLog");
		gl_GetUniformLocation		= (PFNGLGETUNIFORMLOCATIONPROC)      wglGetProcAddress("glGetUniformLocation");
		gl_Uniform1i				= (PFNGLUNIFORM1IPROC)               wglGetProcAddress("glUniform1i");
		gl_Uniform2i				= (PFNGLUNIFORM2IPROC)               wglGetProcAddress("glUniform2i");
		gl_Uniform3i				= (PFNGLUNIFORM3IPROC)               wglGetProcAddress("glUniform3i");
		gl_Uniform4i				= (PFNGLUNIFORM4IPROC)               wglGetProcAddress("glUniform4i");
		gl_Uniform1f				= (PFNGLUNIFORM1FPROC)               wglGetProcAddress("glUniform1f");
		gl_Uniform2f				= (PFNGLUNIFORM2FPROC)               wglGetProcAddress("glUniform2f");
		gl_Uniform3f				= (PFNGLUNIFORM3FPROC)               wglGetProcAddress("glUniform3f");
		gl_Uniform4f				= (PFNGLUNIFORM4FPROC)               wglGetProcAddress("glUniform4f");
		gl_UniformMatrix4fv			= (PFNGLUNIFORMMATRIX4FVPROC)        wglGetProcAddress("glUniformMatrix4fv");
		gl_GetAttribLocation		= (PFNGLGETATTRIBLOCATIONPROC)       wglGetProcAddress("glGetAttribLocation");
		gl_VertexAttrib1f			= (PFNGLVERTEXATTRIB1FPROC)          wglGetProcAddress("glVertexAttrib1f");
		gl_VertexAttrib1fv			= (PFNGLVERTEXATTRIB1FVPROC)         wglGetProcAddress("glVertexAttrib1fv");
		gl_VertexAttrib2fv			= (PFNGLVERTEXATTRIB2FVPROC)         wglGetProcAddress("glVertexAttrib2fv");
		gl_VertexAttrib3fv			= (PFNGLVERTEXATTRIB3FVPROC)         wglGetProcAddress("glVertexAttrib3fv");
		gl_VertexAttrib4fv			= (PFNGLVERTEXATTRIB4FVPROC)         wglGetProcAddress("glVertexAttrib4fv");
		gl_EnableVertexAttribArray	= (PFNGLENABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress("glEnableVertexAttribArray");
		gl_BindAttribLocation		= (PFNGLBINDATTRIBLOCATIONPROC)      wglGetProcAddress("glBindAttribLocation");
		gl_ActiveTexture			= (PFNGLACTIVETEXTUREPROC)           wglGetProcAddress("glActiveTexture");
		gl_CreateShader				= (PFNGLCREATESHADERPROC)            wglGetProcAddress("glCreateShader");
		gl_DeleteShader				= (PFNGLDELETESHADERPROC)            wglGetProcAddress("glDeleteShader");
		gl_ShaderSource				= (PFNGLSHADERSOURCEPROC)            wglGetProcAddress("glShaderSource");
		gl_CompileShader			= (PFNGLCOMPILESHADERPROC)           wglGetProcAddress("glCompileShader");
		gl_GetShaderiv				= (PFNGLGETSHADERIVPROC)             wglGetProcAddress("glGetShaderiv");
		gl_GenFramebuffers			= (PFNGLGENFRAMEBUFFERSPROC)         wglGetProcAddress("glGenFramebuffers");
		gl_BindFramebuffer			= (PFNGLBINDFRAMEBUFFERPROC)         wglGetProcAddress("glBindFramebuffer");
		gl_FramebufferTexture2D		= (PFNGLFRAMEBUFFERTEXTURE2DPROC)    wglGetProcAddress("glFramebufferTexture2D");
		gl_CheckFramebufferStatus	= (PFNGLCHECKFRAMEBUFFERSTATUSPROC)  wglGetProcAddress("glCheckFramebufferStatus");

		// ВЕРШИННЫЙ
		const char *vertexCode = CONVERT_TO_STRING(
			void main(void)
			{
				gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
				gl_FrontColor = gl_Color;
				gl_TexCoord[0] = gl_MultiTexCoord0;
				gl_Position.z = 0.5 + gl_Position.y / 4.0 - gl_Vertex.z / 1024.0;
				gl_Position.y += gl_Vertex.z / 300.0 * gl_ModelViewMatrix[2][2];
			}
		);

		// ПИКСЕЛЬНЫЙ
		const char *fragmentCode = CONVERT_TO_STRING(
			uniform sampler2D colorBuffer;
			uniform sampler2D depthBuffer;
			void main(void)
			{
				// vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z ,1.0);
				gl_FragColor = texture2D(colorBuffer, gl_TexCoord[0]) * gl_Color;
			}
		);

		// ВЕРШИННЫЙ
		const char *vertexCode2 = CONVERT_TO_STRING(
			void main(void)
		{
			gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
			gl_FrontColor = gl_Color;
			gl_TexCoord[0] = gl_MultiTexCoord0;
		}
		);

		// ПИКСЕЛЬНЫЙ
		const char *fragmentCode2 = CONVERT_TO_STRING(
			uniform sampler2D colorBuffer;
			uniform sampler2D depthBuffer;
		void main(void)
		{
			gl_FragColor = texture2D(colorBuffer, gl_TexCoord[0].xy - mod(gl_TexCoord[0].xy, 6.0 / 2048.0)) * gl_Color;
		}
		);

		// SHADERS
		if (!link(program, vertexCode, fragmentCode)) return;
		if (!link(program2, vertexCode2, fragmentCode2)) return;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, SIZE, SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		glGenTextures(1, &rbo);
		glBindTexture(GL_TEXTURE_2D, rbo);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SIZE, SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
						
		gl_GenFramebuffers(1, &fbo); 
		gl_BindFramebuffer(GL_FRAMEBUFFER_EXT, fbo);

		gl_FramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture, 0);
		gl_FramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rbo, 0);

		GLenum status = gl_CheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			showMessage("FBO error");
		}
		
		gl_ActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, rbo);
		gl_ActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		
		gl_UseProgram(program);
		gl_Uniform1i(gl_GetUniformLocation(program, "depthBuffer"), 1);
		gl_Uniform1i(gl_GetUniformLocation(program, "colorBuffer"), 0);
		gl_UseProgram(program2);
		gl_Uniform1i(gl_GetUniformLocation(program2, "depthBuffer"), 1);
		gl_Uniform1i(gl_GetUniformLocation(program2, "colorBuffer"), 0);

		gl_UseProgram(0);
	}

	// COMPILE SHADER
	bool compile(uint type, uint &shader, const char *code, string text)
	{
		shader = gl_CreateShader(type);
		gl_ShaderSource(shader, 1, &code, NULL);
		gl_CompileShader(shader);

		int done;
		gl_GetShaderiv(shader, GL_COMPILE_STATUS, &done);

		if (!done) showMessage(text + " шейдер - ошибка компиляции.");
		return done;
	}

	// LINK SHADER
	bool link(uint &program, const char *vertexCode, const char *fragmentCode)
	{
		// SHADERS
		uint vertex, fragment;
		if (!compile(GL_VERTEX_SHADER, vertex, vertexCode, "Вершинный")
			|| !compile(GL_FRAGMENT_SHADER, fragment, fragmentCode, "Фрагментный"))
			return false;

		int done;
		program = gl_CreateProgram();
		gl_AttachShader(program, vertex);
		gl_AttachShader(program, fragment);
		gl_LinkProgram(program);
		gl_GetProgramiv(program, GL_LINK_STATUS, &done);
		if (!done) showMessage("Ошибка при линковке шейдера.");
		return done;
	}

} shader;