//
// Copyright 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
// These materials are licensed under the Amazon Software License in connection with the Alexa Gadgets Program.
// The Agreement is available at https://aws.amazon.com/asl/.
// See the Agreement for the specific terms and conditions of the Agreement.
// Capitalized terms not defined in this file have the meanings given to them in the Agreement.
//
#include<stdio.h>
#include "pb.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "directiveParser.pb.h"
#include "eventParser.pb.h"
#include "notificationsSetIndicatorDirective.pb.h"
#include "alexaDiscoveryDiscoverResponseEvent.pb.h"
#include "alexaDiscoveryDiscoverDirective.pb.h"
#include "alexaGadgetStateListenerStateUpdateDirective.pb.h"
#include "alexaGadgetSpeechDataSpeechmarksDirective.pb.h"
#include "CustomEventProto.pb.h"
#include "proto_alexa_gadget.h"
#include <asf.h>


typedef unsigned char uint8_t;
typedef unsigned char BOOL;

extern int printf_cdc(const char *fmt, ...);
uint8_t encode_buffer[1024];
alexaDiscovery_DiscoverResponseEventProto discover_response_envelope = alexaDiscovery_DiscoverResponseEventProto_init_default;


size_t encode_custom_event(uint8_t* buffer, size_t len, char *namespace, char *name, char *payload)
{
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, len);
    CustomEventProto custom_event_envelope =
    CustomEventProto_init_default;

    strcpy(custom_event_envelope.event.header.namespace, namespace);
    strcpy(custom_event_envelope.event.header.name, name);

    strcpy(custom_event_envelope.event.payload, payload);

    BOOL status = pb_encode(&stream, CustomEventProto_fields, &custom_event_envelope);
    //decode_event(buffer, stream.bytes_written);
    if (!status)
    {
        printf_cdc("%s: Error encoding message\n", __FUNCTION__);
        return 0;
    }
    
    return stream.bytes_written;
}
// Decode proto buf encoded directive binary and print out the content.
GADET_PACK_TYPE decode_directive(uint8_t* buffer, int len, uint8_t** payload, int* len_payload)
{
    printf_cdc("\nParsing Directive:\r\n");
    pb_istream_t stream = pb_istream_from_buffer(buffer, len);
    directive_DirectiveParserProto envelope = directive_DirectiveParserProto_init_default;
    
    if(payload)
      *payload = NULL;
    *len_payload = 0;

    pb_decode(&stream, directive_DirectiveParserProto_fields, &envelope);

    printf_cdc("name = %s, namespace=%s\r\n", envelope.directive.header.name, envelope.directive.header.namespace);

    if (0 == strcmp(envelope.directive.header.name, "SetIndicator")
            && (0 == strcmp(envelope.directive.header.namespace, "Notifications"))) {
        // Parse the notification payload now
        pb_istream_t stream_notification = pb_istream_from_buffer(buffer, len);
        notifications_SetIndicatorDirectiveProto notifications_envelope = notifications_SetIndicatorDirectiveProto_init_default;
        pb_decode(&stream_notification, notifications_SetIndicatorDirectiveProto_fields, &notifications_envelope);

        printf_cdc("visualIndicator:%d, audioIndicator=%d, assetId=%s, url=%s\r\n",
        notifications_envelope.directive.payload.persistVisualIndicator,
        notifications_envelope.directive.payload.playAudioIndicator,
        notifications_envelope.directive.payload.asset.assetId,
        notifications_envelope.directive.payload.asset.url);
        
        return PACK_SET_INDICATOR;
    } else if (0 == strcmp(envelope.directive.header.name, "Discover")
            && (0 == strcmp(envelope.directive.header.namespace, "Alexa.Discovery"))) {
        pb_istream_t stream = pb_istream_from_buffer(buffer, len);
        alexaDiscovery_DiscoverDirectiveProto envelope = alexaDiscovery_DiscoverDirectiveProto_init_default;
        pb_decode(&stream, alexaDiscovery_DiscoverDirectiveProto_fields, &envelope);
        printf_cdc("scope type: %s\r\n", envelope.directive.payload.scope.type);
        printf_cdc("scope token: %s\r\n", envelope.directive.payload.scope.token);
        
        return PACK_DISCOVER;
    } else if (0 == strcmp(envelope.directive.header.name, "StateUpdate")
            && (0 == strcmp(envelope.directive.header.namespace, "Alexa.Gadget.StateListener"))) {
        pb_istream_t stream = pb_istream_from_buffer(buffer, len);
        alexaGadgetStateListener_StateUpdateDirectiveProto envelope = alexaGadgetStateListener_StateUpdateDirectiveProto_init_default;
        pb_decode(&stream, alexaGadgetStateListener_StateUpdateDirectiveProto_fields, &envelope);
        int states_count = envelope.directive.payload.states_count;
        for (int i = 0; i < states_count; ++i) {
            printf_cdc("state name: %s\r\n", envelope.directive.payload.states[i].name);
            printf_cdc("state value: %s\r\n", envelope.directive.payload.states[i].value);
        }
        
        return PACK_STATE_UPATE;
    } else if (0 == strcmp(envelope.directive.header.name, "Speechmarks")
            && (0 == strcmp(envelope.directive.header.namespace, "Alexa.Gadget.SpeechData"))) {
        pb_istream_t stream = pb_istream_from_buffer(buffer, len);
        alexaGadgetSpeechData_SpeechmarksDirectiveProto envelope = alexaGadgetSpeechData_SpeechmarksDirectiveProto_init_default;
        pb_decode(&stream, alexaGadgetSpeechData_SpeechmarksDirectiveProto_fields, &envelope);
        int speechmarks_count = envelope.directive.payload.speechmarksData_count;
        printf_cdc("player offset: %d\r\n", envelope.directive.payload.playerOffsetInMilliSeconds);
        for (int i = 0; i < speechmarks_count; ++i) {
            printf_cdc("speechmark type: %s\r\n", envelope.directive.payload.speechmarksData[i].type);
            printf_cdc("speechmark value: %s\r\n", envelope.directive.payload.speechmarksData[i].value);
            printf_cdc("speechmark start offset: %d\r\n", envelope.directive.payload.speechmarksData[i].startOffsetInMilliSeconds);
        }
        
        return PACK_SPEECH_MARKS;
    } else if (/*0 == strcmp(envelope.directive.header.name, "LED")
            && */(0 == strcmp(envelope.directive.header.namespace, "Custom.BM78"))) {
        
        printf_cdc("name=%s, namespace=%s, payload=%s\r\n",
        envelope.directive.header.name,
        envelope.directive.header.namespace,
        envelope.directive.payload.bytes);
        
        if(payload){
            *payload = malloc(strlen(envelope.directive.payload.bytes)+1);
            strcpy(*payload, envelope.directive.payload.bytes);
            *len_payload = strlen(envelope.directive.payload.bytes)+1;
        }

        return PACK_CUSTOM_IF;
    }
    else {
        printf_cdc("Error: do not have parsing code for this directive, check directive name\r\n");
        return PACK_TYPE_UNKNOWN;
    }
}

// Decode proto buf encoded event binary and print out the content.
void decode_event(uint8_t* buffer, int len)
{
    printf_cdc("\nParsing Event:\r\n");
    pb_istream_t stream = pb_istream_from_buffer(buffer, len);
    event_EventParserProto envelope = event_EventParserProto_init_default;

    pb_decode(&stream, event_EventParserProto_fields, &envelope);

    printf_cdc("name = %s, namespace=%s\r\n", envelope.event.header.name, envelope.event.header.namespace);

    if (0 == strcmp(envelope.event.header.name, "Discover.Response")
            && 0 == strcmp(envelope.event.header.namespace, "Alexa.Discovery")) {
        // Parse the discover response event binary
        pb_istream_t stream_discover_response = pb_istream_from_buffer(buffer, len);
        pb_decode(&stream_discover_response, alexaDiscovery_DiscoverResponseEventProto_fields, &discover_response_envelope);
        int endpoints_count = discover_response_envelope.event.payload.endpoints_count;
        printf_cdc("endpoint count : %d\r\n", endpoints_count);
        int endpoint_index;
        for (endpoint_index = 0; endpoint_index < endpoints_count; ++endpoint_index) {
            printf_cdc("endpoint id: %s\r\n", discover_response_envelope.event.payload.endpoints[endpoint_index].endpointId);
            printf_cdc("friendly name: %s\r\n", discover_response_envelope.event.payload.endpoints[endpoint_index].friendlyName);
            printf_cdc("description: %s\r\n", discover_response_envelope.event.payload.endpoints[endpoint_index].description);
            printf_cdc("manufacturer name: %s\r\n", discover_response_envelope.event.payload.endpoints[endpoint_index].manufacturerName);
            int capabilities_count = discover_response_envelope.event.payload.endpoints[endpoint_index].capabilities_count;
            int capability_index;
            for (capability_index = 0; capability_index < capabilities_count; ++capability_index) {
                printf_cdc("capability type: %s\r\n", discover_response_envelope.event.payload.endpoints[endpoint_index]
                        .capabilities[capability_index].type);
                printf_cdc("capability interface: %s\r\n", discover_response_envelope.event.payload.endpoints[endpoint_index]
                        .capabilities[capability_index].interface);
                printf_cdc("capability version: %s\r\n", discover_response_envelope.event.payload.endpoints[endpoint_index]
                        .capabilities[capability_index].version);
            }
            printf_cdc("additional id - firmware version: %s\r\n", discover_response_envelope.event.payload.endpoints[endpoint_index]
                    .additionalIdentification.firmwareVersion);
            printf_cdc("additional id - device token: %s\r\n", discover_response_envelope.event.payload.endpoints[endpoint_index]
                    .additionalIdentification.deviceToken);
            printf_cdc("additional id - device token encryption type: %s\r\n", discover_response_envelope.event.payload.endpoints[endpoint_index]
                    .additionalIdentification.deviceTokenEncryptionType);
            printf_cdc("additional id - amazon device type: %s\r\n", discover_response_envelope.event.payload.endpoints[endpoint_index]
                    .additionalIdentification.amazonDeviceType);
            printf_cdc("additional id - model name: %s\r\n", discover_response_envelope.event.payload.endpoints[endpoint_index]
                    .additionalIdentification.modelName);
            printf_cdc("additional id - radioAddress: %s\r\n", discover_response_envelope.event.payload.endpoints[endpoint_index]
                    .additionalIdentification.radioAddress);
        }
    }
}


// Encode set indicator directive and print out the encoded binary in hex.
void encode_set_indicator_directive(BOOL audio, BOOL visual, char* assetId, char* url)
{
/*
{
    "directive": {
        "header": {
            "namespace": "Notifications",
            "name": "SetIndicator",
            "messageId": "messageID1"
        },
        "payload": {
            "persistVisualIndicator": true,
            "playAudioIndicator": true,
            "asset": {
                "assetId": "assetID1",
                "url": "url1"
            }
        }
    }
}
*/

    printf_cdc("\nCreating setindicator directive:\r\n");
    uint8_t buffer[256];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    notifications_SetIndicatorDirectiveProto notifications_envelope = notifications_SetIndicatorDirectiveProto_init_default;

    strcpy(notifications_envelope.directive.header.namespace, "Notifications");
    strcpy(notifications_envelope.directive.header.name, "SetIndicator");

    notifications_envelope.directive.payload.persistVisualIndicator = visual;
    notifications_envelope.directive.payload.playAudioIndicator = audio;
    strcpy(notifications_envelope.directive.payload.asset.assetId, assetId);
    strcpy(notifications_envelope.directive.payload.asset.url, url);

    BOOL status = pb_encode(&stream, notifications_SetIndicatorDirectiveProto_fields, &notifications_envelope);
    if (!status)
    {
      printf_cdc("%s: Error encoding message\r\n", __FUNCTION__);
      return;
    }

    printf_cdc("bytes written:%zu\r\n", stream.bytes_written);
    uint8_t index;
    for(index = 0; index < stream.bytes_written; index++)
    {
        printf_cdc("0x%02x ", buffer[index]);
    }
    printf_cdc("\r\n");
}


void * encode_alexaGadget_discover_response_event(void * len_p)
{
    /*
    {
        "event": {
            "header": {
                "namespace": "Alexa.Discovery",
                "name": "Discover.Response",
                "messageId": ""
            },
            "payload": {
                "endpoints": [{
                    "endpointId": "test id",
                    "friendlyName": "friendly name",
                    "capabilities": [
                    {
                        "type": "test type 1",
                        "interface": "Test interface 1",
                        "version": "1.0"
                    },
                    {
                        "type": "test type 2",
                        "interface": "Test interface 2",
                        "version": "1.0"
                    },
                    {
                        "type": "test type 3",
                        "interface": "Test interface 3",
                        "version": "1.1"
                    }],
                    "additionalIdentification": {
                        "firmwareVersion": "19",
                        "deviceToken": "xxxxxxxxx",
                        "deviceTokenEncryptionType": "yyy",
                        "amazonDeviceType": "aabbccd",
                        "modelName": "mock model name",
                        "radioAddress": "1234567890"
                    }
                }]
            }
        }
    }
    */
    printf_cdc("\nCreating discover response event: size=%d\r\n", sizeof(alexaDiscovery_DiscoverResponseEventProto));
    pb_ostream_t stream = pb_ostream_from_buffer(encode_buffer, sizeof(encode_buffer));
    

    strcpy(discover_response_envelope.event.header.namespace, "Alexa.Discovery");
    strcpy(discover_response_envelope.event.header.name, "Discover.Response");

    discover_response_envelope.event.payload.endpoints_count = 1;
    strcpy(discover_response_envelope.event.payload.endpoints[0].endpointId, "BM78GADGET001");//use to cal SHA256
    strcpy(discover_response_envelope.event.payload.endpoints[0].friendlyName, "BM78Gadget");
    strcpy(discover_response_envelope.event.payload.endpoints[0].manufacturerName, "MICROCHIP");
    strcpy(discover_response_envelope.event.payload.endpoints[0].description, "MICROCHIP BM78 Gadget Demo");
    discover_response_envelope.event.payload.endpoints[0].capabilities_count = 6;
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[0].type, "AlexaInterface");
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[0].interface, "Alerts");
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[0].version, "1.1");
    
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[1].type, "AlexaInterface");
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[1].interface, "Alexa.Gadget.MusicData");
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[1].version, "1.0");
    discover_response_envelope.event.payload.endpoints[0].capabilities[1].configuration.supportedTypes_count = 1;
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[1].configuration.supportedTypes[0].name, "tempo");
        
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[2].type, "AlexaInterface");
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[2].interface, "Alexa.Gadget.SpeechData");
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[2].version, "1.0");
    discover_response_envelope.event.payload.endpoints[0].capabilities[2].configuration.supportedTypes_count = 1;
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[2].configuration.supportedTypes[0].name, "viseme");
    
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[3].type, "AlexaInterface");
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[3].interface, "Alexa.Gadget.StateListener");
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[3].version, "1.0");
    discover_response_envelope.event.payload.endpoints[0].capabilities[3].configuration.supportedTypes_count = 5;
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[3].configuration.supportedTypes[0].name, "alarms");
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[3].configuration.supportedTypes[1].name, "timers");
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[3].configuration.supportedTypes[2].name, "reminders");
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[3].configuration.supportedTypes[3].name, "wakeword");
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[3].configuration.supportedTypes[4].name, "timeinfo");
    
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[4].type, "AlexaInterface");
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[4].interface, "Notifications");
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[4].version, "1.0");
    
    //add for custom interface
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[5].type, "AlexaInterface");
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[5].interface, "Custom.BM78");
    strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[5].version, "1.0");
    
    strcpy(discover_response_envelope.event.payload.endpoints[0].additionalIdentification.firmwareVersion, "001");//below is calculated online with web page
    strcpy(discover_response_envelope.event.payload.endpoints[0].additionalIdentification.deviceToken, "4ce57783cae6a5e750bb4d4012ad8fbe87501513edc9280bbc9eb0240b3cf39e");//UTF-8-encode( SHA256(endpointId+Alexa Gadget Secret) )
    strcpy(discover_response_envelope.event.payload.endpoints[0].additionalIdentification.deviceTokenEncryptionType, "1");//SHA256
    strcpy(discover_response_envelope.event.payload.endpoints[0].additionalIdentification.amazonDeviceType, "A1Q4ZVZ7O79248");//Amazon ID
    strcpy(discover_response_envelope.event.payload.endpoints[0].additionalIdentification.modelName, "BM78Gadget-demo");
    strcpy(discover_response_envelope.event.payload.endpoints[0].additionalIdentification.radioAddress, "1234567890");

    BOOL status = pb_encode(&stream, alexaDiscovery_DiscoverResponseEventProto_fields, &discover_response_envelope);
    if (!status)
    {
      printf_cdc("%s: Error encoding message\r\n", __FUNCTION__);
      return NULL;
    }

    printf_cdc("bytes written:%d\r\n", stream.bytes_written);
    #if 0
    uint8_t index;
    for(index = 0; index < stream.bytes_written; index++)
    {
        printf_cdc("0x%02x ", encode_buffer[index]);
    }
    #endif
    
    printf_cdc("\r\n");
    
    *((uint16_t*)len_p) = stream.bytes_written;
    return ((void*)(encode_buffer));
    //decode_event(encode_buffer, stream.bytes_written);
}


void encode_alexaGadget_state_update_directive(void)
{
    /*
    {
        "directive": {
            "header": {
                "namespace": "Alexa.Gadget.StateListener",
                "name": "StateUpdate",
                "messageId": ""
            },
            "payload": {
                "states": [{
                    "name": "timers",
                    "value": "active"
                }]
            }
        }
    }
    */

    printf_cdc("\nCreating state update directive:\r\n");
    uint8_t buffer[256];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    alexaGadgetStateListener_StateUpdateDirectiveProto envelope = alexaGadgetStateListener_StateUpdateDirectiveProto_init_default;

    strcpy(envelope.directive.header.namespace, "Alexa.Gadget.StateListener");
    strcpy(envelope.directive.header.name, "StateUpdate");
    envelope.directive.payload.states_count = 1;
    strcpy(envelope.directive.payload.states[0].name, "timers");
    strcpy(envelope.directive.payload.states[0].value, "active");

    // discover directive has empty payload right now
    BOOL status = pb_encode(&stream, alexaGadgetStateListener_StateUpdateDirectiveProto_fields, &envelope);
    if (!status)
    {
      printf_cdc("%s: Error encoding message\r\n", __FUNCTION__);
      return;
    }

    printf_cdc("bytes written:%zu\r\n", stream.bytes_written);
    uint8_t index;
    for(index = 0; index < stream.bytes_written; index++)
    {
        printf_cdc("0x%02x ", buffer[index]);
    }
    printf_cdc("\r\n");
    decode_directive(buffer, stream.bytes_written, NULL, NULL);
}

void encode_sample_speechmarks_directive(void)
{
    /*
    {
        "directive": {
            "header": {
                "namespace": "Alexa.Gadget.SpeechData",
                "name": "Speechmarks",
                "messageId": ""
            },
            "payload": {
                "playerOffsetInMilliSeconds": 0
                "speechmarksData": [{
                    "type": "viseme",
                    "value": "s",
                    "startOffsetInMilliSeconds": 130
                }]
            }
        }
    }
    */

    printf_cdc("\nCreating speechmarks directive:\r\n");
    uint8_t buffer[256];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    alexaGadgetSpeechData_SpeechmarksDirectiveProto envelope = alexaGadgetSpeechData_SpeechmarksDirectiveProto_init_default;

    strcpy(envelope.directive.header.namespace, "Alexa.Gadget.SpeechData");
    strcpy(envelope.directive.header.name, "Speechmarks");
    envelope.directive.payload.playerOffsetInMilliSeconds = 0;
    envelope.directive.payload.speechmarksData_count = 1;
    strcpy(envelope.directive.payload.speechmarksData[0].type, "viseme");
    strcpy(envelope.directive.payload.speechmarksData[0].value, "s");
    envelope.directive.payload.speechmarksData[0].startOffsetInMilliSeconds = 130;

    // discover directive has empty payload right now
    BOOL status = pb_encode(&stream, alexaGadgetSpeechData_SpeechmarksDirectiveProto_fields, &envelope);
    if (!status)
    {
      printf_cdc("%s: Error encoding message\r\n", __FUNCTION__);
      return;
    }

    printf_cdc("bytes written:%zu\r\n", stream.bytes_written);
    uint8_t index;
    for(index = 0; index < stream.bytes_written; index++)
    {
        printf_cdc("0x%02x ", buffer[index]);
    }
    printf_cdc("\r\n");
    decode_directive(buffer, stream.bytes_written, NULL, NULL);
}


// Sample byte array that represents a serialized protobuf message
int main_sample(int argc, char** argv)
{
    printf_cdc("\nNotifications - SetIndicator Example\r\n");
    // decoding and encoding of notifcation - set indicator directive
    uint8_t notification_binary[] = {
    		 0x0a, 0x37, 0x0a, 0x1d, 0x0a, 0x0d, 0x4e, 0x6f, 0x74, 0x69,
		 0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x12,
		 0x0c, 0x53, 0x65, 0x74, 0x49, 0x6e, 0x64, 0x69, 0x63, 0x61,
		 0x74, 0x6f, 0x72, 0x12, 0x16, 0x08, 0x01, 0x10, 0x01, 0x1a,
		 0x10, 0x0a, 0x08, 0x61, 0x73, 0x73, 0x65, 0x74, 0x49, 0x44,
		 0x31, 0x12, 0x04, 0x75, 0x72, 0x6c, 0x31};
    decode_directive(notification_binary, sizeof(notification_binary), NULL, NULL);

    // creating sample set indicator directive
    encode_set_indicator_directive(1, 1, "assetID1", "url1");

    printf_cdc("\nAlexa.Discovery - Discover.Response Example\r\n");
    uint16_t len = 0;
    encode_alexaGadget_discover_response_event(&len);

    printf_cdc("\nAlexa.Gadget.SpeechData - Speechmarks Example\r\n");
    encode_sample_speechmarks_directive();

    return 0;
}
