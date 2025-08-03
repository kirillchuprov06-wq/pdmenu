#include "../../../../includes.hpp"

bool elements_t::checkbox( const std::string& label, bool* var ) {
	c_vector_2d position = m_ctx.m_window_pos + c_vector_2d( 0.f, m_ctx.m_offset - m_ctx.m_scroll_offset );
	c_vector_2d box_size = m_ctx.m_window_size;
	c_vector_2d size = c_vector_2d( 8, 8 );

	// enale checkbox
	if ( g_content.m_blocking == 0 && g_utils.in_region( position, c_vector_2d( box_size.x - 50, 20 ) ) && 
		ImGui::IsMouseClicked( ImGuiMouseButton_Left ) ) {
		*var = !*var;
	}

	auto check_enabled = g_anim_base.build( label + "#checkenabled" + g_anim_base.m_cur_child );
	check_enabled.animate(check_enabled.m_value + 3.f * g_anim_base.delta_time(0.5) * (*var && (g_content.m_window_i_alpha > 0.f) ? 1.f : -1.f));

	auto hover_anim = g_anim_base.build(label + "#hoverrino" + g_anim_base.m_cur_child);
	hover_anim.animate(hover_anim.m_value + 3.f * g_anim_base.delta_time(0.5) * ((g_utils.in_region(position, c_vector_2d(box_size.x - 50, 20)) && (g_content.m_window_i_alpha > 0.f)) && !*var ? 1.f : -1.f));

	g_render_engine->rect_filled(position.x + 5, position.y + 5, size.x, size.y, c_color(32, 32, 32).blend(m_ctx.m_theme.accent, check_enabled.m_value).modify_alpha(255 * g_content.m_window_i_alpha)); // .modify_alpha(255 * g_content.m_window_i_alpha
	g_render_engine->rect_filled(position.x + 5, position.y + 5, size.x, size.y, m_ctx.m_theme.accent.modify_alpha(100 * hover_anim.m_value)); // .modify_alpha(255 * g_content.m_window_i_alpha



	g_render_engine->gradient(position.x + 5, position.y + 5, size.x, size.y, c_color(8, 8, 8, 0 * check_enabled.m_value), c_color(8, 8, 8, 120 * check_enabled.m_value), e_gradient_flags::horizontal); // .modify_alpha(255 * g_content.m_window_i_alpha)
	g_render_engine->rect(position.x + 5, position.y + 5, size.x, size.y, c_color(0, 0, 0).modify_alpha(255 * g_content.m_window_i_alpha));

	g_font.verdana.string( position.x + 23, position.y + 2, label, c_color(112, 112, 112).blend(c_color(200, 200, 200), check_enabled.m_value).modify_alpha(255 * g_content.m_window_i_alpha));
	g_font.verdana.string( position.x + 23, position.y + 2, label, c_color(112, 112, 112).blend(c_color(200, 200, 200), hover_anim.m_value).modify_alpha(100 * hover_anim.m_value));

	m_ctx.m_offset += 8 + spacing;

	return *var;
}