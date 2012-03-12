
include(cmake/SubmitJob.cmake)

message(STATUS "Adding killdevil job submision target")
option(CLONE_PROJECT_KILLDEVIL "Clone entire project in killdevil" ON)
option(KILL_JOB_FIRST "Kill job in cluster" OFF)

set(SCRATCH_REMOTE_DIRECTORY /lustre/scr/r/i/ricortiz)
set(MOOPS_REMOTE_DIRECTORY /lustre/scr/r/i/ricortiz/moops)
set(REMOTE_SERVER ricortiz@killdevil.unc.edu)
set(CMAKE_CACHE "-DCMAKE_BUILD_TYPE=Release -DUSE_MPI=ON -DCMAKE_CXX_COMPILER=mpic++ -DCMAKE_C_COMPILER=mpicc -DUSE_CUDA=ON -DUSE_QT_GUI=OFF -DVTK_DIR=~/opt/lib/vtk-5.8 ..")
set(KILLDEVIL_ENV "bash <<EOF \nmodule initclear")
set(KILLDEVIL_ENV "${KILLDEVIL_ENV} \nmodule initadd mvapich2_gcc/4.4.6 cuda cmake git")
set(JOB_NAME moops)
set(QUEUE gpu)
set(PROGRAM_PATH testing/math)
set(PROGRAM_NAME TestFluidSolvers)

set_git_commands(CLONE_PROJECT_KILLDEVIL)

execute_process(COMMAND ${SSH_COMMAND} ${REMOTE_SERVER} ${GIT_COMMANDS} RESULT_VARIABLE out)
execute_process(COMMAND ${SSH_COMMAND} ${REMOTE_SERVER} ${KILLDEVIL_ENV} RESULT_VARIABLE out)

IF(KILL_JOB_FIRST)
    execute_process(COMMAND ${SSH_COMMAND} ${REMOTE_SERVER} bkill `bjobs -u ricortiz | grep ${JOB_NAME} | cut -f1 -d\" \"`)
ENDIF()

set_bsub_commands(${PROGRAM_PATH}/${PROGRAM_NAME} ${JOB_NAME} ${QUEUE})
submit_job(${JOB_NAME} ${CMAKE_CACHE})
