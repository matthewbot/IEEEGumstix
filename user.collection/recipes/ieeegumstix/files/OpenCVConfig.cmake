# As far as I can tell, the CMake find_package Config system is completely incompatible
# with cross compilation. These files directly put things on the include path with no
# understanding of the differences in target and root filesystems. So it seems like
# every package relying on this system will need some sort of hack like this one
# in order to be cross-compiled :/

find_path(OpenCV_INCLUDE_DIR "cv.h" PATH_SUFFIXES "opencv" DOC "")

foreach(CVLIB core features2d ml highgui)
	find_library(OpenCV_${CVLIB}_LIBRARY_DEBUG NAMES ${CVLIB}d opencv_${CVLIB}d)
	find_library(OpenCV_${CVLIB}_LIBRARY_RELEASE NAMES ${CVLIB} opencv_${CVLIB})

	if(OpenCV_${CVLIB}_LIBRARY_DEBUG AND OpenCV_${CVLIB}_LIBRARY_RELEASE)
		    set(OpenCV_LIBS ${OpenCV_LIBS} debug ${OpenCV_${CVLIB}_LIBRARY_DEBUG} optimized ${OpenCV_${CVLIB}_LIBRARY_RELEASE})
	elseif(OpenCV_${CVLIB}_LIBRARY_DEBUG)
		    set(OpenCV_LIBS ${OpenCV_LIBS} ${OpenCV_${CVLIB}_LIBRARY_DEBUG})
	elseif(OpenCV_${CVLIB}_LIBRARY_RELEASE)
		    set(OpenCV_LIBS ${OpenCV_LIBS} ${OpenCV_${CVLIB}_LIBRARY_RELEASE})
	else()
		    message(SEND_ERROR "Can't find OpenCV library ${CVLIB}")
	endif()

endforeach()

