// This sample demonstrates handling intents from an Alexa skill using the Alexa Skills Kit SDK (v2).
// Please visit https://alexa.design/cookbook for additional examples on implementing slots, dialog management,
// session persistence, api calls, and more.
const Alexa = require('ask-sdk-core');
const Uuid = require('uuid/v4');

//add for gadget access
var apiEndpoint = '';
var endpointId; //represent the targeted gadget

//token for custom event filtering
const CUSTOM_EVENT_token = '8643b5c1-f15e-4c0d-9e70-af6ec22e428f'

const https = require('https')
const LEDSTATE = ['on', 'off', 'blink', 'brighter', 'darker']

function buildStartEventHandlerDirective(token, endpoint, namespace, filterMatchAction, durationMs, expirationPayload) {
    let ret = {
        type: "CustomInterfaceController.StartEventHandler",
        token: token,
        eventFilter: {
            filterExpression: {
                'and': [
                        {'==': [{'var': 'header.namespace'}, namespace]},
                        {'==': [{'var': 'endpoint.endpointId' }, endpoint]}
                    ]
            },
            filterMatchAction: filterMatchAction
        },
        expiration: {
            durationInMilliseconds: durationMs,
            expirationPayload: expirationPayload
        }
    };
    console.log('buildStartEventHandlerDirective:', JSON.stringify(ret) )
    return ret;
}
function buildLEDDirective(endpointId, ledId, ledState) {
    return {
        type: 'CustomInterfaceController.SendDirective',
        header: {
            name: 'LED',
            namespace: 'Custom.BM78'
        },
        endpoint: {
            endpointId: endpointId
        },
        payload: {
            ledId: ledId,
            ledState: ledState,
        }
    };
}

function getConnectedEndpoints(apiEndpoint, apiAccessToken) {
    apiEndpoint = (apiEndpoint || '').replace('https://', '');

    return new Promise(((resolve, reject) => {
        var options = {
            host: apiEndpoint,
            path: '/v1/endpoints',
            method: 'GET',
            headers: {
                'Content-Type': 'application/json',
                'Authorization': 'Bearer ' + apiAccessToken
            }
        };

        const request = https.request(options, (response) => {
            response.setEncoding('utf8');
            let returnData = '';

            response.on('data', (chunk) => {
                returnData += chunk;
            });

            response.on('end', () => {
                resolve(JSON.parse(returnData));
            });

            response.on('error', (error) => {
                reject(error);
            });
        });
        request.end();
    }));
}

const LaunchRequestHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'LaunchRequest';
    },
    async handle(handlerInput) {
        const speakOutput = 'Welcome to dodo gadget, you can say light on, or  Help. Which would you like to try?';
        console.log('LaunchRequestHandler:handlerInput:', JSON.stringify(handlerInput) )
        
        //1. prepare the GET 
        var apiAccessToken = handlerInput.requestEnvelope.context.System.apiAccessToken
        if(apiEndpoint === '' ){
          apiEndpoint = handlerInput.requestEnvelope.context.System.apiEndpoint
          apiEndpoint = apiEndpoint.replace('https://','') //remove https:// which casue problem
        }
          
        //2. send GET
        try {
                // Get connected gadget endpointId.
                console.log("Checking endpoint");
                let response = await getConnectedEndpoints(apiEndpoint, apiAccessToken);
                console.log("v1/endpoints response: " + JSON.stringify(response));

                if ((response.endpoints || []).length === 0) {
                    console.log('No connected gadget endpoints available');
                    endpointId = '';//reset it
                    return handlerInput.responseBuilder
                        .speak("No gadgets found. Please try again after connecting your gadget.")
                        .withShouldEndSession(true)
                        .getResponse();
                }

                endpointId = response.endpoints[0].endpointId;

                // Store endpointId for using it to send custom directives later.
                console.log("Received endpoints. Storing Endpoint Id: " + endpointId);
                const attributesManager = handlerInput.attributesManager;
                let sessionAttributes = attributesManager.getSessionAttributes();
                sessionAttributes.endpointId = endpointId;
                attributesManager.setSessionAttributes(sessionAttributes);
            }
            catch (err) {
                console.log("An error occurred while getting endpoints", err);
                return handlerInput.responseBuilder
                    .speak("I wasn't able to get connected endpoints. Please try again.")
                    .withShouldEndSession(true)
                    .getResponse();
            }
        
        let response = handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .withShouldEndSession(false)
            .addDirective(buildStartEventHandlerDirective(Uuid(), endpointId,
                    'Custom.BM78', 'SEND_AND_TERMINATE',
                    80000,
                    { 'data': "You didn't press any button. Good bye!" }))
            .getResponse();
        
        return response
    }
};

const HelpIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.HelpIntent';
    },
    handle(handlerInput) {
        const speakOutput = 'You can say light on, off, or blink to me! How can I help?';

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};

const lightIntentHandler = {
    canHandle(handlerInput) {
        console.log('lightIntentHandler:handlerInput:', JSON.stringify(handlerInput) )
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && (Alexa.getIntentName(handlerInput.requestEnvelope) === 'lightIntent' );
    },
    async handle(handlerInput) {
        console.log('lightIntentHandler:handlerInput2:', JSON.stringify(handlerInput) )
        
        //0. collect slots
        var obj = 'light'
        if( handlerInput.requestEnvelope.request.intent.name === 'lightIntent' ){
            obj = 'light'
             console.log('lightIntentHandler:handlerInput2:obj1', obj )
        }
        else if(handlerInput.requestEnvelope.request.intent.name === 'ledIntent'){
            obj = 'LED'
             console.log('lightIntentHandler:handlerInput2:obj2', obj )
        }
          
        console.log('lightIntentHandler:handlerInput2:obj3', obj )
          
                    
        let ledID = handlerInput.requestEnvelope.request.intent.slots.id.value;
        let ledState = handlerInput.requestEnvelope.request.intent.slots.state.value;
        if( typeof ledID === 'undefined' || ledID === '?')
          ledID = 0;
        if( !LEDSTATE.includes(ledState) ){
            return handlerInput.responseBuilder
                    .speak(`Sorry, ${obj} command is invalid. Please try again.`)
                    .withShouldEndSession(true)
                    .getResponse();
        }
        
        
        
        if( endpointId === ''){
            //1. prepare the GET 
            var apiAccessToken = handlerInput.requestEnvelope.context.System.apiAccessToken
            if(apiEndpoint === '' ){
              apiEndpoint = handlerInput.requestEnvelope.context.System.apiEndpoint
              apiEndpoint = apiEndpoint.replace('https://','') //remove https:// which casue problem
            }
          
            //2. send GET
            try {
                  // Get connected gadget endpointId.
                console.log("Checking endpoint");
                let response = await getConnectedEndpoints(apiEndpoint, apiAccessToken);
                console.log("v1/endpoints response: " + JSON.stringify(response));

                if ((response.endpoints || []).length === 0) {
                     console.log('No connected gadget endpoints available2');
                     endpointId = '';//reset it
                    return handlerInput.responseBuilder
                        .speak("No gadgets found. Please try again after connecting your gadget.")
                        .withShouldEndSession(true)
                        .getResponse();
                }

                endpointId = response.endpoints[0].endpointId;

                // Store endpointId for using it to send custom directives later.
                console.log("Received endpoints. Storing Endpoint Id: " + endpointId);
                const attributesManager = handlerInput.attributesManager;
                let sessionAttributes = attributesManager.getSessionAttributes();
                sessionAttributes.endpointId = endpointId;
                attributesManager.setSessionAttributes(sessionAttributes);
            }
            catch (err) {
                console.log("An error occurred while getting endpoints", err);
                return handlerInput.responseBuilder
                    .speak("Sorry,an error occurred while inquiring gadgets. Please try again.")
                    .withShouldEndSession(true)
                    .getResponse();
            } 
        }
        else{
            console.log('ledId, ledState',ledID, ledState)
            let prompt = `${obj} control is done!`
            if(ledState === 'blink')
                prompt = `${obj} now is blinking`
            else
                prompt = `${obj} now is ${ledState}`
        
            //1. send custom directive if endpoint/gadget exist
            return handlerInput.responseBuilder
                .speak(prompt)
                .withShouldEndSession(true)
                .addDirective(buildLEDDirective(endpointId, ledID, ledState))
                .getResponse();
        }
    }
};
const CustomInterfaceEventHandler = {
        canHandle(handlerInput) {
            let { request } = handlerInput.requestEnvelope;
            console.log('CustomInterfaceEventHandler:handlerInput:', JSON.stringify(handlerInput))
            
            return request.type === 'CustomInterfaceController.EventsReceived';
        },
        handle(handlerInput) {
            let { request } = handlerInput.requestEnvelope;
            let customEvent = request.events[0];
            console.log('customEvent',customEvent)
            
            var state;
            
            if( customEvent.payload.edge === 'failing' )
              state = 'pressed'
            else if( customEvent.payload.edge === 'rising' )
              state = 'released'
            else
              state = 'in unknown state'
              
            let apiAccessToken = handlerInput.requestEnvelope.context.System.apiAccessToken;
            if (customEvent.header.namespace === 'Custom.BM78' && customEvent.header.name === 'BUTTON') {
                return handlerInput.responseBuilder
                .speak('Button' + customEvent.payload.id + 'is' + state)
                //.reprompt('add a reprompt if you want to keep the session open for the user to respond')
                .withShouldEndSession(true)
                .getResponse();
            }
            
            return handlerInput.responseBuilder
                    .speak("Sorry,the event from gadgets is not understood.")
                    .withShouldEndSession(true)
                    //.reprompt('add a reprompt if you want to keep the session open for the user to respond')
                    .getResponse();
        }
    };
const CustomInterfaceExpirationHandler = {
        canHandle(handlerInput) {
            let { request } = handlerInput.requestEnvelope;
            console.log("CustomInterfaceExpirationHandler: checking if it can handle " + request.type);
            return request.type === 'CustomInterfaceController.Expired';
        },
        handle(handlerInput) {
            console.log("== Custom Event Expiration Input ==", JSON.stringify(handlerInput));

            let { request } = handlerInput.requestEnvelope;

            const attributesManager = handlerInput.attributesManager;
            let sessionAttributes = attributesManager.getSessionAttributes();

            // When the EventHandler expires, send StopLED directive to stop LED animation
            // and end skill session.
            return handlerInput.responseBuilder
                .speak(request.expirationPayload.data)
                .withShouldEndSession(true)
                .getResponse();
        }
    };
const CancelAndStopIntentHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest'
            && (Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.CancelIntent'
                || Alexa.getIntentName(handlerInput.requestEnvelope) === 'AMAZON.StopIntent');
    },
    handle(handlerInput) {
        const speakOutput = 'Goodbye!';
        return handlerInput.responseBuilder
            .speak(speakOutput)
            .getResponse();
    }
};
const SessionEndedRequestHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'SessionEndedRequest';
    },
    handle(handlerInput) {
        // Any cleanup logic goes here.
        return handlerInput.responseBuilder.getResponse();
    }
};

// The intent reflector is used for interaction model testing and debugging.
// It will simply repeat the intent the user said. You can create custom handlers
// for your intents by defining them above, then also adding them to the request
// handler chain below.
const IntentReflectorHandler = {
    canHandle(handlerInput) {
        return Alexa.getRequestType(handlerInput.requestEnvelope) === 'IntentRequest';
    },
    handle(handlerInput) {
        const intentName = Alexa.getIntentName(handlerInput.requestEnvelope);
        const speakOutput = `You just triggered ${intentName}`;

        return handlerInput.responseBuilder
            .speak(speakOutput)
            //.reprompt('add a reprompt if you want to keep the session open for the user to respond')
            .getResponse();
    }
};

// Generic error handling to capture any syntax or routing errors. If you receive an error
// stating the request handler chain is not found, you have not implemented a handler for
// the intent being invoked or included it in the skill builder below.
const ErrorHandler = {
    canHandle() {
        return true;
    },
    handle(handlerInput, error) {
        console.log(`~~~~ Error handled: ${error}`);
        const speakOutput = `Sorry, I had trouble doing what you asked. Please try again.`;

        return handlerInput.responseBuilder
            .speak(speakOutput)
            .reprompt(speakOutput)
            .getResponse();
    }
};

// The SkillBuilder acts as the entry point for your skill, routing all request and response
// payloads to the handlers above. Make sure any new handlers or interceptors you've
// defined are included below. The order matters - they're processed top to bottom.
exports.handler = Alexa.SkillBuilders.custom()
    .addRequestHandlers(
        LaunchRequestHandler,
        CustomInterfaceEventHandler,
        CustomInterfaceExpirationHandler,
        HelpIntentHandler,
        CancelAndStopIntentHandler,
        lightIntentHandler,
        SessionEndedRequestHandler,
        IntentReflectorHandler, // make sure IntentReflectorHandler is last so it doesn't override your custom intent handlers
    )
    .addErrorHandlers(
        ErrorHandler,
    )
    .lambda();
