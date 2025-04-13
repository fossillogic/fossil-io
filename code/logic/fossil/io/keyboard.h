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

/** @brief Initialize the mouse event library. */
void fossil_io_mouse_init(void);

/** @brief Shut down the mouse event library. */
void fossil_io_mouse_shutdown(void);

/** @brief Clear all mouse event bindings. */
void fossil_io_mouse_clear_bindings(void);

/**
 * @brief Register a mouse event binding.
 * @param event The mouse event to bind.
 * @param callback The callback function to invoke when the event occurs.
 */
void fossil_io_mouse_register_binding(fossil_io_mouse_event_t event, fossil_io_mouse_callback_t callback);

/**
 * @brief Unregister a mouse event binding.
 * @param event The mouse event to unbind.
 */
void fossil_io_mouse_unregister_binding(fossil_io_mouse_event_t event);

/** @brief Poll and dispatch pending mouse events. */
void fossil_io_mouse_poll_events(void);

/** @brief Initialize the touch event library. */
void fossil_io_touch_init(void);

/** @brief Shut down the touch event library. */
void fossil_io_touch_shutdown(void);

/** @brief Clear all touch event bindings. */
void fossil_io_touch_clear_bindings(void);

/**
 * @brief Register a touch event binding.
 * @param event The touch event to bind.
 * @param callback The callback function to invoke when the event occurs.
 */
void fossil_io_touch_register_binding(fossil_io_touch_event_t event, fossil_io_touch_callback_t callback);

/**
 * @brief Unregister a touch event binding.
 * @param event The touch event to unbind.
 */
void fossil_io_touch_unregister_binding(fossil_io_touch_event_t event);

/** @brief Poll and dispatch pending touch events. */
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
         * @brief Class for managing keyboard input.
         * 
         * This class provides a high-level interface for handling keyboard input.
         * It wraps the underlying C API and manages initialization and cleanup automatically.
         */
        class Keyboard {
        public:
            /**
             * @brief Constructor that initializes the keyboard system.
             * 
             * Call this to set up the keyboard event system.
             * Automatically prepares the library for receiving and processing keyboard events.
             */
            Keyboard() {
                fossil_io_keyboard_init();
            }

            /**
             * @brief Destructor that shuts down the keyboard system.
             * 
             * Automatically cleans up the keyboard event system on destruction.
             * This prevents memory leaks and releases any platform-specific resources.
             */
            ~Keyboard() {
                fossil_io_keyboard_shutdown();
            }

            /**
             * @brief Clear all registered keyboard bindings.
             * 
             * Use this method to remove all current keyboard event bindings.
             * This is useful if you want to reset the input state or rebind controls dynamically.
             */
            void clear_bindings() {
                fossil_io_keyboard_clear_bindings();
            }

            /**
             * @brief Register a keyboard event binding.
             * 
             * Binds a specific keyboard event to a callback function. When the event occurs,
             * the corresponding callback will be executed.
             * 
             * @param event The keyboard event to listen for.
             * @param callback The function to call when the event is triggered.
             */
            void register_binding(fossil_io_keyboard_event_t event, fossil_io_keyboard_callback_t callback) {
                fossil_io_keyboard_register_binding(event, callback);
            }

            /**
             * @brief Unregister a specific keyboard event binding.
             * 
             * Removes the callback associated with a particular keyboard event.
             * 
             * @param event The event for which the callback should be removed.
             */
            void unregister_binding(fossil_io_keyboard_event_t event) {
                fossil_io_keyboard_unregister_binding(event);
            }

            /**
             * @brief Poll for keyboard events and dispatch callbacks.
             * 
             * Call this in your main loop to process pending keyboard events.
             * It will trigger any registered callbacks for events that have occurred.
             */
            void poll_events() {
                fossil_io_keyboard_poll_events();
            }
        };

        /**
         * @brief Class for managing mouse input.
         * 
         * This class provides a modern C++ interface for handling mouse interactions.
         * It wraps the underlying mouse system and ensures proper initialization and cleanup.
         */
        class Mouse {
        public:
            /**
             * @brief Constructor that initializes the mouse system.
             * 
             * Prepares the internal state for processing mouse input.
             * Must be called before polling for events or registering bindings.
             */
            Mouse() {
                fossil_io_mouse_init();
            }

            /**
             * @brief Destructor that shuts down the mouse system.
             * 
             * Cleans up resources associated with mouse input.
             * Should not be called manually if you're using stack allocation.
             */
            ~Mouse() {
                fossil_io_mouse_shutdown();
            }

            /**
             * @brief Clear all mouse bindings.
             * 
             * Removes all registered mouse event bindings.
             * Use this to reset or reconfigure the input system during runtime.
             */
            void clear_bindings() {
                fossil_io_mouse_clear_bindings();
            }

            /**
             * @brief Register a mouse event binding.
             * 
             * Assigns a callback to a specific mouse event.
             * 
             * @param event The mouse event (e.g., click, movement).
             * @param callback The function to invoke when the event occurs.
             */
            void register_binding(fossil_io_mouse_event_t event, fossil_io_mouse_callback_t callback) {
                fossil_io_mouse_register_binding(event, callback);
            }

            /**
             * @brief Unregister a mouse event binding.
             * 
             * Detaches a callback from a previously bound mouse event.
             * 
             * @param event The event to unbind.
             */
            void unregister_binding(fossil_io_mouse_event_t event) {
                fossil_io_mouse_unregister_binding(event);
            }

            /**
             * @brief Poll for mouse events and invoke callbacks.
             * 
             * Should be called regularly (e.g., in your main loop) to process
             * incoming mouse events and dispatch them to registered handlers.
             */
            void poll_events() {
                fossil_io_mouse_poll_events();
            }
        };

        /**
         * @brief Class for managing touch input.
         * 
         * Provides an abstraction layer for working with touchscreen input.
         * Ensures the input system is initialized and destroyed cleanly.
         */
        class Touch {
        public:
            /**
             * @brief Constructor that initializes the touch system.
             * 
             * Sets up the library to begin listening for touch interactions.
             */
            Touch() {
                fossil_io_touch_init();
            }

            /**
             * @brief Destructor that shuts down the touch system.
             * 
             * Releases any resources used for handling touch input.
             */
            ~Touch() {
                fossil_io_touch_shutdown();
            }

            /**
             * @brief Clear all touch bindings.
             * 
             * Removes all existing event-to-callback mappings for touch input.
             */
            void clear_bindings() {
                fossil_io_touch_clear_bindings();
            }

            /**
             * @brief Register a touch event binding.
             * 
             * Associates a callback with a specific touch event.
             * 
             * @param event The type of touch event to respond to.
             * @param callback The function to invoke when the event occurs.
             */
            void register_binding(fossil_io_touch_event_t event, fossil_io_touch_callback_t callback) {
                fossil_io_touch_register_binding(event, callback);
            }

            /**
             * @brief Unregister a touch event binding.
             * 
             * Removes the callback associated with the given touch event.
             * 
             * @param event The event whose binding should be removed.
             */
            void unregister_binding(fossil_io_touch_event_t event) {
                fossil_io_touch_unregister_binding(event);
            }

            /**
             * @brief Poll for touch events and dispatch them to callbacks.
             * 
             * Should be called frequently to handle active touch events.
             */
            void poll_events() {
                fossil_io_touch_poll_events();
            }
        };
    }
}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
