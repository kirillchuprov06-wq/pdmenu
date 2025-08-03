#include "../../../../includes.hpp"

void elements_t::button( const std::string& label, std::function<void( )> fn ) {
	c_vector_2d position = m_ctx.m_window_pos + c_vector_2d( 0.f, m_ctx.m_offset );
	c_vector_2d box_size = m_ctx.m_window_size - c_vector_2d(20, 0);
	c_vector_2d size = c_vector_2d(box_size.x, 20);

	//g_content.create_box( label, position, box_size, 30 );

	auto label_total_size = g_font.verdana.measure_size( label );
	c_vector_2d button_pos = c_vector_2d(
		position.x + 23 + ((size.x-46) * 0.5) - (label_total_size.x * 0.5), position.y + 2
	);

	//g_render_engine->rect_filled(position.x + 23, position.y, box_size.x - 46, size.y, c_color(32, 32, 32).modify_alpha(255 * g_content.m_window_i_alpha), 2);
	g_render_engine->gradient(position.x + 23, position.y, size.x - 46, size.y, c_color(23, 23, 23).modify_alpha(255 * g_content.m_window_i_alpha), c_color(15, 15, 15).modify_alpha(255 * g_content.m_window_i_alpha), e_gradient_flags::horizontal);
	g_render_engine->rect(position.x + 23, position.y, box_size.x - 46, size.y, c_color(0, 0, 0).modify_alpha(255 * g_content.m_window_i_alpha), 2);


	//g_render_engine->rect( position.x + 23, button_pos.y + 1, size.x, size.y, m_ctx.m_theme.accent );
	g_font.verdana.string( button_pos.x, button_pos.y + 2, label, c_color(200, 200, 200).modify_alpha(255 * g_content.m_window_i_alpha));

	if ( g_utils.in_region( c_vector_2d( position.x + 23, button_pos.y + 5 ), size ) && ImGui::IsMouseClicked( ImGuiButtonFlags_MouseButtonLeft ) ) {
		fn( );
	}

	m_ctx.m_offset += 28;

	// handle calculation
	g_group.handle_calculation( g_anim_base.group_id, 39 );
}
