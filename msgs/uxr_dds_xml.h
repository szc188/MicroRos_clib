#ifndef __SZC_UXR_DDS_XML_H_
#define __SZC_UXR_DDS_XML_H_

#ifdef __cplusplus
extern "C" {
#endif

#define UXR_PARTICIPANT_XML(name) "<dds><participant><rtps><name>"#name"</name></rtps></participant></dds>"
#define UXR_WRITER_XML(name, type) "<dds><data_writer><topic><kind>NO_KEY</kind><name>"#name"</name><dataType>"#type"</dataType></topic></data_writer></dds>"
#define UXR_READER_XML(name, type) "<dds><data_reader><topic><kind>NO_KEY</kind><name>"#name"</name><dataType>"#type"</dataType></topic></data_reader></dds>"
#define UXR_TOPIC_XML(name, type) "<dds><topic><name>"#name"</name><dataType>"#type"</dataType></topic></dds>"

#ifdef __cplusplus
}
#endif

#endif
