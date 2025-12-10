cmake_minimum_required(VERSION 3.16)

project(easyvm)

################################################################################
# Source groups
################################################################################
set(Headers
    "include/easyvm/OpCodes.h"
    "include/easyvm/Value.h"
    "include/easyvm/VM.h"
    "include/easyvm/VMHelper.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "source/OpCodes.cpp"
    "source/Value.cpp"
    "source/VM.cpp"
    "source/VMHelper.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
)

################################################################################
# Target
################################################################################

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} STATIC ${ALL_FILES})
