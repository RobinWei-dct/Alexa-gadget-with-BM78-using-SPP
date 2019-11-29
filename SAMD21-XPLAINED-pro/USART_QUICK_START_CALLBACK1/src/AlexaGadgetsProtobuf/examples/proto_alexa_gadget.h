/*
 * proto_alexa_gadget.h
 *
 * Created: 9/17/2019 10:52:57 AM
 *  Author: A16263
 */ 


#ifndef PROTO_ALEXA_GADGET_H_
#define PROTO_ALEXA_GADGET_H_

typedef enum ALEXA_GADGET_PACK_TYPE{
    PACK_TYPE_INVALID = 0x00,
    PACK_SET_ALERT,
    PACK_DEL_ALERT,
    PACK_DISCOVER,
    PACK_TEMPO,
    PACK_SPEECH_MARKS,
    PACK_STATE_UPATE,
    PACK_SET_INDICATOR,
    PACK_CLEAR_INDICATOR,
    PACK_CUSTOM_IF, 
    PACK_TYPE_UNKNOWN,   
}GADET_PACK_TYPE;    

GADET_PACK_TYPE decode_directive(uint8_t* buffer, int len, uint8_t** payload, int* len_payload);
void decode_event(uint8_t* buffer, int len);
void * encode_alexaGadget_discover_response_event(void * len_p);
void encode_alexaGadget_state_update_directive(void);
size_t encode_custom_event(uint8_t* buffer, size_t len, char *namespace, char *name, char *payload);



#endif /* PROTO_ALEXA_GADGET_H_ */