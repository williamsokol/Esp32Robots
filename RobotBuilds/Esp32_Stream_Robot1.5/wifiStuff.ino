
bool WifiInited = false;

bool initWifi() {
  Serial.println("Trying to connect to router");
  if(WifiInited){
    return false;
  }
  Serial.println("test1");
  //your other setup stuff...
  preferences.begin("my-pref", false);
  Serial.println("test2");
  is_setup_done = preferences.getBool("is_setup_done", false);
  ssidString = preferences.getString("rec_ssid", "Sample_SSID");
  passwordString = preferences.getString("rec_password", "abcdefgh");
  Serial.println("test3");
  if (!is_setup_done)
  {
//    StartCaptivePortal();
  }
  else
  {
    Serial.println("Using saved SSID and Password to attempt WiFi Connection!");
    Serial.print("Saved SSID is ");Serial.println(ssidString);
    Serial.print("Saved Password is ");Serial.println(passwordString);
    Serial.println("test3.5");
    WiFiStationSetup(ssidString, passwordString);
  }
  Serial.println("test4");
}
void WifiLoop(){
  Serial.println("test5");

//  delay(100);
  if (valid_ssid_received && valid_password_received)
  {
    Serial.println("test6");
    Serial.println("Attempting WiFi Connection!");
    WiFiStationSetup(ssidString, passwordString);
  }
}
void WiFiStationSetup(String rec_ssid, String rec_password)
{
  StopCaptivePortal();
  
  delay(100);
  Serial.println("test10");
  wifi_timeout = false;
  WiFi.mode(WIFI_STA);
  char ssid_arr[20];
  char password_arr[20];
  rec_ssid.toCharArray(ssid_arr, rec_ssid.length() + 1);
  rec_password.toCharArray(password_arr, rec_password.length() + 1);
  Serial.print("Received SSID: "); Serial.println(ssid_arr); Serial.print("And password: "); Serial.println(password_arr);
  WiFi.begin(ssid_arr, password_arr);

  uint32_t t1 = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
    if (millis() - t1 > 50000) //50 seconds elapsed connecting to WiFi
    {
      Serial.println();
      Serial.println("Timeout connecting to WiFi. The SSID and Password seem incorrect.");
      valid_ssid_received = false;
      valid_password_received = false;
      is_setup_done = false;
      preferences.putBool("is_setup_done", is_setup_done);

      StartCaptivePortal();
      wifi_timeout = true;
      break;
    }
  }
  if (!wifi_timeout)
  {
    is_setup_done = true;
    Serial.println("");  Serial.print("WiFi connected to: "); Serial.println(rec_ssid);
    Serial.print("IP address: ");  Serial.println(WiFi.localIP());
    preferences.putBool("is_setup_done", is_setup_done);
    preferences.putString("rec_ssid", rec_ssid);
    preferences.putString("rec_password", rec_password);
  }
}
