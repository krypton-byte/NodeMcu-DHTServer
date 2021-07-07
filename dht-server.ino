#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

int HUMIDITY = 0;
float TEMPERATURE=0;
unsigned long timestamp = 0;
const char* ssid = "server DHT";
int HTTP = 80;
int WEBSOCKET = 1337;
String index_html = "<!DOCTYPE html>\n<html lang='en'>\n<head>\n    <meta charset='UTF-8'>\n    <meta http-equiv='X-UA-Compatible' content='IE=edge'>\n    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n    <title>Document</title>\n</head>\n<body>\n    <script src='web_socket.min.js'></script>\n\t<div class='content'>\n        <p>\n            <span class='cardInfo'>\n                <span class='icon'>\n                    <svg xmlns='http://www.w3.org/2000/svg' width='48px' height='48px' viewBox='0 0 24 24' style='fill:rgba(0, 0, 0, 1);transform:;-ms-filter:'>\n                        <path d='M17.726 13.02L14 16H9v-1h4.065c.399 0 .638-.445.416-.777l-.888-1.332C12.223 12.334 11.599 12 10.93 12H9 3c-.553 0-1 .447-1 1v6c0 1.104.896 2 2 2h9.639c.865 0 1.688-.373 2.258-1.024L22 13l-1.452-.484C19.583 12.194 18.521 12.384 17.726 13.02zM15.403 12c1.657 0 3-1.343 3-3 0-2.708-3-6-3-6s-3 3.271-3 6C12.403 10.657 13.747 12 15.403 12z'></path>\n                    </svg>\n                </span>\n                <span class='text' id='humi'>humidity: 0</span>\n            </span>\n        </p>\n        <p>\n            <span class='cardInfo'>\n                <span class='icon'>\n                    <svg xmlns='http://www.w3.org/2000/svg' width='48px' height='48px' viewBox='0 0 24 24' style='fill:rgba(0, 0, 0, 1);transform:;-ms-filter:'>\n                        <path d='M9,16c0,1.656,1.344,3,3,3s3-1.344,3-3c0-0.353-0.072-0.686-0.184-1H9.184C9.072,15.314,9,15.647,9,16z'></path>\n                        <path d='M18,6V4h-3.185C14.401,2.839,13.302,2,12,2c-1.654,0-3,1.346-3,3v5.8c-1.842,1.072-3,3.048-3,5.2c0,3.309,2.691,6,6,6 s6-2.691,6-6c0-2.152-1.158-4.128-3-5.2V10h3V8h-3V6H18z M13.595,12.324C15.056,12.973,16,14.415,16,16c0,2.206-1.794,4-4,4 s-4-1.794-4-4c0-1.585,0.944-3.027,2.405-3.676L11,12.061V5c0-0.552,0.448-1,1-1s1,0.448,1,1v1v6.061L13.595,12.324z'></path>\n                    </svg>\n                </span>\n                <span class='text' id='temp' >Temp: 0°C</span>\n            </span>\n        </p>\n    </div>\n    <style>\n        .icon {\n          vertical-align: middle;\n          display: inline-block;\n          padding-left:15%;\n        }\n        .text {\n          vertical-align: middle;\n          display: inline-block;\n          font-size:2.1em;\n        }\n        .content {\n            border-style:solid;\n            border-color: black;\n            border-width: 2px;\n        }\n    </style>\n    <script>\n        ws = new WebSocket('ws://192.168.4.1:1337')\n        ws.onopen = function(){\n            console.log('connected');\n        }\n        ws.onmessage = function (data){\n            var parse = JSON.parse(data.data)\n            document.getElementById('temp').innerHTML = `Temp: ${parse.temperature}°C`\n            document.getElementById('humi').innerHTML = `humidity: ${parse.humidity}`\n        }\n        setInterval(()=>ws.send('o'), 1000)\n    </script>\n</body>\n</html>";
String javascript = "// Copyright: Hiroshi Ichikawa <http://gimite.net/en/>\n// License: New BSD License\n// Reference: http://dev.w3.org/html5/websockets/\n// Reference: http://tools.ietf.org/html/rfc6455\n(function(){if(window.WEB_SOCKET_FORCE_FLASH);else{if(window.WebSocket)return;if(window.MozWebSocket)return window.WebSocket=MozWebSocket,void 0}var a;return a=window.WEB_SOCKET_LOGGER?WEB_SOCKET_LOGGER:window.console&&window.console.log&&window.console.error?window.console:{log:function(){},error:function(){}},10>swfobject.getFlashPlayerVersion().major?(a.error('Flash Player >= 10.0.0 is required.'),void 0):('file:'==location.protocol&&a.error('WARNING: web-socket-js doesn't work in file:///... URL unless you set Flash Security Settings properly. Open the page via Web server i.e. http://...'),window.WebSocket=function(a,b,c,d,e){var f=this;f.__id=WebSocket.__nextId++,WebSocket.__instances[f.__id]=f,f.readyState=WebSocket.CONNECTING,f.bufferedAmount=0,f.__events={},b?'string'==typeof b&&(b=[b]):b=[],f.__createTask=setTimeout(function(){WebSocket.__addTask(function(){f.__createTask=null,WebSocket.__flash.create(f.__id,a,b,c||null,d||0,e||null)})},0)},WebSocket.prototype.send=function(a){if(this.readyState==WebSocket.CONNECTING)throw'INVALID_STATE_ERR: Web Socket connection has not been established';var b=WebSocket.__flash.send(this.__id,encodeURIComponent(a));return 0>b?!0:(this.bufferedAmount+=b,!1)},WebSocket.prototype.close=function(){return this.__createTask?(clearTimeout(this.__createTask),this.__createTask=null,this.readyState=WebSocket.CLOSED,void 0):(this.readyState!=WebSocket.CLOSED&&this.readyState!=WebSocket.CLOSING&&(this.readyState=WebSocket.CLOSING,WebSocket.__flash.close(this.__id)),void 0)},WebSocket.prototype.addEventListener=function(a,b){a in this.__events||(this.__events[a]=[]),this.__events[a].push(b)},WebSocket.prototype.removeEventListener=function(a,b){if(a in this.__events)for(var d=this.__events[a],e=d.length-1;e>=0;--e)if(d[e]===b){d.splice(e,1);break}},WebSocket.prototype.dispatchEvent=function(a){for(var b=this.__events[a.type]||[],c=0;b.length>c;++c)b[c](a);var d=this['on'+a.type];d&&d.apply(this,[a])},WebSocket.prototype.__handleEvent=function(a){'readyState'in a&&(this.readyState=a.readyState),'protocol'in a&&(this.protocol=a.protocol);var b;if('open'==a.type||'error'==a.type)b=this.__createSimpleEvent(a.type);else if('close'==a.type)b=this.__createSimpleEvent('close'),b.wasClean=a.wasClean?!0:!1,b.code=a.code,b.reason=a.reason;else{if('message'!=a.type)throw'unknown event type: '+a.type;var c=decodeURIComponent(a.message);b=this.__createMessageEvent('message',c)}this.dispatchEvent(b)},WebSocket.prototype.__createSimpleEvent=function(a){if(document.createEvent&&window.Event){var b=document.createEvent('Event');return b.initEvent(a,!1,!1),b}return{type:a,bubbles:!1,cancelable:!1}},WebSocket.prototype.__createMessageEvent=function(a,b){if(document.createEvent&&window.MessageEvent&&!window.opera){var c=document.createEvent('MessageEvent');return c.initMessageEvent('message',!1,!1,b,null,null,window,null),c}return{type:a,data:b,bubbles:!1,cancelable:!1}},WebSocket.CONNECTING=0,WebSocket.OPEN=1,WebSocket.CLOSING=2,WebSocket.CLOSED=3,WebSocket.__isFlashImplementation=!0,WebSocket.__initialized=!1,WebSocket.__flash=null,WebSocket.__instances={},WebSocket.__tasks=[],WebSocket.__nextId=0,WebSocket.loadFlashPolicyFile=function(a){WebSocket.__addTask(function(){WebSocket.__flash.loadManualPolicyFile(a)})},WebSocket.__initialize=function(){if(!WebSocket.__initialized){if(WebSocket.__initialized=!0,WebSocket.__swfLocation&&(window.WEB_SOCKET_SWF_LOCATION=WebSocket.__swfLocation),!window.WEB_SOCKET_SWF_LOCATION)return a.error('[WebSocket] set WEB_SOCKET_SWF_LOCATION to location of WebSocketMain.swf'),void 0;if(!window.WEB_SOCKET_SUPPRESS_CROSS_DOMAIN_SWF_ERROR&&!WEB_SOCKET_SWF_LOCATION.match(/(^|\\/)WebSocketMainInsecure\\.swf(\\?.*)?$/)&&WEB_SOCKET_SWF_LOCATION.match(/^\\w+:\\/\\/([^\\/]+)/)){var b=RegExp.$1;location.host!=b&&a.error('[WebSocket] You must host HTML and WebSocketMain.swf in the same host (''+location.host+'' != ''+b+''). '+'See also 'How to host HTML file and SWF file in different domains' section '+'in README.md. If you use WebSocketMainInsecure.swf, you can suppress this message '+'by WEB_SOCKET_SUPPRESS_CROSS_DOMAIN_SWF_ERROR = true;')}var c=document.createElement('div');c.id='webSocketContainer',c.style.position='absolute',WebSocket.__isFlashLite()?(c.style.left='0px',c.style.top='0px'):(c.style.left='-100px',c.style.top='-100px');var d=document.createElement('div');d.id='webSocketFlash',c.appendChild(d),document.body.appendChild(c),swfobject.embedSWF(WEB_SOCKET_SWF_LOCATION,'webSocketFlash','1','1','10.0.0',null,null,{hasPriority:!0,swliveconnect:!0,allowScriptAccess:'always'},null,function(b){b.success||a.error('[WebSocket] swfobject.embedSWF failed')})}},WebSocket.__onFlashInitialized=function(){setTimeout(function(){WebSocket.__flash=document.getElementById('webSocketFlash'),WebSocket.__flash.setCallerUrl(location.href),WebSocket.__flash.setDebug(!!window.WEB_SOCKET_DEBUG);for(var a=0;WebSocket.__tasks.length>a;++a)WebSocket.__tasks[a]();WebSocket.__tasks=[]},0)},WebSocket.__onFlashEvent=function(){return setTimeout(function(){try{for(var b=WebSocket.__flash.receiveEvents(),c=0;b.length>c;++c)WebSocket.__instances[b[c].webSocketId].__handleEvent(b[c])}catch(d){a.error(d)}},0),!0},WebSocket.__log=function(b){a.log(decodeURIComponent(b))},WebSocket.__error=function(b){a.error(decodeURIComponent(b))},WebSocket.__addTask=function(a){WebSocket.__flash?a():WebSocket.__tasks.push(a)},WebSocket.__isFlashLite=function(){if(!window.navigator||!window.navigator.mimeTypes)return!1;var a=window.navigator.mimeTypes['application/x-shockwave-flash'];return a&&a.enabledPlugin&&a.enabledPlugin.filename?a.enabledPlugin.filename.match(/flashlite/i)?!0:!1:!1},window.WEB_SOCKET_DISABLE_AUTO_INITIALIZATION||swfobject.addDomLoadEvent(function(){WebSocket.__initialize()}),void 0)})();";





AsyncWebServer server(HTTP);
WebSocketsServer websocket(WEBSOCKET);
DHT dht(2, DHT11);

void websocketEvent(uint8_t client_num, WStype_t type, uint8_t* payload, size_t length){
  switch(type){
    case WStype_DISCONNECTED:
      Serial.println("terputus");
      break;
    case WStype_CONNECTED:
      Serial.println("terhubung");
      break;
    case WStype_TEXT:
      websocket.sendTXT(client_num, String("{\"humidity\":"+String(HUMIDITY)+",\"temperature\":"+String(TEMPERATURE)+"}").c_str());  
      break;
  }
}

void dht_runner(){
  HUMIDITY=dht.readHumidity();
  TEMPERATURE=dht.readTemperature();
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.softAP(ssid);
  Serial.print("HTTP berjalan di ");
  Serial.print(WiFi.softAPIP());
  Serial.println(":"+String(HTTP));
  Serial.print("WebSocket berjalan di: ");
  Serial.print(WiFi.softAPIP());
  Serial.println(":"+String(WEBSOCKET));
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
   request->send(200, "text/html", index_html) ;
  });
  server.on("/web_socket.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
   request->send(200, "application/javascript", javascript);
  });
  server.begin();
  websocket.begin();
  dht.begin();
  websocket.onEvent(websocketEvent);
  
  
}

void loop() {
  if(millis() > timestamp){
    dht_runner();
    timestamp = millis()+1000;
  }
  websocket.loop();

}
