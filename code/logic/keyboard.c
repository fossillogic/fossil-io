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
#include "fossil/io/keyboard.h"
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

typedef struct {
    fossil_io_keyboard_event_t event;
    fossil_io_keyboard_callback_t callback;
} fossil_io_keyboard_binding_t;

typedef struct {
    fossil_io_keyboard_binding_t bindings[MAX_KEYBINDS];
    size_t count;
} fossil_io_keyboard_manager_t;

static fossil_io_keyboard_manager_t keyboard_manager = { .count = 0 };

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
#if defined(_WIN32) || defined(_WIN64)
    // Windows doesn't require explicit setup for raw mode.
#else
    fossil_io_keyboard_enable_raw_mode();
    atexit(fossil_io_keyboard_disable_raw_mode);
#endif
}

void fossil_io_keyboard_shutdown(void) {
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
        fprintf(stderr, "Max keybindings reached.\n");
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
    fprintf(stderr, "No matching keybinding to unregister.\n");
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
