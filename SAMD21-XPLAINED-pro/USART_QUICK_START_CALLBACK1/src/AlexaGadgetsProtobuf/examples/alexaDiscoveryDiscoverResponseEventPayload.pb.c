/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9.3 at Sun Sep 22 15:19:30 2019. */

#include "alexaDiscoveryDiscoverResponseEventPayload.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t alexaDiscovery_DiscoverResponseEventPayloadProto_fields[2] = {
    PB_FIELD(  1, MESSAGE , REPEATED, STATIC  , FIRST, alexaDiscovery_DiscoverResponseEventPayloadProto, endpoints, endpoints, &alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_fields),
    PB_LAST_FIELD
};

const pb_field_t alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_fields[7] = {
    PB_FIELD(  1, STRING  , SINGULAR, STATIC  , FIRST, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints, endpointId, endpointId, 0),
    PB_FIELD(  2, STRING  , SINGULAR, STATIC  , OTHER, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints, friendlyName, endpointId, 0),
    PB_FIELD(  3, STRING  , SINGULAR, STATIC  , OTHER, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints, description, friendlyName, 0),
    PB_FIELD(  4, STRING  , SINGULAR, STATIC  , OTHER, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints, manufacturerName, description, 0),
    PB_FIELD( 11, MESSAGE , REPEATED, STATIC  , OTHER, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints, capabilities, manufacturerName, &alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_fields),
    PB_FIELD( 12, MESSAGE , SINGULAR, STATIC  , OTHER, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints, additionalIdentification, capabilities, &alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification_fields),
    PB_LAST_FIELD
};

const pb_field_t alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_fields[5] = {
    PB_FIELD(  1, STRING  , SINGULAR, STATIC  , FIRST, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities, type, type, 0),
    PB_FIELD(  2, STRING  , SINGULAR, STATIC  , OTHER, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities, interface, type, 0),
    PB_FIELD(  3, STRING  , SINGULAR, STATIC  , OTHER, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities, version, interface, 0),
    PB_FIELD(  4, MESSAGE , SINGULAR, STATIC  , OTHER, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities, configuration, version, &alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_fields),
    PB_LAST_FIELD
};

const pb_field_t alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_fields[2] = {
    PB_FIELD(  1, MESSAGE , REPEATED, STATIC  , FIRST, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration, supportedTypes, supportedTypes, &alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_fields),
    PB_LAST_FIELD
};

const pb_field_t alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_fields[2] = {
    PB_FIELD(  1, STRING  , SINGULAR, STATIC  , FIRST, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes, name, name, 0),
    PB_LAST_FIELD
};

const pb_field_t alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification_fields[7] = {
    PB_FIELD(  1, STRING  , SINGULAR, STATIC  , FIRST, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification, firmwareVersion, firmwareVersion, 0),
    PB_FIELD(  2, STRING  , SINGULAR, STATIC  , OTHER, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification, deviceToken, firmwareVersion, 0),
    PB_FIELD(  3, STRING  , SINGULAR, STATIC  , OTHER, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification, deviceTokenEncryptionType, deviceToken, 0),
    PB_FIELD(  4, STRING  , SINGULAR, STATIC  , OTHER, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification, amazonDeviceType, deviceTokenEncryptionType, 0),
    PB_FIELD(  5, STRING  , SINGULAR, STATIC  , OTHER, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification, modelName, amazonDeviceType, 0),
    PB_FIELD(  6, STRING  , SINGULAR, STATIC  , OTHER, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification, radioAddress, modelName, 0),
    PB_LAST_FIELD
};


/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_32BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in 8 or 16 bit
 * field descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(alexaDiscovery_DiscoverResponseEventPayloadProto, endpoints[0]) < 65536 && pb_membersize(alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints, capabilities[0]) < 65536 && pb_membersize(alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints, additionalIdentification) < 65536 && pb_membersize(alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities, configuration) < 65536 && pb_membersize(alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration, supportedTypes[0]) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_alexaDiscovery_DiscoverResponseEventPayloadProto_alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_16BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in the default
 * 8 bit descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(alexaDiscovery_DiscoverResponseEventPayloadProto, endpoints[0]) < 256 && pb_membersize(alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints, capabilities[0]) < 256 && pb_membersize(alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints, additionalIdentification) < 256 && pb_membersize(alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities, configuration) < 256 && pb_membersize(alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration, supportedTypes[0]) < 256), YOU_MUST_DEFINE_PB_FIELD_16BIT_FOR_MESSAGES_alexaDiscovery_DiscoverResponseEventPayloadProto_alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification)
#endif


/* @@protoc_insertion_point(eof) */
