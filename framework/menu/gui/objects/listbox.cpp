#include "../../../../includes.hpp"

void elements_t::listbox( const std::string& label, int* var, std::vector<std::string> elements, int total_height, std::string filter ) { 
	c_vector_2d position = m_ctx.m_window_pos + c_vector_2d( 0.f, m_ctx.m_offset );
	c_vector_2d box_size = m_ctx.m_window_size;
	c_vector_2d size = c_vector_2d( box_size.x - 46, total_height - 10 );

	g_content.create_box( label, position, box_size, total_height );

	g_render_engine->rect( position.x + 23, position.y + 5, size.x, size.y, m_ctx.m_theme.accent );

	{
		if ( elements.empty( ) )
			return;

		// explanaition of empty {}
		// so i can return in this call and wont go further than the last }

		// iteration
		int start_y = position.y + 8;
		for ( int i = 0; i < elements.size( ); i++ ) {
			auto item = elements[ i ];

			// create animation
			auto builder = g_anim_base.build( item + std::to_string( i ) + g_anim_base.m_cur_child );
			builder.animate( builder.m_value + 3.f * g_anim_base.delta_time( 0.5 ) * ( *var == i ? 1.f : -1.f ) );

			// text size
			auto l_size = g_font.verdana.measure_size( item );

			// draw it
			g_font.verdana.string( position.x + 26 + ( 3 * builder.m_value ), start_y, item, c_color( ).blend( m_ctx.m_theme.accent, builder.m_value ) );

			// push new data
			start_y += l_size.y;
		}
	}


	m_ctx.m_offset += 157;

	// handle calculation
	g_group.handle_calculation( g_anim_base.group_id, 157 );
}