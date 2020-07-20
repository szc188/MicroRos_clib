#include "std_msg_string.h"

bool std_msg_string_serialize(ucdrBuffer* writer, const std_msg_string *topic)
{
	(void) ucdr_serialize_string(writer, topic->data);
	return !writer->error;
}

bool std_msg_string_deserialize(ucdrBuffer* reader, std_msg_string *topic)
{
	(void) ucdr_deserialize_string(reader, topic->data, 255);
	return !reader->error;
}

uint32_t std_msg_string_size_of(const std_msg_string *topic, uint32_t size)
{
	uint32_t previousSize = size;
	size += (uint32_t)(ucdr_alignment(size, 4) + 4 + (uint32_t)strlen(topic->data) + 1);
	return size - previousSize;
}