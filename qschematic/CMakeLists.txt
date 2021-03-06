# Project settings
project(qschematic
    VERSION 0.1.0
    LANGUAGES CXX
)

# External dependencies
include(external.cmake)

# CMake settings
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

# List of source files
set(SOURCES_PRIVATE
    commands/commandbase.cpp
    commands/commanditemadd.cpp
    commands/commanditemmove.cpp
    commands/commanditemremove.cpp
    commands/commanditemvisibility.cpp
    commands/commandlabelrename.cpp
    commands/commandnoderesize.cpp
    commands/commandnoderotate.cpp
    commands/commandwirenetrename.cpp
    commands/commandwirepointmove.cpp
    items/connector.cpp
    items/item.cpp
    items/itemfactory.cpp
    items/itemmimedata.cpp
    items/label.cpp
    items/node.cpp
    items/splinewire.cpp
    items/wire.cpp
    items/wirenet.cpp
    items/wireroundedcorners.cpp
    wire_system/line.cpp
    wire_system/manager.cpp
    wire_system/wire.cpp
    wire_system/point.cpp
    wire_system/net.cpp
    scene.cpp
    settings.cpp
    utils.cpp
    view.cpp
)

# List of header files
set(HEADERS_PUBLIC
    commands/commandbase.h
    commands/commanditemadd.h
    commands/commanditemmove.h
    commands/commanditemremove.h
    commands/commanditemvisibility.h
    commands/commandlabelrename.h
    commands/commandnoderesize.h
    commands/commandnoderotate.h
    commands/commands.h
    commands/commandwirenetrename.h
    commands/commandwirepointmove.h
    items/itemfunctions.h
    items/connector.h
    items/item.h
    items/itemfactory.h
    items/itemmimedata.h
    items/label.h
    items/node.h
    items/splinewire.h
    items/wire.h
    items/wirenet.h
    items/wireroundedcorners.h
    utils/itemscontainerutils.h
    utils/itemscustodian.h
    wire_system/connectable.h
    wire_system/line.h
    wire_system/manager.h
    wire_system/wire.h
    wire_system/point.h
    wire_system/net.h
    netlist.h
    netlistgenerator.h
    scene.h
    settings.h
    types.h
    utils.h
    view.h
)

# Add the wire system
add_subdirectory(wire_system)

################################################################################
# Object library                                                               #
################################################################################

add_library(qschematic-objs OBJECT "")

target_compile_features(
    qschematic-objs
    PUBLIC
        cxx_std_17
)

target_sources(qschematic-objs
    PRIVATE
        ${SOURCES_PRIVATE}
)

target_include_directories(
    qschematic-objs
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

target_link_libraries(
    qschematic-objs
    PUBLIC
        Qt5::Core Qt5::Gui Qt5::Widgets
        ${DEPENDENCY_GPDS_TARGET}
)

set_target_properties(
    qschematic-objs
    PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
)


################################################################################
# Shared library                                                               #
################################################################################

add_library(qschematic-shared SHARED)
target_link_libraries(
    qschematic-shared
    PUBLIC
        qschematic-objs
)


################################################################################
# Static library                                                               #
################################################################################

add_library(qschematic-static STATIC)
target_compile_definitions(
    qschematic-static
    PUBLIC
        QSCHEMATIC_STATIC_DEFINE
)
target_link_libraries(
    qschematic-static
    PUBLIC
        qschematic-objs
)


################################################################################
# Common library properties                                                    #
################################################################################

set_target_properties(
    qschematic-objs
    qschematic-static
    qschematic-shared
    PROPERTIES
        OUTPUT_NAME "qschematic"
        ARCHIVE_OUTPUT_NAME "qschematic"
        VERSION ${PROJECT_VERSION}
        POSITION_INDEPENDENT_CODE 1
)



################################################################################
# Export header                                                                #
################################################################################

include(GenerateExportHeader)
generate_export_header(qschematic-shared
    BASE_NAME qschematic
    DEPRECATED_MACRO_NAME "QSCHEMATIC_DEPRECATED"
    NO_DEPRECATED_MACRO_NAME "QSCHEMATIC_NO_DEPRECATED"
    EXPORT_MACRO_NAME "QSCHEMATIC_EXPORT"
    NO_EXPORT_MACRO_NAME "QSCHEMATIC_NO_EXPORT"
    STATIC_DEFINE "QSCHEMATIC_STATIC_DEFINE"
    DEFINE_NO_DEPRECATED
)



################################################################################
# Install                                                                      #
################################################################################

# Some basic stuff we'll need in this section
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
set(ConfigPackageLocation ${CMAKE_INSTALL_LIBDIR}/cmake/qschematic)

# Install headers
foreach( file ${HEADERS_PUBLIC} )
    get_filename_component( dir ${file} DIRECTORY )
    install( FILES ${file} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/qschematic/${dir} )
endforeach()

# Install export header
install(
    FILES ${PROJECT_BINARY_DIR}/qschematic_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/qschematic
)

# Targets
install(
    TARGETS
        qschematic-shared
        qschematic-static
        qschematic-objs
    EXPORT qschematic-targets
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT lib
    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT lib
    RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT bin
    INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/qschematic
)

# Package version
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/qschematic/qschematic-config-version.cmake"
    VERSION ${QSchematic_VERSION}
    COMPATIBILITY AnyNewerVersion
)
install(
    FILES
        qschematic-config.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/qschematic/qschematic-config-version.cmake"
    DESTINATION ${ConfigPackageLocation}
)

# Package configuration
configure_file(qschematic-config.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/qschematic/qschematic-config.cmake"
    COPYONLY
)

# Package export targets
export(
    EXPORT qschematic-targets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/qschematic/qschematic-targets.cmake"
    NAMESPACE qschematic::
)
install(
    EXPORT qschematic-targets
    FILE qschematic-targets.cmake
    NAMESPACE qschematic::
    DESTINATION ${ConfigPackageLocation}
)
