#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include <ESP8266HTTPClient.h>
#define APPID   "Your NETPIE AppID"
#define KEY     "Your NETPIE Device key"
#define SECRET  "Your NETPIE Device"
#define ALIAS   "Please set this device name here"
const char* ssid     = "Wifi SSID";
const char* password = "Wifi Network";
WiFiClient client;
MicroGear microgear(client);
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  msg[msglen] = '\0'; 
  String message = getValue((char *)msg, ',', 0);
  String token = getValue((char *)msg, ',', 1);
  Line_Reply(message,token);
}
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("Connected to NETPIE..."); /* Set the alias of this microgear ALIAS */ 
  microgear.setAlias(ALIAS); 
}
void setup() { 
  pinMode(LED_BUILTIN, OUTPUT);
  /* Add Event listeners */ 
  microgear.on(MESSAGE,onMsghandler);/* Call onMsghandler() when new message arraives */ 
  microgear.on(CONNECTED,onConnected);/* Call onConnected() when NETPIE connection is established */ 
  Serial.begin(115200); 
  Serial.println("Starting...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(300);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  microgear.init(KEY,SECRET,ALIAS);  /* Initial with KEY, SECRET and also set the ALIAS here */ 
  microgear.connect(APPID);   /* connect to NETPIE to a specific APPID */ 
}
void loop() {
    if (microgear.connected())
    {
       digitalWrite(LED_BUILTIN, LOW);
       microgear.loop();
    }
   else 
   {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("connection lost, reconnect...");
    microgear.connect(APPID);
   }
    delay(100);
}
void Line_Reply(String message,String replytoken) {
HTTPClient http;
  String prn = "";
  prn+= "http://pocketbotbroker.herokuapp.com/reply.php?message=";
  prn+= message;
  prn+="&token=";
  prn+=replytoken;
  http.begin(prn);
  int httpCode = http.GET();
  Serial.println(httpCode);
  delay(500);
}
// Split string function
 String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
