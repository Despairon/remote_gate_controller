#include <remote_gate_control/uart_iface_impl.h>
#include <stdlib.h>
#include <Arduino.h>

typedef struct uart_iface_data_s
{
    const uart_iface_t *iface;
} uart_iface_data_t;

static iface_ctx_t create_uart_iface(void *init_data);
static void destroy_uart_iface(iface_ctx_t ctx);

static void uart_iface_begin(iface_ctx_t ctx, uint32_t baudrate);
static size_t uart_iface_available_for_read(iface_ctx_t ctx);
static size_t uart_iface_write(iface_ctx_t ctx, buffer_t *buffer);
static size_t uart_iface_read(iface_ctx_t ctx, buffer_t *buffer);
static void uart_iface_flush(iface_ctx_t ctx);
static void uart_iface_end(iface_ctx_t ctx);

const init_iface_t uart_iface_init =
{
    .create  = &create_uart_iface,
    .destroy = &destroy_uart_iface
};

static const uart_iface_t uart_iface =
{
    .begin              = &uart_iface_begin,
    .available_for_read = &uart_iface_available_for_read,
    .write              = &uart_iface_write,
    .read               = &uart_iface_read,
    .flush              = &uart_iface_flush,
    .end                = &uart_iface_end
};

static iface_ctx_t create_uart_iface(void *init_data )
{
    (void)init_data;
    
    iface_ctx_t ctx = NULL;

    uart_iface_data_t *iface_data = (uart_iface_data_t*)malloc(sizeof(uart_iface_data_t));
    if (iface_data)
    {
       iface_data->iface = &uart_iface;

       ctx = (iface_ctx_t)iface_data;
    }

    return ctx;
}

static void destroy_uart_iface(iface_ctx_t ctx)
{
    if (ctx)
        free((uart_iface_data_t*)ctx);
}

static void uart_iface_begin(iface_ctx_t ctx, uint32_t baudrate)
{
    (void)ctx;

    Serial.begin(baudrate);
}

static size_t uart_iface_available_for_read(iface_ctx_t ctx)
{
    (void)ctx;

    return (size_t)Serial.available();
}

static size_t uart_iface_write(iface_ctx_t ctx, buffer_t *buffer)
{
    (void)ctx;

    size_t written_bytes = 0;
    
    if (buffer)
    {

        if ((buffer->data != NULL) && (buffer->size > 0))
        {
            written_bytes = Serial.write(buffer->data, buffer->size);
        }
    }

    return written_bytes;
}

static size_t uart_iface_read(iface_ctx_t ctx, buffer_t *buffer)
{
    (void)ctx;

    size_t read_bytes = 0;

    if (buffer)
    {
        if ((buffer->data != NULL) && (buffer->size > 0))
        {
            read_bytes = Serial.readBytes(buffer->data, buffer->size);
        }
    }

    return read_bytes;
}

static void uart_iface_flush(iface_ctx_t ctx)
{
    (void)ctx;

    Serial.flush();
}

static void uart_iface_end(iface_ctx_t ctx)
{
    (void)ctx;

    Serial.end();
}