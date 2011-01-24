DESCRIPTION = "IEEE Hardware team gumstix code"
DEPENDS = "boost wxwidgets opencv"

PR = "r1"

IEEE_GUMSTIX_LOCAL_PATH ?= "missing-IEEE_GUMSTIX_LOCAL_PATH"
SRC_URI = "file://${IEEE_GUMSTIX_LOCAL_PATH} file://OpenCVConfig.cmake"

S = "${WORKDIR}/IEEEGumstix"

EXTRA_OECMAKE = "-DOpenCV_DIR=${WORKDIR} -DICONLIB_INSTALL_ICONS=TRUE -DICONLIB_MANUAL_INSTALL_DIR=${datadir}/applications"

inherit cmake

FILES_${PN} += "${datadir}/applications"

