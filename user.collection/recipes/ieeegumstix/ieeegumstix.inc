DESCRIPTION = "IEEE Hardware team gumstix code"
DEPENDS = "boost wxwidgets opencv"

inherit cmake
EXTRA_OECMAKE = "-DOpenCV_DIR=${WORKDIR} -DICONLIB_INSTALL_ICONS=TRUE -DICONLIB_MANUAL_INSTALL_DIR=${datadir}/applications"
SRC_URI += "file://OpenCVConfig.cmake"

FILES_${PN} += "${datadir}/applications/*"

