/**
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
#ifndef FOSSIL_IO_TUI_H
#define FOSSIL_IO_TUI_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Basic Cell Structure
/**
 * Represents a single character on the screen, along with its foreground and background
 * colors, and any text attributes (such as bold or underline).
 */
typedef struct {
    char ch;        // The character displayed in the cell (e.g., 'A', '1', etc.)
    uint8_t fg;     // Foreground color (index to color palette)
    uint8_t bg;     // Background color (index to color palette)
    uint8_t attr;   // Text attributes (e.g., bold, underline, etc.)
} fossil_tui_cell_t;

// Framebuffer for off-screen drawing
/**
 * A framebuffer stores a grid of `fossil_tui_cell_t` cells that can be used for off-screen
 * drawing and manipulation before being displayed to the user.
 */
typedef struct {
    int width, height;      // Dimensions of the framebuffer (width and height in cells)
    fossil_tui_cell_t* cells; // Pointer to an array of cells that holds the screen data
} fossil_tui_framebuffer_t;

// Brush for reusable draw settings
/**
 * A brush defines the drawing settings (character, foreground color, background color, 
 * and attributes) that can be reused for drawing multiple cells or characters.
 */
typedef struct {
    char ch;        // The character to be drawn
    uint8_t fg;     // Foreground color
    uint8_t bg;     // Background color
    uint8_t attr;   // Text attributes (e.g., bold, underline)
} fossil_tui_brush_t;

// Viewport for scrolling regions
/**
 * A viewport defines a rectangular region on the screen where content is visible.
 * It can be used for managing scrolling or clipping content inside specific areas.
 */
typedef struct {
    int x, y;       // Starting position (x, y) of the viewport (top-left corner)
    int width, height; // Dimensions of the viewport (width and height in cells)
} fossil_tui_viewport_t;

// Basic init/teardown
/**
 * Initializes the TUI system and prepares the screen for drawing and interaction.
 */
void fossil_io_tui_init(void);

/**
 * Shuts down the TUI system, cleaning up any resources allocated during initialization.
 */
void fossil_io_tui_shutdown(void);

// Drawing
/**
 * Clears the screen and resets all cells to their default state (empty).
 */
void fossil_io_tui_clear(void);

/**
 * Presents the current state of the framebuffer to the user (renders the screen).
 */
void fossil_io_tui_present(void);

/**
 * Draws a character at the specified position with the specified foreground, background,
 * and text attributes.
 */
void fossil_io_tui_draw(int x, int y, char ch, uint8_t fg, uint8_t bg, uint8_t attr);

/**
 * Draws a character at the specified position using the settings from the given brush.
 */
void fossil_io_tui_draw_brush(int x, int y, fossil_tui_brush_t brush);

// Framebuffer
/**
 * Creates a framebuffer with the specified width and height.
 */
fossil_tui_framebuffer_t fossil_io_tui_create_framebuffer(int w, int h);

/**
 * Destroys the framebuffer, freeing any resources associated with it.
 */
void fossil_io_tui_destroy_framebuffer(fossil_tui_framebuffer_t* fb);

/**
 * Draws the entire contents of a framebuffer to the screen.
 */
void fossil_io_tui_draw_framebuffer(fossil_tui_framebuffer_t* fb);

// Viewport
/**
 * Sets the current viewport, determining the area of the screen that will be visible.
 */
void fossil_io_tui_set_viewport(fossil_tui_viewport_t vp);

// Text Output
/**
 * Draws text at the specified position (x, y) on the screen.
 */
void fossil_io_tui_draw_text(int x, int y, const char* text);

/**
 * Draws text at the specified y-coordinate, centered horizontally on the screen.
 */
void fossil_io_tui_draw_text_centered(int y, const char* text);

/**
 * Draws text at the specified x, y position, aligned to the right side of the screen.
 */
void fossil_io_tui_draw_text_right(int x, int y, const char* text);

/**
 * Draws text with word wrapping at the specified x, y position with a maximum width.
 */
void fossil_io_tui_draw_text_wrapped(int x, int y, int max_width, const char* text);

// Widget Components
/**
 * Draws a label at the specified position.
 */
void fossil_io_tui_draw_label(int x, int y, const char* text);

/**
 * Draws a button at the specified position with the given width and label.
 * The `focused` parameter indicates whether the button is currently focused (selected).
 */
void fossil_io_tui_draw_button(int x, int y, int w, const char* label, int focused);

/**
 * Draws a checkbox at the specified position with the given label.
 * The `checked` parameter indicates whether the checkbox is checked (selected).
 */
void fossil_io_tui_draw_checkbox(int x, int y, const char* label, int checked);

/**
 * Draws a progress bar at the specified position with the given width and percentage (0.0 to 1.0).
 */
void fossil_io_tui_draw_progress(int x, int y, int w, float percent);

// Input Handling
/**
 * Enables raw mode for the terminal, where input is immediately available without buffering.
 */
void fossil_io_tui_enable_raw_mode(void);

/**
 * Disables raw mode for the terminal, restoring standard input behavior.
 */
void fossil_io_tui_disable_raw_mode(void);

/**
 * Reads a single keypress from the user and returns the corresponding key code.
 */
int fossil_io_tui_read_key(void);

// Event Loop
/**
 * An event structure for handling keyboard input and widget focus.
 */
typedef struct {
    int key;               // The key pressed by the user
    int focused_widget;    // The index or identifier of the currently focused widget
} fossil_tui_event_t;

/**
 * A function pointer type for handling events in the TUI event loop.
 */
typedef void (*fossil_tui_event_handler_t)(fossil_tui_event_t);

/**
 * Runs the event loop, processing input events and passing them to the provided handler function.
 */
void fossil_io_tui_event_loop(fossil_tui_event_handler_t handler);

// Styling DSL (Mini Markup Language)
/**
 * Draws text at the specified position with inline markup for colors and text attributes.
 * Example: "Hello {red,bold}! Welcome to {blue}TUI World{reset}."
 */
void fossil_io_tui_draw_markup(int x, int y, const char* markup);

// DSL Parser Utilities
/**
 * Represents a token in the parsed markup language, containing the attributes and
 * the portion of text associated with them.
 */
typedef struct fossil_tui_markup_token {
    const char* start;  // Start of the token text
    int length;         // Length of the token
    uint8_t fg;         // Foreground color (from the color palette)
    uint8_t bg;         // Background color (from the color palette)
    uint8_t attr;       // Text attributes (e.g., bold, underline)
} fossil_tui_markup_token_t;

/**
 * Parses the given markup string and extracts the individual markup tokens (such as
 * colors and attributes).
 */
int fossil_io_tui_parse_markup(const char* markup, fossil_tui_markup_token_t* out_tokens, size_t max_tokens);

#ifdef __cplusplus
}

#include <string>
#include <vector>
#include <functional>

/**
 * Namespace for the Fossil Logic I/O library.
 */
namespace fossil {

    /**
     * Namespace for the I/O utilities.
     */
    namespace io {

        /**
         * A class that provides an object-oriented wrapper around the C TUI library.
         * It allows for easy drawing, event handling, text rendering, and more.
         */
        class TUI {
        public:
            /**
             * Constructor for the TUI class. Initializes the TUI system.
             */
            TUI() {
                fossil_io_tui_init();
            }
        
            /**
             * Destructor for the TUI class. Shuts down the TUI system.
             */
            ~TUI() {
                fossil_io_tui_shutdown();
            }
        
            /**
             * Clears the screen.
             */
            void clear() {
                fossil_io_tui_clear();
            }
        
            /**
             * Presents the current frame to the screen.
             */
            void present() {
                fossil_io_tui_present();
            }
        
            /**
             * Draws a single character at the specified position with specified colors and attributes.
             *
             * @param x The x-coordinate of the position to draw the character.
             * @param y The y-coordinate of the position to draw the character.
             * @param ch The character to draw.
             * @param fg The foreground color code.
             * @param bg The background color code.
             * @param attr The attribute code (e.g., bold, underline).
             */
            void draw(int x, int y, char ch, uint8_t fg, uint8_t bg, uint8_t attr) {
                fossil_io_tui_draw(x, y, ch, fg, bg, attr);
            }
        
            /**
             * Draws using a brush for repeated characters, colors, and attributes.
             *
             * @param x The x-coordinate of the position to start drawing.
             * @param y The y-coordinate of the position to start drawing.
             * @param brush The brush object containing the character, foreground, background, and attribute.
             */
            void draw_brush(int x, int y, const TUIBrush& brush) {
                fossil_io_tui_draw_brush(x, y, brush.to_c_brush());
            }
        
            /**
             * Draws text at a specific position on the screen.
             *
             * @param x The x-coordinate where the text should be drawn.
             * @param y The y-coordinate where the text should be drawn.
             * @param text The text to be displayed.
             */
            void draw_text(int x, int y, const std::string& text) {
                fossil_io_tui_draw_text(x, y, text.c_str());
            }
        
            /**
             * Draws centered text at a specific vertical position.
             *
             * @param y The y-coordinate for the centered text.
             * @param text The text to be centered and drawn.
             */
            void draw_text_centered(int y, const std::string& text) {
                fossil_io_tui_draw_text_centered(y, text.c_str());
            }
        
            /**
             * Draws text with wrapping at a specific position and within a specified width.
             *
             * @param x The x-coordinate where the text should start.
             * @param y The y-coordinate where the text should start.
             * @param max_width The maximum width allowed for the text before wrapping.
             * @param text The text to be wrapped and drawn.
             */
            void draw_text_wrapped(int x, int y, int max_width, const std::string& text) {
                fossil_io_tui_draw_text_wrapped(x, y, max_width, text.c_str());
            }
        
            /**
             * Draws a label at the specified position.
             *
             * @param x The x-coordinate for the label.
             * @param y The y-coordinate for the label.
             * @param text The label text to be drawn.
             */
            void draw_label(int x, int y, const std::string& text) {
                fossil_io_tui_draw_label(x, y, text.c_str());
            }
        
            /**
             * Draws a button at the specified position.
             *
             * @param x The x-coordinate for the button.
             * @param y The y-coordinate for the button.
             * @param w The width of the button.
             * @param label The label text on the button.
             * @param focused True if the button is focused, false otherwise.
             */
            void draw_button(int x, int y, int w, const std::string& label, bool focused) {
                fossil_io_tui_draw_button(x, y, w, label.c_str(), focused);
            }
        
            /**
             * Draws a checkbox at the specified position.
             *
             * @param x The x-coordinate for the checkbox.
             * @param y The y-coordinate for the checkbox.
             * @param label The label text next to the checkbox.
             * @param checked True if the checkbox is checked, false otherwise.
             */
            void draw_checkbox(int x, int y, const std::string& label, bool checked) {
                fossil_io_tui_draw_checkbox(x, y, label.c_str(), checked);
            }
        
            /**
             * Draws a progress bar at the specified position.
             *
             * @param x The x-coordinate for the progress bar.
             * @param y The y-coordinate for the progress bar.
             * @param w The width of the progress bar.
             * @param percent The percentage value of the progress (0.0 to 1.0).
             */
            void draw_progress(int x, int y, int w, float percent) {
                fossil_io_tui_draw_progress(x, y, w, percent);
            }
        
            /**
             * Enables raw mode, where input is read without waiting for the user to press Enter.
             */
            void enable_raw_mode() {
                fossil_io_tui_enable_raw_mode();
            }
        
            /**
             * Disables raw mode, where input is buffered until the user presses Enter.
             */
            void disable_raw_mode() {
                fossil_io_tui_disable_raw_mode();
            }
        
            /**
             * Reads a key from the input buffer.
             *
             * @return The key that was pressed.
             */
            int read_key() {
                return fossil_io_tui_read_key();
            }
        
            /**
             * Runs an event loop, calling a handler function for each event.
             *
             * @param handler The event handler function that processes each event.
             */
            void event_loop(std::function<void(const fossil_tui_event_t&)> handler) {
                auto event_handler = [](fossil_tui_event_t event) {
                    handler(event);
                };
                fossil_io_tui_event_loop(event_handler);
            }
        
            /**
             * Draws text with markup, interpreting special formatting codes.
             *
             * @param x The x-coordinate for the markup text.
             * @param y The y-coordinate for the markup text.
             * @param markup The markup string containing formatting codes.
             */
            void draw_markup(int x, int y, const std::string& markup) {
                fossil_io_tui_draw_markup(x, y, markup.c_str());
            }
        
            /**
             * Parses a DSL markup string into individual tokens.
             *
             * @param markup The markup string to be parsed.
             * @return A vector of parsed markup tokens.
             */
            std::vector<MarkupToken> parse_markup(const std::string& markup) {
                std::vector<MarkupToken> tokens;
                fossil_tui_markup_token_t raw_tokens[256];
                int num_tokens = fossil_io_tui_parse_markup(markup.c_str(), raw_tokens, 256);
        
                for (int i = 0; i < num_tokens; ++i) {
                    tokens.push_back(MarkupToken{
                        std::string(raw_tokens[i].start, raw_tokens[i].length),
                        raw_tokens[i].fg,
                        raw_tokens[i].bg,
                        raw_tokens[i].attr
                    });
                }
        
                return tokens;
            }
        
            /**
             * A helper struct for representing a brush used for repeated drawing with specific colors and attributes.
             */
            struct TUIBrush {
                char ch;   ///< Character to be drawn.
                uint8_t fg; ///< Foreground color code.
                uint8_t bg; ///< Background color code.
                uint8_t attr; ///< Attribute code (e.g., bold, underline).
        
                /**
                 * Converts the TUIBrush object into a C-compatible brush structure.
                 * 
                 * @return A fossil_tui_brush_t struct.
                 */
                fossil_tui_brush_t to_c_brush() const {
                    fossil_tui_brush_t brush = {ch, fg, bg, attr};
                    return brush;
                }
            };
        
            /**
             * Sets the viewport for drawing, limiting drawing to a specific area.
             *
             * @param x The x-coordinate of the top-left corner of the viewport.
             * @param y The y-coordinate of the top-left corner of the viewport.
             * @param width The width of the viewport.
             * @param height The height of the viewport.
             */
            void set_viewport(int x, int y, int width, int height) {
                fossil_tui_viewport_t vp = {x, y, width, height};
                fossil_io_tui_set_viewport(vp);
            }
        
            /**
             * Creates an offscreen framebuffer for drawing UI components off-screen.
             *
             * @param width The width of the framebuffer.
             * @param height The height of the framebuffer.
             * @return A TUIFramebuffer object for managing the offscreen drawing.
             */
            TUIFramebuffer create_framebuffer(int width, int height) {
                fossil_tui_framebuffer_t fb = fossil_io_tui_create_framebuffer(width, height);
                return TUIFramebuffer(fb);
            }
        
            /**
             * Destroys an offscreen framebuffer when it is no longer needed.
             *
             * @param framebuffer The framebuffer object to be destroyed.
             */
            void destroy_framebuffer(TUIFramebuffer& framebuffer) {
                fossil_io_tui_destroy_framebuffer(&framebuffer.get_c_frame());
            }
        
        private:
            /**
             * Internal class to manage offscreen framebuffers.
             */
            class TUIFramebuffer {
            public:
                /**
                 * Constructor that initializes the framebuffer.
                 *
                 * @param fb A C-compatible framebuffer structure.
                 */
                TUIFramebuffer(fossil_tui_framebuffer_t fb) : framebuffer(fb) {}
        
                /**
                 * Destructor that frees the framebuffer resources.
                 */
                ~TUIFramebuffer() {
                    fossil_io_tui_destroy_framebuffer(&framebuffer);
                }
        
                /**
                 * Draws the contents of the framebuffer to the screen.
                 */
                void draw() {
                    fossil_io_tui_draw_framebuffer(&framebuffer);
                }
        
                /**
                 * Returns the underlying C-compatible framebuffer structure.
                 *
                 * @return The C-compatible framebuffer structure.
                 */
                fossil_tui_framebuffer_t& get_c_frame() {
                    return framebuffer;
                }
        
            private:
                fossil_tui_framebuffer_t framebuffer; ///< The underlying C framebuffer.
            };
        };

    }

}

#endif

#endif /* FOSSIL_IO_FRAMEWORK_H */
