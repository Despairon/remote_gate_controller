#include <remote_gate_control/gpio_iface_impl.h>
#include <stdlib.h>
#include <Arduino.h>

typedef struct gpio_ctx_s
{
    gpio_iface_init_data_t init_data;
    int state;
} gpio_ctx_t;

static iface_ctx_t create_gpio(void *init_data);
static void destroy_gpio(iface_ctx_t ctx);

static size_t gpio_read(iface_ctx_t ctx, void *data_buf);
static size_t gpio_write(iface_ctx_t ctx, void *data_buf);
static void gpio_toggle(iface_ctx_t ctx);

const interface_t gpio_iface =
{
    .init = 
    {
        .create  = &create_gpio,
        .destroy = &destroy_gpio
    },

    .variant.gpio_iface =
    {
        .blocking_io = 
        {
            .read  = &gpio_read,
            .write = &gpio_write
        },

        .toggle = &gpio_toggle
    }
};

static iface_ctx_t create_gpio(void *init_data)
{
    iface_ctx_t ctx = NULL;

    if (init_data)
    {
        if (gpio_ctx_t *iface_ctx = (gpio_ctx_t*)malloc(sizeof(gpio_ctx_t)))
        {
            (void)memcpy(&(iface_ctx->init_data), init_data, sizeof(gpio_iface_init_data_t));

            pinMode(iface_ctx->init_data.pin_num, OUTPUT);

            ctx = (iface_ctx_t)iface_ctx;

            gpio_state_t state = LOW;
            gpio_write(ctx, &state);
        }
    }

    return ctx;
}

static void destroy_gpio(iface_ctx_t ctx)
{
    free((gpio_ctx_t*)ctx);
}

static size_t gpio_read(iface_ctx_t ctx, void *data_buf)
{
    if (ctx && data_buf)
    {
        *((gpio_state_t*)data_buf) = (gpio_state_t)digitalRead(((gpio_ctx_t*)ctx)->init_data.pin_num);
    }

    return 0;
}

static size_t gpio_write(iface_ctx_t ctx, void *data_buf)
{
    if (ctx && data_buf)
    {
        digitalWrite(((gpio_ctx_t*)ctx)->init_data.pin_num, *((gpio_state_t*)data_buf));
        return sizeof(gpio_state_t);
    }

    return 0;
}

static void gpio_toggle(iface_ctx_t ctx)
{
    int pin = ((gpio_ctx_t*)ctx)->init_data.pin_num;
	uint8_t bit = digitalPinToBitMask(pin);
    volatile uint8_t *out = portOutputRegister(digitalPinToPort(pin));
	uint8_t oldSREG = SREG;

    cli();

	*out |= (*out & bit) ^ bit; // TODO: incorrect

	SREG = oldSREG;
}