

//--------------------------------------------------------------------------------------------
// Parse JSON String. Returns Reference to a JSONObject
//--------------------------------------------------------------------------------------------
JsonObject& parseJSON(String json) {
        const size_t bufferSize = 2000;
        DynamicJsonBuffer jsonBuffer(bufferSize);
        JsonObject& j = jsonBuffer.parseObject(json);
        return j;
}
//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------
// h2int - Hex to Integer. Used by urlencode/decode functions
//--------------------------------------------------------------------------------------------
unsigned char h2int(char c)
{
    if (c >= '0' && c <='9'){
        return((unsigned char)c - '0');
    }
    if (c >= 'a' && c <='f'){
        return((unsigned char)c - 'a' + 10);
    }
    if (c >= 'A' && c <='F'){
        return((unsigned char)c - 'A' + 10);
    }
    return(0);
}
//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------
// URLDecode a String
//--------------------------------------------------------------------------------------------
String urldecode(String str)
{
    
    String encodedString="";
    char c;
    char code0;
    char code1;
    for (int i =0; i < str.length(); i++){
        c=str.charAt(i);
      if (c == '+'){
        encodedString+=' ';  
      }else if (c == '%') {
        i++;
        code0=str.charAt(i);
        i++;
        code1=str.charAt(i);
        c = (h2int(code0) << 4) | h2int(code1);
        encodedString+=c;
      } else{
        
        encodedString+=c;  
      }
      
      yield();
    }
    
   return encodedString;
}
//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------
// URLEncode a String
//--------------------------------------------------------------------------------------------
String urlencode(String str)
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
}
//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------
// Duet GET. Returns JSON object containing results
//--------------------------------------------------------------------------------------------
JsonObject& duetGet (String url) {
    HTTPClient http;
    
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      return parseJSON(http.getString());
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      return parseJSON("{\"Error\":\"HTTP Get Error\"}");
    }

    http.end();  
}
//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------
// Connect to Duet WiFi over network
//--------------------------------------------------------------------------------------------
void duetConnect (String duetIP, String duetPassword) {
    JsonObject& j = duetGet("http://" + duetIP + "/rr_connect?password=" + duetPassword + "&time=00:00");
    const char* boardType = j["boardType"];
    Serial.print("Connected to Duet WiFi. Board Type=");
    Serial.println(boardType);
}
//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------
// Get Duet Status
//--------------------------------------------------------------------------------------------
void duetStatus (String duetIP) {
    JsonObject& j = duetGet("http://" + duetIP + "/rr_status?type=2");
    const char* status = j["status"];
//    const char* name = j["name"];
//    const char* firmwareName = j["firmwareName"];
//    const char* tempBed = j["temps"]["bed"]["current"];
//    const char* fanName4 = j["params"]["fanNames"][4];
    
    Serial.print("Duet Status=");
    Serial.println(status);
    
//    Serial.print("Duet Name=");
//    Serial.println(name);
//    
//    Serial.print("Duet Firmware=");
//    Serial.println(firmwareName);
//
//    Serial.print("Bed Temperature=");
//    Serial.println(tempBed);
//    
//    Serial.print("Fan4 Name=");
//    Serial.println(fanName4);

}
//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------
// Send a gcode to Duet
//--------------------------------------------------------------------------------------------
void duetGCode (String duetIP, String gcode) {
    Serial.print("[ " + gcode + " ]\t\t");
    JsonObject& j = duetGet("http://" + duetIP + "/rr_gcode?gcode=" + urlencode(gcode));
    const char* buff = j["buff"];
    Serial.println(buff);
}
//--------------------------------------------------------------------------------------------




