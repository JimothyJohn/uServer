# Layout

All web FILES (.html, .css, .js) are inside [`include/WebStrings.h`](include/WebStrings.h) as Strings.

'''cpp
String cssString
String javaString
String headerString
String footerString
// Each page is a bodyFunction()
String bodyfunctionStatic()
// You can pass values to these functions
String bodyfunctionNumber(uint8_t num)
'''

When assigning callback functions you can use these bodyFunctions() to both send and receive values

'''cpp
// Send bodyfunctionNumber() for http://userver.local/number
server.on("/number", HTTP_GET, [](AsyncWebServerRequest *request){
    String number;
    if (request->hasParam("number")) {
        number = request->getParam("number")->value();
        numberInt = number.toInt();
    }
    request->send(200, "text/html", bodyfunctionNumber(numberInt));
});
'''

