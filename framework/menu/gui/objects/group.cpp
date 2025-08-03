#include "../../../../includes.hpp"

void group_controller_t::setup(group_context_t context) {
	m_ctx = context;
}

void group_controller_t::begin(std::function<void()> content) {
	elements.groups.clear();

	content();
	invoke();
}

void group_controller_t::object(const std::string& child_name, int id, std::function<void()> child_contents, bool first_instance) {
	elements.groups.push_back({ id, child_name, child_contents, first_instance });
}

void group_controller_t::invoke() {
	static auto normal_size = 20;
	int size[128]{};
	auto position_y = m_ctx.m_window_pos.y;

	const auto arrow = [&](const c_vector_2d& base, const bool reverse, const bool active = false) -> void
		{
			const auto col = c_color();

			if (!reverse)
			{
				g_render_engine->rect_filled(base.x, base.y, 1, 1, col);
				g_render_engine->rect_filled(base.x - 1, base.y - 1, 3, 1, col);
				g_render_engine->rect_filled(base.x - 2, base.y - 2, 5, 1, col);
				g_render_engine->rect_filled(base.x - 3, base.y - 3, 7, 1, col);
			}
			else
			{
				g_render_engine->rect_filled(base.x, base.y, 1, 1, col);
				g_render_engine->rect_filled(base.x - 1, base.y + 1, 3, 1, col);
				g_render_engine->rect_filled(base.x - 2, base.y + 2, 5, 1, col);
				g_render_engine->rect_filled(base.x - 3, base.y + 3, 7, 1, col);
			}
		};

	for (auto& group : elements.groups) {
		// update size;
		size[group.m_id] = normal_size;

		// setup open
		if (g_utils.in_region(c_vector_2d(m_ctx.m_window_pos.x, position_y), c_vector_2d(m_ctx.m_window_size.x, 20)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && g_content.m_blocking == 0) {
			elements.m_opened[group.m_id] = !elements.m_opened[group.m_id];
		}

		auto group_open_animation = g_anim_base.build(group.m_name + "#animatioas" + std::to_string(group.m_id) + g_anim_base.m_cur_child);
		group_open_animation.animate(group_open_animation.m_value + 3.f * g_anim_base.delta_time(0.7) * (elements.m_opened[group.m_id] ? 1.f : -1.f));

		int total_size = 23 + elements.m_obj[group.m_id];
		size[ group.m_id ] = total_size * group_open_animation.m_value;

		// fast fix
		// if animation makes our group go less than 20 just force it set by default to 20
		if ( size[ group.m_id ] < 20 )
			size[ group.m_id ] = 20;

		elements.m_obj[ group.m_id ] = 0;

		auto hover_group_a = g_anim_base.build(group.m_name + std::to_string(group.m_id) + g_anim_base.m_cur_child);
		hover_group_a.animate(hover_group_a.m_value + 3.f * g_anim_base.delta_time(0.5) *
			(g_utils.in_region(c_vector_2d(m_ctx.m_window_pos.x, position_y), c_vector_2d(m_ctx.m_window_size.x, size[group.m_id])) ? 1.f : -1.f));

		g_render_engine->shadow(m_ctx.m_window_pos.x, position_y, m_ctx.m_window_size.x, size[group.m_id], m_ctx.m_theme.accent.modify_alpha( 255 * hover_group_a.m_value ));
		g_render_engine->rect_filled(m_ctx.m_window_pos.x, position_y, m_ctx.m_window_size.x, size[group.m_id], m_ctx.m_theme.group_backround);
		g_render_engine->rect(m_ctx.m_window_pos.x, position_y, m_ctx.m_window_size.x, size[group.m_id], m_ctx.m_theme.accent.modify_alpha( 150 ));
		g_render_engine->inner_dropped_shadow(m_ctx.m_window_pos.x, position_y, m_ctx.m_window_size.x, size[group.m_id], 5, m_ctx.m_theme.accent.modify_alpha(50));

		arrow(c_vector_2d(m_ctx.m_window_pos.x + 10, elements.m_opened[group.m_id] ? position_y + 8 : position_y + 11), elements.m_opened[group.m_id]);

		g_font.verdana.string(m_ctx.m_window_pos.x + 25, position_y + 3, group.m_name, c_color());

		m_object.update({ 15, c_vector_2d(m_ctx.m_window_pos.x + 5, position_y + 10), c_vector_2d(m_ctx.m_window_size.x - 10, size[group.m_id]), base.m_theme });

		g_anim_base.group_id = group.m_id;
		if (elements.m_opened[group.m_id])
			group.m_contents(); // this should work like this

		// push for next entry
		position_y += size[group.m_id] + 7;
	}
}

void elements_t::update(element_context_t ctx) {
	m_ctx = ctx;
}