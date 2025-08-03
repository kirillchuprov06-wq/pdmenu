#include "../../../../includes.hpp"

// menu close and menu open
// handle throught this
void window_t::window_visiblity( bool& v ) {
	// call this once with & 1 so it doesnt fuck around and work while u hold the menu open key
	// think like this, is a key_pressed returnable
	if ( GetAsyncKeyState( m_key ) & 1 )
		v = !v;
}

// handle window movement
void window_t::window_movement( c_vector_2d& position, c_vector_2d size ) {
	// static variables so we can update them later
	static bool drag{ false };
	static c_vector_2d offset{};

	// get mouse delta and position and mouse delta
	auto mouse = g_utils.get_mouse_position( );
	c_vector_2d delta = mouse - offset;

	// check if we our drag variable is true and we are not clicking anywhere
	if ( drag && !GetAsyncKeyState( VK_LBUTTON ) ) {
		drag = false; // set this retard to false
	}

	// update position
	if ( drag && GetAsyncKeyState( VK_LBUTTON ) ) {
		position = delta;
	}

	// update the mouse position
	if ( g_utils.in_region( position, c_vector_2d( 150, 30 ) ) ) {
		drag = true;
		offset = mouse - position;
	}

	// note: "TO DO"
	// block drawing if menu gets out of the screen

	// clamping so we fix shit bugs
	if ( position.x < 0 ) { position.x = 0; }
	if ( position.y < 0 ) { position.y = 0; }
}

// update window context
void window_t::update_context( c_vector_2d position, c_vector_2d size ) {
	util.m_pos = position;
	util.m_size = size;
	if ( !util.m_fill_size ) {
		util.m_min_size = size;
		util.m_fill_size = true;
	}
}

std::string to_lower(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return tolower(c); });
	return s;
}

void window_t::object( const std::string& title, c_vector_2d& position, c_vector_2d& size, std::function<void( )> fn ) {
	// handle menu visibility
	window_visiblity( m_open );

	if ( !m_open )
		g_content.m_blocking = 0;

	// window lerping so we make it smooth open / close
	m_alpha = std::clamp( m_alpha + 3.f * g_anim_base.delta_time( 0.4f ) * ( m_open ? 1.f : -1.f ), 0.f, 1.f );
	g_content.m_window_alpha = m_alpha;

	auto menu_initialized = g_anim_base.build("initialized state");
	menu_initialized.animate(menu_initialized.m_value + 3.f * g_anim_base.delta_time(0.5) * (m_alpha >= 0.99f ? 1.f : -1.f));

	auto menu_initialized2 = g_anim_base.build("initialized state2");
	menu_initialized2.animate(menu_initialized2.m_value + 3.f * g_anim_base.delta_time(0.30) * (menu_initialized.m_value >= 0.99f ? 1.f : -1.f));
	g_content.m_window_i_alpha = menu_initialized2.m_value;

	// if we have no alpha data just return this fucker
	if ( !m_alpha )
		return; // exit;

	// update window context and window movement
	update_context( position, size );
	window_movement( position, size );

	//// drawing
	g_render_engine->rect_filled( 0, 0, 2560, 1440, c_color( 0, 0, 0, 35 * m_alpha ) );

	g_render_engine->rect_filled(position.x, position.y, size.x, size.y, c_color(11, 11, 11).modify_alpha(255 * m_alpha), 3);
	g_render_engine->rect_filled(position.x, position.y, size.x, 20, c_color(8, 8, 8).modify_alpha(255 * m_alpha), 3);
	g_render_engine->rect(position.x, position.y, size.x, 20, c_color(0, 0, 0).modify_alpha(255 * m_alpha), 0);
	g_render_engine->rect_filled(position.x, position.y + 20, size.x, 1, c_color(32, 32, 32).modify_alpha(255 * m_alpha), 3);

	ImGui::GetBackgroundDrawList()->PushClipRect(c_vector_2d(position.x + 5, position.y + 1).convert(), c_vector_2d(position.x + 5 + g_font.icons_shadowed.measure_size("2").x, position.y + 1 + 18).convert(), true);
	g_font.icons_shadowed.string(position.x + 5, position.y + 1, "2", util.m_theme.accent.modify_alpha(70 * m_alpha));
	ImGui::GetBackgroundDrawList()->PopClipRect();

	g_font.verdana.double_layered_string(position.x + 5, position.y + 3, "phant", "osia", c_color(200, 200, 200).modify_alpha(255 * m_alpha), util.m_theme.accent.modify_alpha(255 * m_alpha));

	g_render_engine->gradient(position.x + size.x * (1.0f - menu_initialized2.m_value), position.y + 22, size.x * menu_initialized2.m_value, 1, util.m_theme.accent.modify_alpha(0), util.m_theme.accent.modify_alpha(255), e_gradient_flags::vertical);

	g_render_engine->rect_filled(position.x, position.y + (size.y - 20), size.x, 20, c_color(8, 8, 8).modify_alpha(255 * m_alpha), 3);
	g_render_engine->rect(position.x, position.y + (size.y - 20), size.x, 20, c_color(0, 0, 0).modify_alpha(255 * m_alpha), 0);
	g_render_engine->rect_filled(position.x, position.y + 20 + (size.y - 41), size.x, 1, c_color(32, 32, 32).modify_alpha(255 * m_alpha), 3);

	const std::string compilation_date = __DATE__;

	g_font.verdana.double_layered_string(position.x + 5, position.y + 3 + (size.y - 19), "date: ", to_lower(compilation_date), c_color().modify_alpha(255 * m_alpha), util.m_theme.accent.modify_alpha(255 * m_alpha));
	g_font.verdana.double_layered_string(position.x + 5 + g_font.verdana.measure_size( std::string("date: " + compilation_date)).x, position.y + 3 + (size.y - 19), " ~ ", "dev", c_color(200, 200, 200).modify_alpha(255 * m_alpha), util.m_theme.accent.modify_alpha(255 * m_alpha));

	g_font.verdana.double_layered_string(position.x - 5 + size.x - g_font.verdana.measure_size("counter strike 2").x, position.y + 3 + (size.y - 19), "", "counter strike 2", c_color(200, 200, 200).modify_alpha(255 * m_alpha), util.m_theme.accent.modify_alpha(255 * m_alpha));

	// for subtabs
	g_render_engine->rect_filled(position.x + 12, position.y + 34, 118, 363, c_color(8, 8, 8).modify_alpha(255 * m_alpha));
	g_render_engine->rect(position.x + 12, position.y + 34, 118, 363, c_color(32, 32, 32).modify_alpha(255 * m_alpha));


	g_render_engine->rect(position.x - 1 , position.y - 1, size.x + 2, size.y + 2, c_color(32, 32, 32).modify_alpha(255 * m_alpha), 3);
	g_render_engine->rect(position.x - 2 , position.y - 2, size.x + 4, size.y + 4, c_color(0, 0, 0).modify_alpha(255 * m_alpha), 3);

	// clear and empty theme struct as we dont need to init anything there
	// update tab context
	g_tab.setup( { position, size, theme_t( ) } );

	// run other stuff that are inited in the lambda
	fn( );
}