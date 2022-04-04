#include <Arduino.h>
#include<M5StickC.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include "dns/packet.h"
#include "dns/utils.h"
#include "display.h"

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASS "YOUR_WIFI_PASS"

#define HOST "m5.local"
#define MDNS_PORT 5353
#define DNS_SD_SRV_HTTP "_http._tcp.local"
IPAddress mdnsGroup = IPAddress(224,0,0,251);

WiFiUDP udp;
WiFiServer server;

int rcv_qa = 0;
int rcv_ptr = 0;
int rcv_http = 0;

/**
 * DNSパケットを処理する
**/
void handleDNSRequest() {
  int len;
  // 未処理のUDPパケットが無ければ何もしない
  if((len = udp.parsePacket()) == 0) return;

  //処理対象のパケットを取得&パース
  auto packet = std::make_unique<uint8_t[]>(len);
  udp.readBytes(packet.get(), len);
  auto inPacket = DNSPacket(std::move(packet));
  Serial.println(inPacket.summarize().c_str());

  // 問い合わせが無いパケットは無視
  if(inPacket.questions.empty()) return;

  // 問い合わせが1つのみだと仮定（多くの場合は1つなので）
  auto q = inPacket.questions[0];

  // m5.localのAレコード問い合わせの場合
  if(q.qtype == QTYPE::A && join(inPacket.questions[0].qname, ".") == HOST) {
    rcv_qa++;

    // 受信パケットに対して応答を作って返答用のAレコードを付与
    auto outPacket = DNSPacket::answerFor(inPacket);
    outPacket.answers.push_back(DNSResource::createARecord(HOST, WiFi.localIP()));

    Serial.println("answer--------------");
    Serial.println(outPacket.summarize().c_str());

    // 送信用に生のバイナリにエンコード
    auto outPacketSize = outPacket.size();
    auto outPacketRawData = std::make_unique<uint8_t[]>(outPacketSize);
    outPacket.write(outPacketRawData.get());

    // 送信
    udp.beginMulticastPacket();
    udp.write(outPacketRawData.get(), outPacketSize);
    udp.endPacket();
  }
  // DNS-SDで_http._tcp サービスの応答が求められた場合
  else if(q.qtype == QTYPE::PTR && join(q.qname, ".") == DNS_SD_SRV_HTTP) {
    rcv_ptr++;

    // 受信パケットに対して応答を作って返答用のPTRレコードを付与 
    auto outPacket = DNSPacket::answerFor(inPacket);
    outPacket.answers.push_back(DNSResource::createPTRRecord(DNS_SD_SRV_HTTP, "m5." DNS_SD_SRV_HTTP));

    Serial.println("answer--------------");
    Serial.println(outPacket.summarize().c_str());

    // 送信用に生のバイナリにエンコード
    auto outPacketSize = outPacket.size();
    auto outPacketRawData = std::make_unique<uint8_t[]>(outPacketSize);
    outPacket.write(outPacketRawData.get());

    // 送信
    udp.beginMulticastPacket();
    udp.write(outPacketRawData.get(), outPacketSize);
    udp.endPacket();
  }
  
}

/**
 * HTTPリクエストを処理する
**/
void handleHTTPRequest() {
  auto client = server.available();
  // リクエストが存在したら
  if(client) {
    // 接続まで待機
    while(client.connected() && !client.available()) { delay(1); }

    rcv_http++;
    client.readString();

    // リクエストの内容に関わらず固定的なhttpレスポンスを返す
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println(
      R"(
        <!DOCTYPE html>
        <html lang="en">
        <head>
          <meta charset="UTF-8">
          <meta name="viewport" content="width=device-width, initial-scale=1.0">
          <link rel="stylesheet" href="https://cdn.simplecss.org/simple.min.css">
          <title>M5StickC</title>
        </head>
        <body>
          <header>
            <h1>M5.local</h1>
            <p>hosted by M5StickC</p>
          </header>

          <main>
            <p>This page is hosted by M5StickC!</p>
            <p>
              <a href="http://m5.local">m5.local</a> is resolved by mDNS/DNS-SD. 
              You can find source code at <a href="https://github.com/SIY1121">my GitHub repository</a>
            </p>
          </main>

          <footer>
            <p>SIY1121</p>
          </footer>
        </body>
        </html>
      )"
    );
    client.println();
    client.stop();
  }
}

void setup() {
  M5.begin();
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  M5.Lcd.setRotation(3);
  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    displayProgress("Connecting", rcv_qa, rcv_ptr, rcv_http);
  }
  M5.Lcd.setCursor(0, 16);
  M5.Lcd.println(WiFi.localIP().toString());
  udp.beginMulticast(mdnsGroup, MDNS_PORT);
  server.begin(80);
}

void loop() {
  handleDNSRequest();
  handleHTTPRequest();
  
  displayProgress("Running    ", rcv_qa, rcv_ptr, rcv_http, 50);
}