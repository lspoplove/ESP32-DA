#include "BluetoothA2DPSink.h"
#include <Adafruit_NeoPixel.h>

// --- 引脚定义 ---
#define LED_PIN     4      // WS2812B 数据引脚
#define NUM_LEDS    1      // 如果你有一串灯珠，请修改此数字
#define I2S_BCK     27
#define I2S_LRCK    26
#define I2S_DIN     14

// --- 初始化对象 ---
BluetoothA2DPSink a2dp_sink;
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// 蓝牙连接状态回调函数
void connection_state_callback(esp_a2d_connection_state_t state, void *obj) {
  if (state == ESP_A2D_CONNECTION_STATE_CONNECTED) {
    // 连接成功：显示绿色
    for(int i=0; i<NUM_LEDS; i++) strip.setPixelColor(i, strip.Color(0, 255, 0));
    strip.show();
    Serial.println("Bluetooth Connected!");
  } else if (state == ESP_A2D_CONNECTION_STATE_DISCONNECTED) {
    // 断开连接：显示红色
    for(int i=0; i<NUM_LEDS; i++) strip.setPixelColor(i, strip.Color(255, 0, 0));
    strip.show();
    Serial.println("Bluetooth Disconnected!");
  }
}

void setup() {
  Serial.begin(115200);

  // 初始化灯珠
  strip.begin();
  strip.setBrightness(50); // 设置亮度 (0-255)
  for(int i=0; i<NUM_LEDS; i++) strip.setPixelColor(i, strip.Color(255, 0, 0)); // 默认红色
  strip.show();

  // 配置 I2S 引脚映射
  i2s_pin_config_t my_pin_config = {
    .bck_io_num = I2S_BCK,
    .ws_io_num = I2S_LRCK,
    .data_out_num = I2S_DIN,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
  a2dp_sink.set_pin_config(my_pin_config);

  // 设置连接状态回调
  a2dp_sink.set_on_connection_state_changed(connection_state_callback);

  // 启动蓝牙音频，设置设备名称
  a2dp_sink.start("DSTIKE-DA");
}

void loop() {
  // 蓝牙音频库在后台运行，这里通常不需要放代码
}