# Copyright: (C) 2009 RobotCub Consortium
# Author:  Giorgio Metta, Lorenzo Natale
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 
IF (COMPILE_DEVICE_LIBRARY)
  YARP_PREPARE_DEVICE(meiMotionControl TYPE MEIMotionControl INCLUDE MEIMotionControl.h WRAPPER controlboard)
ENDIF (COMPILE_DEVICE_LIBRARY)

IF (NOT SKIP_meiMotionControl)

  FIND_PACKAGE(YARP REQUIRED)
	SET(YARP_DEPENDENCIES "medvc60f.lib")
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

  ADD_LIBRARY(meiMotionControl MEIMotionControl.cpp  MEIMotionControl.h )

ENDIF (NOT SKIP_meiMotionControl)

