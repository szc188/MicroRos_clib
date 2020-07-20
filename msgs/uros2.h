#ifndef __SZC_UROS2_H_
#define __SZC_UROS2_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <uxr/client/config.h>
#include <ucdr/microcdr.h>
#include <uxr/client/client.h>

struct uros2 {
#if defined PROFILE_UDP_TRANSPORT
	uxrUDPTransport transport;
	uxrUDPPlatform udp_platform;
#elif defined PROFILE_TCP_TRANSPORT
	uxrTCPTransport transport;
	uxrTCPPlatform tcp_platform;
#elif defined PROFILE_SERIAL_TRANSPORT
	uxrSerialTransport transport;
	uxrSerialPlatform serial_platform;
#endif
};

struct uros2_topic {
	
};


bool uros2_init(struct uros2 *ctx);

#if defined PROFILE_UDP_TRANSPORT
bool uros2_connect(	
	struct uros2 *ctx, 
	uxrIpProtocol protocol, 
	const char *ip, 
	const char *port);
#elif defined PROFILE_TCP_TRANSPORT
bool uros2_connect(
	struct uros2 *ctx, 
	uxrIpProtocol protocol, 
	const char *ip, 
	const char *port);
#elif defined PROFILE_SERIAL_TRANSPORT
bool uros2_connect(
	struct uros2 *ctx,
	const int fd,
	uint8_t remote_addr,
        uint8_t local_addr);
#endif

bool uros2_disconnect(struct uros2 *ctx);


#ifdef __cplusplus
}
#endif

#endif
