

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "WebSocket_Task.h"
#include "string.h"
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)

int level = 0;
gpio_num_t motorpin1 = GPIO_NUM_32;
gpio_num_t motorpin2 = GPIO_NUM_25;
gpio_num_t motorpin3 = GPIO_NUM_26;
gpio_num_t motorpin4 = GPIO_NUM_33;
gpio_num_t motor2pin1 = GPIO_NUM_16;
gpio_num_t motor2pin2 = GPIO_NUM_17;
gpio_num_t motor2pin3 = GPIO_NUM_5;
gpio_num_t motor2pin4 = GPIO_NUM_18;
int motorSpeed = 1500  ;
int count = 0;
int countsprr = 512;
int lookup[8] = {0b01000, 0b01100, 0b00100, 0b00110, 0b00010, 0b00011, 0b00001, 0b01001};
int Xpos[30];
char * adr;
bool _isMove = false;
int array_number = 0 ;


//WebSocket frame receive queue
QueueHandle_t WebSocket_rx_queue;
void setLED(int mode){
switch(mode){
  case 0:
  gpio_set_level(GPIO_NUM_22 , 0);
  gpio_set_level(GPIO_NUM_19 , 0);
  gpio_set_level(GPIO_NUM_21 , 0);
  break;
  case 1:
  gpio_set_level(GPIO_NUM_22 , 1);
  gpio_set_level(GPIO_NUM_19 , 0);
  gpio_set_level(GPIO_NUM_21 , 0);
  break;
  case 2:
  gpio_set_level(GPIO_NUM_22 , 0);
  gpio_set_level(GPIO_NUM_19 , 1);
  gpio_set_level(GPIO_NUM_21 , 0);
  break;
  case 3:
  gpio_set_level(GPIO_NUM_22 , 0);
  gpio_set_level(GPIO_NUM_19 , 0);
  gpio_set_level(GPIO_NUM_21 , 1);
  break;
  case 4:
  gpio_set_level(GPIO_NUM_22 , 0);
  gpio_set_level(GPIO_NUM_19 , 1);
  gpio_set_level(GPIO_NUM_21 , 1);
  break;
  case 5:
  gpio_set_level(GPIO_NUM_22 , 1);
  gpio_set_level(GPIO_NUM_19 , 1);
  gpio_set_level(GPIO_NUM_21 , 0);
  break;
  case 6:
  gpio_set_level(GPIO_NUM_22 , 1);
  gpio_set_level(GPIO_NUM_19 , 0);
  gpio_set_level(GPIO_NUM_21 , 1);
  break;
  case 7:
  gpio_set_level(GPIO_NUM_22 , 1);
  gpio_set_level(GPIO_NUM_19 , 1);
  gpio_set_level(GPIO_NUM_21 , 1);
  break;
}

}
void setOutput(int out)
{
  gpio_set_level(motorpin1, bitRead(lookup[out], 0));
  gpio_set_level(motorpin2, bitRead(lookup[out], 1));
  gpio_set_level(motorpin3, bitRead(lookup[out], 2));
  gpio_set_level(motorpin4, bitRead(lookup[out], 3));
}

void setOutput2(int out)
{
  gpio_set_level(motor2pin1, bitRead(lookup[out], 0));
  gpio_set_level(motor2pin2, bitRead(lookup[out], 1));
  gpio_set_level(motor2pin3, bitRead(lookup[out], 2));
  gpio_set_level(motor2pin4, bitRead(lookup[out], 3));
}



void clockwise_R(int motor)
{
  for (int i = 7; i >= 0; i--)
  {
    setOutput(i);
    ets_delay_us(motor);
  }
}
void clockwise_L(int motor)
{
  for (int i = 7; i >= 0; i--)
  {
    setOutput2(i);
    ets_delay_us(motor);
  }
}
void anticlockwise_R(int motor)
{
  if(motor < 0){
    clockwise_R(abs(motor));
    return;
  }
  for (int i = 0; i < 8; i++)
  {
    setOutput(i);
    ets_delay_us(motor);
  }
}
void anticlockwise_L(int motor)
{
  if(motor < 0){
    clockwise_L(abs(motor));
    return;
  }
  for (int i = 0; i < 8; i++)
  {
    setOutput2(i);
    ets_delay_us(motor);
  }
}
void motor_control(){
  while(1){
    if(_isMove){
      for(int number = 0; number < array_number ; number++){
        printf("%d\n",Xpos[number]);
        int n = 100 ;
        int R = 1800;
        int L = 1800;
        if (Xpos[number] >= -15 && Xpos[number] <=15){
            R = 1800;
            L = 1800;
            n = 100;
            setLED(4);
        }
        if(Xpos[number] > 15 && Xpos[number] <= 80){
            R = 0;
            n = Xpos[number]*3+100;
            setLED(3);
        }
        if(Xpos[number] < -15 && Xpos[number] >= -80){
            L = 0;
            n = Xpos[number]*-3+100;
            setLED(2);
        }
        if(Xpos[number] > 80){
            R = -1800;
            L = 1800;
            n = 100 + Xpos[number]*5;
            setLED(5);
        }
        if(Xpos[number] < -80){
            R = 1800;
            L = -1800;
            n = 100 + Xpos[number]*-5;
            setLED(6);
        }
        for(int i = 0 ; i < n ;i++){
          anticlockwise_R(R);
          anticlockwise_L(L);

        }

        //vTaskDelay(2000 / portTICK_RATE_MS);
      }
        setLED(0);
        _isMove = false;
    }
  }


}
void task_process_WebSocket( void *pvParameters ){
  (void)pvParameters;


  //frame buffer
  WebSocket_frame_t __RX_frame;

  //create WebSocket RX Queue
  WebSocket_rx_queue = xQueueCreate(10,sizeof(WebSocket_frame_t));


  while (1){
    //receive next WebSocket frame from queue
    if(xQueueReceive(WebSocket_rx_queue,&__RX_frame, 3*portTICK_PERIOD_MS)==pdTRUE){
      adr = strtok( __RX_frame.payload , "," ) ;
      setLED(1);
      //write frame inforamtion to UART
      printf("%d frame. Length %d, payload %.*s \r\n",level, __RX_frame.payload_length, __RX_frame.payload_length, __RX_frame.payload);
      //loop back frame
      if(atoi(__RX_frame.payload) == 190){
        _isMove = true;
        vTaskDelete(NULL);
        printf("oK" );
        return ;

      }
      Xpos[array_number] = atoi(__RX_frame.payload);
      //  WS_write_data(__RX_frame.payload, __RX_frame.Xpos[array_number] = atoi(__RX_frame.payload);
      array_number ++;
      setLED(7);
      ets_delay_us(1000);
      setLED(0);
      //free memory
      if (__RX_frame.payload != NULL){
        free(__RX_frame.payload);
      }


    }

  }
}

esp_err_t event_handler(void *ctx, system_event_t *event)
{
  return ESP_OK;
}

void app_main(void)
{
  gpio_set_direction(GPIO_NUM_22, GPIO_MODE_OUTPUT);
  gpio_set_direction(GPIO_NUM_19,GPIO_MODE_OUTPUT);
  gpio_set_direction(GPIO_NUM_21,GPIO_MODE_OUTPUT);
  gpio_set_direction(motorpin1, GPIO_MODE_OUTPUT);
  gpio_set_direction(motorpin3 , GPIO_MODE_OUTPUT);
  gpio_set_direction(motorpin4 , GPIO_MODE_OUTPUT);
  gpio_set_direction(motor2pin1 , GPIO_MODE_OUTPUT);
  gpio_set_direction(motor2pin2 , GPIO_MODE_OUTPUT);
  gpio_set_direction(motor2pin3 , GPIO_MODE_OUTPUT);
  gpio_set_direction(motor2pin4 , GPIO_MODE_OUTPUT);
  nvs_flash_init();
  tcpip_adapter_init();
  ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
  ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
  wifi_config_t sta_config = {
    .sta = {
      .ssid = "saitetu",
      .password = "duos22home",
      .bssid_set = false
    }
  };
  ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
  ESP_ERROR_CHECK( esp_wifi_start() );
  ESP_ERROR_CHECK( esp_wifi_connect() );

  //create WebSocker receive task
  xTaskCreate(&task_process_WebSocket, "ws_process_rx", 2048, NULL, 1, NULL);
  xTaskCreate(&motor_control,"motor_control", 2048, NULL, 1, NULL );
  //Create Websocket Server Task
  xTaskCreate(&ws_server, "ws_server", 2048, NULL, 1, NULL);

}
//
