#include <remote_gate_control/rgc_common.h>
#include <remote_gate_control/rgc_platform_ifaces.h>
#include <Arduino.h>

#define RGC_PLATFORM_SWITCH_GPIO_PIN (4)

static void rgc_platform_debug_uart_begin(uint16_t baudrate);
static void rgc_platform_debug_uart_flush();
static uint8_t rgc_platform_debug_uart_read_byte();
static size_t rgc_platform_debug_uart_read(uint8_t *buffer, size_t buffer_sz);
static size_t rgc_platform_debug_uart_write(uint8_t *buffer, size_t buffer_sz);
static void rgc_platform_debug_uart_end();

static void rgc_platform_receiver_uart_begin(uint16_t baudrate);
static void rgc_platform_receiver_uart_flush();
static uint8_t rgc_platform_receiver_uart_read_byte();
static size_t rgc_platform_receiver_uart_read(uint8_t *buffer, size_t buffer_sz);
static size_t rgc_platform_receiver_uart_write(uint8_t *buffer, size_t buffer_sz);
static void rgc_platform_receiver_uart_end();

void rgc_platform_switch_gpio_setup()
{
    pinMode(RGC_PLATFORM_SWITCH_GPIO_PIN, OUTPUT);
    digitalWrite(RGC_PLATFORM_SWITCH_GPIO_PIN, LOW);
}

void rgc_platform_switch_gpio_write(uint8_t state)
{
    digitalWrite(RGC_PLATFORM_SWITCH_GPIO_PIN, state ? HIGH : LOW);
}

uint64_t rgc_platform_get_time_ms()
{
    return millis();
}

const rgc_platform_uart_iface_t rgc_platform_debug_uart_iface =
{
    .begin     = &rgc_platform_debug_uart_begin,
    .flush     = &rgc_platform_debug_uart_flush,
    .read_byte = &rgc_platform_debug_uart_read_byte,
    .read      = &rgc_platform_debug_uart_read,
    .write     = &rgc_platform_debug_uart_write,
    .end       = &rgc_platform_debug_uart_end
};

const rgc_platform_uart_iface_t rgc_platform_receiver_uart_iface =
{
    .begin     = &rgc_platform_receiver_uart_begin,
    .flush     = &rgc_platform_receiver_uart_flush,
    .read_byte = &rgc_platform_receiver_uart_read_byte,
    .read      = &rgc_platform_receiver_uart_read,
    .write     = &rgc_platform_receiver_uart_write,
    .end       = &rgc_platform_receiver_uart_end
};

static void rgc_platform_debug_uart_begin(uint16_t baudrate)
{
    // TODO: implement
}

static void rgc_platform_debug_uart_flush()
{
    // TODO: implement
}

static uint8_t rgc_platform_debug_uart_read_byte()
{
    // TODO: implement
}

static size_t rgc_platform_debug_uart_read(uint8_t *buffer, size_t buffer_sz)
{
    // TODO: implement
}

static size_t rgc_platform_debug_uart_write(uint8_t *buffer, size_t buffer_sz)
{
    // TODO: implement
}

static void rgc_platform_debug_uart_end()
{
    // TODO: implement
}

static void rgc_platform_receiver_uart_begin(uint16_t baudrate)
{
    Serial.begin(baudrate);
}

static void rgc_platform_receiver_uart_flush()
{
    Serial.flush();
}

static uint8_t rgc_platform_receiver_uart_read_byte()
{
    uint8_t byte = '\0';
    
    return Serial.readBytes(&byte, 1) ? byte : '\0';
}

static size_t rgc_platform_receiver_uart_read(uint8_t *buffer, size_t buffer_sz)
{
    return (buffer && buffer_sz) ? Serial.readBytes(buffer, buffer_sz) : 0;
}

static size_t rgc_platform_receiver_uart_write(uint8_t *buffer, size_t buffer_sz)
{
    return (buffer && buffer_sz) ? Serial.write(buffer, buffer_sz) : 0;
}

static void rgc_platform_receiver_uart_end()
{
    Serial.end();
}