#include <remote_gate_control/generic_serial_iface_impl.h>
#include <stdlib.h>
#include <Arduino.h>

typedef struct serial_iface_data_s
{
    const generic_serial_iface_t *iface;
} serial_iface_data_t;

static generic_iface_handle_t create_serial_iface(void *init_data);
static void destroy_serial_iface(generic_iface_handle_t handle);

static void serial_iface_begin(generic_iface_handle_t handle, uint32_t baudrate);
static size_t serial_iface_available_for_read(generic_iface_handle_t handle);
static size_t serial_iface_write(generic_iface_handle_t handle, buffer_t *buffer);
static size_t serial_iface_read(generic_iface_handle_t handle, buffer_t *buffer);
static void serial_iface_flush(generic_iface_handle_t handle);
static void serial_iface_end(generic_iface_handle_t handle);

const generic_iface_ctor_dtor_t generic_serial_iface_ctor_dtor =
{
    .create  = &create_serial_iface,
    .destroy = &destroy_serial_iface
};

static const generic_serial_iface_t serial_iface =
{
    .begin              = &serial_iface_begin,
    .available_for_read = &serial_iface_available_for_read,
    .write              = &serial_iface_write,
    .read               = &serial_iface_read,
    .flush              = &serial_iface_flush,
    .end                = &serial_iface_end
};

static generic_iface_handle_t create_serial_iface(void *init_data )
{
    (void)init_data;
    
    generic_iface_handle_t handle = NULL;

    serial_iface_data_t *iface_data = (serial_iface_data_t*)malloc(sizeof(serial_iface_data_t));
    if (iface_data)
    {
       iface_data->iface = &serial_iface;

       handle = (generic_iface_handle_t)iface_data;
    }

    return handle;
}

static void destroy_serial_iface(generic_iface_handle_t handle)
{
    if (handle)
        free((serial_iface_data_t*)handle);
}

static void serial_iface_begin(generic_iface_handle_t handle, uint32_t baudrate)
{
    (void)handle;

    Serial.begin(baudrate);
}

static size_t serial_iface_available_for_read(generic_iface_handle_t handle)
{
    (void)handle;

    return (size_t)Serial.available();
}

static size_t serial_iface_write(generic_iface_handle_t handle, buffer_t *buffer)
{
    (void)handle;

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

static size_t serial_iface_read(generic_iface_handle_t handle, buffer_t *buffer)
{
    (void)handle;

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

static void serial_iface_flush(generic_iface_handle_t handle)
{
    (void)handle;

    Serial.flush();
}

static void serial_iface_end(generic_iface_handle_t handle)
{
    (void)handle;

    Serial.end();
}