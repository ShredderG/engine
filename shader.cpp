#define TO_STRING(A) #A

// Shader
class Shader {
public:
	uint program = 0;

	void create(
		const char* fragment = "uniform sampler2D a;void main(){gl_FragColor=texture2D(a,gl_TexCoord[0])*gl_Color;}",
		const char* vertex = "void main(){gl_Position=gl_ModelViewProjectionMatrix*gl_Vertex;gl_FrontColor=gl_Color;gl_TexCoord[0]=gl_MultiTexCoord0;}") {
		program = link(vertex, fragment);
	}

	~Shader() {
		if (program != 0) {
			gl_DeleteProgram(program);
		}
	}

	// Use program uint
	void operator = (uint _program) {
		gl_UseProgram(program = _program);
	}

	// Use program Shader
	void operator = (Shader &shader) {
		gl_UseProgram(program = shader.program);
	}

private:
	// Compile shader
	uint compile(uint type, const char *code, string text) {
		uint shader = gl_CreateShader(type);
		gl_ShaderSource(shader, 1, &code, nullptr);
		gl_CompileShader(shader);

		int done;
		gl_GetShaderiv(shader, GL_COMPILE_STATUS, &done);
		if (!done) {
			showMessage(text + " shader - compile error");
			shader = 0;
		}

		return shader;
	}

	// Link shader
	uint link(const char* vertexCode, const char* fragmentCode) {
		uint program  = 0;
		uint vertex   = compile(GL_VERTEX_SHADER,   vertexCode,   "Vertex");
		uint fragment = compile(GL_FRAGMENT_SHADER, fragmentCode, "Fragment");

		if (vertex && fragment) {
			program = gl_CreateProgram();
			gl_AttachShader(program, vertex);
			gl_AttachShader(program, fragment);
			gl_LinkProgram(program);

			int done;
			gl_GetProgramiv(program, GL_LINK_STATUS, &done);
			if (!done) {
				showMessage("Shaders link error");
				program = 0;
			}

			gl_DetachShader(program, vertex);
			gl_DetachShader(program, fragment);

			gl_DeleteShader(vertex);
			gl_DeleteShader(fragment);
		}

		return program;
	}
} shader;