######################################################################
# 设置最小cmake版本
######################################################################
cmake_minimum_required(VERSION 3.5.0)

######################################################################
# 设置项目名称
######################################################################
project(MicroXrceDDS_Test)

######################################################################
# 设置编译app的源文件列表
######################################################################
set(APP_SRC_LIST
	main.c
	# std_msg_string.c
)

######################################################################
# 设置编译uxr的源文件列表
######################################################################
set(UXR_SRC_LIST
	# core/log
	# $<$<OR:$<BOOL:${UCLIENT_VERBOSE_MESSAGE}>,$<BOOL:${UCLIENT_VERBOSE_SERIALIZATION}>>:src/c/core/log/log.c>
	
	# core/serialization
	uxr/src/c/core/serialization/xrce_header.c
	uxr/src/c/core/serialization/xrce_subheader.c
	uxr/src/c/core/serialization/xrce_types.c

	# core/session
	uxr/src/c/core/session/common_create_entities.c
	uxr/src/c/core/session/create_entities_ref.c
	uxr/src/c/core/session/create_entities_xml.c
	uxr/src/c/core/session/object_id.c
	uxr/src/c/core/session/read_access.c
	uxr/src/c/core/session/session_info.c
	uxr/src/c/core/session/session.c
	uxr/src/c/core/session/submessage.c
	uxr/src/c/core/session/write_access.c
	# core/session/stream
	uxr/src/c/core/session/stream/input_best_effort_stream.c
	uxr/src/c/core/session/stream/input_reliable_stream.c
	uxr/src/c/core/session/stream/output_best_effort_stream.c
	uxr/src/c/core/session/stream/output_reliable_stream.c
	uxr/src/c/core/session/stream/seq_num.c
	uxr/src/c/core/session/stream/stream_id.c
	uxr/src/c/core/session/stream/stream_storage.c
	
	# profile/discovery
	# uxr/src/c/profile/discovery/discovery.c
	# # profile/discovery/transport
	# uxr/src/c/profile/discovery/transport/udp_transport_datagram_posix.c
	# $<$<AND:$<BOOL:${PROFILE_DISCOVERY}>,$<PLATFORM_ID:Windows>>:src/c/profile/discovery/transport/udp_transport_datagram_windows.c>

	# # profile/transport/ip
	# $<$<OR:$<BOOL:${UCLIENT_PLATFORM_POSIX}>,$<BOOL:${UCLIENT_PLATFORM_POSIX_NOPOLL}>>:src/c/profile/transport/ip/ip_posix.c>
	# $<$<BOOL:$<PLATFORM_ID:Windows>>:src/c/profile/transport/ip/ip_windows.c>
	# # profile/transport/ip/tcp
	# $<$<BOOL:${UCLIENT_EXTERNAL_TCP}>:src/c/profile/transport/ip/tcp/tcp_transport_external.c>
	# $<$<AND:$<BOOL:${PROFILE_TCP_TRANSPORT}>,$<BOOL:${UCLIENT_PLATFORM_POSIX}>>:src/c/profile/transport/ip/tcp/tcp_transport_posix.c>
	# $<$<AND:$<BOOL:${PROFILE_TCP_TRANSPORT}>,$<PLATFORM_ID:Windows>>:src/c/profile/transport/ip/tcp/tcp_transport_windows.c>
	# $<$<BOOL:${PROFILE_TCP_TRANSPORT}>:src/c/profile/transport/ip/tcp/tcp_transport.c>
	# # profile/transport/ip/udp
	uxr/src/c/profile/transport/ip/udp/udp_transport_external.c
	# $<$<AND:$<BOOL:${PROFILE_UDP_TRANSPORT}>,$<BOOL:${UCLIENT_PLATFORM_POSIX_NOPOLL}>>:src/c/profile/transport/ip/udp/udp_transport_posix_nopoll.c>
	# $<$<AND:$<BOOL:${PROFILE_UDP_TRANSPORT}>,$<BOOL:${UCLIENT_PLATFORM_POSIX}>>:src/c/profile/transport/ip/udp/udp_transport_posix.c>
	# $<$<AND:$<BOOL:${PROFILE_UDP_TRANSPORT}>,$<PLATFORM_ID:Windows>>:src/c/profile/transport/ip/udp/udp_transport_windows.c>
	uxr/src/c/profile/transport/ip/udp/udp_transport.c
	# $<$<BOOL:${PROFILE_DISCOVERY}>:src/c/profile/transport/ip/udp/udp_transport.c>
	# profile/transport/serial
	# uxr/src/c/profile/transport/serial/serial_protocol.c
	# uxr/src/c/profile/transport/serial/serial_transport_external.c
	# $<$<AND:$<BOOL:${PROFILE_SERIAL_TRANSPORT}>,$<BOOL:${UCLIENT_PLATFORM_POSIX}>>:src/c/profile/transport/serial/serial_transport_posix.c>
	# uxr/src/c/profile/transport/serial/serial_transport.c

	# profile/util
	uxr/src/c/util/time.c
)

######################################################################
# 设置编译ucdr的源文件列表
######################################################################
set(UCDR_SRC_LIST
	# .
	ucdr/src/c/common.c

	# types
	ucdr/src/c/types/array.c
	ucdr/src/c/types/basic.c
	ucdr/src/c/types/sequence.c
	ucdr/src/c/types/string.c
)

######################################################################
# 设置编译类型
######################################################################
add_executable(${PROJECT_NAME} ${APP_SRC_LIST} ${UXR_SRC_LIST} ${UCDR_SRC_LIST}) # 生成可执行文件

######################################################################
# 设置头文件路径
######################################################################
target_include_directories(${PROJECT_NAME}
PUBLIC
	uxr/include
	ucdr/include
	.
PRIVATE
	uxr/src/c
	ucdr/src/c
)
