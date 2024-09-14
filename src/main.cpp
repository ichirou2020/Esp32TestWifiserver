#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

// https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/WiFiAccessPoint/WiFiAccessPoint.ino
const char *ssid = "esp32_apppoint";	//!< SSID
const char *password = "yourPassword";	//!< パスワード

WiFiServer server(1999);				//!< Wifiサーバー(port:1999)

/// @brief セットアップ
void setup()
{
	Serial.begin(115200);				//デバッガー向けシリアル
	Serial.println();
	Serial.println("初期化中...");

	// アクセスポイントを設定
	if (!WiFi.softAP(ssid, password))
	{
		log_e("アクセスポイントを設定できませんでした");
		while (1)
			;
	}
	IPAddress myIP = WiFi.softAPIP();	// IPアドレスを取得
	Serial.print("AP IP address: ");
	Serial.println(myIP);
	server.begin();						// サーバーを開始

	Serial.println("TCPサーバー開始");
}

/// @brief メインループ
void loop()
{
	WiFiClient client = server.accept(); 	// リスナーの接続受入開始

	if (client)								// クライアントが接続している場合
	{								   
		Serial.println("クライアントが接続した"); 

		while (client.connected())
		{ // クライアントが接続している間ループ
			if (client.available())			// 受信データ有り
			{							
				String recvString = client.readString(); 	// バッファからデータを読み込む
				recvString+="\n";							// 改行を追加
				Serial.write(recvString.c_str());			// 受信データをシリアルに出力
				client.write("氏ね");						// クライアントにデータを送信				
			}
			delay(10);										// 10ms待機
		}
		// close the connection:
		client.stop();
		Serial.println("切断されました");
	}
	else
	{
		sleep(1);
	}
}
