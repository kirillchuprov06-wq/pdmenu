#pragma once
#define spacing 8

// run it as inline so we dont get redefinition error
inline const char* const key_names[ ] = {
	"none",
	"m1",
	"m2",
	"something",
	"m3",
	"m4",
	"m5",
	"unk",
	"back",
	"tab",
	"unk",
	"unk",
	"clear",
	"enter",
	"unk",
	"unk",
	"shift",
	"control",
	"menu",
	"pause",
	"capital",
	"kana",
	"unk",
	"junja",
	"final",
	"kanja",
	"unk",
	"escape",
	"convert",
	"nonconvert",
	"accept",
	"modechange",
	"space",
	"prior",
	"next",
	"end",
	"home",
	"left",
	"up",
	"right",
	"down",
	"select",
	"print",
	"exec",
	"snap",
	"insert",
	"delete",
	"help",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"a",
	"b",
	"c",
	"d",
	"e",
	"f",
	"g",
	"h",
	"i",
	"j",
	"k",
	"l",
	"m",
	"n",
	"o",
	"p",
	"q",
	"r",
	"s",
	"t",
	"u",
	"v",
	"w",
	"x",
	"y",
	"z",
	"lwin",
	"rwin",
	"apps",
	"unk",
	"sleep",
	"num0",
	"num1",
	"num2",
	"num3",
	"num4",
	"num5",
	"num6",
	"num7",
	"num8",
	"num9",
	"multiply",
	"add",
	"separator",
	"subtract",
	"dec",
	"divide",
	"f1",
	"f2",
	"f3",
	"f4",
	"f5",
	"f6",
	"f7",
	"f8",
	"f9",
	"f10",
	"f11",
	"f12",
	"f13",
	"f14",
	"f15",
	"f16",
	"f17",
	"f18",
	"f19",
	"f20",
	"f21",
	"f22",
	"f23",
	"f24",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"numlock",
	"scroll",
	"VK_OEM_NEC_EQUAL",
	"VK_OEM_FJ_MASSHOU",
	"VK_OEM_FJ_TOUROKU",
	"VK_OEM_FJ_LOYA",
	"VK_OEM_FJ_ROYA",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"lshift",
	"rshit",
	"lcontrol",
	"rcontrol",
	"lwin",
	"rwin"
};

// here we will modify menu theme easier
struct theme_t {
	c_color backround = c_color( 19, 17, 20 );
	c_color accent = c_color(110, 131, 255);

	// title bar
	c_color title_bar_first = c_color( 34, 32, 35 );
	c_color title_bar_second = c_color( 39, 37, 41 );

	c_color child = c_color( 25, 23, 27 );
	c_color group_backround = c_color( 23, 22, 24 );
	c_color check_backround = c_color( 23, 21, 25 );
	c_color slider_backround = c_color( 45, 44, 47 );
};

// extern access it
inline theme_t g_theme;

struct context_t {
	c_vector_2d m_window_pos{}, m_window_size{};
	theme_t m_theme{};
};

// child stuff
enum e_child_placement {
	child_top_left,
	child_bottom_left,
	child_top_right,
	child_bottom_right,
};

enum e_child_size {
	child_size_half,
	child_size_full
};

struct child_content_t {
	std::string m_name;
	e_child_placement m_child_placement;
	e_child_size m_child_size;
	float m_offset;
	std::function<void( )> m_contents;

	int id;

	child_content_t( std::string name, e_child_placement child_placement, e_child_size child_size, float offset, std::function<void( )> contents, int id ) {
		this->m_name = name;
		this->m_child_placement = child_placement;
		this->m_child_size = child_size;
		this->m_offset = offset;
		this->m_contents = contents;
		this->id = id;
	}
};

struct group_content_t {
	int m_id;
	std::string m_name;
	std::function<void()> m_contents;
	bool m_first_instance;

	group_content_t(int id, std::string name, std::function<void()> contents, bool first_isntance) {
		this->m_id = id;
		this->m_name = name;
		this->m_contents = contents;
		this->m_first_instance = first_isntance;
	}
};

struct subtab_t {
	int index;
	std::string name;
	std::function<void()> content;
	int* current_index;
};

struct inner_content_t {
	int m_index{ };
	int* m_active{};
	float m_alpha{ };
	std::string m_name{ };
	//td::function<void( )> m_contents;
	std::vector<subtab_t> subtab;
};

struct inner_subtab_content_t {
	int index;
	float alpha;
	std::string name;
	std::function<void()> contents;
	std::string parent_name;
};

struct tab_controller_t {
	struct {
		bool m_has_tabs{ false };

		c_vector_2d m_window_pos{}, m_window_size{};
		theme_t m_theme{};
	} base;

	struct {
		int m_cur_tab{ 0 }, m_current_subtab{0}, m_last_tab{-1}, m_tab_index{0}, m_subtab_index{0};

		std::vector< inner_content_t > m_tabs{};
		std::vector<inner_subtab_content_t> m_subtabs{};
	} elements;

	void setup( context_t ctx );
	void object( std::function<void( )> content );
	void invoke( );

	void object( const std::string& tab_name, int* tab_index, std::vector<subtab_t> subtabs); // std::function<void( )> contents
	void subtab(const std::string& parent, const std::string& tab_name, std::function<void( )> contents );
};
inline tab_controller_t g_tab;

// we will update this in out child system so we can use element context in the group controller
// we dont need scrolling in the group so we wont add that
struct group_context_t {
	float m_offset{};

	c_vector_2d m_window_pos{}, m_window_size{};
	theme_t m_theme{};
};

struct element_context_t {
	float m_offset{};

	c_vector_2d m_window_pos{}, m_window_size{};
	theme_t m_theme;
	float m_scroll_offset{};
};

struct elements_t {
	void update(element_context_t d_context);
	element_context_t m_ctx;

	bool checkbox(const std::string& label, bool* var);

	void slider( const std::string& label, int* var, int min, int max, const std::wstring& suffix );
	void slider( const std::string& label, float* var, float min, float max, const std::wstring& suffix );
	void dropdown( const std::string& label, int* var, std::vector<std::string> items );
	void multi_selection( const std::string& label, std::vector<bool>* vars, std::vector<std::string> elements );

	void keybind( const std::string& lable, int* key, int* key_mode );
	void colorpicker( const std::string& lable, float* color );
	void button( const std::string& label, std::function<void( )> fn );

	void textinput( const std::string& label, std::string* var );
	void listbox( const std::string& label, int* var, std::vector < std::string > elements, int total_height = 150, std::string filter = "" );
};

struct group_controller_t {
	struct {
		float m_offset{};

		c_vector_2d m_window_pos{}, m_window_size{};
		theme_t m_theme{};
	} base;

	struct {
		std::vector<group_content_t> groups;
		bool m_opened[128]{ false };

		// as always we have to hardcode this shit a bit
		// im sorry for the people who see this
		// its not my fault ok ?
		float m_object_calculation[64][128]{ false };

		float m_obj[ 128 ]{ false };
	} elements;

	elements_t m_object{};

	void setup(group_context_t context);
	group_context_t m_ctx;

	void begin(std::function<void()> content);
	void object(const std::string& child_name, int id, std::function<void()> child_contents, bool first_instance = false);

	void handle_calculation( int id, float size ) {
		elements.m_obj[ id ] += size;
	}

	void invoke();
};
inline group_controller_t g_group;

struct child_controller_t {
	struct {
		c_vector_2d m_window_pos{}, m_window_size{};
		theme_t m_theme{};

		float window_header_size = 28.f;
		float child_header_size = 25;
	} base;

	struct {
		std::vector<child_content_t> childs;
	} elements;

	// child_elements_t child_elem;

	void setup( context_t context );
	void begin( std::function<void( )> content );
	void object( const std::string& child_name, e_child_placement child_placement, e_child_size child_size, std::function<void( )> child_contents, int id );
	void invoke( );
}; inline child_controller_t g_child;

struct util_t {
	bool in_region( c_vector_2d pos, c_vector_2d size ) {
		return ( ImGui::GetIO( ).MousePos.x > pos.x && ImGui::GetIO( ).MousePos.y > pos.y && 
			ImGui::GetIO( ).MousePos.x < pos.x + size.x && ImGui::GetIO( ).MousePos.y < pos.y + size.y );
	}

	c_vector_2d get_mouse_position( ) {
		return c_vector_2d( ImGui::GetMousePos( ).x, ImGui::GetMousePos( ).y );
	}

	template <typename T>
	inline T map_number( const T& input, const T& input_min, const T& input_max, const T& output_min, const T& output_max ) {
		return ( input - input_min ) / ( input_max - input_min ) * ( output_max - output_min ) + output_min;
	}

	int scroll_amount;
	void handle_scroll_amount( UINT msg, WPARAM param ) {
		if ( msg == WM_MOUSEWHEEL )
			scroll_amount = -( GET_WHEEL_DELTA_WPARAM( param ) / ( WHEEL_DELTA / 4 ) );
	}
}; inline util_t g_utils;

struct content_controller_t {
	float m_window_alpha = 0.f;
	float m_window_i_alpha = 0.f;
	size_t m_blocking = 0;

	int m_current_tab = 0;
	std::vector<float> m_alpha;

	float get_alpha( ) {
		return m_window_alpha;
	}

	std::vector< float* > m_saved_colors;

	void create_box(std::string name, c_vector_2d pos, c_vector_2d size, float ssize = 20) {
		auto hover_group_a = g_anim_base.build(name + "#hoveranimas" + g_anim_base.m_cur_child);
		hover_group_a.animate(hover_group_a.m_value + 3.f * g_anim_base.delta_time(0.5) *
			(g_utils.in_region(pos, c_vector_2d( size.x, ssize ) ) ? 1.f : -1.f));

		g_render_engine->shadow(pos.x, pos.y, size.x, ssize, g_theme.accent.modify_alpha(255 * hover_group_a.m_value));
		g_render_engine->rect_filled(pos.x, pos.y, size.x, ssize, g_theme.group_backround);
		g_render_engine->rect(pos.x, pos.y, size.x, ssize, g_theme.accent.modify_alpha(150));
		g_render_engine->inner_dropped_shadow(pos.x, pos.y, size.x, ssize, 5, g_theme.accent.modify_alpha(50));
	};
}; inline content_controller_t g_content;

struct window_t {
	bool m_open{ false };
	float m_alpha{ 0.f };
	int m_key = VK_INSERT;

	struct {
		struct {
			bool m_invoked{};
			c_vector_2d m_click_pos{}, m_menu_click_pos{};
		} dragging;

		c_vector_2d m_pos{}, m_size{}, m_min_size{};
		bool m_fill_size{ false };
		theme_t m_theme{};
	} util;

	// shared controlers.
	struct {
		tab_controller_t m_tab;
		child_controller_t m_child;
	} shared;

	void window_visiblity( bool& v );
	void window_movement( c_vector_2d& position, c_vector_2d size );
	void update_context( c_vector_2d pos, c_vector_2d size );

	void object( const std::string& title, c_vector_2d& position, c_vector_2d& size, std::function<void( )> fn );
};
inline window_t g_window;