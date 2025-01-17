set(SRCFILES
        src/BinManager.cc
        src/CalcChisq.cc
        src/EigenDecomp.cc
        src/OptParser.cc
        src/PlotStyle.cc
        src/ToyThrower.cc
        src/GlobalVariables.cc
        src/GeneralizedFitBin.cc
        src/DataBinSet.cpp
        src/DataBin.cpp
        src/JsonUtils.cpp
        src/YamlUtils.cpp
        src/GundamGreetings.cpp
        src/ApplyCondition.cpp
        src/ScanConfig.cpp
        )
set(HEADERS
        include/BinManager.hh
        include/CalcChisq.hh
        include/EigenDecomp.hh
        include/OptParser.hh
        include/PlotStyle.hh
        include/ToyThrower.hh
        include/GlobalVariables.h
        include/GeneralizedFitBin.h
        include/DataBin.h
        include/DataBinSet.h
        include/JsonUtils.h
        include/YamlUtils.h
        include/YamlUtils.impl.h
        include/GundamGreetings.h
        include/ApplyCondition.h
        include/ScanConfig.h
        )

if( USE_STATIC_LINKS STREQUAL "1" )
  add_library(GundamUtils STATIC ${SRCFILES})
else()
  add_library(GundamUtils SHARED ${SRCFILES})
endif()

target_link_libraries( GundamUtils PUBLIC
        ${ROOT_LIBRARIES}
        ${YAML_CPP_LIBRARIES}
        )

set_target_properties(GundamUtils PROPERTIES VERSION
  "${gundam_VERSION_MAJOR}.${gundam_VERSION_MINOR}.${gundam_VERSION_REVISION}${gundam_VERSION_TAG}")

install(TARGETS GundamUtils DESTINATION lib)
#Can uncomment this to install the headers... but is it really neccessary?
# install(FILES ${HEADERS} DESTINATION include)

set(MODULETargets ${MODULETargets} GundamUtils PARENT_SCOPE)
