/* source/include/sgl_math.h
 *
 * MIT License
 *
 * Copyright(c) 2023-present All contributors of SGL  
 * Li, Shanwen  (1477153217@qq.com)
 * Document reference link: docs directory
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __SGL_MATH_H__
#define __SGL_MATH_H__


#include <sgl_core.h>

#ifdef __cplusplus
extern "C" {
#endif


#define sgl_min(a, b)                     ((a) < (b) ? (a) : (b))
#define sgl_min3(a, b, c)                 (sgl_min(sgl_min((a), (b)), (c)))
#define sgl_min4(a, b, c, d)              (sgl_min(sgl_min((a), (b)), sgl_min((c), (d))))


#define sgl_max(a, b)                     ((a) > (b) ? (a) : (b))
#define sgl_max3(a, b, c)                 (sgl_max(sgl_max((a), (b)), (c)))
#define sgl_max4(a, b, c, d)              (sgl_max(sgl_max((a), (b)), sgl_max((c), (d))))

#define sgl_clamp(min, val, max)          (sgl_max(min, (sgl_min((val), (max)))))

#define sgl_abs(x)                        ((x) > 0 ? (x) : (- (x)))

#define sgl_pow2(x)                       ((x) * (x))
#define sgl_pow3(x)                       ((x) * (x) * (x))
#define sgl_pow4(x)                       ((x) * (x) * (x) * (x))

#define sgl_is_pow2(x)                    (((x) & ((x) - 1)) == 0)


/**
 * @brief Calculate the sine of an angle
 * @param angle: Angle in degrees such 0-359
 * @return sine of the angle from sin0_90_table
 */
int32_t sgl_sin(int16_t angle);


/**
 * @brief Calculate the cos of an angle
 * @param angle: Angle in degrees such 0-359
 * @return cos of the angle from sin0_90_table
 */
static inline int32_t sgl_cos(int16_t angle)
{
    return sgl_sin(angle + 90);
}


/**
 * @brief  Calculate x number square root
 * @param  x: x number
 * @retval x number square root
 */
uint16_t sgl_sqrt(uint32_t x);


/**
 * @brief Calculate error of the square root for a number.
 * @param x The number to calculate the error for.
 * @return The error of the square root. [0 ~ 255]
 */
uint8_t sgl_sqrt_error(uint32_t x);


/**
 * @brief Calculate the angle based on the x and y coordinates. This function is a fast algorithm
 *         implementation, with reference address: www.RomanBlack.com (Fast XY vector to integer degree algorithm)
 *
 * @param x:  The x coordinate on a circle
 * @param y:  The y coordinate on a circle
 * @return angle 
 * @note return angle [-32767 ~ 32767]
 */
int32_t sgl_atan2_raw(int x, int y);


/**
 * @brief Calculate the angle based on the x and y coordinates. This function is a fast algorithm 
 *        implementation, with reference address: www.RomanBlack.com (Fast XY vector to integer degree algorithm)
 * 
 * @param x:  The x coordinate on a circle
 * @param y:  The y coordinate on a circle
 * @return angle
 * @note return angle [0 ~ 359]
*/
uint16_t sgl_atan2_angle(int x, int y);


/**
 * @brief Calculate whether the x and y coordinates have a component.
 * @param x:  The x coordinate on a circle
 * @param y:  The y coordinate on a circle
 * @param xv: The x coordinate of the vector
 * @param yv: The y coordinate of the vector
 * @return 1: The x and y coordinates have a component. 0: The x and y coordinates do not have a component.
 */
static inline int sgl_xy_has_component( int x, int y, int xv, int yv)
{
    return ((x != 0 && ((xv ^ x) < 0)) || (y != 0 && ((yv ^ y) < 0)));
}


#ifdef __cplusplus
}
#endif


#endif // !__SGL_MATH_H__
