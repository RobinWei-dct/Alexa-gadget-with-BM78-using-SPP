/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9.3 at Sun Sep 22 15:19:27 2019. */

#include "alexaGadgetSpeechDataSpeechmarksDirectivePayload.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t alexaGadgetSpeechData_SpeechmarksDirectivePayloadProto_fields[3] = {
    PB_FIELD(  1, INT32   , SINGULAR, STATIC  , FIRST, alexaGadgetSpeechData_SpeechmarksDirectivePayloadProto, playerOffsetInMilliSeconds, playerOffsetInMilliSeconds, 0),
    PB_FIELD(  2, MESSAGE , REPEATED, STATIC  , OTHER, alexaGadgetSpeechData_SpeechmarksDirectivePayloadProto, speechmarksData, playerOffsetInMilliSeconds, &alexaGadgetSpeechData_SpeechmarksDirectivePayloadProto_SpeechmarksData_fields),
    PB_LAST_FIELD
};

const pb_field_t alexaGadgetSpeechData_SpeechmarksDirectivePayloadProto_SpeechmarksData_fields[4] = {
    PB_FIELD(  1, STRING  , SINGULAR, STATIC  , FIRST, alexaGadgetSpeechData_SpeechmarksDirectivePayloadProto_SpeechmarksData, value, value, 0),
    PB_FIELD(  2, STRING  , SINGULAR, STATIC  , OTHER, alexaGadgetSpeechData_SpeechmarksDirectivePayloadProto_SpeechmarksData, type, value, 0),
    PB_FIELD(  3, INT32   , SINGULAR, STATIC  , OTHER, alexaGadgetSpeechData_SpeechmarksDirectivePayloadProto_SpeechmarksData, startOffsetInMilliSeconds, type, 0),
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
PB_STATIC_ASSERT((pb_membersize(alexaGadgetSpeechData_SpeechmarksDirectivePayloadProto, speechmarksData[0]) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_alexaGadgetSpeechData_SpeechmarksDirectivePayloadProto_alexaGadgetSpeechData_SpeechmarksDirectivePayloadProto_SpeechmarksData)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_16BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in the default
 * 8 bit descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(alexaGadgetSpeechData_SpeechmarksDirectivePayloadProto, speechmarksData[0]) < 256), YOU_MUST_DEFINE_PB_FIELD_16BIT_FOR_MESSAGES_alexaGadgetSpeechData_SpeechmarksDirectivePayloadProto_alexaGadgetSpeechData_SpeechmarksDirectivePayloadProto_SpeechmarksData)
#endif


/* @@protoc_insertion_point(eof) */