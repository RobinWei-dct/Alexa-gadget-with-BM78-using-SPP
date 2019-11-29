# Alexa-gadget-with-BM78-using-SPP
This is an Alexa Gadget demo using MICROCHIP BM78+SAMD21 EVB. Refer to Alexa-gadget-using-BM78.jpg for whole demo set configuration.

Alexa gadget required feature is implemented in BM78.

BM78 EVB: https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/BM-78-PICTAIL

Alexa gadget application level is implemented in SAMD21.

SAMD21 EVB: https://www.microchip.com/DevelopmentTools/ProductDetails/ATSAMD21-XPRO

1. wakeup the echo dot by speaking 'Alexa'
2. open the demo gadget by speaking 'Open dodo gadget'
3. User can speak 'light on/off/blink' to echo speaker to control the LED0 on SAMD21 EVB.
4. User can press the SW0 button to send an event to Alexa Skill during the active session.

# KNOWN ISSUES
1. Alexa can't find the gadget after Echo speaker pairing with the gadget

   [fix]: Power up Echo speaker first and check if it's online or not.
          If Echo speacker is online, power up the gadget later. If everything goes well the LED on BM78 EVB is blinking and LED0 on SAMD21  EVB is blinking fast.If BM78 LED doesn't in connected blinking mode, press RESET button on the BM78 EVB to force BM78 to conduct a link back to the paired Echo speaker. If the LED0 on SAMD21 isn't blinking fast, also press RESET button on the BM78 EVB.

    [hints]: BM78 LED blinking fast indicates the SPP link to Echo speaker is created.
             SAMD21 LED0 blinking fast indicates SAMD21 has received the 'Discover' instructive sent from Echo speaker and replied with 'Discover.Response' event back to Echo speaker.

2. Alexa skill received only once event reported by alexa gadget

   [fix]: to be provided.
   
# References
1. Alexa gadget design guide
   https://developer.amazon.com/docs/alexa-gadgets-toolkit/understand-alexa-gadgets-toolkit.html
   
2. Some JS code refers to 
   https://github.com/alexa/Alexa-Gadgets-Raspberry-Pi-Samples/tree/master/src/examples/colorcycler/skill/NodeJS/lambda/custom
