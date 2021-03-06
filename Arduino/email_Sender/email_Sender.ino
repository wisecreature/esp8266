#include <ESP8266WiFi.h>

const char* ssid = "test";   // Enter the namme of your WiFi Network.

const char* password = "03334624342s";  // Enter the Password of your WiFi Network.

char server[] = "in-v3.mailjet.com";   // The SMTP Server

WiFiClient espClient;

void setup()
{
  Serial.begin(115200);
  delay(10);
  Serial.println("");
  Serial.println("");
  Serial.print("Connecting To: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("*");
  }
  Serial.println("");
  Serial.println("WiFi Connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  byte ret = sendEmail();
 }
 
void loop()
{

}
 
byte sendEmail()
{
  if (espClient.connect(server, 587) == 1)
  {
    Serial.println(F("connected"));
  }
  else
  {
    Serial.println(F("connection failed"));
    return 0;
  }
  if (!emailResp())
    return 0;
  Serial.println(F("Sending EHLO"));
  espClient.println("EHLO in-v3.mailjet.com");
  if (!emailResp())
    return 0;
  Serial.println(F("Sending auth login"));
  espClient.println("AUTH LOGIN");
  if (!emailResp())
    return 0;
  Serial.println(F("Sending User"));
  espClient.println("0d467209b45c110ff010fbc0dc41d5c1"); // Your encoded Username
  if (!emailResp())
    return 0;
  Serial.println(F("Sending Password"));
  espClient.println("2c6334704b00454e79abc426aeff023f");// Your encoded Password
  if (!emailResp())
    return 0;
  Serial.println(F("Sending From"));
  espClient.println(F("MAIL From: xyz@gmail.com")); // Enter Sender Mail Id
  if (!emailResp())
    return 0;
  Serial.println(F("Sending To"));
  espClient.println(F("RCPT To: tada@mailhub.top")); // Enter Receiver Mail Id
  if (!emailResp())
    return 0;
  Serial.println(F("Sending DATA"));
  espClient.println(F("DATA"));
  if (!emailResp())
    return 0;
  Serial.println(F("Sending email"));
  espClient.println(F("To:  xyz@gmail.com")); // Enter Receiver Mail Id
  // change to your address
  espClient.println(F("From: xyz@gmail.com")); // Enter Sender Mail Id
  espClient.println(F("Subject: ESP8266 test e-mail\r\n"));
  espClient.println(F("This is is a test e-mail sent from ESP8266.\n"));
  espClient.println(F("Second line of the test e-mail."));
  espClient.println(F("Third line of the test e-mail."));
  espClient.println(F("."));
  if (!emailResp())
    return 0;
  Serial.println(F("Sending QUIT"));
  espClient.println(F("QUIT"));
  if (!emailResp())
    return 0;

  //
  espClient.stop();
  Serial.println(F("disconnected"));
  return 1;
}
 

byte emailResp()
{
  byte responseCode;
  byte readByte;
  int loopCount = 0;

  while (!espClient.available())
  {
    delay(1);
    loopCount++;

    if (loopCount > 20000)
    {
      espClient.stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }
  responseCode = espClient.peek();
  while (espClient.available())
  {
    readByte = espClient.read();
    Serial.write(readByte);
  }
  if (responseCode >= '4')
  {
    return 0;
  }
  return 1;
}
