#include "../../includes.hpp"
#include "hxd/spixel.hpp"

bool c_render::initialize_fonts()
{
	// do this again
	IMGUI_CHECKVERSION( );
	ImGui::CreateContext( );
	ImGuiIO& io = ImGui::GetIO( );

	ImFontConfig* cfg = new ImFontConfig( );
	cfg->FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Bold;

	g_font.verdana_bold = c_font( "c:/windows/fonts/verdana.ttf", 14, cfg, NULL ); // default constructor
	if (g_font.verdana_bold.m_handle == nullptr)
		return false; // throw it

	cfg = new ImFontConfig( );
	cfg->FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_Monochrome;//ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_Monochrome;
	cfg->OversampleH = 1;
	cfg->PixelSnapH = true;
	g_font.verdana = c_font( "c:/windows/fonts/verdana.ttf", 13, cfg, NULL ); // default constructor
	if ( g_font.verdana.m_handle == nullptr )
		return false; // throw it

	g_font.pixel = c_font( _smallest_pixel, sizeof( _smallest_pixel ), 10 );
	if ( g_font.pixel.m_handle == nullptr )
		return false; // throw it

	cfg = new ImFontConfig();
	cfg->FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_NoHinting;
	g_font.icons = c_font(awesome_icons, sizeof(awesome_icons), 14, cfg, NULL);
	if (g_font.icons.m_handle == nullptr)
		return false; // throw it

	cfg = new ImFontConfig();
	cfg->FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_NoHinting;
	g_font.icons_medium = c_font(awesome_icons, sizeof(awesome_icons), 18, cfg, NULL);
	if (g_font.icons_medium.m_handle == nullptr)
		return false; // throw it

	cfg = new ImFontConfig();
	cfg->FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_NoHinting;
	g_font.icons_big = c_font(awesome_icons, sizeof(awesome_icons), 24, cfg, NULL);
	if (g_font.icons_big.m_handle == nullptr)
		return false; // throw it

	cfg = new ImFontConfig();
	cfg->FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_NoHinting;
	g_font.icons_shadowed = c_font(awesome_icons, sizeof(awesome_icons), 40, cfg, NULL);
	if (g_font.icons_shadowed.m_handle == nullptr)
		return false; // throw it

	// build font
	ImGuiFreeType::BuildFontAtlas( io.Fonts, 0 );

	// delete the created pointer as we do not want any memory remaining there
	delete cfg;

	// we initialized with success
	return true;
}

bool c_render::intialize_renderer( IDirect3DDevice9* device, HWND window )
{
	// if our device or window is invalid return false so we wont continue further
	if ( !device || !window )
		return false;

	// useless but why not
	ImGui::StyleColorsDark( );

	// init
	ImGui_ImplWin32_Init( window );
	ImGui_ImplDX9_Init( device );

	if ( !this->initialize_fonts( ) )
		return false;

	return true;
} 

void c_render::override_draw_list(e_draw_list_data data)
{
	// note: if our drawlist is on the first layer then we can draw all over the drawn elements/windows
	// always when u override for backround you have used with oposite drawlist
	// example:
	// if u have used the first layer then after u end drawing with it reset it for backround drawlist	
	// note 2:
	// // WARNING!
	// check if we have to do some check if the drawlist is the same as the one set by the override function
	// do not set it again to avoid memory leaks i guess
	// we will test this out later
	// WARNING!
	this->m_draw_list = data == e_draw_list_data::first_layer ? ImGui::GetForegroundDrawList() :
		ImGui::GetBackgroundDrawList();

	// set font drawlist
	g_render_globals.m_draw_list = this->m_draw_list;
}

void c_render::rect( float x, float y, float w, float h, c_color color, int rounding, float tickness ) { 
	this->m_draw_list->AddRect( ImVec2( x, y ), ImVec2( x + w, y + h ), color.convert( ), rounding, 0, tickness );
}

void c_render::rect_filled( float x, float y, float w, float h, c_color color, int rounding ) {
	this->m_draw_list->AddRectFilled( ImVec2( x, y ), ImVec2( x + w, y + h ), color.convert( ), rounding );
}

void c_render::shadow( float x, float y, float w, float h, c_color color, int rounding, int shadow_tickness ) { 
	this->m_draw_list->AddShadowRect( ImVec2( x, y ), ImVec2( x + w, y + h ), color.convert( ), shadow_tickness, ImVec2( 0, 0 ), 0, rounding );
}

void c_render::line( c_vector_2d from, c_vector_2d to, c_color color, int tickness ) { 
	this->m_draw_list->AddLine( from.convert( ), to.convert( ), color.convert( ), tickness );
}

void c_render::circle( c_vector_2d position, c_color color, float radius, e_circle_flags flags ) { 
	if ( flags == e_circle_flags::filled ) {
		this->m_draw_list->AddCircleFilled( position.convert( ), radius, color.convert( ) );
	} else if ( flags == e_circle_flags::outlined ) {
		this->m_draw_list->AddCircle( position.convert( ), radius, color.convert( ), 34 );
	}
}

void c_render::gradient( float x, float y, float w, float h, c_color c1, c_color c2, 
	e_gradient_flags flags, int rounding, c_color backround, ImDrawFlags drawing_flags ) {
	if ( flags == e_gradient_flags::vertical ) {
		if ( rounding != 0 ) {
			// run gradient rounding
			this->m_draw_list->AddRectFilledMultiColorRounded( ImVec2( x, y ), ImVec2( x + w, y + h ),
				backround.convert( ), c1.convert( ), c2.convert( ), c2.convert( ), c1.convert( ), rounding, drawing_flags );
		} else {
			this->m_draw_list->AddRectFilledMultiColor( ImVec2( x, y ), ImVec2( x + w, y + h ), 
				c1.convert( ), c2.convert( ), c2.convert( ), c1.convert( ) );
		}
	} else if ( flags == e_gradient_flags::horizontal ) {
		if ( rounding != 0 ) {
			// run gradient rounding
			this->m_draw_list->AddRectFilledMultiColorRounded( ImVec2( x, y ), ImVec2( x + w, y + h ),
				backround.convert( ), c1.convert( ), c1.convert( ), c2.convert( ), c2.convert( ), rounding, drawing_flags );
		} else {
			this->m_draw_list->AddRectFilledMultiColor( ImVec2( x, y ), ImVec2( x + w, y + h ), 
				c1.convert( ), c1.convert( ), c2.convert( ), c2.convert( ) );
		}
	}
}

void c_render::inner_dropped_shadow(float x, float y, float w, float h, float size, c_color main_col)
{
	this->gradient(x, y, w, size, main_col, main_col.modify_alpha(0), e_gradient_flags::horizontal );
	this->gradient(x, y + h - size, w, size, main_col.modify_alpha(0), main_col, e_gradient_flags::horizontal);

	this->gradient(x, y, size, h, main_col, main_col.modify_alpha(0), e_gradient_flags::vertical);
	this->gradient(x + w - size, y, size, h, main_col.modify_alpha(0), main_col, e_gradient_flags::vertical);
}
