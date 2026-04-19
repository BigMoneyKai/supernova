#include "event.h"
#include "logger.h"
#include "container/darray.h"
#include "memory.h"

#include <stddef.h>

typedef struct registered_event {
    void* listener;
    sys_event callback;
} registered_event;

typedef struct event_code_entry {
    darray events;
} event_code_entry;

typedef struct event_sys_state {
    event_code_entry registered[EVENT_CODE_MAX_NUM];
} event_sys_state;

static b8 is_inited = SN_FALSE;
static event_sys_state state;
static b8 last_event_valid = SN_FALSE;
static u16 last_event_code = 0;
static event_context last_event_ctx;

b8 event_init() {
    if(is_inited) {
        FATAL("Event system registered before");
        return SN_FALSE;
    }

    snmzero(&state, sizeof(state));
    is_inited = SN_TRUE;

    return SN_TRUE;
}

b8 event_register(u16 code, void* listener, sys_event event) {
    if(!is_inited) {
        FATAL("event system not initialized");
        return SN_FALSE;
    }
    if(code >= EVENT_CODE_MAX_NUM) {
        ERROR("Invalid event code: %u", code);
        return SN_FALSE;
    }
    if(!event) {
        ERROR("event_register requires a valid callback");
        return SN_FALSE;
    }

    darray* arr = &state.registered[code].events;
    if(!arr->data) {
        darray_create(arr, registered_event);
    }

    registered_event* events = (registered_event*)arr->data;
    for(u64 i = 0; i < arr->length; i++) {
        if(events[i].listener == listener && events[i].callback == event) {
            return SN_FALSE;
        }
    }

    registered_event reg = {listener, event};
    darray_push(arr, reg);
    return SN_TRUE;
}

b8 event_unregister(u16 code, void* listener, sys_event event) {
    if(!is_inited) {
        FATAL("event system not initialized");
        return SN_FALSE;
    }
    if(code >= EVENT_CODE_MAX_NUM) {
        ERROR("Invalid event code: %u", code);
        return SN_FALSE;
    }

    darray* arr = &state.registered[code].events;
    if(!arr->data || arr->length == 0) {
        return SN_FALSE;
    }

    registered_event* events = (registered_event*)arr->data;
    for(u64 i = 0; i < arr->length; i++) {
        if(events[i].listener == listener && events[i].callback == event) {
            darray_pop(arr, NULL);
            if(arr->length == 0) {
                darray_destroy(arr);
            }
            return SN_TRUE;
        }
    }

    return SN_FALSE;
}

b8 event_fire(u16 code, void* sender, event_context context) {
    if(!is_inited) {
        FATAL("event system not initialized");
        return SN_FALSE;
    }
    if(code >= EVENT_CODE_MAX_NUM) {
        ERROR("Invalid event code: %u", code);
        return SN_FALSE;
    }

    darray* arr = &state.registered[code].events;
    if(!arr->data || arr->length == 0) {
        return SN_FALSE;
    }

    last_event_code = code;
    last_event_ctx = context;
    last_event_valid = SN_TRUE;

    registered_event* events = (registered_event*)arr->data;
    for(u64 i = 0; i < arr->length; i++) {
        if(events[i].callback &&
            events[i].callback(code, sender, events[i].listener, &context)) {
            return SN_TRUE;
        }
    }

    return SN_FALSE;
}

static const char* event_code_to_string(u16 code) {
    switch(code) {
        case EVENT_CODE_APP_QUIT: return "APP_QUIT";
        case EVENT_CODE_KEY_PRESSED: return "KEY_PRESSED";
        case EVENT_CODE_KEY_RELEASED: return "KEY_RELEASED";
        case EVENT_CODE_BUTTON_PRESSED: return "BUTTON_PRESSED";
        case EVENT_CODE_BUTTON_RELEASE: return "BUTTON_RELEASED";
        case EVENT_CODE_MOUSE_MOVE: return "MOUSE_MOVE";
        case EVENT_CODE_MOUSE_SCROLLED: return "MOUSE_SCROLLED";
        case EVENT_CODE_WINDOW_RESIZED: return "WINDOW_RESIZED";
        default: return "UNKNOWN";
    }
}

void print_event_msg() {
    if(!last_event_valid) {
        return;
    }

    INFO("event: %s (%u)", event_code_to_string(last_event_code), last_event_code);
    last_event_valid = SN_FALSE;
}
