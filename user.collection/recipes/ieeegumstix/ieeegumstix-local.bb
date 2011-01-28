require ieeegumstix.inc

PR = "r1"

IEEE_GUMSTIX_LOCAL_PATH ?= "missing-IEEE_GUMSTIX_LOCAL_PATH"
SRC_URI += "file://${IEEE_GUMSTIX_LOCAL_PATH} "
S = "${WORKDIR}/IEEEGumstix"

