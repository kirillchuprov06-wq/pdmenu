#include "../../../../includes.hpp"

std::string to_upper( std::string s ) {
	std::transform( s.begin( ), s.end( ), s.begin( ),
		[ ]( unsigned char c ) { return toupper( c ); } );
	return s;
}

void elements_t::textinput( const std::string& label, std::string* var ) {
	c_vector_2d position = m_ctx.m_window_pos + c_vector_2d( 0.f, m_ctx.m_offset );
	c_vector_2d box_size = m_ctx.m_window_size - c_vector_2d(20, 0);
	c_vector_2d size = c_vector_2d(box_size.x, 20);

	auto this_hash = std::hash<std::string>( )( label + g_anim_base.m_cur_child );

	///g_content.create_box( label, position, box_size, 44 );
	g_font.verdana.string(position.x + 23, position.y + 3, label, (g_content.m_blocking == this_hash) ? c_color(200, 200, 200).modify_alpha(255 * g_content.m_window_i_alpha) : c_color(112, 112, 112).modify_alpha(255 * g_content.m_window_i_alpha));

	position.y += 18;

	g_render_engine->rect_filled(position.x + 23, position.y, box_size.x - 46, size.y, c_color(32, 32, 32).modify_alpha(255 * g_content.m_window_i_alpha), 2);
	g_render_engine->rect(position.x + 23, position.y, box_size.x - 46, size.y, c_color(0, 0, 0).modify_alpha(255 * g_content.m_window_i_alpha), 2);

	bool can_interact = g_utils.in_region( position + c_vector_2d( 23, 0 ), c_vector_2d( box_size.x - 46, 18 ) );
	bool clicked = can_interact && ImGui::IsMouseClicked( ImGuiMouseButton_Left );

	bool skip_next_frame = false;
	if ( clicked ) {
		g_content.m_blocking = this_hash;
		skip_next_frame = true;
	}

	if ( g_content.m_blocking == this_hash ) {
		if ( !g_utils.in_region( c_vector_2d( position.x + 23, position.y + 5 ), c_vector_2d( size.x, size.y ) ) && ImGui::IsMouseClicked( ImGuiMouseButton_Left ) ) {
			g_content.m_blocking = 0;
		}

		auto str = *var;

		for ( size_t i = 0; i <= 255; i++ ) {
			if ( ImGui::IsKeyPressed( i, false ) ) {
				if ( i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT || i == VK_DELETE ) {
					g_content.m_blocking = 0;
					continue;
				}

				if ( i == VK_SPACE ) {
					str.append( " " );
					continue;
				}

				if ( i == VK_BACK ) {
					str = str.substr( 0, strlen( str.data( ) ) - 1 );
					continue;
				}

				if ( i == VK_SHIFT ) {
					continue;
				}

				if ( key_names[ i ] != nullptr ) {
					if ( ImGui::IsKeyDown( ( ImGuiKey )VK_SHIFT ) ) {
						str.append( to_upper( key_names[ i ] ) );
					} else {
						str.append( key_names[ i ] );
					}
				}
			}
		}

		*var = str;
	}

	/*static */std::string preview{*var};

	// check if prewiew is empty
	if ( preview.empty( ) ) {
		preview = "no input data."; // push this so we can have it drawn and u know there is no input
	} else {
		preview = *var;
	}

	auto variable_size = g_font.verdana.measure_size( preview );

	// check if the size is larger than we can draw
	if ( variable_size.x > size.x - 20 ) {
		// resize
		preview.resize( 30 );
		preview += "...";
	}

	g_font.verdana.string( position.x + 27, position.y + 3, preview, (g_content.m_blocking == this_hash) ? c_color(200, 200, 200).modify_alpha(255 * g_content.m_window_i_alpha) : c_color(112,112,112).modify_alpha(255 * g_content.m_window_i_alpha));

	m_ctx.m_offset += 42;

	// handle calculation
	g_group.handle_calculation( g_anim_base.group_id, 51 );
}
