#include "../../../../includes.hpp"

static std::vector<std::string> keys_type = { "Always", "Toggle", "Hold", "Hold off" };

void elements_t::keybind( const std::string& label, int* key, int* key_mode ) {
	auto this_hash = std::hash<std::string>( )( label + g_anim_base.m_cur_child );
	auto this_hash2 = std::hash<std::string>( )( label + "#otherigth" + g_anim_base.m_cur_child );

	std::string render_str = *key <= 0 ? "..." : key_names[ *key ] == nullptr ? "err" : key_names[ *key ];
	c_vector_2d key_size = g_font.pixel.measure_size( render_str ) + c_vector_2d(6, 0);

	c_vector_2d box_size = m_ctx.m_window_size;
	c_vector_2d position = m_ctx.m_window_pos + c_vector_2d( box_size.x - 20, m_ctx.m_offset /*- 27*/ );
	c_vector_2d size = c_vector_2d( 10, 10 );

	g_render_engine->override_draw_list( e_draw_list_data::first_layer );

	g_render_engine->rect( position.x + 5 - key_size.x, position.y + 5, key_size.x, size.y, m_ctx.m_theme.accent );
	g_font.pixel.string( position.x + 9 - key_size.x, position.y + 5, render_str, c_color( ), e_font_flags::outline );

	bool can_interact = g_utils.in_region( position - c_vector_2d( key_size.x, 0 ), c_vector_2d( box_size.x - 46, 15 ) );
	bool clicked = can_interact && ImGui::IsMouseClicked( ImGuiMouseButton_Left );
	bool clicked_r = can_interact && ImGui::IsMouseClicked( ImGuiMouseButton_Right );

	bool skip_next_frame = false;
	if ( clicked ) {
		g_content.m_blocking= this_hash;
		skip_next_frame = true;
	} else if ( clicked_r ) {
		g_content.m_blocking = this_hash2;
		skip_next_frame = true;
	}

	auto ania = g_anim_base.build( label + g_anim_base.m_cur_child );
	ania.animate( ania.m_value + 3.f * g_anim_base.delta_time( 0.5 ) * ( ( g_content.m_blocking == this_hash2 ) ? 1.f : -1.f ) );

	// do not go if we clicked right
	if (  g_content.m_blocking == this_hash  ) {
		if ( ImGui::IsKeyPressed( ImGuiKey_Escape, false ) || ImGui::IsKeyPressed( ImGuiKey_Insert, false ) ) {
			*key = 0;
			g_content.m_blocking = 0;
		} else {
			for ( auto i = 0; i < 5; i++ ) {
				if ( ImGui::GetIO( ).MouseClicked[ i ] && !skip_next_frame ) {
					switch ( i ) {
					case 0:
						*key = VK_LBUTTON;
						break;
					case 1:
						*key = VK_RBUTTON;
						break;
					case 2:
						*key = VK_MBUTTON;
						break;
					case 3:
						*key = VK_XBUTTON1;
						break;
					case 4:
						*key = VK_XBUTTON2;
						break;
					}

					g_content.m_blocking = 0;
				}
			}

			for ( int i = 0; i < 255; i++ ) {
				if ( ImGui::IsKeyPressed( i, false ) ) {
					*key = i;
					g_content.m_blocking = 0;
				}
			}
		}
	}

	{
		static auto drop_size = 15 * keys_type.size( );
		static c_vector_2d new_pos = c_vector_2d( position.x + 5, position.y + 17 );
		g_render_engine->rect_filled( new_pos.x, new_pos.y, 70, drop_size, m_ctx.m_theme.group_backround.modify_alpha( ania.m_value * 255 ) );
		g_render_engine->rect( new_pos.x, new_pos.y, 70, drop_size, m_ctx.m_theme.accent.modify_alpha( ania.m_value * 255 ) );

		int new_y = new_pos.y + 2;
		for ( int i2 = 0; i2 < keys_type.size( ); i2++ ) {
			auto key = keys_type[ i2 ];
			auto key_size = g_font.verdana.measure_size( key );

			if ( g_utils.in_region( c_vector_2d( new_pos.x + 5, new_y ), c_vector_2d( 70, 15 ) ) && ImGui::IsMouseClicked( ImGuiMouseButton_Left ) && ania.m_value > 0.f ) {
				*key_mode = i2;
				g_content.m_blocking = 0;
			}

			auto selected = g_anim_base.build( keys_type[ i2 ] + std::to_string( new_y ) + g_anim_base.m_cur_child );
			selected.animate( selected.m_value + 3.f + g_anim_base.delta_time( 0.5 ) * ( ( *key_mode == i2 ) ? 1.f : -1.f ) );

			g_font.verdana.string( new_pos.x + 5, new_y, key, ( ( *key_mode == i2 ) ? m_ctx.m_theme.accent.modify_alpha( 255 * ania.m_value ) : c_color( ).modify_alpha( 255 * ania.m_value ) ) );

			new_y += key_size.y;
		}
	}


	g_render_engine->override_draw_list( e_draw_list_data::second_layer );
}
