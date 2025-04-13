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
#ifndef FOSSIL_IO_KEYBOARD_H
#define FOSSIL_IO_KEYBOARD_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int x;      // X position of the mouse
    int y;      // Y position of the mouse
    int button; // Mouse button (0 = left, 1 = right, etc.)
    int shift;  // 1 if Shift is pressed, 0 otherwise
    int ctrl;   // 1 if Ctrl is pressed, 0 otherwise
    int alt;    // 1 if Alt is pressed, 0 otherwise
} fossil_io_mouse_event_t;

typedef void (*fossil_io_mouse_callback_t)(fossil_io_mouse_event_t event);

#define MAX_MOUSEBINDS 256

typedef struct {
    fossil_io_mouse_event_t event;
    fossil_io_mouse_callback_t callback;
} fossil_io_mouse_binding_t;

typedef struct {
    fossil_io_mouse_binding_t bindings[MAX_MOUSEBINDS];
    size_t count;
} fossil_io_mouse_manager_t;

static fossil_io_mouse_manager_t mouse_manager = { .count = 0 };

typedef struct {
    int x;          // X position of the touch
    int y;          // Y position of the touch
    int touch_id;   // Unique identifier for the touch (for multi-touch)
    int action;     // Action: 0 = start, 1 = move, 2 = end
    int shift;      // 1 if Shift is pressed, 0 otherwise
    int ctrl;       // 1 if Ctrl is pressed, 0 otherwise
    int alt;        // 1 if Alt is pressed, 0 otherwise
} fossil_io_touch_event_t;

typedef void (*fossil_io_touch_callback_t)(fossil_io_touch_event_t event);

#define MAX_TOUCHBINDS 256

typedef struct {
    fossil_io_touch_event_t event;
    fossil_io_touch_callback_t callback;
} fossil_io_touch_binding_t;

typedef struct {
    fossil_io_touch_binding_t bindings[MAX_TOUCHBINDS];
    size_t count;
} fossil_io_touch_manager_t;

static fossil_io_touch_manager_t touch_manager = { .count = 0 };

// Define a keyboard event structure
typedef struct {
    int shift; // 1 if Shift is pressed, 0 otherwise
    int ctrl;  // 1 if Ctrl is pressed, 0 otherwise
    int alt;   // 1 if Alt is pressed, 0 otherwise
    char key;  // The character of the key pressed
} fossil_io_keyboard_event_t;

// Define a callback type for key events
typedef void (*fossil_io_keyboard_callback_t)(fossil_io_keyboard_event_t event);

/**
 * Initialize the keyboard library.
 * Sets up any platform-specific configurations.
 */
void fossil_io_keyboard_init(void);

/**
 * Shut down the keyboard library.
 * Cleans up any platform-specific configurations.
 */
void fossil_io_keyboard_shutdown(void);

/**
 * Clear all keybindings from the library.
 */
void fossil_io_keyboard_clear_bindings(void);

/**
 * Register a keybinding with the library.
 *
 * @param event     The keyboard event to bind to.
 * @param callback  The callback function to call when the event occurs.
 */
void fossil_io_keyboard_register_binding(fossil_io_keyboard_event_t event, fossil_io_keyboard_callback_t callback);

/**
 * Unregister a keybinding with the library.
 *
 * @param event     The keyboard event to unbind.
 */
void fossil_io_keyboard_unregister_binding(fossil_io_keyboard_event_t event);

/**
 * Poll for keyboard events and trigger any registered callbacks.
 * This function should be called in the main loop of the application.
 */
void fossil_io_keyboard_poll_events(void);

// Functions for mouse handling
void fossil_io_mouse_init(void);                    // Initialize the mouse event library
void fossil_io_mouse_shutdown(void);                // Shut down the mouse event library
void fossil_io_mouse_register_binding(fossil_io_mouse_event_t event, fossil_io_mouse_callback_t callback); // Register a mouse binding
void fossil_io_mouse_unregister_binding(fossil_io_mouse_event_t event); // Unregister a mouse binding
void fossil_io_mouse_poll_events(void);

// Functions for touch handling
void fossil_io_touch_init(void);                     // Initialize the touch event library
void fossil_io_touch_shutdown(void);                 // Shut down the touch event library
void fossil_io_touch_register_binding(fossil_io_touch_event_t event, fossil_io_touch_callback_t callback); // Register a touch binding
void fossil_io_touch_unregister_binding(fossil_io_touch_event_t event); // Unregister a touch binding
void fossil_io_touch_poll_events(void);

#ifdef __cplusplus
}

/**
 * C++ wrapper for the SOAP API.
 */
namespace fossil {

    /**
     * Namespace for I/O operations.
     */
    namespace io {
        /**
         * Class for interacting with the keyboard.
         */
        class keyboard {
        public:
            /**
             * Initialize the keyboard library.
             * Sets up any platform-specific configurations.
             */
            static void init() {
                fossil_io_keyboard_init();
            }

            /**
             * Shut down the keyboard library.
             * Cleans up any platform-specific configurations.
             */
            static void shutdown() {
                fossil_io_keyboard_shutdown();
            }

            /**
             * Clear all keybindings from the library.
             */
            static void clear_bindings() {
                fossil_io_keyboard_clear_bindings();
            }

            /**
             * Register a keybinding with the library.
             *
             * @param event     The keyboard event to bind to.
             * @param callback  The callback function to call when the event occurs.
             */
            static void register_binding(fossil_io_keyboard_event_t event, fossil_io_keyboard_callback_t callback) {
                fossil_io_keyboard_register_binding(event, callback);
            }

            /**
             * Unregister a keybinding with the library.
             *
             * @param event     The keyboard event to unbind.
             */
            static void unregister_binding(fossil_io_keyboard_event_t event) {
                fossil_io_keyboard_unregister_binding(event);
            }

            /**
             * Poll for keyboard events and trigger any registered callbacks.
             * This function should be called in the main loop of the application.
             */
            static void poll_events() {
                fossil_io_keyboard_poll_events();
            }
        };
    }
}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
