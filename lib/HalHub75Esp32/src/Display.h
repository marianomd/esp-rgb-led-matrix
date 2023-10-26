/* MIT License
 *
 * Copyright (c) 2019 - 2023 Andreas Merkle <web@blue-andi.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*******************************************************************************
    DESCRIPTION
*******************************************************************************/
/**
 * @brief  HUB75 matrix display
 * @author Mariano Dupont <marianomd@gmail.com>
 *
 * @addtogroup gfx
 *
 * @{
 */

#ifndef DISPLAY_H
#define DISPLAY_H

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

#include <YAGfxBitmap.h>
#include <stdint.h>

#include <ColorDef.hpp>
#include <IDisplay.hpp>

#include "Board.h"
/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/**
 * This display represents a HUB75 matrix.
 */
class Display : public IDisplay {
   public:
    /**
     * Get display instance.
     *
     * @return Display
     */
    static Display& getInstance() {
        static Display instance; /* singleton idiom to force initialization in the first usage. */

        return instance;
    }

    /**
     * Initialize base driver for the display.
     *
     * @return If successful, returns true otherwise false.
     */
    bool begin() final {
        return panel.begin();
    }

    /**
     * Show framebuffer on physical display. This may be synchronous
     * or asynchronous.
     */
    void show() final {
    }

    /**
     * The display is ready, when the last physical pixel update is finished.
     * A asynchronous display update, triggered by show() can be observed this way.
     *
     * @return If ready for another update via show(), it will return true otherwise false.
     */
    bool isReady() const final {
        return true;
    }

    /**
     * Set brightness from 0 to 255.
     *
     * @param[in] brightness    Brightness value [0; 255]
     */
    void setBrightness(uint8_t brightness) final {
        panel.setBrightness(brightness);
    }

    /**
     * Clear display.
     */
    void clear() final {
        panel.fillScreen(ColorDef::BLACK);
        m_ledMatrix.fillScreen(ColorDef::BLACK);
    }

    /**
     * Get width in pixel.
     *
     * @return Canvas width in pixel
     */
    uint16_t getWidth() const final
    {
        return m_ledMatrix.getWidth();
    }

    /**
     * Get height in pixel.
     *
     * @return Canvas height in pixel
     */
    uint16_t getHeight() const final
    {
        return m_ledMatrix.getHeight();
    }

    /**
     * Get pixel color at given position.
     *
     * @param[in] x x-coordinate
     * @param[in] y y-coordinate
     *
     * @return Color in RGB888 format.
     */
    Color& getColor(int16_t x, int16_t y) final {
        return m_ledMatrix.getColor(x, y);
    }

    /**
     * Get pixel color at given position.
     *
     * @param[in] x x-coordinate
     * @param[in] y y-coordinate
     *
     * @return Color in RGB888 format.
     */
    const Color& getColor(int16_t x, int16_t y) const final {
        return m_ledMatrix.getColor(x, y);
    }

   private:
    HUB75_I2S_CFG::i2s_pins _pins = {R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN};
    HUB75_I2S_CFG mxconfig{
        CONFIG_LED_MATRIX_WIDTH,   // Module width
        CONFIG_LED_MATRIX_HEIGHT,  // Module height
        1,                         // chain length
        _pins                      // pin mapping
    };

    /**
     * Pixel representation of the LED matrix. Gamma correction disabled.
     */
    MatrixPanel_I2S_DMA panel;

    /**
     * The LED matrix framebuffer.
     * This is the drawback for the direct color manipulation via getColor().
     */
    YAGfxStaticBitmap<Board::LedMatrix::width, Board::LedMatrix::height> m_ledMatrix;

    /**
     * Construct display.
     */
    Display();

    /**
     * Destroys display.
     */
    ~Display();

    Display(const Display& display);
    Display& operator=(const Display& display);

    /**
     * Draw a single pixel on the display.
     *
     * @param[in] x     x-coordinate
     * @param[in] y     y-coordinate
     * @param[in] color Pixel color in RGB888 format
     */
    void drawPixel(int16_t x, int16_t y, const Color& color) final {
        panel.drawPixelRGB888(x, y, color.getRed(), color.getGreen(), color.getBlue());
        m_ledMatrix.drawPixel(x, y, color);
    }
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* DISPLAY_H */

/** @} */