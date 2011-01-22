DESCRIPTION = "IEEE Hardware team gumstix code"
DEPENDS = "boost wxwidgets opencv"

PR = "r0"

SRC_URI = "git://github.com/matthewbot/IEEEGumstix.git;tag=master;protocol=git file://OpenCVConfig.cmake"

S = "${WORKDIR}/git"

EXTRA_OECMAKE = "-DOpenCV_DIR=${WORKDIR}"

inherit cmake

