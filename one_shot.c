#include <stdio.h> 
#include "pico/stdlib.h" 
#include "pico/time.h"

#define led_green 11
#define led_blue 12
#define led_red 13
#define button_a 5

int64_t turn_of_green(alarm_id_t id, void *user_data);
int64_t turn_of_blue(alarm_id_t id, void *user_data);
int64_t turn_of_red(alarm_id_t id, void *user_data);

volatile bool processando = false;

int64_t alarm_callback(alarm_id_t id, void *user_data){
    processando = true;

    gpio_put(led_red,true);
    gpio_put(led_green,true);
    gpio_put(led_blue,true);

    add_alarm_in_ms(2000, turn_of_green, NULL, false);

    return 0;

}

int64_t turn_of_green(alarm_id_t id, void *user_data){
    gpio_put(led_green,false);

    add_alarm_in_ms(1000, turn_of_blue, NULL, false);

    return 0;
}

int64_t turn_of_blue(alarm_id_t id, void *user_data){
    gpio_put(led_blue,false);

    add_alarm_in_ms(1000, turn_of_red, NULL, false);

    return 0;
}

int64_t turn_of_red(alarm_id_t id, void *user_data) {
    gpio_put(led_red,false);
    
    processando = false;

    return 0;
    
}

int main(){
    stdio_init_all();

    gpio_init(led_red);
    gpio_set_dir(led_red,GPIO_OUT);

    gpio_init(led_green);
    gpio_set_dir(led_green,GPIO_OUT);

    gpio_init(led_blue);
    gpio_set_dir(led_blue,GPIO_OUT);

    gpio_init(button_a);
    gpio_set_dir(button_a,GPIO_IN);
    gpio_pull_up(button_a);

        while(true){
            if (!gpio_get(button_a) && !processando) {
                add_alarm_in_ms(3000, alarm_callback, NULL, false);
            }
            sleep_ms(50);
        }
}


