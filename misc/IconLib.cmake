set(ICONLIB_INSTALL_ICONS "FALSE" CACHE BOOL "IconLib will be used to install .desktop icons")
set(ICONLIB_MANUAL_INSTALL_DIR "" CACHE PATH "IconLib will directly install .desktop files to this path instead of using the xdg-desktop-menu command")

set(iconbase ${CMAKE_SOURCE_DIR}/misc)
set(dirfile ${iconbase}/ieee.directory)

function(install_desktop_file name)
	configure_file(${iconbase}/${name}.desktop.in ${name}.desktop)
	if(ICONLIB_MANUAL_INSTALL_DIR)
		install(FILES ${name}.desktop DESTINATION ${ICONLIB_MANUAL_INSTALL_DIR})
	else()
		install(CODE "execute_process(COMMAND xdg-desktop-menu install --novendor ${dirfile} ${CMAKE_CURRENT_BINARY_DIR}/${name}.desktop)")
	endif()
endfunction()

