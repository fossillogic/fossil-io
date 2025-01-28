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
 * Register a keybinding with the library.
 *
 * @param event     The keyboard event to bind to.
 * @param callback  The callback function to call when the event occurs.
 */
void fossil_io_keyboard_register_binding(fossil_io_keyboard_event_t event, fossil_io_keyboard_callback_t callback);

/**
 * Poll for keyboard events and trigger any registered callbacks.
 * This function should be called in the main loop of the application.
 */
void fossil_io_keyboard_poll_events(void);

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
        class Keyboard {
        public:
            /**
             * Initialize the keyboard library.
             * Sets up any platform-specific configurations.
             */
            static void initialize() {
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
             * Register a keybinding with the library.
             *
             * @param event     The keyboard event to bind to.
             * @param callback  The callback function to call when the event occurs.
             */
            static void registerBinding(fossil_io_keyboard_event_t event, fossil_io_keyboard_callback_t callback) {
                fossil_io_keyboard_register_binding(event, callback);
            }

            /**
             * Poll for keyboard events and trigger any registered callbacks.
             * This function should be called in the main loop of the application.
             */
            static void pollEvents() {
                fossil_io_keyboard_poll_events();
            }
        };
    }
}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
