# Copyright (C) 2010 Marco Randazzo
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

IF (COMPILE_DEVICE_LIBRARY)
	YARP_PREPARE_DEVICE(laserHokuyo TYPE laserHokuyo INCLUDE laserHokuyo.h)
	YARP_ADD_DEVICE_FINGERPRINT(laserHokuyo.ini laserHokuyo)
ENDIF (COMPILE_DEVICE_LIBRARY)

if (NOT SKIP_laserHokuyo)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                       ${YARP_INCLUDE_DIRS})

yarp_add_plugin(laserHokuyo laserHokuyo.h laserHokuyo.cpp)
target_link_libraries(laserHokuyo ${YARP_LIBRARIES})

endif (NOT SKIP_laserHokuyo)
