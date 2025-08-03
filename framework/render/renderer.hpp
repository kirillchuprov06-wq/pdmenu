#pragma once

// drawlist data
enum e_draw_list_data {
	first_layer, // foreground / override all windows
	second_layer // backround
};

// font flags
// we are going to use this to initialze the flags
enum e_font_flags {
	none,
	drop_shadow,
	outline
};

// circle flags
enum e_circle_flags {
	filled,
	outlined
};

enum e_gradient_flags {
	horizontal,
	vertical
};

struct render_globals_t {
	ImDrawList* m_draw_list{};
};

// heap
inline render_globals_t g_render_globals;

class c_font {
public: // constructors, handler and size
	ImFont* m_handle;
	c_vector_2d m_size;

	// default constructor
	c_font() {
		// if we point to the default constructor just add the default font that imgui gives us
		IMGUI_CHECKVERSION( );
		ImGui::CreateContext( );
		ImGuiIO& io = ImGui::GetIO();

		// font handler
		this->m_handle = io.Fonts->AddFontDefault( );
		if ( this->m_handle == nullptr ) {
			printf( "Failed to load default font!\n" );
			return;
		}

		// calculate text size
		// this should work like this
		this->m_size = c_vector_2d(this->m_handle->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1, "A").x,
			this->m_handle->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1, "A").y);

		printf("default constructor has got a link to it and created a font with automatically calculated font size on text (A) for x pixels: %.2f and y pixels: %.2f\n",
			this->m_size.x, this->m_size.y);
	}

	// only font location and font size
	c_font(const std::string& font_location, float size ) {
		// if we point to the default constructor just add the default font that imgui gives us
		IMGUI_CHECKVERSION( );
		ImGui::CreateContext( );
		ImGuiIO& io = ImGui::GetIO( );

		// setup handler
		this->m_handle = io.Fonts->AddFontFromFileTTF(font_location.c_str(), size);

		// calculate text size
		// this should work like this
		this->m_size = c_vector_2d(this->m_handle->CalcTextSizeA(size, FLT_MAX, -1, "A").x, 
			this->m_handle->CalcTextSizeA(size, FLT_MAX, -1, "A").y);

		// add a log
		printf("font: %s, has been created using size: %.2f, automatically calculated font size on text (A) for x pixels: %.2f and y pixels: %.2f\n",
			font_location.c_str(), size, this->m_size.x, this->m_size.y);

		ImGuiFreeType::BuildFontAtlas( io.Fonts, ImGuiFreeTypeBuilderFlags_Bold );
	}

	// font with cfg and gyplh
	c_font(const std::string& font_location, float size, const ImFontConfig* cfg_template, const ImWchar* glyph) {
		// ImFont* ImFontAtlas::AddFontFromFileTTF(const char* filename, float size_pixels, 
		// const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges)
		// if we point to the default constructor just add the default font that imgui gives us
		IMGUI_CHECKVERSION( );
		ImGui::CreateContext( );
		ImGuiIO& io = ImGui::GetIO( );

		// setup handler
		this->m_handle = io.Fonts->AddFontFromFileTTF(font_location.c_str(), size, cfg_template, glyph);

		// calculate text size
		// this should work like this
		this->m_size = c_vector_2d(this->m_handle->CalcTextSizeA(size, FLT_MAX, -1, "A").x,
			this->m_handle->CalcTextSizeA(size, FLT_MAX, -1, "A").y);

		// add a log
		printf("multi-mode font: %s, has been created using size: %.2f, automatically calculated font size on text (A) for x pixels: %.2f and y pixels: %.2f\n",
			font_location.c_str(), size, this->m_size.x, this->m_size.y);
	}

	// font from memory
	// imgui initializer
	// ImFont* ImFontAtlas::AddFontFromMemoryTTF(void* ttf_data, int ttf_size, float size_pixels, 
	// const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges)
	c_font(void* ttf_data, int ttf_size, float size) {
		// one layer font setup
		// if we point to the default constructor just add the default font that imgui gives us
		IMGUI_CHECKVERSION( );
		ImGui::CreateContext( );
		ImGuiIO& io = ImGui::GetIO( );

		// setup handler
		this->m_handle = io.Fonts->AddFontFromMemoryTTF(ttf_data, ttf_size, size);

		// calculate text size
		// this should work like this
		this->m_size = c_vector_2d(this->m_handle->CalcTextSizeA(size, FLT_MAX, -1, "A").x,
			this->m_handle->CalcTextSizeA(size, FLT_MAX, -1, "A").y);

		// add a log
		printf("memory font has been created with %i allocated bytes using drawing size: %.2f, automatically calculated font size on text (A) for x pixels: %.2f and y pixels: %.2f\n",
			ttf_size, size, this->m_size.x, this->m_size.y);
	}

	// multi-mode
	c_font(void* ttf_data, int ttf_size, float size, const ImFontConfig* cfg_template, const ImWchar* glyph) {
		// if we point to the default constructor just add the default font that imgui gives us
		IMGUI_CHECKVERSION( );
		ImGui::CreateContext( );
		ImGuiIO& io = ImGui::GetIO( );

		// setup handler
		this->m_handle = io.Fonts->AddFontFromMemoryTTF(ttf_data, ttf_size, size, cfg_template, glyph);

		// calculate text size
		// this should work like this
		this->m_size = c_vector_2d(this->m_handle->CalcTextSizeA(size, FLT_MAX, -1, "A").x,
			this->m_handle->CalcTextSizeA(size, FLT_MAX, -1, "A").y);

		// add a log
		printf("multi-mode memory font has been created with %i allocated bytes using drawing size: %.2f, automatically calculated font size on text (A) for x pixels: %.2f and y pixels: %.2f\n",
			ttf_size, size, this->m_size.x, this->m_size.y);
	}

	// font from compressed memory ttf
	// imgui intiializer
	// ImFont* ImFontAtlas::AddFontFromMemoryCompressedTTF(const void* compressed_ttf_data, int compressed_ttf_size, 
	// float size_pixels, const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges)
	c_font(const void* ttf_data, int ttf_size, float size) {
		// single-mode
		// if we point to the default constructor just add the default font that imgui gives us
		IMGUI_CHECKVERSION( );
		ImGui::CreateContext( );
		ImGuiIO& io = ImGui::GetIO( );

		// setup handler
		this->m_handle = io.Fonts->AddFontFromMemoryCompressedTTF(ttf_data, ttf_size, size);

		// calculate text size
		// this should work like this
		this->m_size = c_vector_2d(this->m_handle->CalcTextSizeA(size, FLT_MAX, -1, "A").x,
			this->m_handle->CalcTextSizeA(size, FLT_MAX, -1, "A").y);

		// add a log
		printf("multi-mode memory font has been created with %i allocated bytes using drawing size: %.2f, automatically calculated font size on text (A) for x pixels: %.2f and y pixels: %.2f\n",
			ttf_size, size, this->m_size.x, this->m_size.y);
	}

	// multi-mode
	c_font(const void* ttf_data, int ttf_size, float size, const ImFontConfig* cfg_template, const ImWchar* glyph) {
		// if we point to the default constructor just add the default font that imgui gives us
		IMGUI_CHECKVERSION( );
		ImGui::CreateContext( );
		ImGuiIO& io = ImGui::GetIO( );

		// setup handler
		this->m_handle = io.Fonts->AddFontFromMemoryCompressedTTF(ttf_data, ttf_size, size, cfg_template, glyph);

		// calculate text size
		// this should work like this
		this->m_size = c_vector_2d(this->m_handle->CalcTextSizeA(size, FLT_MAX, -1, "A").x,
			this->m_handle->CalcTextSizeA(size, FLT_MAX, -1, "A").y);

		// add a log
		printf("multi-mode memory font has been created with %i allocated bytes using drawing size: %.2f, automatically calculated font size on text (A) for x pixels: %.2f and y pixels: %.2f\n",
			ttf_size, size, this->m_size.x, this->m_size.y);
	}
public:
	__forceinline void string(float x, float y, std::string text, c_color color) {
		// push font
		ImGui::PushFont( m_handle );

		// drawlist
		g_render_globals.m_draw_list->AddText(ImVec2(x, y), color.convert(), text.c_str());

		ImGui::PopFont();
	}

	__forceinline void double_layered_string(float x, float y, std::string texth1, std::string texth2, c_color c1, c_color c2) {
		// push font
		ImGui::PushFont(m_handle);

		// drawlist
		g_render_globals.m_draw_list->AddText(ImVec2(x, y), c1.convert(), texth1.c_str());
		g_render_globals.m_draw_list->AddText(ImVec2(x + measure_size(texth1).x, y), c2.convert(), texth2.c_str());

		ImGui::PopFont();
	}

	__forceinline void string( float x, float y, std::string text, c_color color, e_font_flags flags, c_color second_color = c_color( 0, 0, 0, 200 ) ) {
		ImGui::PushFont( m_handle );

		// font flags
		if ( flags == e_font_flags::drop_shadow ) {
			this->string( x + 1, y + 1, text, second_color );
		} else if ( flags == e_font_flags::outline ) {
			this->string( x + 1, y - 1, text, second_color );
			this->string( x - 1, y + 1, text, second_color );
			this->string( x - 1, y - 1, text, second_color );
			this->string( x + 1, y + 1, text, second_color );

			this->string( x, y + 1, text, second_color );
			this->string( x, y - 1, text, second_color );
			this->string( x + 1, y, text, second_color );
			this->string( x - 1, y, text, second_color );
		}

		// draw normal text with no addons
		this->string( x, y, text, color );

		ImGui::PopFont( );
	}

	__forceinline c_vector_2d measure_size( std::string text ) {
		// we shouldnt have to push a new font as this
		// it should get it directly from the handler ptr
		// depends on how this is managed, i didnt properly check that much
		auto imgui_size = this->m_handle->CalcTextSizeA( this->m_handle->FontSize, FLT_MAX, -1.0f, text.c_str( ) );

		// return after we pop
		return c_vector_2d( imgui_size.x, imgui_size.y );
	}
private:

};

struct fonts_t {
	c_font verdana_bold{};
	c_font verdana{};
	c_font pixel{};
	c_font icons{};
	c_font icons_medium{};
	c_font icons_big{};
	c_font icons_shadowed{};
};

// heap
inline fonts_t g_font;

class c_render {
public:
	bool initialize_fonts();
	bool intialize_renderer( IDirect3DDevice9* device, HWND window );

	void override_draw_list(e_draw_list_data data);
public:
	void rect( float x, float y, float w, float h, c_color color, int rounding = 0, float tickness = 1 );
	void rect_filled( float x, float y, float w, float h, c_color color, int rounding = 0 );
	void shadow( float x, float y, float w, float h, c_color color, int rounding = 0, int shadow_tickness = 15 );
	void line( c_vector_2d from, c_vector_2d to, c_color color, int tickness = 1 );
	void circle( c_vector_2d position, c_color color, float radius, e_circle_flags flags );
	void gradient( float x, float y, float w, float h, c_color color, c_color color1, e_gradient_flags flags, int rounding = 0, c_color backround = c_color( 0, 0, 0, 0 ), ImDrawFlags drawing_flags = ImDrawFlags_RoundCornersAll );

	void inner_dropped_shadow(float x, float y, float w, float h, float size, c_color color);
private:
	ImDrawList* m_draw_list{};

	bool m_initialized{};
};
inline const auto g_render_engine = std::make_unique<c_render>();