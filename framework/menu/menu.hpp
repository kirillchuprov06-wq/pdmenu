#pragma once

class c_menu {
public:
	void run( );
};
inline const auto g_menu = std::make_unique < c_menu >( );