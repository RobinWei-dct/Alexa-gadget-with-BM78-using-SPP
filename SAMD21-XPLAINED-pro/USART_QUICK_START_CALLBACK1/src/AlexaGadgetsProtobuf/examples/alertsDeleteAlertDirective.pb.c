/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9.3 at Sun Sep 22 15:19:26 2019. */

#include "alertsDeleteAlertDirective.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t alerts_DeleteAlertDirectiveProto_fields[2] = {
    PB_FIELD(  1, MESSAGE , SINGULAR, STATIC  , FIRST, alerts_DeleteAlertDirectiveProto, directive, directive, &alerts_DeleteAlertDirectiveProto_Directive_fields),
    PB_LAST_FIELD
};

const pb_field_t alerts_DeleteAlertDirectiveProto_Directive_fields[3] = {
    PB_FIELD(  1, MESSAGE , SINGULAR, STATIC  , FIRST, alerts_DeleteAlertDirectiveProto_Directive, header, header, &header_DirectiveHeaderProto_fields),
    PB_FIELD(  2, MESSAGE , SINGULAR, STATIC  , OTHER, alerts_DeleteAlertDirectiveProto_Directive, payload, header, &alerts_DeleteAlertDirectivePayloadProto_fields),
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
PB_STATIC_ASSERT((pb_membersize(alerts_DeleteAlertDirectiveProto, directive) < 65536 && pb_membersize(alerts_DeleteAlertDirectiveProto_Directive, payload) < 65536 && pb_membersize(alerts_DeleteAlertDirectiveProto_Directive, header) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_alerts_DeleteAlertDirectiveProto_alerts_DeleteAlertDirectiveProto_Directive)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_16BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in the default
 * 8 bit descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(alerts_DeleteAlertDirectiveProto, directive) < 256 && pb_membersize(alerts_DeleteAlertDirectiveProto_Directive, payload) < 256 && pb_membersize(alerts_DeleteAlertDirectiveProto_Directive, header) < 256), YOU_MUST_DEFINE_PB_FIELD_16BIT_FOR_MESSAGES_alerts_DeleteAlertDirectiveProto_alerts_DeleteAlertDirectiveProto_Directive)
#endif


/* @@protoc_insertion_point(eof) */