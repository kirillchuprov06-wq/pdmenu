#include "../../../../includes.hpp"

c_color port_to_color_system( float* var ) {
	return c_color( var[ 0 ] * 255.f, var[ 1 ] * 255.f, var[ 2 ] * 255.f, var[ 3 ] * 255.f );
}

void elements_t::colorpicker( const std::string& lable, float* color ) { 
	auto this_hash = std::hash<std::string>( )( lable + g_anim_base.m_cur_child );
	auto this_hash2 = std::hash<std::string>( )( lable + "#otheri211gth" + g_anim_base.m_cur_child );

	c_vector_2d box_size = m_ctx.m_window_size;
	c_vector_2d position = m_ctx.m_window_pos + c_vector_2d( box_size.x - 30, m_ctx.m_offset /*- 27*/ );
	c_vector_2d size = c_vector_2d( 16, 10 );

	g_render_engine->override_draw_list( e_draw_list_data::first_layer );

	g_render_engine->rect_filled( position.x + 5, position.y + 5, size.x, size.y, port_to_color_system( color ) );
	g_render_engine->rect( position.x + 5, position.y + 5, size.x, size.y, m_ctx.m_theme.group_backround, 0, 2 );
	g_render_engine->rect( position.x + 5, position.y + 5, size.x, size.y, m_ctx.m_theme.accent );

	bool can_interact = g_utils.in_region( position - c_vector_2d( 16, 0 ), c_vector_2d( box_size.x - 46, 15 ) );
	bool clicked = can_interact && ImGui::IsMouseClicked( ImGuiMouseButton_Left );
	bool clicked_r = can_interact && ImGui::IsMouseClicked( ImGuiMouseButton_Right );

	bool skip_next_frame = false;
	if ( clicked && ( g_content.m_blocking == 0 ) ) {
		g_content.m_blocking = this_hash;
		skip_next_frame = true;
	} else if ( clicked_r ) {
		g_content.m_blocking = this_hash2;
		skip_next_frame = true;
	}

	auto ania = g_anim_base.build( lable + g_anim_base.m_cur_child );
	ania.animate( ania.m_value + 3.f * g_anim_base.delta_time( 0.5 ) * ( ( g_content.m_blocking == this_hash ) ? 1.f : -1.f ) );

	auto ania2 = g_anim_base.build( lable + "#sasdga" + g_anim_base.m_cur_child );
	ania2.animate( ania2.m_value + 3.f * g_anim_base.delta_time( 0.5 ) * ( ( g_content.m_blocking == this_hash2 ) ? 1.f : -1.f ) );

	static int blocking = 0;
	if ( ania.m_value > 0.f ) {
		c_vector_2d new_pos = c_vector_2d( position.x, position.y + 12 );
		static c_vector_2d picker_size = c_vector_2d( 160, 175 );

		g_render_engine->rect_filled( new_pos.x, new_pos.y, picker_size.x, picker_size.y, m_ctx.m_theme.group_backround.modify_alpha( 255 * ania.m_value ) );
		g_render_engine->rect( new_pos.x, new_pos.y, picker_size.x, picker_size.y, m_ctx.m_theme.accent.modify_alpha( 255 * ania.m_value ) );

		g_render_engine->rect_filled( new_pos.x + 2, new_pos.y + 1, picker_size.x - 4, 20, m_ctx.m_theme.title_bar_first.modify_alpha( 255 * ania.m_value ) );
		g_font.verdana.string( new_pos.x + 6, new_pos.y + 4, "Choose a color", c_color( ).modify_alpha( 255 * ania.m_value ), e_font_flags::drop_shadow );

		if ( !g_utils.in_region( new_pos, picker_size ) && ImGui::IsMouseClicked( ImGuiMouseButton_Left ) ) {
			g_content.m_blocking = 0;
		}

		c_vector_2d second_pos = c_vector_2d( new_pos.x + 7, new_pos.y + 28 );
		static c_vector_2d second_size = c_vector_2d( 130, 130 );

		c_vector_2d alpha_pos = c_vector_2d( second_pos.x, second_pos.y + 135 );
		static c_vector_2d alpha_size = c_vector_2d( 130, 6 ); // we wont update this

		float h, s, v;
		float r{}, g{}, b{};
		float display_r, display_g, display_b;
		ImGui::ColorConvertRGBtoHSV( color[ 0 ], color[ 1 ], color[ 2 ], h, s, v );
		ImGui::ColorConvertHSVtoRGB( h, 1.f, 1.f, display_r, display_g, display_b );
	
		if ( g_utils.in_region( second_pos, second_size ) && ImGui::IsMouseClicked( ImGuiMouseButton_Left ) ) {
			blocking = 1;
		}

		if ( g_utils.in_region( c_vector_2d( second_pos.x + second_size.x + 8, second_pos.y ), c_vector_2d( 6, 130 ) ) && ImGui::IsMouseClicked( ImGuiMouseButton_Left ) ) {
			blocking = 2;
		}

		if ( g_utils.in_region( c_vector_2d( alpha_pos.x, alpha_pos.y ), c_vector_2d( 130, 6 ) ) && ImGui::IsMouseClicked( ImGuiMouseButton_Left ) ) {
			blocking = 3;
		}

		if ( !ImGui::IsMouseDown( ImGuiMouseButton_Left ) ) {
			blocking = 0;
		}

		if ( blocking == 1 ) {
			s = std::clamp( ( ImGui::GetIO( ).MousePos.x - second_pos.x ) / second_size.x, 0.01f, 1.f );
			v = std::clamp( 1.f - ( ImGui::GetIO( ).MousePos.y - second_pos.y ) / second_size.y, 0.01f, 1.f );
		}

		if ( blocking == 2 ) {
			h = std::clamp( ( ImGui::GetIO( ).MousePos.y - second_pos.y ) / 130, 0.01f, 1.f );
		}

		if ( blocking == 3 ) {
			color[ 3 ] = std::clamp( ( ImGui::GetIO( ).MousePos.x - alpha_pos.x ) / alpha_size.x, 0.01f, 1.f );
		}

		g_render_engine->gradient( second_pos.x, second_pos.y, second_size.x, second_size.y, c_color( ).modify_alpha( 255 * ania.m_value ), c_color( display_r * 255, display_g * 255, display_b * 255 ).modify_alpha( 255 * ania.m_value ), e_gradient_flags::vertical);
		g_render_engine->gradient( second_pos.x, second_pos.y, second_size.x, second_size.y, c_color( 0, 0, 0, 0 ), c_color( 0, 0, 0 ).modify_alpha( 255 * ania.m_value ), e_gradient_flags::horizontal );
		g_render_engine->rect( second_pos.x, second_pos.y, second_size.x, second_size.y, m_ctx.m_theme.accent.modify_alpha( 255 * ania.m_value ) );

		const c_color col_hues[ 6 + 1 ] = { c_color( 255,0,0 ).modify_alpha( 255 * ania.m_value ), c_color( 255,255,0 ).modify_alpha( 255 * ania.m_value ),
			c_color( 0,255,0 ).modify_alpha( 255 * ania.m_value ), c_color( 0,255,255 ).modify_alpha( 255 * ania.m_value ), c_color( 0,0,255 ).modify_alpha( 255 * ania.m_value ), c_color( 255,0,255 ).modify_alpha( 255 * ania.m_value ), c_color( 255,0,0 ).modify_alpha( 255 * ania.m_value ) };
	
		for ( size_t i = 0; i < 6; i++ ) {
			g_render_engine->gradient( second_pos.x + second_size.x + 8, second_pos.y + ( ( 131.1 / 6 ) * i ), 5, ( 131.1 / 6 ), col_hues[ i ], col_hues[ i + 1 ], e_gradient_flags::horizontal );
		}

		g_render_engine->rect( second_pos.x + second_size.x + 8, second_pos.y, 5, 130, m_ctx.m_theme.accent );

		// first line
		int first_line_start = second_pos.x + 2;
		int second_line_start = second_pos.x + 4;
		for ( int first_line = 0; first_line < 32; first_line++ ) {
			g_render_engine->rect_filled( first_line_start, alpha_pos.y + 1, 2, 2, c_color( 255, 255, 255, 150 ) );
			g_render_engine->rect_filled( second_line_start, alpha_pos.y + 3, 2, 2, c_color( 255, 255, 255, 150 ) );
			first_line_start += 4;
			second_line_start += 4;
		}

		g_render_engine->gradient( alpha_pos.x, alpha_pos.y, alpha_size.x, alpha_size.y, port_to_color_system( color ).modify_alpha( 0 ), port_to_color_system( color ).modify_alpha(255), e_gradient_flags::vertical );
		g_render_engine->rect( alpha_pos.x, alpha_pos.y, alpha_size.x, alpha_size.y, m_ctx.m_theme.accent );

		ImGui::ColorConvertHSVtoRGB( h, s, v, color[ 0 ], color[ 1 ], color[ 2 ] );
	}

	if ( ania2.m_value > 0.f ) {
		c_vector_2d new_pos = c_vector_2d( position.x, position.y + 12 );
		static c_vector_2d picker_size = c_vector_2d( 175, 60 );

		g_render_engine->rect_filled( new_pos.x, new_pos.y, picker_size.x, picker_size.y, m_ctx.m_theme.group_backround.modify_alpha( 255 * ania2.m_value ) );
		g_render_engine->rect( new_pos.x, new_pos.y, picker_size.x, picker_size.y, m_ctx.m_theme.accent.modify_alpha( 255 * ania2.m_value ) );

		if ( !g_utils.in_region( new_pos, picker_size ) && ImGui::IsMouseClicked( ImGuiMouseButton_Left ) ) {
			g_content.m_blocking = 0;
		}

		c_vector_2d plus_pos = c_vector_2d( new_pos.x + 7, new_pos.y + 7 );

		g_render_engine->rect( plus_pos.x, plus_pos.y, 12, 12, m_ctx.m_theme.accent );
		g_font.verdana.string( plus_pos.x + 2, plus_pos.y - 2, "+", c_color( ) );

		// push new color
		if ( g_utils.in_region( plus_pos, c_vector_2d( 12, 12 ) ) && ImGui::IsMouseClicked( ImGuiMouseButton_Left ) ) {
			if ( g_content.m_saved_colors.size( ) < 8) // max color index
				g_content.m_saved_colors.push_back( color );
		}

		// max colors: null
		int new_color_start = plus_pos.x + 18; // 12 + 5 spacing
		for ( int saved = 0; saved < g_content.m_saved_colors.size( ); saved++ ) {
			auto saved_data = g_content.m_saved_colors[ saved ];

			g_render_engine->rect_filled( new_color_start, plus_pos.y, 12, 12, port_to_color_system( saved_data ) );

			// set the color system
			if ( g_utils.in_region( c_vector_2d( new_color_start, plus_pos.y ), c_vector_2d( 12, 12 ) ) && ImGui::IsMouseClicked( ImGuiMouseButton_Left ) ) {
				color = saved_data;
			}


			new_color_start += 18;
		}
	}

	g_render_engine->override_draw_list( e_draw_list_data::second_layer );
}
