DESCRIPTION = "IEEE Hardware team gumstix code"
DEPENDS = "boost wxwidgets"

inherit cmake
EXTRA_OECMAKE = "-DICONLIB_INSTALL_ICONS=TRUE -DICONLIB_MANUAL_INSTALL_DIR=${datadir}/applications"

FILES_${PN} += "${datadir}/applications/*"

