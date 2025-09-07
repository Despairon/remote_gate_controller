#include <rgc_platform_ifaces.h>
#include <Arduino.h>
#include <VirtualWire.h>

#define RGC_PLATFORM_RECEIVER_UART_RX_PIN (2)
#define RGC_PLATFORM_RECEIVER_UART_TX_PIN (2)
#define RGC_PLATFORM_SWITCH_GPIO_PIN (4)
#define RGC_PLATFORM_TRANSMITTER_BUTTON_PIN (3)

static void rgc_platform_debug_uart_begin(uint16_t baudrate);
static void rgc_platform_debug_uart_flush();
static bool rgc_platform_debug_uart_data_available();
static size_t rgc_platform_debug_uart_read(uint8_t *buffer, size_t buffer_sz);
static size_t rgc_platform_debug_uart_write(uint8_t *buffer, size_t buffer_sz);
static void rgc_platform_debug_uart_end();

static void rgc_platform_receiver_uart_begin(uint16_t baudrate);
static void rgc_platform_receiver_uart_flush();
static bool rgc_platform_receiver_uart_data_available();
static size_t rgc_platform_receiver_uart_read(uint8_t *buffer, size_t buffer_sz);
static size_t rgc_platform_receiver_uart_write(uint8_t *buffer, size_t buffer_sz);
static void rgc_platform_receiver_uart_end();

static void rgc_platform_transmitter_uart_begin(uint16_t baudrate);
static void rgc_platform_transmitter_uart_flush();
static bool rgc_platform_transmitter_uart_data_available();
static size_t rgc_platform_transmitter_uart_read(uint8_t *buffer, size_t buffer_sz);
static size_t rgc_platform_transmitter_uart_write(uint8_t *buffer, size_t buffer_sz);
static void rgc_platform_transmitter_uart_end();

const rgc_platform_uart_iface_t rgc_platform_debug_uart_iface =
{
    .begin          = &rgc_platform_debug_uart_begin,
    .flush          = &rgc_platform_debug_uart_flush,
    .data_available = &rgc_platform_debug_uart_data_available,
    .read           = &rgc_platform_debug_uart_read,
    .write          = &rgc_platform_debug_uart_write,
    .end            = &rgc_platform_debug_uart_end
};

const rgc_platform_uart_iface_t rgc_platform_receiver_uart_iface =
{
    .begin          = &rgc_platform_receiver_uart_begin,
    .flush          = &rgc_platform_receiver_uart_flush,
    .data_available = &rgc_platform_receiver_uart_data_available,
    .read           = &rgc_platform_receiver_uart_read,
    .write          = &rgc_platform_receiver_uart_write,
    .end            = &rgc_platform_receiver_uart_end
};

const rgc_platform_uart_iface_t rgc_platform_transmitter_uart_iface =
{
    .begin          = &rgc_platform_transmitter_uart_begin,
    .flush          = &rgc_platform_transmitter_uart_flush,
    .data_available = &rgc_platform_transmitter_uart_data_available,
    .read           = &rgc_platform_transmitter_uart_read,
    .write          = &rgc_platform_transmitter_uart_write,
    .end            = &rgc_platform_transmitter_uart_end
};

void rgc_platform_switch_gpio_setup()
{
    pinMode(RGC_PLATFORM_SWITCH_GPIO_PIN, OUTPUT);
    digitalWrite(RGC_PLATFORM_SWITCH_GPIO_PIN, HIGH);
}

void rgc_platform_switch_gpio_write(uint8_t state)
{
    digitalWrite(RGC_PLATFORM_SWITCH_GPIO_PIN, state ? HIGH : LOW);
}

void rgc_platform_transmitter_button_setup()
{
    pinMode(RGC_PLATFORM_TRANSMITTER_BUTTON_PIN, INPUT);
}

bool rgc_platform_transmitter_is_button_pressed()
{
    return (digitalRead(RGC_PLATFORM_TRANSMITTER_BUTTON_PIN) == LOW) ? false : true;
}

uint64_t rgc_platform_get_time_ms()
{
    return millis();
}

static void rgc_platform_debug_uart_begin(uint16_t baudrate)
{
    Serial.begin(baudrate);
}

static void rgc_platform_debug_uart_flush()
{
    Serial.flush();
}

static bool rgc_platform_debug_uart_data_available()
{
    return Serial.available() ? true : false;
}

static size_t rgc_platform_debug_uart_read(uint8_t *buffer, size_t buffer_sz)
{
    return (buffer && buffer_sz) ? Serial.readBytes(buffer, buffer_sz) : 0;
}

static size_t rgc_platform_debug_uart_write(uint8_t *buffer, size_t buffer_sz)
{
    return (buffer && buffer_sz) ? Serial.write(buffer, buffer_sz) : 0;
}

static void rgc_platform_debug_uart_end()
{
    Serial.end();
}

static void rgc_platform_receiver_uart_begin(uint16_t baudrate)
{
    vw_set_rx_pin(RGC_PLATFORM_RECEIVER_UART_RX_PIN);
    vw_setup(baudrate);
    vw_rx_start();
}

static void rgc_platform_receiver_uart_flush()
{
    // Do nothing for VirtualWire
}

static bool rgc_platform_receiver_uart_data_available()
{
    return (bool)vw_have_message();
}

static size_t rgc_platform_receiver_uart_read(uint8_t *buffer, size_t buffer_sz)
{
    if (buffer && buffer_sz)
        if (vw_get_message(buffer, (uint8_t*)(&buffer_sz)))
            return buffer_sz;

    return 0;
}

static size_t rgc_platform_receiver_uart_write(uint8_t *buffer, size_t buffer_sz)
{
    // Receiver doesn't do writes
    return 0;
}

static void rgc_platform_receiver_uart_end()
{
    vw_rx_stop();
}

static void rgc_platform_transmitter_uart_begin(uint16_t baudrate)
{
    vw_set_tx_pin(RGC_PLATFORM_RECEIVER_UART_TX_PIN);
    vw_setup(baudrate);
}

static void rgc_platform_transmitter_uart_flush()
{
    // Do nothing for VirtualWire
}

static bool rgc_platform_transmitter_uart_data_available()
{
    // Nothing to read for VirtualWire
    return false;
}

static size_t rgc_platform_transmitter_uart_read(uint8_t *buffer, size_t buffer_sz)
{
    // Transmitter doesn't do reads
    return 0;
}

static size_t rgc_platform_transmitter_uart_write(uint8_t *buffer, size_t buffer_sz)
{
    if (buffer && buffer_sz)
    {
        if (vw_send(buffer, (uint8_t)buffer_sz))
        {
            vw_wait_tx();
            return buffer_sz;
        }
    }

    return 0;
}

static void rgc_platform_transmitter_uart_end()
{
    // Do nothing for VirtualWire
}
