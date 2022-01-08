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
		enum class CommandType {
			CLEAR = 0,
			DRAW = 1,
			DRAW_COUNT = 2,

			SET_CLEAR_COLOR = 3,
			PUSH_DRAW_TYPE = 4,
			POP_DRAW_TYPE = 5,
			ENABLE_DEPTH_TEST = 6,
			ENABLE_BACKFACE_CULLING = 7,

			BIND = 8,
			UNIFORM = 9,
			RENDER_TO_TARGET = 10
		};

		//Drawtype
		struct DrawType {
			RenderDrawType Type;
			bool WireFrame;
		};

		//queueable render-related command
		struct Command {
			CommandType Type;
			std::variant<
				CommandFormDraw,
				CommandFormDrawCount,
				CommandFormSetClearColor,
				DrawType,
				CommandFormBool,
				CommandFormBind,
				CommandFormUniform,
				CommandFormRenderToTarget
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