#include "./SD.h"
#include "./BarometerThermohygrometer.h"
#include "./DCMotor.h"
#include "./DCMotor2.h"
#include "./IMU.h"
#include "./Light.h"
#include "./GPS.h"

void log();
void turn();

void setup()
{
	// Wire(Arduino-I2C)の初期化
	Wire.begin();
	// デバック用シリアル通信は9600bps
	Serial.begin(9600);

	SD_Init();			// SDカードの初期化．これは絶対最初に初期化！
	BTH_Init();			// 気圧・温度計を初期化
  // DCM_Init();
  // DCM2_Init();
	IMU_Init();			// 9軸センサの初期化
  // LIT_Init();
	GPS_Init();			// GPSの初期化

	Serial.println(F("Init done"));
	delay(300);
}

void loop()
{
	// 気圧・温度の値を更新
	BTH_Update();
	// 9軸センサの値を更新
	IMU_UpdateAll();
	// GPSの値を更新
	GPS_Update();
	log();
  // turn();
}


void log()
{
	// 気圧の値を取得
	float pressure = BTH_GetPressure();
	// 加速度センサのX軸の値を取得
	float acc_x = IMU_GetAccX();
	// 加速度センサのY軸の値を取得
	float acc_y = IMU_GetAccY();
	// 加速度センサのZ軸の値を取得
	float acc_z = IMU_GetAccZ();
	// ジャイロセンサのX軸の値を取得
	float gyr_x = IMU_GetGyrX();
	// ジャイロセンサのY軸の値を取得
	float gyr_y = IMU_GetGyrY();
	// ジャイロセンサのZ軸の値を取得
	float gyr_z = IMU_GetGyrZ();
	// 磁気センサのX軸の値を取得
	int mag_x = IMU_GetMagX();
	// 磁気センサのY軸の値を取得
	int mag_y = IMU_GetMagY();
	// 磁気センサのZ軸の値を取得
	int mag_z = IMU_GetMagZ();

	// 経度の値を取得
	float lat = GPS_GetLat();
	// 緯度の値を取得
	float lng = GPS_GetLng();
	// 高度の値を取得
	float height = GPS_GetHeight();

	// TODO: 本当にこれで望んでいたデータが保存できてるか確認しよう
	SD_Write(","
			 + String(pressure) + ","
			 + String(acc_x) + ","
			 + String(acc_y) + ","
			 + String(acc_z) + ","
			 + String(gyr_x) + ","
			 + String(gyr_y) + ","
			 + String(gyr_z) + ","
			 + String(mag_x) + ","
			 + String(mag_y) + ","
			 + String(mag_z) + ","
			 + String(lat) + ","
			 + String(lng) + ","
			 + String(height) + ","
			 );		// SDカードに保存
}

void turn() 
{
  if (LIT_Get() < 300) return 0;
  if (IMU_GetGyrZ() < 20 && IMU_GetGyrZ() > -20) return 0;
  if (IMU_GetGyrZ() < 0){
    DCM_FORWARD();
    DCM2_REVERSE();
    delay(500);
    DCM_BRAKE();
    DCM2_BRAKE();
  }else{
    DCM2_FORWARD();
    DCM_REVERSE();
    delay(500);
    DCM2_BRAKE();
    DCM_BRAKE();
  }
}
