#include <ruby.h>
#include <math.h>
#include "color.h"
#include "tools.h"
#include "rgb.h"
#include "hsv.h"
#include "hsl.h"
#include "cmyk.h"
#include "gray.h"

VALUE rb_mColor;
VALUE rb_cRGB;
VALUE rb_cHSV;
VALUE rb_cHSL;
VALUE rb_cCMYK;
VALUE rb_cGray;
VALUE rb_cXYZ;


/*
 *  call-seq:
 *     Color::native? -> true/false
 *
 *  Whether this is a native build or plain ruby.
 */
extern VALUE
rb_color__native(VALUE class)
{
	return Qtrue;
}

void
Init_ccolor()
{
	rb_mColor = rb_define_module("Color");
	rb_cRGB   = rb_define_class_under(rb_mColor, "RGB",  rb_cObject);
	rb_cHSV   = rb_define_class_under(rb_mColor, "HSV",  rb_cObject);
	rb_cHSL   = rb_define_class_under(rb_mColor, "HSL",  rb_cObject);
	rb_cXYZ   = rb_define_class_under(rb_mColor, "XYZ",  rb_cObject);
	rb_cCMYK  = rb_define_class_under(rb_mColor, "CMYK", rb_cObject);
	rb_cGray  = rb_define_class_under(rb_mColor, "Gray", rb_cObject);

	rb_define_singleton_method(rb_mColor, "native?", rb_color__native, 0);

	rb_define_alloc_func(rb_cRGB,  rb_color_rgb__allocate);
	rb_define_alloc_func(rb_cHSV,  rb_color_hsv__allocate);
	rb_define_alloc_func(rb_cHSL,  rb_color_hsl__allocate);
	rb_define_alloc_func(rb_cCMYK, rb_color_cmyk__allocate);
	rb_define_alloc_func(rb_cGray, rb_color_gray__allocate);

	rb_define_singleton_method(rb_cRGB, "from_int", rb_color_rgb__from_int, 1);

	rb_define_method(rb_cRGB, "initialize",      rb_color_rgb_initialize, -1);
	rb_define_method(rb_cRGB, "initialize_copy", rb_color_rgb_initialize_copy, 1);
	rb_define_method(rb_cRGB, "red",         rb_color_rgb_red,         0);
	rb_define_method(rb_cRGB, "green",       rb_color_rgb_green,       0);
	rb_define_method(rb_cRGB, "blue",        rb_color_rgb_blue,        0);
	rb_define_method(rb_cRGB, "alpha",       rb_color_rgb_alpha,       0);
	rb_define_method(rb_cRGB, "+",           rb_color_rgb_add,         1);
	rb_define_method(rb_cRGB, "-",           rb_color_rgb_sub,         1);
	rb_define_method(rb_cRGB, "closest",     rb_color_rgb_closest,     1);
	rb_define_method(rb_cRGB, "complement",  rb_color_rgb_complement,  0);
	rb_define_method(rb_cRGB, "distance",    rb_color_rgb_distance,    1);
	rb_define_method(rb_cRGB, "interpolate", rb_color_rgb_interpolate, 2);
	rb_define_method(rb_cRGB, "sequence",    rb_color_rgb_sequence,    2);
	rb_define_method(rb_cRGB, "hash",        rb_color_rgb_hash,        0);
	rb_define_method(rb_cRGB, "eql?",        rb_color_rgb_eql,         1);
	rb_define_alias(rb_cRGB, "==", "eql?");
	rb_define_method(rb_cRGB, "to_i",        rb_color_rgb_to_i,   -1);
	rb_define_method(rb_cRGB, "to_hsv",      rb_color_rgb_to_hsv,  0);
	rb_define_method(rb_cRGB, "to_hsl",      rb_color_rgb_to_hsl,  0);
	rb_define_method(rb_cRGB, "to_cmyk",     rb_color_rgb_to_cmyk, 0);
	rb_define_method(rb_cRGB, "to_gray",     rb_color_rgb_to_gray, 0);

	rb_define_method(rb_cHSV, "initialize",      rb_color_hsv_initialize, -1);
	rb_define_method(rb_cHSV, "initialize_copy", rb_color_hsv_initialize_copy, 1);
	rb_define_method(rb_cHSV, "hue",        rb_color_hsv_hue, 0);
	rb_define_method(rb_cHSV, "saturation", rb_color_hsv_saturation, 0);
	rb_define_method(rb_cHSV, "value",      rb_color_hsv_value, 0);
	rb_define_method(rb_cHSV, "alpha",      rb_color_hsv_alpha, 0);
	rb_define_method(rb_cHSV, "+",          rb_color_hsv_add, 1);
	rb_define_method(rb_cHSV, "-",          rb_color_hsv_sub, 1);
	rb_define_method(rb_cHSV, "complement", rb_color_hsv_complement, 0);
	rb_define_method(rb_cHSV, "distance",   rb_color_hsv_distance, 1);
	rb_define_method(rb_cHSV, "hash",       rb_color_hsv_hash, 0);
	rb_define_method(rb_cHSV, "eql?",       rb_color_hsv_eql, 1);
	rb_define_alias(rb_cHSV, "==", "eql?");
	rb_define_method(rb_cHSV, "to_rgb",     rb_color_hsv_to_rgb, 0);

	rb_define_method(rb_cHSL, "initialize",      rb_color_hsl_initialize, -1);
	rb_define_method(rb_cHSL, "initialize_copy", rb_color_hsl_initialize_copy, 1);
	rb_define_method(rb_cHSL, "hue",        rb_color_hsl_hue, 0);
	rb_define_method(rb_cHSL, "saturation", rb_color_hsl_saturation, 0);
	rb_define_method(rb_cHSL, "luminance",  rb_color_hsl_luminance, 0);
	rb_define_method(rb_cHSL, "alpha",      rb_color_hsl_alpha, 0);
	rb_define_method(rb_cHSL, "+",          rb_color_hsl_add, 1);
	rb_define_method(rb_cHSL, "-",          rb_color_hsl_sub, 1);
	rb_define_method(rb_cHSL, "complement", rb_color_hsl_complement, 0);
	rb_define_method(rb_cHSL, "distance",   rb_color_hsl_distance, 1);
	rb_define_method(rb_cHSL, "hash",       rb_color_hsl_hash, 0);
	rb_define_method(rb_cHSL, "eql?",       rb_color_hsl_eql, 1);
	rb_define_alias(rb_cHSL, "==", "eql?");
	rb_define_method(rb_cHSL, "to_rgb",     rb_color_hsl_to_rgb, 0);

	rb_define_method(rb_cCMYK, "initialize",      rb_color_cmyk_initialize, -1);
	rb_define_method(rb_cCMYK, "initialize_copy", rb_color_cmyk_initialize_copy, 1);
	rb_define_method(rb_cCMYK, "cyan",     rb_color_cmyk_cyan, 0);
	rb_define_method(rb_cCMYK, "magenta",  rb_color_cmyk_magenta, 0);
	rb_define_method(rb_cCMYK, "yellow",   rb_color_cmyk_yellow, 0);
	rb_define_method(rb_cCMYK, "key",      rb_color_cmyk_key, 0);
	rb_define_alias(rb_cCMYK, "black", "key");
	rb_define_method(rb_cCMYK, "alpha",    rb_color_cmyk_alpha, 0);
	rb_define_method(rb_cCMYK, "+",        rb_color_cmyk_add, 1);
	rb_define_method(rb_cCMYK, "-",        rb_color_cmyk_sub, 1);
	rb_define_method(rb_cCMYK, "distance", rb_color_cmyk_distance, 1);
	rb_define_method(rb_cCMYK, "hash",     rb_color_cmyk_hash, 0);
	rb_define_method(rb_cCMYK, "eql?",     rb_color_cmyk_eql, 1);
	rb_define_alias(rb_cCMYK, "==", "eql?");
	rb_define_method(rb_cCMYK, "to_rgb",   rb_color_cmyk_to_rgb, 0);
	rb_define_method(rb_cCMYK, "to_gray",  rb_color_cmyk_to_gray, 0);

	rb_define_method(rb_cGray, "initialize",      rb_color_gray_initialize, -1);
	rb_define_method(rb_cGray, "initialize_copy", rb_color_gray_initialize_copy, 1);
	rb_define_method(rb_cGray, "white",    rb_color_gray_white, 0);
	rb_define_alias(rb_cGray, "value", "white");
	rb_define_method(rb_cGray, "alpha",    rb_color_gray_alpha, 0);
	rb_define_method(rb_cGray, "+",        rb_color_gray_add, 1);
	rb_define_method(rb_cGray, "-",        rb_color_gray_sub, 1);
	rb_define_method(rb_cGray, "distance", rb_color_gray_distance, 1);
	rb_define_method(rb_cGray, "to_rgb",   rb_color_gray_to_rgb, 0);
	rb_define_method(rb_cGray, "to_cmyk",  rb_color_gray_to_cmyk, 0);
	rb_define_method(rb_cGray, "to_i",     rb_color_gray_to_i, 0);
	rb_define_method(rb_cGray, "eql?",     rb_color_gray_eql, 1);
	rb_define_alias(rb_cGray, "==", "eql?");
	rb_define_method(rb_cGray, "hash",     rb_color_gray_hash, 0);
}
