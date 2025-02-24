#include <Arduino.h>
#include "driver/rmt.h"

const int b1PIN = 21;
const int b2PIN = 22;
const int b3PIN = 23;
const int b4PIN = 19;
const int b5PIN = 18;


const int signalPIN = 13; // GPIO pin to output the pulse
const int clockDivider = 20; // RMT clock divider (80 MHz / 20 = 4 MHz)
// f = 4 MHz  T = 0.25 us = 250 ns

rmt_item32_t pulseItem1;
rmt_item32_t pulseItem2;
rmt_item32_t pulseItem3;
rmt_item32_t pulseItem4;
rmt_item32_t pulseItem5;

void setupRMTChannel( rmt_channel_t channel, gpio_num_t gpio_num );


uint32_t lastCheck;

void handleInterrupt1();
void handleInterrupt2();
void handleInterrupt3();
void handleInterrupt4();
void handleInterrupt5();


void setup()
{
    Serial.begin( 115200 );
    Serial.println( "Hello" );

    pinMode( b1PIN, INPUT_PULLUP );
    pinMode( b2PIN, INPUT_PULLUP );
    pinMode( b3PIN, INPUT_PULLUP );
    pinMode( b4PIN, INPUT_PULLUP );
    pinMode( b5PIN, INPUT_PULLUP );

    lastCheck = 0;


    setupRMTChannel( RMT_CHANNEL_0, ( gpio_num_t )signalPIN );

    // Prepare the pulse data
    pulseItem1.level0 = 1; // Set HIGH
    pulseItem1.duration0 = 2; // Duration of HIGH
    // f = 4 MHz  T = 0.25 us = 250 ns,  2*T = 0,5 us
    pulseItem1.level1 = 0; // Set LOW
    pulseItem1.duration1 = 2; // Duration of LOW (optional, keeps LOW afterward)

    pulseItem2.level0 = 1;
    pulseItem2.duration0 = 8; // 8 * 0.25 us = 2 us
    pulseItem2.level1 = 0;
    pulseItem2.duration1 = 2;

    pulseItem3.level0 = 1;
    pulseItem3.duration0 = 32; // 8 us
    pulseItem3.level1 = 0;
    pulseItem3.duration1 = 2;

    pulseItem4.level0 = 1;
    pulseItem4.duration0 = 128; // 32 us
    pulseItem4.level1 = 0;
    pulseItem4.duration1 = 2;

    pulseItem5.level0 = 1;
    pulseItem5.duration0 = 256; // 64 us
    pulseItem5.level1 = 0;
    pulseItem5.duration1 = 2;
}

void loop()
{
    if( ( ( millis() - lastCheck ) > 2500 ) )
    {
        if( !digitalRead( b1PIN ) )
        {
            handleInterrupt1();
        }
        else if( !digitalRead( b2PIN ) )
        {
            handleInterrupt2();
        }
        else if( !digitalRead( b3PIN ) )
        {
            handleInterrupt3();
        }
        else if( !digitalRead( b4PIN ) )
        {
            handleInterrupt4();
        }
        else if( !digitalRead( b5PIN ) )
        {
            handleInterrupt5();
        }

        lastCheck = millis();
    }

    delay( 100 );
}

void setupRMTChannel( rmt_channel_t channel, gpio_num_t gpio_num )
{
    rmt_config_t rmtConfig;
    rmtConfig.rmt_mode = RMT_MODE_TX; // Set RMT to transmit mode
    rmtConfig.channel = channel; // Set the RMT channel
    rmtConfig.gpio_num = gpio_num; // GPIO for this channel
    rmtConfig.mem_block_num = 1; // Use one memory block
    rmtConfig.tx_config.loop_en = false; // Disable loop mode
    rmtConfig.tx_config.carrier_en = false; // No carrier signal
    rmtConfig.tx_config.idle_output_en = true; // Enable idle level
    rmtConfig.tx_config.idle_level = RMT_IDLE_LEVEL_LOW; // Idle level is LOW
    rmtConfig.clk_div = clockDivider; // Set clock divider

    rmt_config(&rmtConfig ); // Apply the configuration
    rmt_driver_install( channel, 0, 0 ); // Install the driver for this channel
}


void handleInterrupt1()
{
    Serial.println( "b1 interrupted" );
    rmt_write_items( RMT_CHANNEL_0, &pulseItem1, 1, true ); // Send the pulse
}

void handleInterrupt2()
{
    Serial.println( "b2 interrupted" );
    rmt_write_items( RMT_CHANNEL_0, &pulseItem2, 1, true ); // Send the pulse
}

void handleInterrupt3()
{
    Serial.println( "b3 interrupted" );
    rmt_write_items( RMT_CHANNEL_0, &pulseItem3, 1, true ); // Send the pulse
}

void handleInterrupt4()
{
    Serial.println( "b4 interrupted" );
    rmt_write_items( RMT_CHANNEL_0, &pulseItem4, 1, true ); // Send the pulse
}

void handleInterrupt5()
{
    Serial.println( "b5 interrupted" );
    rmt_write_items( RMT_CHANNEL_0, &pulseItem5, 1, false ); // Send the pulse
}