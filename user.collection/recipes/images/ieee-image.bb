# customized version of omap3-palmtop-image for ieee

require recipes/images/omap3-console-image.bb

SPLASH ?= "psplash"

IMAGE_INSTALL += " \
  angstrom-gpe-task-base \
  angstrom-gpe-task-settings \
  angstrom-x11-base-depends \
  midori \
  ${SPLASH} \
  xlsfonts \
  xrefresh \
  links \
  mplayer \
  ieeegumstix \
 "

