#include "../../../../includes.hpp"
#pragma warning (disable : 4996)
#define slider_spacing 23

#include <codecvt>
std::string wstring_to_string( const wchar_t* wstr ) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes( wstr );
}

template <typename T>
__forceinline std::string precision( const T a_value, const int n = 3 ) {
	std::ostringstream out;
	out.precision( n );
	out << std::fixed << a_value;
	return out.str( );
}

void elements_t::slider( const std::string& label, int* var, int min, int max, const std::wstring& suffix ) {
	c_vector_2d position = m_ctx.m_window_pos + c_vector_2d( 0.f, m_ctx.m_offset - m_ctx.m_scroll_offset);
	c_vector_2d box_size = m_ctx.m_window_size - c_vector_2d(20, 0);

	auto hash = std::hash<std::string>( )( label + g_anim_base.m_cur_child );

	g_font.verdana.string( position.x + 23, position.y + 3, label, c_color( 200, 200, 200 ).modify_alpha(255 * g_content.m_window_i_alpha));

	position.y += 18;

	bool can_interact = g_utils.in_region( position + c_vector_2d( 23, 0 ), c_vector_2d( box_size.x - 46, 8 ) );
	bool clicked = can_interact && ImGui::IsMouseClicked( ImGuiMouseButton_Left );

	// drawing
	g_render_engine->rect_filled( position.x + 23, position.y, box_size.x - 46, 8, c_color(32, 32, 32).modify_alpha(255 * g_content.m_window_i_alpha));

	if ( g_content.m_blocking == 0 ) {
		if ( clicked )
			g_content.m_blocking = hash;
	} else if ( g_content.m_blocking == hash ) {
		if ( ImGui::IsMouseDown( ImGuiMouseButton_Left ) ) {
			float offset = std::clamp( ImGui::GetIO( ).MousePos.x - position.x - 23, 0.f, ( box_size.x - 46 ) );
			*var = std::clamp( g_utils.map_number<float>( offset, 0, ( box_size.x - 46 ), ( float )min, max ), ( float )min, ( float )max );
		} else {
			g_content.m_blocking = 0;
		}
	}

	auto hover_element2 = g_anim_base.build( label + "#auhb" + g_anim_base.m_cur_child );
	hover_element2.animate( hover_element2.m_value + 3.f * g_anim_base.delta_time( 0.5 ) * ( g_content.m_blocking == hash ? 1.f : -1.f ) );

	auto width = g_utils.map_number<float>( *var, min, max, 0.f, ( box_size.x - 46 ) );
	if ( *var != min ) {
		g_render_engine->rect_filled(position.x + 23, position.y, width, 8, m_ctx.m_theme.accent.modify_alpha(255 * g_content.m_window_i_alpha));
		g_render_engine->gradient(position.x + 23, position.y, width, 8, c_color(8, 8, 8, 0), c_color(8, 8, 8, 115).modify_alpha(120 * g_content.m_window_i_alpha), e_gradient_flags::horizontal); // .modify_alpha(255 * g_content.m_window_i_alpha)
	}

	g_render_engine->rect(position.x + 23, position.y, box_size.x - 46, 8, c_color(0, 0, 0).modify_alpha(255 * g_content.m_window_i_alpha));


	std::string full = std::to_string( *var ) + wstring_to_string( suffix.c_str( ) );
	c_vector_2d measure = g_font.verdana.measure_size( full );

	g_font.verdana.string( position.x + 23 + ( box_size.x - 46 ) - measure.x, position.y - 15, full, c_color( 200, 200, 200 ).modify_alpha(255 * g_content.m_window_i_alpha));

	m_ctx.m_offset += 28;

	// handle calculation
	g_group.handle_calculation( g_anim_base.group_id, 37 );
}

void elements_t::slider( const std::string& label, float* var, float min, float max, const std::wstring& suffix ) { 
	c_vector_2d position = m_ctx.m_window_pos + c_vector_2d( 0.f, m_ctx.m_offset - m_ctx.m_scroll_offset);
	c_vector_2d box_size = m_ctx.m_window_size - c_vector_2d(20, 0);

	auto hash = std::hash<std::string>( )( label + g_anim_base.m_cur_child );
	g_font.verdana.string(position.x + 23, position.y + 3, label, c_color(200, 200, 200).modify_alpha(255 * g_content.m_window_i_alpha));


	position.y += 18;

	bool can_interact = g_utils.in_region( position + c_vector_2d( 23, 0 ), c_vector_2d( box_size.x - 46, 8 ) );
	bool clicked = can_interact && ImGui::IsMouseClicked( ImGuiMouseButton_Left );

	// drawing
	g_render_engine->rect_filled(position.x + 23, position.y, box_size.x - 46, 8, c_color(32, 32, 32).modify_alpha(255 * g_content.m_window_i_alpha));
	if ( g_content.m_blocking == 0 ) {
		if ( clicked )
			g_content.m_blocking = hash;
	} else if ( g_content.m_blocking == hash ) {
		if ( ImGui::IsMouseDown( ImGuiMouseButton_Left ) ) {
			float offset = std::clamp( ImGui::GetIO( ).MousePos.x - position.x - 23, 0.f, ( box_size.x - 46 ) );
			*var = std::clamp( g_utils.map_number<float>( offset, 0, ( box_size.x - 46 ), ( float )min, max ), ( float )min, ( float )max );
		} else {
			g_content.m_blocking = 0;
		}
	}

	auto hover_element2 = g_anim_base.build( label + "#au54hb" + g_anim_base.m_cur_child );
	hover_element2.animate( hover_element2.m_value + 3.f * g_anim_base.delta_time( 0.5 ) * ( g_content.m_blocking == hash ? 1.f : -1.f ) );

	auto width = g_utils.map_number<float>( *var, min, max, 0.f, ( box_size.x - 46 ) );
	if (*var != min) {
		g_render_engine->rect_filled(position.x + 23, position.y, width, 8, m_ctx.m_theme.accent.modify_alpha(255 * g_content.m_window_i_alpha));
		g_render_engine->gradient(position.x + 23, position.y, width, 8, c_color(8, 8, 8, 0), c_color(8, 8, 8, 115).modify_alpha(120 * g_content.m_window_i_alpha), e_gradient_flags::horizontal); // .modify_alpha(255 * g_content.m_window_i_alpha)
	}

	g_render_engine->rect(position.x + 23, position.y, box_size.x - 46, 8, c_color(0, 0, 0).modify_alpha(255 * g_content.m_window_i_alpha));

	std::string full = precision( *var, 1 ) + wstring_to_string( suffix.c_str( ) );
	c_vector_2d measure = g_font.verdana.measure_size( full );

	g_font.verdana.string(position.x + 23 + (box_size.x - 46) - measure.x, position.y - 15, full, c_color(200, 200, 200).modify_alpha(255 * g_content.m_window_i_alpha));
	m_ctx.m_offset += 28;

	// handle calculation
	g_group.handle_calculation( g_anim_base.group_id, 37 );
}
