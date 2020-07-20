#include <uxr/client/client.h>
#include <ucdr/microcdr.h>

#include <stdio.h>  //printf
#include <string.h> //strcmp
#include <stdlib.h> //atoi

typedef struct HelloWorld
{
	char message[255];
} HelloWorld;

bool HelloWorld_serialize_topic(ucdrBuffer *writer, const HelloWorld *topic)
{
	(void)ucdr_serialize_string(writer, topic->message);
	return !writer->error;
}

bool HelloWorld_deserialize_topic(ucdrBuffer *reader, HelloWorld *topic)
{
	(void)ucdr_deserialize_string(reader, topic->message, 255);
	return !reader->error;
}

uint32_t HelloWorld_size_of_topic(const HelloWorld *topic, uint32_t size)
{
	uint32_t previousSize = size;
	size += (uint32_t)(ucdr_alignment(size, 4) + 4 + strlen(topic->message) + 1);
	return size - previousSize;
}

#define STREAM_HISTORY 8
#define BUFFER_SIZE UXR_CONFIG_UDP_TRANSPORT_MTU *STREAM_HISTORY

uint8_t output_reliable_stream_buffer[BUFFER_SIZE];
uint8_t input_reliable_stream_buffer[BUFFER_SIZE];

void on_topic(
    uxrSession *session,
    uxrObjectId object_id,
    uint16_t request_id,
    uxrStreamId stream_id,
    struct ucdrBuffer *ub,
    uint16_t length,
    void *args)
{
	(void)session;
	(void)object_id;
	(void)request_id;
	(void)stream_id;
	(void)length;

	HelloWorld topic;
	HelloWorld_deserialize_topic(ub, &topic);

	printf("Received topic: %s\n", topic.message);
}

int main(void)
{
	uint32_t max_topics = UINT32_MAX;
	char *ip = "192.168.1.210";
	char *port = "2018";
	// Transport
	uxrUDPTransport transport;
	uxrUDPPlatform udp_platform;
	if (!uxr_init_udp_transport(&transport, &udp_platform, UXR_IPv4, ip, port))
	{
		printf("Error at create transport.\n");
		return 1;
	}
	// Session
	uxrSession session;
	uxr_init_session(&session, &transport.comm, 0xAAAABBB1);
	uxr_set_topic_callback(&session, on_topic, 0);
	if (!uxr_create_session(&session))
	{
		printf("Error at create session.\n");
		return 1;
	}

	// Streams
	uxrStreamId reliable_out = uxr_create_output_reliable_stream(&session, output_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);

	uxrStreamId reliable_in = uxr_create_input_reliable_stream(&session, input_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);
	// Create entities
	uxrObjectId participant_id = uxr_object_id(0x01, UXR_PARTICIPANT_ID);
	const char *participant_xml = "<dds>"
					"<participant>"
						"<rtps>"
							"<name>szc_test</name>"
						"</rtps>"
					"</participant>"
				      "</dds>";
	uint16_t participant_req = uxr_buffer_create_participant_xml(&session, reliable_out, participant_id, 0, participant_xml, UXR_REPLACE);

	uxrObjectId topic_id = uxr_object_id(0x01, UXR_TOPIC_ID);
	const char *topic_xml = "<dds>"
					"<topic>"
						"<name>rt/szc_test/pub</name>"
						"<dataType>std_msgs::msg::dds_::String_</dataType>"
					"</topic>"
				"</dds>";
	uint16_t topic_req = uxr_buffer_create_topic_xml(&session, reliable_out, topic_id, participant_id, topic_xml, UXR_REPLACE);

	uxrObjectId topic_sub_id = uxr_object_id(0x01, UXR_TOPIC_ID);
	const char *topic_sub_xml = "<dds>"
					"<topic>"
						"<name>rt/szc_test/sub</name>"
						"<dataType>std_msgs::msg::dds_::String_</dataType>"
					"</topic>"
				    "</dds>";
	uint16_t topic_sub_req = uxr_buffer_create_topic_xml(&session, reliable_out, topic_sub_id, participant_id, topic_sub_xml, UXR_REPLACE);

	uxrObjectId publisher_id = uxr_object_id(0x01, UXR_PUBLISHER_ID);
	const char *publisher_xml = "";
	uint16_t publisher_req = uxr_buffer_create_publisher_xml(&session, reliable_out, publisher_id, participant_id, publisher_xml, UXR_REPLACE);

	uxrObjectId subscriber_id = uxr_object_id(0x01, UXR_SUBSCRIBER_ID);
	const char *subscriber_xml = "";
	uint16_t subscriber_req = uxr_buffer_create_subscriber_xml(&session, reliable_out, subscriber_id, participant_id, subscriber_xml, UXR_REPLACE);

	uxrObjectId datawriter_id = uxr_object_id(0x01, UXR_DATAWRITER_ID);
	const char *datawriter_xml = "<dds>"
					"<data_writer>"
						"<topic>"
							"<kind>NO_KEY</kind>"
							"<name>rt/szc_test/pub</name>"
							"<dataType>std_msgs::msg::dds_::String_</dataType>"
						"</topic>"
					"</data_writer>"
				     "</dds>";
	uint16_t datawriter_req = uxr_buffer_create_datawriter_xml(&session, reliable_out, datawriter_id, publisher_id, datawriter_xml, UXR_REPLACE);

	uxrObjectId datareader_id = uxr_object_id(0x01, UXR_DATAREADER_ID);
	const char *datareader_xml = "<dds>"
					"<data_reader>"
						"<topic>"
							"<kind>NO_KEY</kind>"
							"<name>rt/szc_test/sub</name>"
							"<dataType>std_msgs::msg::dds_::String_</dataType>"
						"</topic>"
					"</data_reader>"
				     "</dds>";
	uint16_t datareader_req = uxr_buffer_create_datareader_xml(&session, reliable_out, datareader_id, subscriber_id, datareader_xml, UXR_REPLACE);

	// Send create entities message and wait its status
	uint8_t status[7];
	uint16_t requests[7] = {participant_req, topic_req, publisher_req, datawriter_req, topic_sub_req, subscriber_req, datareader_req};
	if (!uxr_run_session_until_all_status(&session, 1000, requests, status, 7))
	{
		printf("Error at create entities: participant: %i topic: %i publisher: %i darawriter: %i\n", status[0], status[1], status[2], status[3]);
		return 1;
	}

	// Request topics
	uxrDeliveryControl delivery_control = {0};
	delivery_control.max_samples = UXR_MAX_SAMPLES_UNLIMITED;
	uint16_t read_data_req = uxr_buffer_request_data(&session, reliable_out, datareader_id, reliable_in, &delivery_control);

	// Write topics
	bool connected = true;
	uint32_t count = 0;
	while (connected) {
		// uint8_t read_data_status;

		// printf(".");
		
		HelloWorld topic = {"Hello DDS world! szc"};
		// strcpy(topic.message, "hello");
		sprintf(topic.message, "count = %d", count);
		ucdrBuffer ub;
		uint32_t topic_size = HelloWorld_size_of_topic(&topic, 0);
		uxr_prepare_output_stream(&session, reliable_out, datawriter_id, &ub, topic_size);
		HelloWorld_serialize_topic(&ub, &topic);

		printf("Send topic: %s\n", topic.message);
		// connected = uxr_run_session_until_all_status(&session, 100, &read_data_req, &read_data_status, 1);
		// connected = uxr_run_session_until_confirm_delivery(&session, 1000);
		connected = uxr_run_session_time(&session, 10);
		// printf("%d\r\n", count);
		// osDelay(1000);
		count++;
	}

	// Delete resources
	uxr_delete_session(&session);
	uxr_close_udp_transport(&transport);
	printf("over\r\n");
	return 0;
}
