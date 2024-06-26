#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.27)
project(example_MyPythia)

find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find HepMC (required package)
#
find_package(HepMC QUIET)
if(NOT HEPMC_FOUND)
  message(STATUS "G4 Examples: HepMC package not found. --> example_MyPythia example disabled")  
  return()
endif()

#----------------------------------------------------------------------------
# Find Pythia6 (optional package)
#
find_package(Pythia6 QUIET)
if(Pythia6_FOUND)
  message(STATUS "G4 Examples: Pythia6 found. --> example_MyPythia with Pythia6 enabled.") 
endif()

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
include_directories(${HEPMC_INCLUDE_DIR})
target_compile_definitions(example_MyPythia PRIVATE $<$<BOOL:${Pythia6_FOUND}>:G4LIB_USE_PYTHIA>)
add_executable(example_MyPythia example_MyPythia.cxx)
target_link_libraries(example_MyPythia
                      ${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES}
                      $<$<BOOL:${Pythia6_FOUND}>:Pythia6::Pythia6>
                      gfortran)

# if pythia is compiled with g77, link with -lg2c instead.
#target_link_libraries(example_MyPythia
#                      ${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES}
#                      $<$<BOOL:${Pythia6_FOUND}>:Pythia6::Pythia6>
#                      g2c)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS example_MyPythia DESTINATION bin)

