#pragma once
#include "Tara/Core/Base.h"
#include "Tara/Renderer/Uniform.h"
//#include "Tara/Renderer/Renderer.h"
#include "tarapch.h"

namespace Tara {
	
	//forward declaration
	REFTYPE(Bindable);
	REFTYPE(VertexArray);
	REFTYPE(Shader);
	REFTYPE(RenderTarget);
	REFTYPE(MaterialBase);
	REFTYPE(VertexBuffer);
	struct RenderSceneData;

	/// <summary>
	/// An Enum of the drawing types available to the render backend
	/// </summary>
	enum class RenderDrawType {
		/// <summary>
		/// Keep the current drawtype
		/// </summary>
		Keep, 
		/// <summary>
		/// Draw points at each vertex
		/// </summary>
		Points,
		/// <summary>
		/// Draw lines between each pair of vertecies
		/// </summary>
		Lines,
		/// <summary>
		/// Draw triangles with each set of three vertecies
		/// </summary>
		Triangles,
		/// <summary>
		/// Draw lines connecting the vertecies in drawing order
		/// </summary>
		LineStrip,
		/// <summary>
		/// Draw lines connecting the vertecies in drawing order, and close the shape
		/// </summary>
		LineLoop,
		/// <summary>
		/// Draw a triangle strip
		/// </summary>
		TriangleStrip,
		/// <summary>
		/// Draw a triangle fan
		/// </summary>
		TriangleFan
	};

	enum class RenderBlendMode {
		NORMAL,
		ADD,
		MULTIPLY,
		REPLACE
	};

	/// <summary>
	/// The RenderCommand class is a (mostly?) static class, providing an interface to a private 
	/// PlatformRenderCommand instance. (Unique Pointer)
	/// </summary>
	class RenderCommand {
	

	public:
		/// <summary>
		/// Initialize the RenderCommand internal instance
		/// </summary>
		static void Init();

		/// <summary>
		/// Begin a render command queue (both forward and deferred)
		/// </summary>
		static void BeginQueue();

		/// <summary>
		/// Execute a render command queue (forward or deferred, depending on current mode)
		/// </summary>
		static void ExecuteQueue();

		/// <summary>
		/// Discard a render command queue (forward or deferred, depending on current mode)
		/// </summary>
		static void DiscardQueue();

		/// <summary>
		/// Stop enqueuing commands. Queues are not cleared.
		/// </summary>
		inline static void StopQueue() { s_EnqueingCommands = false; };

		/// <summary>
		/// Check if currently enqueueing commands.
		/// </summary>
		/// <returns>True if currently enqueueing commands, false otherwise</returns>
		inline static bool CheckQueue() { return s_EnqueingCommands; }
	public:
		//Drawing functions. Can be queued.

		/// <summary>
		/// clear the screen
		/// </summary>
		static void Clear();
		
		/// <summary>
		/// Draw the current VertexArray. Does not bind the VertexArray passed to it.
		/// </summary>
		/// <param name="vertexArray"></param>
		static void Draw(VertexArrayRef vertexArray);

		/// <summary>
		/// Draw the current VertexArray, but a specific count of vertecies instead of via an IndexBuffer. 
		/// Does not bind the VertexArray passed to it.
		/// </summary>
		/// <param name="vertexArray"></param>
		static void DrawCount(uint32_t count);

	public:
		//State change functions. Can be queued.

		/// <summary>
		/// Set the clear color
		/// </summary>
		/// <param name="color">clear color as a vec3</param>
		inline static void SetClearColor(glm::vec3 color) { SetClearColor(color.r, color.g, color.b); }

		/// <summary>
		/// Set the clear color
		/// </summary>
		/// <param name="color">clear color as a vec4 (alpha is ignored)</param>
		inline static void SetClearColor(glm::vec4 color) { SetClearColor(color.r, color.g, color.b); }

		/// <summary>
		/// Set the clear color
		/// </summary>
		/// <param name="r">red component of clear color</param>
		/// <param name="g">green component of clear color</param>
		/// <param name="b">blue component of clear color</param>
		static void SetClearColor(float r, float g, float b);

		/// <summary>
		/// Push a new draw type for future draws
		/// </summary>
		/// <param name="drawType">the new draw type</param>
		/// <param name="wireframe">if wireframe should be turned on (false by default)</param>
		static void PushDrawType(RenderDrawType drawType, bool wireframe = false);

		/// <summary>
		/// Pop the current drawType for future draws
		/// </summary>
		static void PopDrawType();

		/// <summary>
		/// Enable and Disable depth testing in the rendering system. Should be disabled for 2D.
		/// </summary>
		/// <param name="enable"></param>
		static void EnableDepthTesting(bool enable);

		/// <summary>
		/// Enable backface culling. Front face is defined by Clockwise winding order.
		/// </summary>
		/// <param name="enable"></param>
		static void EnableBackfaceCulling(bool enable);

		/// <summary>
		/// Enabel frontface culling. Front face is defined by Clockwise winding order.
		/// </summary>
		/// <param name="enable"></param>
		static void EnableFrontfaceCulling(bool enable);

		/// <summary>
		/// Set the Renderer's blend mode
		/// </summary>
		/// <param name="mode"></param>
		static void SetBlendMode(RenderBlendMode mode);

	public:
		//binding functions. Can be queued.

		/// <summary>
		/// Bind a bindable
		/// </summary>
		/// <param name="ref">the bindable</param>
		/// <param name="a">paramater for the bind</param>
		/// <param name="b">paramater for the bind</param>
		static void Bind(BindableRef ref, bool binding, int a, int b);

		/// <summary>
		/// Send a uniform to a shader
		/// </summary>
		/// <param name="shader">the shader</param>
		/// <param name="name">the uniform name</param>
		/// <param name="data">the uniform data</param>
		static void SendUniform(ShaderRef shader, const std::string& name, const Uniform& data);

		/// <summary>
		/// Render to a RenderTarget
		/// </summary>
		/// <param name="ref">the RenderTarget</param>
		static void RenderToTartet(RenderTargetRef ref, bool render);

		/// <summary>
		/// Set the data in a vertex buffer
		/// </summary>
		/// <param name="ref">the vertex buffer</param>
		/// <param name="data">the data as float array</param>
		/// <param name="count">the float count</param>
		static void SetVertexBufferData(VertexBufferRef ref, const float* data, uint32_t count);

	public:
		//Query functions, not queued

		/// <summary>
		/// Gets the max number of textures that can be sent to the fragment shader.
		/// </summary>
		/// <returns>that number.</returns>
		inline static uint32_t GetMaxTextureSlotsPerShader() { return s_RC->IGetMaxTextureSlotsPerShader(); }

		/// <summary>
		/// Enable/Disable future commands to be treated as Deferred. NOT QUEUED, but effects what queue future commands enter.
		/// </summary>
		/// <param name="enable"></param>
		inline static void EnableDeferred(bool enable) { s_CurrentModeDeferred = enable; }

		virtual ~RenderCommand() = default;
	protected:
		/// <summary>
		/// Protected SetClearColor, for underlying implementation to override
		/// </summary>
		/// <param name="r">red component of clear color</param>
		/// <param name="g">green component of clear color</param>
		/// <param name="b">blue component of clear color</param>
		virtual void ISetClearColor(float r, float g, float b) = 0;

		/// <summary>
		/// Protected SetDrawType, for underlying implementation to override
		/// </summary>
		/// <param name="drawType">the drawtype</param>
		/// <param name="wireframe">if wireframe should be turned on</param>
		virtual void ISetDrawType(RenderDrawType drawType, bool wireframe) = 0;

		/// <summary>
		/// Protected Clear, for underlying implementation to override
		/// </summary>
		virtual void IClear() = 0;
		/// <summary>
		/// Protected Draw, for underlying implementation to override
		/// </summary>
		/// <param name="vertexArray"></param>
		virtual void IDraw(VertexArrayRef vertexArray) = 0;

		/// <summary>
		/// Protected DrawCount, for underlying implementation to override
		/// </summary>
		/// <param name="vertexArray"></param>
		virtual void IDrawCount(uint32_t count) = 0;

		/// <summary>
		/// Protected GetMaxTextureSlotsPerShader, for underlying implementation to override
		/// </summary>
		virtual uint32_t IGetMaxTextureSlotsPerShader() = 0;

		/// <summary>
		/// Protected EnableDepthTesting for underlying implementation to override
		/// </summary>
		/// <param name="enable"></param>
		virtual void IEnableDepthTesting(bool enable) = 0;

		/// <summary>
		/// Protected EnableBackfaceCulling for underlying implementation to override
		/// </summary>
		/// <param name="enable"></param>
		virtual void IEnableBackfaceCulling(bool enable) = 0;

		/// <summary>
		/// Protected EnableFrontfaceCulling for underlying implementation to override
		/// </summary>
		/// <param name="enable"></param>
		virtual void IEnableFrontfaceCulling(bool enable) = 0;

		/// <summary>
		/// Protected SetBlendMode for underlying implementation to override
		/// </summary>
		/// <param name="mode"></param>
		virtual void ISetBlendMode(RenderBlendMode mode) = 0;

	private:
		//Command Queue structures
		struct CommandFormDraw {
			VertexArrayRef VertexArray;
		};

		struct CommandFormDrawCount {
			uint32_t Count;
		};

		struct CommandFormSetClearColor {
			float r, g, b;
		};

		struct CommandFormBool {
			bool Enable;
		};

		struct CommandFormBind {
			BindableRef Bindable;
			bool bind;
			int	a, b;
		};

		struct CommandFormUniform {
			ShaderRef Shader;
			std::string Name;
			Uniform uniform;
		};

		struct CommandFormRenderToTarget {
			RenderTargetRef Target;
			bool Render;
		};

		struct CommandFormSetVertexBufferData {
			VertexBufferRef Target;
			float* Data; //This will have to be memory-managed
			uint32_t Count;
		};

		enum class CommandType {
			CLEAR = 0,
			DRAW,
			DRAW_COUNT,

			SET_CLEAR_COLOR,
			PUSH_DRAW_TYPE,
			POP_DRAW_TYPE,
			ENABLE_DEPTH_TEST,
			ENABLE_BACKFACE_CULLING,
			ENABLE_FRONTFACE_CULLING,
			SET_BLENDMODE,

			BIND,
			UNIFORM,
			RENDER_TO_TARGET,
			SET_VERTEX_BUFFER_DATA,
		};

		//Drawtype
		struct DrawType {
			RenderDrawType Type;
			bool WireFrame;
		};
		using CommandFormDrawType = DrawType;

		using CommandFormBlend = RenderBlendMode;

		//queueable render-related command
		struct Command {
			CommandType Type;
			std::variant<
				CommandFormDraw,
				CommandFormDrawCount,
				CommandFormSetClearColor,
				CommandFormDrawType,
				CommandFormBool,
				CommandFormBlend,
				CommandFormBind,
				CommandFormUniform,
				CommandFormRenderToTarget,
				CommandFormSetVertexBufferData
			> Params;
		};

		
		static void PushCommand(Command& c);

		static void ExecuteCommand(const Command& c);

		/// <summary>
		/// the unique pointer to the underlying RenderCommand instance.
		/// </summary>
		static std::unique_ptr<RenderCommand> s_RC;
		static std::list<DrawType> s_DrawTypeStack;
		static std::vector<Command> s_CommandQueueDeferred;
		static std::vector<Command> s_CommandQueueForward;
		static bool s_CurrentModeDeferred;
		static bool s_EnqueingCommands;
	};

}