#include "../../includes.hpp"

static c_vector_2d window_position = c_vector_2d( 
	/* render the menu in middle */
	( 2560 * 0.5 ) - 250, ( 1440 * 0.5 ) - 200
);
static c_vector_2d window_size = c_vector_2d( 600, 430 );
static int tab_index;
static int stab_index;
static bool a[500]{ false };
static int b[500]{ false };
static float c[500]{ false };
static c_color d[ 10 ]{ };
static std::vector<std::string> aa = { "Item 1", "Item 2", "Item 3", "Item 4", "Item 5" };
static std::vector<std::string> aa2 = { "Item 1", "Item 2", "Item 3" };

static float p[4][ 20 ]{1.f, 1.f, 1.f, 1.f};
static std::string var;

static std::vector<bool> ceau = {
	a[50 ], a[51], a[52]
};

#include <cmath> // For sin() and cos()

void cursor_arrow_with_cutout(c_vector_2d position, float rotation_angle_degrees, c_color color) {
	// Adjust arrow shape to approximate the size of a small Windows cursor (16x16)
	float arrow_base_width = 8;  // Slightly smaller base width for a compact cursor
	float arrow_height = 9;      // Similar height to a small cursor
	float arrow_middle_width = 7;  // Middle width for the notch
	float outline_thickness = 2.0f;  // Thin outline to match a small cursor

	// Vertices of the arrow
	ImVec2 verts[4];
	verts[0] = ImVec2(position.x, position.y);                                  // Top vertex (point)
	verts[1] = ImVec2(position.x - arrow_base_width / 2, position.y + arrow_height);  // Bottom left vertex
	verts[2] = ImVec2(position.x, position.y + arrow_middle_width);             // Middle vertex (creates the notch)
	verts[3] = ImVec2(position.x + arrow_base_width / 2, position.y + arrow_height);  // Bottom right vertex

	// Apply rotation to the vertices
	ImVec2 center = ImVec2(position.x, position.y + arrow_height / 2);  // Center point for rotation
	float angle_radians = rotation_angle_degrees * (3.14159f / 180.0f);

	auto rotate_vertex = [&](ImVec2 point, ImVec2 center, float angle) -> ImVec2 {
		// Translate point to origin
		float translated_x = point.x - center.x;
		float translated_y = point.y - center.y;

		// Apply rotation
		float rotated_x = translated_x * cos(angle) - translated_y * sin(angle);
		float rotated_y = translated_x * sin(angle) + translated_y * cos(angle);

		// Translate back to original position
		return ImVec2(rotated_x + center.x, rotated_y + center.y);
		};

	// Rotate all the vertices
	verts[0] = rotate_vertex(verts[0], center, angle_radians);
	verts[1] = rotate_vertex(verts[1], center, angle_radians);
	verts[2] = rotate_vertex(verts[2], center, angle_radians);
	verts[3] = rotate_vertex(verts[3], center, angle_radians);

	// Get ImDrawList to draw the polygon
	ImDrawList* draw_list = ImGui::GetForegroundDrawList();

	// Draw the soft, semi-transparent outline to simulate anti-aliasing
	draw_list->AddPolyline(verts, 4, color.convert(), true, outline_thickness);

	// Draw the inner filled arrow
	draw_list->AddConvexPolyFilled(verts, 4, color.convert());
}

void c_menu::run( ) { 
	g_window.object( "phantosia", window_position, window_size, [ & ]( ) {
		std::vector<subtab_t> aimbot_subtabs;
		std::vector<subtab_t> visuals_subtabs;
		std::vector<subtab_t> misc_subtabs;
		std::vector<subtab_t> profile_subtabs;
		{
			aimbot_subtabs.push_back(subtab_t{ 0, "Aimbot", [&]() {
						g_child.begin([&]() {
								g_child.object("Linear", e_child_placement::child_top_left, e_child_size::child_size_full, [&]() {
									g_group.m_object.checkbox("Enabled", &a[499]);
									g_group.m_object.slider("Field of view", &b[0], 0, 360, L"");
									g_group.m_object.slider("Float value", &c[0], 0, 360, L"");
									g_group.m_object.dropdown("Dropdown", &b[20], aa);
									g_group.m_object.multi_selection("Multidropdown", &ceau, aa2);
									//g_group.m_object.checkbox("Enabled 2", &a[497]);
									g_group.m_object.textinput("Text input", &var);
									g_group.m_object.button("Button", [&]() {});
									g_group.m_object.button("Second button", [&]() {});
								},0);

								g_child.object("Accuracy", e_child_placement::child_top_right, e_child_size::child_size_full, [&]() {

									},1);
						});

				}, &stab_index });

			aimbot_subtabs.push_back(subtab_t{ 1, "Triggerbot", [&]() {


				},&stab_index });
		}
		{
			visuals_subtabs.push_back(subtab_t{ 0, "Player", [&]() {
							

				}, &stab_index });

			visuals_subtabs.push_back(subtab_t{ 1, "World", [&]() {


				},&stab_index });
		}
		{
			misc_subtabs.push_back(subtab_t{ 0, "Movement", [&]() {
						

				}, &stab_index });

			misc_subtabs.push_back(subtab_t{ 1, "Other", [&]() {


				},&stab_index });
		}
		{
			profile_subtabs.push_back(subtab_t{ 0, "Configs", [&]() {
					
				}, &stab_index });

			
		}



		g_child.setup({ window_position, window_size, theme_t() });

		g_tab.object([&]() {
			g_tab.object("Aimbot", &tab_index, aimbot_subtabs);
			g_tab.object("Visuals", &tab_index, visuals_subtabs);
			g_tab.object("Misc", &tab_index, misc_subtabs);
			g_tab.object("Profile", &tab_index, profile_subtabs);
		});

		//g_child.setup( { window_position, window_size, theme_t( ) } );
		////g_group.setup( { 15, window_position, window_size, theme_t( ) } );
		//
		//g_tab.object( [ & ]( ) {
		//	g_tab.object( "Ragebot", &tab_index, [ & ]( ) { 
		//		g_child.begin( [ & ]( ) {
		//			g_child.object( "Linear", e_child_placement::child_top_left, e_child_size::child_size_full, [ & ]( ) { 
		//				g_group.begin([&]() {
		//					g_group.object("Players", 0, [&]() {
		//						g_group.m_object.keybind( "ciocan", & b[ 20 ], &b[ 21 ] );
		//						if ( GetAsyncKeyState(b[20] ) ) {
		//							printf( "key on" );
		//						}
		//
		//						g_group.m_object.checkbox( "Checkbox 3", &a[ 2 ] );
		//						g_group.m_object.dropdown( "Dropdown", &b[20 ], aa );
		//						g_group.m_object.multi_selection( "Multidropdown", &ceau, aa2 );
		//						g_group.m_object.button( "Button", [ & ]( ) { } );
		//						g_group.m_object.textinput( "Text input", &var );
		//						}, true);
		//					g_group.object("World", 1, [&]() {
		//						g_group.m_object.colorpicker( "Colorpicker", p[ 0 ] );
		//						g_group.m_object.checkbox( "Checkbox 3", &a[ 2 ] );
		//						g_group.m_object.checkbox( "Checkbox 3", &a[ 2 ] );
		//						g_group.m_object.checkbox( "Checkbox 3", &a[ 2 ] );
		//						});
		//				});
		//			} );	
		//			g_child.object( "Masks", e_child_placement::child_top_right, e_child_size::child_size_half, [ & ]( ) { 
		//				g_group.begin( [ & ]( ) {
		//					g_group.object( "Players", 2, [ & ]( ) {
		//						g_group.m_object.checkbox( "Checkbox", &a[ 0 ] );
		//						g_group.m_object.checkbox( "Checkbox 2", &a[ 1 ] );
		//						g_group.m_object.slider( "Slider int", &b[ 0 ], 0, 100, L"" );
		//						}, true );
		//					} );
		//				} );	
		//			g_child.object( "Misc", e_child_placement::child_bottom_right, e_child_size::child_size_half, [ & ]( ) {
		//				g_group.begin( [ & ]( ) {
		//					g_group.object( "Configs", 3, [ & ]( ) {
		//						g_group.m_object.listbox( "Config list", &b[ 33 ], aa );
		//
		//						} );
		//					} ); 
		//				} );
		//		} );
		//	} );
		//	g_tab.object( "Visuals", &tab_index, [ & ]( ) { } );
		//	g_tab.object( "Misc", &tab_index, [ & ]( ) { } );
		//	g_tab.object( "Settings", &tab_index, [ & ]( ) { } );
		//} );
	} );

	// hidecursor func
	if (g_window.m_open) {
		//ShowCursor(FALSE);
		SetCursor(NULL);
		cursor_arrow_with_cutout(g_utils.get_mouse_position() + c_vector_2d(2, 4), 338, g_theme.accent);
	}
	else {
		//ShowCursor(TRUE);
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}

}