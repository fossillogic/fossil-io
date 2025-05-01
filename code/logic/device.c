/*
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * This file is part of the Fossil Logic project, which aims to develop high-
 * performance, cross-platform applications and libraries. The code contained
 * herein is subject to the terms and conditions defined in the project license.
 *
 * Author: Michael Gene Brockus (Dreamer)
 *
 * Copyright (C) 2024 Fossil Logic. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "fossil/io/device.h"
#include "fossil/io/output.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/select.h>  // For fd_set, FD_ZERO, FD_SET, select
    #include <sys/time.h>    // For struct timeval
    #include <sys/types.h>   // For system types
#endif

#define MAX_KEYBINDS 256
#define MAX_TOUCHBINDS 256
#define MAX_MOUSEBINDS 256

typedef struct {
    fossil_io_mouse_event_t event;
    fossil_io_mouse_callback_t callback;
} fossil_io_mouse_binding_t;

typedef struct {
    fossil_io_mouse_binding_t bindings[MAX_MOUSEBINDS];
    size_t count;
} fossil_io_mouse_manager_t;

typedef struct {
    fossil_io_touch_event_t event;
    fossil_io_touch_callback_t callback;
} fossil_io_touch_binding_t;

typedef struct {
    fossil_io_touch_binding_t bindings[MAX_TOUCHBINDS];
    size_t count;
} fossil_io_touch_manager_t;

typedef struct {
    fossil_io_keyboard_event_t event;
    fossil_io_keyboard_callback_t callback;
} fossil_io_keyboard_binding_t;

typedef struct {
    fossil_io_keyboard_binding_t bindings[MAX_KEYBINDS];
    size_t count;
} fossil_io_keyboard_manager_t;

static fossil_io_keyboard_manager_t keyboard_manager;
static fossil_io_touch_manager_t touch_manager;
static fossil_io_mouse_manager_t mouse_manager;


#if defined(_WIN32) || defined(_WIN64)

static int fossil_io_keyboard_is_key_pressed(void) {
    return _kbhit();
}

static fossil_io_keyboard_event_t fossil_io_keyboard_get_event(void) {
    fossil_io_keyboard_event_t event = {0};
    int key = _getch();

    // Check for extended keys (e.g., arrow keys, function keys)
    if (key == 0 || key == 224) {
        key = _getch();  // Fetch the actual key code
    }

    // Check modifiers
    event.shift = GetKeyState(VK_SHIFT) & 0x8000;
    event.ctrl = GetKeyState(VK_CONTROL) & 0x8000;
    event.alt = GetKeyState(VK_MENU) & 0x8000;
    event.key = (char)key;

    return event;
}

#else
static struct termios old_termios, new_termios;

static void fossil_io_keyboard_enable_raw_mode(void) {
    tcgetattr(STDIN_FILENO, &old_termios);
    new_termios = old_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

static void fossil_io_keyboard_disable_raw_mode(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
}

static int fossil_io_keyboard_is_key_pressed(void) {
    struct timeval timeout = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &timeout) > 0;
}

static fossil_io_keyboard_event_t fossil_io_keyboard_get_event(void) {
    fossil_io_keyboard_event_t event = {0};
    char c;
    read(STDIN_FILENO, &c, 1);

    event.key = c;

    // Modifier keys (POSIX doesn't have direct access to key states; approximation)
    if (c == 27) { // Alt key (ESC sequence)
        event.alt = 1;
        read(STDIN_FILENO, &c, 1);
        event.key = c;
    } else if (c < 32) { // Ctrl key (control characters)
        event.ctrl = 1;
        event.key = c + 96; // Map to lowercase ASCII
    } else if (c == 127) { // Backspace key with Ctrl
        event.ctrl = 1;
        event.key = 8;
    }

    return event;
}
#endif

void fossil_io_keyboard_init(void) {
    memset(&keyboard_manager, 0, sizeof(keyboard_manager));
#if defined(_WIN32) || defined(_WIN64)
    // Windows doesn't require explicit setup for raw mode.
#else
    fossil_io_keyboard_enable_raw_mode();
    atexit(fossil_io_keyboard_disable_raw_mode);
#endif
}

void fossil_io_keyboard_shutdown(void) {
    memset(&keyboard_manager, 0, sizeof(keyboard_manager));
#if defined(_WIN32) || defined(_WIN64)
    // Windows doesn't require explicit cleanup for raw mode.
#else
    fossil_io_keyboard_disable_raw_mode();
#endif
}

void fossil_io_keyboard_clear_bindings(void) {
    keyboard_manager.count = 0;
}

void fossil_io_keyboard_register_binding(fossil_io_keyboard_event_t event, fossil_io_keyboard_callback_t callback) {
    if (keyboard_manager.count < MAX_KEYBINDS) {
        keyboard_manager.bindings[keyboard_manager.count].event = event;
        keyboard_manager.bindings[keyboard_manager.count].callback = callback;
        keyboard_manager.count++;
    } else {
        fossil_io_fprintf(FOSSIL_STDERR, "Max keybindings reached.\n");
    }
}

void fossil_io_keyboard_unregister_binding(fossil_io_keyboard_event_t event) {
    for (size_t i = 0; i < keyboard_manager.count; ++i) {
        if (keyboard_manager.bindings[i].event.key == event.key &&
            keyboard_manager.bindings[i].event.shift == event.shift &&
            keyboard_manager.bindings[i].event.ctrl == event.ctrl &&
            keyboard_manager.bindings[i].event.alt == event.alt) {
            for (size_t j = i; j < keyboard_manager.count - 1; ++j) {
                keyboard_manager.bindings[j] = keyboard_manager.bindings[j + 1];
            }
            keyboard_manager.count--;
            return;
        }
    }
    fossil_io_fprintf(FOSSIL_STDERR, "No matching keybinding to unregister.\n");
}

void fossil_io_keyboard_poll_events(void) {
    if (fossil_io_keyboard_is_key_pressed()) {
        fossil_io_keyboard_event_t event = fossil_io_keyboard_get_event();

        for (size_t i = 0; i < keyboard_manager.count; ++i) {
            fossil_io_keyboard_binding_t *binding = &keyboard_manager.bindings[i];
            if (binding->event.key == event.key &&
                binding->event.shift == event.shift &&
                binding->event.ctrl == event.ctrl &&
                binding->event.alt == event.alt) {
                if (binding->callback) {
                    binding->callback(event);
                }
                break;
            }
        }
    }
}

// MOUSE
static int fossil_io_mouse_event_match(const fossil_io_mouse_event_t* a, const fossil_io_mouse_event_t* b) {
    return (a->button == b->button &&
            (a->shift == b->shift || a->shift == -1 || b->shift == -1) &&
            (a->ctrl == b->ctrl || a->ctrl == -1 || b->ctrl == -1) &&
            (a->alt == b->alt || a->alt == -1 || b->alt == -1));
}

// Dummy OS-agnostic implementation for now
static void fossil_io_mouse_get_event(fossil_io_mouse_event_t* event) {
    static int toggle = 0;
    event->x = 100 + (toggle++ % 10);
    event->y = 200 + (toggle % 5);
    event->button = toggle % 2;
    event->shift = -1;
    event->ctrl = -1;
    event->alt = -1;
}

void fossil_io_mouse_register_binding(fossil_io_mouse_event_t event, fossil_io_mouse_callback_t callback) {
    if (mouse_manager.count >= MAX_MOUSEBINDS) {
        fossil_io_fprintf(FOSSIL_STDERR, "[mouse] Max bindings reached\n");
        return;
    }

    mouse_manager.bindings[mouse_manager.count++] = (fossil_io_mouse_binding_t){ event, callback };
}

void fossil_io_mouse_unregister_binding(fossil_io_mouse_event_t event) {
    for (size_t i = 0; i < mouse_manager.count; ++i) {
        if (fossil_io_mouse_event_match(&mouse_manager.bindings[i].event, &event)) {
            memmove(&mouse_manager.bindings[i], &mouse_manager.bindings[i + 1],
                    sizeof(fossil_io_mouse_binding_t) * (mouse_manager.count - i - 1));
            --mouse_manager.count;
            return;
        }
    }
    fossil_io_fprintf(FOSSIL_STDERR, "[mouse] Binding not found\n");
}

void fossil_io_mouse_poll_events(void) {
    fossil_io_mouse_event_t event = {0};
    fossil_io_mouse_get_event(&event);

    for (size_t i = 0; i < mouse_manager.count; ++i) {
        if (fossil_io_mouse_event_match(&mouse_manager.bindings[i].event, &event)) {
            if (mouse_manager.bindings[i].callback) {
                mouse_manager.bindings[i].callback(event);
            }
        }
    }
}

void fossil_io_mouse_clear_bindings(void) {
    mouse_manager.count = 0;
    for (size_t i = 0; i < MAX_MOUSEBINDS; ++i) {
        mouse_manager.bindings[i].callback = NULL;
    }
}

void fossil_io_mouse_init(void) {
    memset(&mouse_manager, 0, sizeof(mouse_manager));
}

void fossil_io_mouse_shutdown(void) {
    memset(&mouse_manager, 0, sizeof(mouse_manager));
}

// TOUCH
static int fossil_io_touch_event_match(const fossil_io_touch_event_t* a, const fossil_io_touch_event_t* b) {
    return (a->touch_id == b->touch_id &&
            a->action == b->action &&
            (a->shift == b->shift || a->shift == -1 || b->shift == -1) &&
            (a->ctrl == b->ctrl || a->ctrl == -1 || b->ctrl == -1) &&
            (a->alt == b->alt || a->alt == -1 || b->alt == -1));
}

// Dummy implementation for simulation/testing
static void fossil_io_touch_get_event(fossil_io_touch_event_t* event) {
    static int state = 0;
    event->x = 320 + (state % 3);
    event->y = 240 + (state % 2);
    event->touch_id = 1;
    event->action = state++ % 3;  // 0=start, 1=move, 2=end
    event->shift = -1;
    event->ctrl = -1;
    event->alt = -1;
}

void fossil_io_touch_register_binding(fossil_io_touch_event_t event, fossil_io_touch_callback_t callback) {
    if (touch_manager.count >= MAX_TOUCHBINDS) {
        fossil_io_fprintf(FOSSIL_STDERR, "[touch] Max bindings reached\n");
        return;
    }

    touch_manager.bindings[touch_manager.count++] = (fossil_io_touch_binding_t){ event, callback };
}

void fossil_io_touch_unregister_binding(fossil_io_touch_event_t event) {
    for (size_t i = 0; i < touch_manager.count; ++i) {
        if (fossil_io_touch_event_match(&touch_manager.bindings[i].event, &event)) {
            memmove(&touch_manager.bindings[i], &touch_manager.bindings[i + 1],
                    sizeof(fossil_io_touch_binding_t) * (touch_manager.count - i - 1));
            --touch_manager.count;
            return;
        }
    }
    fossil_io_fprintf(FOSSIL_STDERR, "[touch] Binding not found\n");
}

void fossil_io_touch_poll_events(void) {
    fossil_io_touch_event_t event = {0};
    fossil_io_touch_get_event(&event);

    for (size_t i = 0; i < touch_manager.count; ++i) {
        if (fossil_io_touch_event_match(&touch_manager.bindings[i].event, &event)) {
            if (touch_manager.bindings[i].callback) {
                touch_manager.bindings[i].callback(event);
            }
        }
    }
}

void fossil_io_touch_clear_bindings(void) {
    touch_manager.count = 0;
    for (size_t i = 0; i < MAX_TOUCHBINDS; ++i) {
        touch_manager.bindings[i].callback = NULL;
    }
}

void fossil_io_touch_init(void) {
    memset(&touch_manager, 0, sizeof(touch_manager));
}

void fossil_io_touch_shutdown(void) {
    memset(&touch_manager, 0, sizeof(touch_manager));
}
