#include "../../../../includes.hpp"

void elements_t::multi_selection( const std::string& label, std::vector<bool>* vars, std::vector<std::string> elements ) { 
	c_vector_2d position = m_ctx.m_window_pos + c_vector_2d( 0.f, m_ctx.m_offset - m_ctx.m_scroll_offset );
	c_vector_2d box_size = m_ctx.m_window_size - c_vector_2d(20, 0);
	c_vector_2d size = c_vector_2d(box_size.x, 20);

	auto center_drop = [](int x, int y, c_color c) -> void {
		/* first line of pixels */
		g_render_engine->rect_filled(x, y, 2, 2, c);
		g_render_engine->rect_filled(x + 3, y, 2, 2, c);
		g_render_engine->rect_filled(x + 6, y, 2, 2, c);

		/* 2nd line */
		g_render_engine->rect_filled(x, y + 3, 2, 2, c);
		g_render_engine->rect_filled(x + 3, y + 3, 2, 2, c);
		g_render_engine->rect_filled(x + 6, y + 3, 2, 2, c);

		/* last line */
		g_render_engine->rect_filled(x, y + 6, 2, 2, c);
		g_render_engine->rect_filled(x + 3, y + 6, 2, 2, c);
		g_render_engine->rect_filled(x + 6, y + 6, 2, 2, c);
		};

	auto hash = std::hash<std::string>( )( label + g_anim_base.m_cur_child );



	g_font.verdana.string(position.x + 23, position.y + 3, label, c_color(200, 200, 200).modify_alpha(255 * g_content.m_window_i_alpha));
	position.y += 18;

	// 23 - 15

	g_render_engine->gradient(position.x + 23, position.y, size.x - 46, size.y, c_color(23, 23, 23).modify_alpha(255 * g_content.m_window_i_alpha), c_color(15, 15, 15).modify_alpha(255 * g_content.m_window_i_alpha), e_gradient_flags::horizontal);


	bool can_interact = g_utils.in_region( position + c_vector_2d( 23, 0 ), c_vector_2d( box_size.x - 46, 18 ) );
	bool clicked = can_interact && ImGui::IsMouseClicked( ImGuiMouseButton_Left );

	bool skip_next_frame = false;
	if ( clicked ) {
		g_content.m_blocking = hash;
		skip_next_frame = true;
	}

	auto dropdown_opened = g_anim_base.build( label + "#drpoe12n" + g_anim_base.m_cur_child );
	dropdown_opened.animate( dropdown_opened.m_value + 3.f * g_anim_base.delta_time( 0.5 ) * ( g_content.m_blocking == hash ? 1.f : -1.f ) );

	

	int v_size = 0;
	for ( size_t i = 0; i < vars->size( ); i++ ) {
		if ( vars->at( i ) )
			v_size += 1;
	}

	std::string preview = v_size == 0 ? "-" : "";
	for ( auto i = 0, j = 0; i < vars->size( ); i++ ) {
		if ( vars->at( i ) ) {
			if ( j )
				preview += ( ", " ) + ( std::string )elements.at( i );
			else
				preview = elements.at( i );

			j++;
		}
	}

	auto sis = g_font.verdana.measure_size(preview).x;
	if (sis > size.x - 70)
		preview.resize(20);
	
	//g_font.verdana.string( position.x + 27, position.y + 7, preview, c_color( ) );
	g_font.verdana.string(position.x + 27, position.y + 3, preview, (g_content.m_blocking == hash) ? c_color(200, 200, 200).modify_alpha(255 * g_content.m_window_i_alpha) : c_color(112, 112, 112).modify_alpha(255 * g_content.m_window_i_alpha));

	center_drop(position.x + 23 + size.x - 61, position.y + 6, (g_content.m_blocking == hash) ? c_color(200, 200, 200).modify_alpha(255 * g_content.m_window_i_alpha) : c_color(112, 112, 112).modify_alpha(255 * g_content.m_window_i_alpha));
	g_render_engine->rect(position.x + 23, position.y, box_size.x - 46, size.y, c_color(0, 0, 0).modify_alpha(255 * g_content.m_window_i_alpha));

	position.y += 21;

	if ( g_content.m_blocking == hash || dropdown_opened.m_value > 0.f ) {
		g_render_engine->override_draw_list( e_draw_list_data::first_layer );

		float dropdown_size = ( 18 * elements.size( ) ) * dropdown_opened.m_value;
		if ( !g_utils.in_region( c_vector_2d( position.x + 27, position.y ), c_vector_2d( size.x, dropdown_size ) ) && ImGui::IsMouseClicked( ImGuiMouseButton_Left ) && !skip_next_frame ) {
			g_content.m_blocking = 0;
		}

		g_render_engine->rect_filled(position.x + 23, position.y + 5, size.x - 46, dropdown_size, c_color(18, 18, 18));
		g_render_engine->rect(position.x + 23, position.y + 5, size.x - 46, dropdown_size, c_color(0, 0, 0));

		// proper positionating
		int proper = position.y + 8;

		for ( int i = 0; i < elements.size( ); i++ ) {
			c_vector_2d text_calculation = g_font.verdana.measure_size( elements[ i ] );

			auto ani = g_anim_base.build( elements[ i ] + "#de21bos" + std::to_string( i ) + g_anim_base.m_cur_child );
			ani.animate( ani.m_value + 3.f * g_anim_base.delta_time( 0.5 ) * ( vars->at( i ) ? 1.f : -1.f ) );

			if ( g_utils.in_region( c_vector_2d( position.x + 27, proper ), c_vector_2d( size.x, 18 ) ) && ImGui::IsMouseClicked( ImGuiMouseButton_Left ) ) {
				vars->at( i ) = !vars->at( i );
			}

			//g_font.verdana.string( position.x + 27 + ( 3 * ani.m_value ), proper, elements[ i ], c_color( ).modify_alpha( 150 ).blend( m_ctx.m_theme.accent.modify_alpha( 255 ), ani.m_value ) );
			g_font.verdana.string(position.x + 27 + (3 * ani.m_value), proper, elements[i], c_color(200, 200, 200).modify_alpha(150).blend(m_ctx.m_theme.accent.modify_alpha(255), ani.m_value).modify_alpha(255 * dropdown_opened.m_value));

			proper += text_calculation.y + 4;
		}
	}
	else {
		skip_next_frame = false;
	}

	g_render_engine->override_draw_list( e_draw_list_data::second_layer );

	m_ctx.m_offset += 42;

	// handle calculation
	g_group.handle_calculation( g_anim_base.group_id, 51 );
}
