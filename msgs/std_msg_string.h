#ifndef __SZC_STD_MSG_STRING_H_
#define __SZC_STD_MSG_STRING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "topic.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ucdr/microcdr.h>

#define STD_MSG_STRING_NAME	"String"
#define STD_MSG_STRING_TYPE	"std_msgs::msg::dds_::String_"
#define STD_MSG_STRING_ID	STD_MSGS_STRING_ID

typedef struct std_msg_string {
	char data[255];
} std_msg_string;

bool std_msg_string_serialize(ucdrBuffer* writer, const std_msg_string *topic);
bool std_msg_string_deserialize(ucdrBuffer* reader, std_msg_string *topic);
uint32_t std_msg_string_size_of(const std_msg_string *topic, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif
