#include "../../../../includes.hpp"

void child_controller_t::setup( context_t context ) {
	base.m_window_pos = context.m_window_pos;
	base.m_window_size = context.m_window_size;
	base.m_theme = context.m_theme;
}

void child_controller_t::begin( std::function<void( )> fn ) {
	elements.childs.clear( );

	fn( );
	invoke( );
}

void child_controller_t::object( const std::string& child_name, e_child_placement child_placement, e_child_size child_size, std::function<void( )> child_contents, int id ) { 
	elements.childs.push_back( child_content_t( child_name, child_placement, child_size, 0.f, child_contents, id ) );
}

void child_controller_t::invoke( ) {
    static bool interacted[1000]{ false };

	for ( auto& child : elements.childs ) {
		c_vector_2d position;

        auto size = c_vector_2d(217, 363);// base.m_window_size - c_vector_2d(18, 39);

        if ( child.m_child_size == child_size_full ) {
            size = c_vector_2d(217, 363);
            //size.x /= 2.f;
        } else {
            size -= c_vector_2d( 27, 30 );
            size.y /= 2.f;
            size.x /= 2.f;
        }

        switch ( child.m_child_placement ) {
        case child_top_left:
            position = base.m_window_pos + c_vector_2d( 142, 34 );
            break;
        case child_bottom_left:
            position = base.m_window_pos + c_vector_2d( 15, 40 + size.y + 13.f );
            break;
        case child_top_right:
            position = base.m_window_pos + c_vector_2d(142 + size.x + 12.f, 34 );
            break;
        case child_bottom_right:
            position = base.m_window_pos + c_vector_2d( 15 + size.x + 13.f, 40 + size.y + 13.f );
            break;
        }

        // && g_content.m_window_i_alpha > 0.f

        // 16.x3 - 9.x3

        g_render_engine->rect_filled(position.x, position.y, size.x, size.y, c_color(8, 8, 8).modify_alpha(255 * g_content.m_window_i_alpha));

        g_render_engine->gradient(position.x, position.y, size.x, 21, c_color(16, 16, 16, 255 * g_content.m_window_i_alpha), c_color(9, 9, 9, 255 * g_content.m_window_i_alpha), e_gradient_flags::horizontal);
        g_render_engine->gradient(position.x, position.y + 22, 100, 1, c_color(32, 32, 32, 255 * g_content.m_window_i_alpha), c_color(8, 8, 8, 255 * g_content.m_window_i_alpha), e_gradient_flags::vertical);
        g_render_engine->gradient(position.x + size.x - 100, position.y + 22, 100, 1, c_color(8, 8, 8, 255 * g_content.m_window_i_alpha), c_color(32, 32, 32, 255 * g_content.m_window_i_alpha), e_gradient_flags::vertical);

        g_font.verdana.string(position.x + 6, position.y + 4, child.m_name, c_color(112, 112, 112, 255 * g_content.m_window_i_alpha));

        g_render_engine->rect(position.x, position.y, size.x, size.y, c_color(32, 32, 32).modify_alpha(255 * g_content.m_window_i_alpha));
      
       // g_group.setup({ 15, position + c_vector_2d(15, 40), c_vector_2d(size.x - 30, size.y - 30), base.m_theme });

        auto scroll_amount = g_anim_base.build(child.m_name + "#scroll_amount");
       // auto interf = g_anim_base.build(child.m_name + "#inetrafce");
        auto scroll_offset_static = g_anim_base.build(child.m_name + "#scroll_offset_static");
        auto scroll_offset_interpolated = g_anim_base.build(child.m_name + "#scroll_offset_interpolated");
        auto visible_size = size.y - 30;
        auto scroll_max = scroll_amount.m_value - visible_size;

        g_group.m_object.update({ 15, c_vector_2d(position.x + 10, position.y + 15), c_vector_2d(size.x, size.y), base.m_theme, scroll_offset_interpolated.m_value });
    
        if (scroll_offset_interpolated.m_value != scroll_offset_static.m_value) {
            scroll_offset_interpolated.m_value += (scroll_offset_static.m_value - scroll_offset_interpolated.m_value) * 0.2f;
            scroll_offset_interpolated.animate(scroll_offset_interpolated.m_value, false);
        }

        if (scroll_amount.m_value > visible_size && g_content.m_blocking == 0 && g_utils.in_region(position + c_vector_2d(0, 30), c_vector_2d(size.x, size.y - 30))) {
            if (ImGui::GetIO().MouseWheel > 0) {
                scroll_offset_static.animate(std::clamp(scroll_offset_static.m_value - 15, 0.f, scroll_max), false);
            }

            if (ImGui::GetIO().MouseWheel < 0) {
                scroll_offset_static.animate(std::clamp(scroll_offset_static.m_value + 15, 0.f, scroll_max), false);
            }
        }

        // show it when we need to
        if (scroll_amount.m_value > visible_size) {
            // the code above is just nanotechnology
             // scroll parameters
            float scrollbar_width = 3.f;
            float track_x = position.x - 2 + size.x - scrollbar_width;
            float track_y = position.y + 26;
            float track_height = size.y - 30;

            // calculate the scroll ratio and be sure it fits in the track
            float scrollbar_ratio = visible_size / scroll_amount.m_value;
            float thumb_height = std::fmax(scrollbar_ratio * track_height, 10.0f); // minimum height is 10
            thumb_height = std::fmin(thumb_height, track_height);  // make sure it doesnt get out of the track

            // calculate dumb position in the track
            float thumb_position_y = track_y + (scroll_offset_static.m_value / scroll_max) * (track_height - thumb_height);

            // check if we interact with scroll
            if (ImGui::IsMouseClicked(0) && g_utils.in_region(c_vector_2d(track_x, thumb_position_y), c_vector_2d(scrollbar_width, thumb_height))) {
                interacted[child.id] = true;  // interacted, start it now
            }

            // continue interaction even if we are not anymore in that
            if (interacted[child.id] && ImGui::IsMouseDown(0)) {
                float mouse_delta_y = ImGui::GetIO().MouseDelta.y;
                float new_scroll_offset = scroll_offset_static.m_value + mouse_delta_y / track_height * scroll_amount.m_value;
                scroll_offset_static.animate(std::clamp(new_scroll_offset, 0.f, scroll_max), false);
            }
            else if (!ImGui::IsMouseDown(0)) {
                // reset interaction if mouse is released
                interacted[child.id] = false;
            }

            auto interacted_scroll = g_anim_base.build(child.m_name + "#titlescrll" + g_anim_base.m_cur_child);
            interacted_scroll.animate(interacted_scroll.m_value + 3.f * g_anim_base.delta_time(0.5) * (interacted[child.id] && (g_content.m_window_i_alpha > 0.f) ? 1.f : -1.f));

           // auto hovver = g_anim_base.build(child.m_name + "#titlescrll" + g_anim_base.m_cur_child);
           // hovver.animate(hovver.m_value + 3.f * g_anim_base.delta_time(0.5) * (g_utils.in_region(position, size) && (g_content.m_window_i_alpha > 0.f) ? 1.f : -1.f));

            g_render_engine->rect_filled(track_x, thumb_position_y, scrollbar_width, thumb_height, c_color(32, 32, 32).blend(base.m_theme.accent, interacted_scroll.m_value).modify_alpha(255 * g_content.m_window_i_alpha), 12);
        } 

        g_anim_base.m_cur_child = child.m_name;

        ImGui::GetBackgroundDrawList()->PushClipRect(c_vector_2d(position.x, position.y + 26).convert(), c_vector_2d(position.x + size.x, position.y + 26 + ( size.y - 30 )).convert(), true);
        child.m_contents();
        ImGui::GetBackgroundDrawList()->PopClipRect();

        scroll_amount.animate(g_group.m_object.m_ctx.m_offset, false);
	}
}