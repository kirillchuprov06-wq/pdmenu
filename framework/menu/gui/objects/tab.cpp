#include "../../../../includes.hpp"

void tab_controller_t::setup( context_t context ) {
	base.m_window_pos = context.m_window_pos;
	base.m_window_size = context.m_window_size;
	base.m_theme = context.m_theme;
}

void tab_controller_t::object( std::function<void()> fn ) { 
	base.m_has_tabs = true;

	elements.m_tab_index = 0;
	elements.m_tabs.clear( );

	fn( );
	invoke( );
}

void tab_controller_t::object( const std::string& tab_name, int* tab_index, std::vector<subtab_t> subtabs) {
	elements.m_tabs.push_back( { elements.m_tab_index++, tab_index, 0.f, tab_name, subtabs } );
}

void tab_controller_t::subtab(const std::string& parent, const std::string& tab_name, std::function<void()> contents)
{
	elements.m_subtabs.push_back({ elements.m_subtab_index++, 0.f, tab_name, contents });
}

void tab_controller_t::invoke( ) {
	int start_x = base.m_window_pos.x + 100;

	// do not run if there are no tabs
	if ( elements.m_tabs.empty( ) )
		return;

	//if (!g_content.m_window_i_alpha)
	//	return; 

	int subtab_start_y = base.m_window_pos.y + 40;
	//for ( int i = 0; i < elements.m_tabs.size( ); i++ ) {
	//	// tab pointer so we can access it faster
	//	auto& tab = elements.m_tabs[ i ];
	for (auto& tab : elements.m_tabs) {
		// measure tab size
		auto tab_pos = g_font.verdana.measure_size( tab.m_name );

		// do input system
		if ( g_utils.in_region( c_vector_2d( start_x, base.m_window_pos.y + 2 ), c_vector_2d( tab_pos.x, 20 ) ) && ImGui::IsMouseClicked( ImGuiMouseButton_Left ) ) {
			//*tab.m_active = i;

			// set this
			elements.m_last_tab = elements.m_cur_tab, elements.m_cur_tab = tab.m_index;
		}

		auto subtabs = tab.subtab;

		if (tab.m_index == elements.m_cur_tab) {
			for (auto& current_subtab : subtabs) {
				auto subtab_size = g_font.verdana.measure_size(current_subtab.name);

				// 17-9
				g_render_engine->gradient(base.m_window_pos.x + 17, subtab_start_y, 102, 18, c_color(17, 17, 17, 255 * g_content.m_window_i_alpha), c_color(9, 9, 9, 255 * g_content.m_window_i_alpha), e_gradient_flags::vertical);

				// *current_subtab.current_index, current_subtab.index
				// active data

				auto tabulia = g_anim_base.build(current_subtab.name + std::to_string(current_subtab.index) + "#subtab");
				tabulia.animate(tabulia.m_value + 3.f * g_anim_base.delta_time(0.5) * (*current_subtab.current_index == current_subtab.index && g_content.m_window_i_alpha > 0.f ? 1.f : -1.f));

				g_render_engine->rect_filled(base.m_window_pos.x + 17, subtab_start_y, 1, 18, base.m_theme.accent.modify_alpha(125 * g_content.m_window_i_alpha));
				g_render_engine->rect_filled(base.m_window_pos.x + 17, subtab_start_y, 1, 18, base.m_theme.accent.modify_alpha(255 * tabulia.m_value));

				if (g_utils.in_region(c_vector_2d(base.m_window_pos.x + 17, subtab_start_y), c_vector_2d(102, 18)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
					//*tab.m_active = i;
					//
					//// set this
					//elements.m_last_tab = elements.m_cur_tab = i;

					*current_subtab.current_index = current_subtab.index;
					*tab.m_active = current_subtab.index;
				}

				g_font.verdana.string(base.m_window_pos.x + 24, subtab_start_y + 2, current_subtab.name,
					c_color(112, 112, 112, 255 * g_content.m_window_i_alpha).blend(base.m_theme.accent.modify_alpha(255 * tabulia.m_value), tabulia.m_value));


				subtab_start_y += 21 * g_content.m_window_i_alpha;
			}
		}
		
		// animation for the tab that is selected
		auto tab_active = g_anim_base.build( tab.m_name + std::to_string(tab.m_index) );
		tab_active.animate( tab_active.m_value + 3.f * g_anim_base.delta_time( 0.5 ) * (tab.m_index == elements.m_cur_tab && g_content.m_window_i_alpha > 0.f ? 1.f : -1.f ) );

		auto tab_active2 = g_anim_base.build( tab.m_name + "#ciocan" + std::to_string(tab.m_index) );
		tab_active2.animate( tab_active2.m_value + 3.f * g_anim_base.delta_time( 0.4 ) * (tab.m_index == elements.m_cur_tab && g_content.m_window_i_alpha > 0.f ? 1.f : -1.f ) );

		ImGui::GetBackgroundDrawList()->PushClipRect(c_vector_2d(base.m_window_pos.x + 5, base.m_window_pos.y + 1).convert(), c_vector_2d(base.m_window_pos.x + base.m_window_size.x, base.m_window_pos.y + 1 + 20).convert(), true);

		g_render_engine->rect_filled(start_x - 5, base.m_window_pos.y + 2, tab_pos.x + 10, 22, c_color(11, 11, 11).modify_alpha(255 * tab_active2.m_value), 3);
		g_render_engine->rect(start_x - 5, base.m_window_pos.y + 2, tab_pos.x + 10, 22, c_color(32, 32, 32).modify_alpha(255 * tab_active2.m_value), 3);
		ImGui::GetBackgroundDrawList()->PopClipRect();

		ImGui::GetBackgroundDrawList()->PushClipRect(c_vector_2d(base.m_window_pos.x + 5, base.m_window_pos.y + 1).convert(), c_vector_2d(base.m_window_pos.x + base.m_window_size.x, base.m_window_pos.y + 1 + 6).convert(), true);
		c_vector_2d scalled = c_vector_2d((tab_pos.x + 10) * tab_active2.m_value, 1 * tab_active2.m_value);
		c_vector_2d offset = c_vector_2d(((tab_pos.x + 10) - scalled.x) * 0.5, (1 - scalled.y) * 0.5);

		auto bar_position = c_vector_2d(start_x - 5 + offset.x, base.m_window_pos.y + 2);

		g_render_engine->rect(bar_position.x, bar_position.y, scalled.x, 22, base.m_theme.accent.modify_alpha(255 * tab_active2.m_value), 3);

		ImGui::GetBackgroundDrawList()->PopClipRect();

		// render tabs
		g_font.verdana.string( start_x, base.m_window_pos.y + 4, tab.m_name, c_color(112, 112, 112, 255 * g_content.m_window_i_alpha).blend(base.m_theme.accent.modify_alpha(255 * tab_active.m_value), tab_active.m_value));

		// set this
		g_content.m_current_tab = elements.m_cur_tab;

		// iterate position for the next tabs
		start_x += tab_pos.x + 20 * g_content.m_window_i_alpha; // 15 should be ok
	}

	//elements.m_tabs.at( elements.m_cur_tab ).m_contents( );

	auto subtab_index = *elements.m_tabs.at(elements.m_cur_tab).m_active;
	elements.m_tabs.at(elements.m_cur_tab).subtab.at(subtab_index).content();
}