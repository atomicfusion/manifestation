add_library(manifestation SHARED service.cpp service_ipc_client.cpp sockets.cpp)

add_executable(manifestation_daemon service_daemon.cpp service_ipc_server.cpp common.cpp diy.cpp sockets.cpp)

add_executable(manifestation_test test.cpp)
target_link_libraries(manifestation_test
	manifestation
)

install(TARGETS manifestation DESTINATION lib)
install(FILES manifestation.h DESTINATION include)

install(TARGETS manifestation_daemon DESTINATION sbin)

##FIXME: Clean this up. Should it exist at all? As a different package?
install(TARGETS manifestation_test DESTINATION bin)