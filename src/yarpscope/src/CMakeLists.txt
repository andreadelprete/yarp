# Copyright (C) 2012  iCub Facility, Istituto Italiano di Tecnologia
# Author: Daniele E. Domenichelli <daniele.domenichelli@iit.it>
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT


set(YarpScope_SRCS
    main.cpp
    DataPlot.cpp
    Debug.cpp
    Graph.cpp
    MainWindow.cpp
    PortReader.cpp
    PlotManager.cpp
    SimpleLoader.cpp
    XmlLoader.cpp)

include_directories(${TinyXML_INCLUDE_DIRS}
                    ${GtkDataboxMM_INCLUDE_DIRS})

if(WIN32)
    add_definitions(/wd4250)
endif(WIN32)

add_executable(yarpscope ${YarpScope_SRCS})

target_link_libraries(yarpscope
                      YARP_init
                      YARP_OS
                      ${TinyXML_LIBRARIES}
                      ${GtkDataboxMM_LIBRARIES})

install(TARGETS yarpscope COMPONENT utilities DESTINATION bin)
