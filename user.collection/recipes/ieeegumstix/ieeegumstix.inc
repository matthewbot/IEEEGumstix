DESCRIPTION = "IEEE Hardware team gumstix code"
DEPENDS = "boost wxwidgets"

inherit cmake
EXTRA_OECMAKE = "-DICONLIB_INSTALL_ICONS=TRUE -DICONLIB_MANUAL_INSTALL_DIR=${datadir}/applications -DIEEE_CONFIG_INSTALL_DEFAULTS=TRUE -DIEEE_CONFIG_DIR=${sysconfdir}/ieee"

FILES_${PN} += "${datadir}/applications/* ${sysconfdir}/ieee/*"
CONFFILES_${PN} += "${sysconfdir}/ieee/*"

