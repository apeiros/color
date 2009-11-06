#include <ruby.h>
#include <math.h>
#include "color.h"
#include "tools.h"
#include "rgb.h"
#include "hsv.h"
#include "hsl.h"
#include "cmyk.h"
#include "gray.h"

/* 
 *  :nodoc:
 */
extern VALUE
rb_color_rgb__allocate(VALUE class)
{
	cRGB *color;
	VALUE rb_color = Data_Make_Struct(class, cRGB, NULL, free, color);
	color->r     = 0;
	color->g     = 0;
	color->b     = 0;
	color->alpha = 0;
	return rb_color;
}

/* 
 *  :nodoc:
 */
/*
extern VALUE
rb_color_rgb__from_html(VALUE class, VALUE string)
{
	cRGB *color;
	VALUE rb_color = Data_Make_Struct(class, cRGB, NULL, free, color);
	u_long k  = 0;
	u_int  v  = 0;
	char *s = RubyStringValue(string)
	for(i=RSTRING(string)->len; i > 0; i--) {
		v = s[i] > 57 ? s[i] - 87 : s[i] - 48
		k = (k << 4) + v;
	}
	color->alpha   = (k >> 24) & 0xff;
	color->r       = (k >> 16) & 0xff;
	color->g       = (k >>  8) & 0xff;
	color->b       = (k >>  0) & 0xff;
	return rb_color;
}
*/

/*
 *  call-seq:
 *     Color::RGB.from_int(integer) -> RGB instance
 *
 *  Create a color from an integer of the form 0xaarrggbb.
 *  See Color::RGB#to_i.
 */
extern VALUE
rb_color_rgb__from_int(VALUE class, VALUE integer)
{
	cRGB *color;
	VALUE rb_color  = Data_Make_Struct(class, cRGB, NULL, free, color);
	unsigned long i = NUM2ULONG(integer);
	color->alpha    = (i >> 24) & 0xff;
	color->r        = (i >> 16) & 0xff;
	color->g        = (i >>  8) & 0xff;
	color->b        = (i >>  0) & 0xff;
	return rb_color;
}

/*
 *  call-seq:
 *     Color::RGB.new(red, green, blue[, alpha])
 *
 *  Create a new RGB instance. Red, green, blue and alpha are
 *  Integers within 0 and 255 each, where for alpha 0 means
 *  opaque and 255 fully transparent.
 */
extern VALUE
rb_color_rgb_initialize(int argc, VALUE *argv, VALUE self)
{
	cRGB *color;
	Data_Get_Struct(self, cRGB, color);
	VALUE red, green, blue, alpha;
	rb_scan_args(argc, argv, "31", &red, &green, &blue, &alpha);

	int   r,g,b,a;
	r = (NUM2INT(red));
	g = (NUM2INT(green));
	b = (NUM2INT(blue));
	a = (NIL_P(alpha) ? 0 : NUM2INT(alpha));
	
	if (0 > r || r > 255) {
		rb_raise(rb_eArgError, "Invalid value for red, must be between 0 and 255");
	}
	if (0 > g || g > 255) {
		rb_raise(rb_eArgError, "Invalid value for green, must be between 0 and 255");
	}
	if (0 > b || b > 255) {
		rb_raise(rb_eArgError, "Invalid value for blue, must be between 0 and 255");
	}
	if (0 > a || a > 255) {
		rb_raise(rb_eArgError, "Invalid value for alpha, must be between 0 and 255");
	}

	color->r     = r;
	color->g     = g;
	color->b     = b;
	color->alpha = a;

	return self;
}

/*
 * :nodoc:
 */
extern VALUE
rb_color_rgb_initialize_copy(VALUE self, VALUE original)
{
	cRGB *color1, *color2;
	Data_Get_Struct(self, cRGB, color1);
	Data_Get_Struct(original, cRGB, color2);
	*color1 = *color2;
	return self;
}

/*
 *  call-seq:
 *     rgb.red -> fixnum
 *
 *  The red portion of this color. A value between 0 and 255.
 */
extern VALUE
rb_color_rgb_red(VALUE self)
{
	cRGB *color;
	Data_Get_Struct(self, cRGB, color);
	return CHR2FIX(color->r);
}

/*
 *  call-seq:
 *     rgb.green -> fixnum
 *
 *  The green portion of this color. A value between 0 and 255.
 */
extern VALUE
rb_color_rgb_green(VALUE self)
{
	cRGB *color;
	Data_Get_Struct(self, cRGB, color);
	return CHR2FIX(color->g);
}

/*
 *  call-seq:
 *     rgb.blue -> fixnum
 *
 *  The blue portion of this color. A value between 0 and 255.
 */
extern VALUE
rb_color_rgb_blue(VALUE self)
{
	cRGB *color;
	Data_Get_Struct(self, cRGB, color);
	return CHR2FIX(color->b);
}

/*
 *  call-seq:
 *     rgb.alpha -> fixnum
 *
 *  The transparency of this color. A value between 0 and 255, where
 *  0 means opaque and 255 fully transparent.
 */
extern VALUE
rb_color_rgb_alpha(VALUE self)
{
	cRGB *color;
	Data_Get_Struct(self, cRGB, color);
	return CHR2FIX(color->alpha);
}

/*
 *  call-seq:
 *     rgb1 + rgb2 -> rgb
 *
 *  Component wise sum.
 */
extern VALUE
rb_color_rgb_add(VALUE self, VALUE other)
{
	cRGB *color1, *color2, *color3;
	Data_Get_Struct(self, cRGB, color1);
	Data_Get_Struct(other, cRGB, color2);
	VALUE rb_color = Data_Make_Struct(rb_cRGB, cRGB, NULL, free, color3);
	color3->r     = color_cap(CHR2LONG(color1->r) + CHR2LONG(color2->r), 0, 255);
	color3->g     = color_cap(CHR2LONG(color1->g) + CHR2LONG(color2->g), 0, 255);
	color3->b     = color_cap(CHR2LONG(color1->b) + CHR2LONG(color2->b), 0, 255);
	color3->alpha = color_cap(CHR2LONG(color1->alpha) + CHR2LONG(color2->alpha), 0, 255);
	return rb_color;
}

/*
 *  call-seq:
 *     rgb1 - rgb2 -> rgb
 *
 *  Component wise difference.
 */
extern VALUE
rb_color_rgb_sub(VALUE self, VALUE other)
{
	cRGB *color1, *color2, *color3;
	Data_Get_Struct(self, cRGB, color1);
	Data_Get_Struct(other, cRGB, color2);
	VALUE rb_color = Data_Make_Struct(rb_cRGB, cRGB, NULL, free, color3);
	color3->r     = color_cap(CHR2LONG(color1->r) - CHR2LONG(color2->r), 0, 255);
	color3->g     = color_cap(CHR2LONG(color1->g) - CHR2LONG(color2->g), 0, 255);
	color3->b     = color_cap(CHR2LONG(color1->b) - CHR2LONG(color2->b), 0, 255);
	color3->alpha = color_cap(CHR2LONG(color1->alpha) - CHR2LONG(color2->alpha), 0, 255);
	return rb_color;
}

/*
 *  call-seq:
 *     rgb.sequence(to, steps) -> array_of_rgb
 *
 *  FIXME doesn't raise ArgumentError (steps >= 1)
 */
extern VALUE
rb_color_rgb_sequence(VALUE self, VALUE r_to, VALUE r_steps)
{
	cRGB *start, *end, *step;
	int steps    = FIX2INT(r_steps);
	double delta = 1.0/steps;

	Data_Get_Struct(self, cRGB, start);
	Data_Get_Struct(r_to, cRGB, end);
	
	VALUE rb_array = rb_ary_new2(steps+1);
	rb_ary_push(rb_array, self);
	for (int i = 1; i < steps; i++) {
		VALUE rb_color = Data_Make_Struct(rb_cRGB, cRGB, NULL, free, step);
		color_rgb_interpolate(start, end, step, i*delta);
		rb_ary_push(rb_array, rb_color);
	}
	rb_ary_push(rb_array, r_to);
	
	return rb_array;
}

/*
 *  call-seq:
 *     rgb.closest(ary_out_of) -> closest_color
 *  
 *  See Color::Common#closest
 */
extern VALUE
rb_color_rgb_closest(VALUE self, VALUE r_ary_out_of)
{
	cRGB *color, *compare;
	VALUE closest, other_rgb, other;
	float value;
	ID rb_coerce;
	rb_coerce = rb_intern("coerce");
	closest   = rb_ary_entry(r_ary_out_of, 0);
	other_rgb = closest;
	if (CLASS_OF(other_rgb) != rb_cRGB) {
		other_rgb = rb_funcall(self, rb_coerce, 1, other_rgb);
	}
	Data_Get_Struct(self, cRGB, color);
	Data_Get_Struct(other_rgb, cRGB, compare);
	value     = color_rgb_distance(color, compare);
	
	for (long i = 1; i < RARRAY(r_ary_out_of)->len; i++) {
		other     = rb_ary_entry(r_ary_out_of, i);
		if (CLASS_OF(other) != rb_cRGB) {
			other_rgb = rb_funcall(self, rb_coerce, 1, other);
		} else {
			other_rgb = other;
		}
		Data_Get_Struct(other_rgb, cRGB, compare);
		float tmp = color_rgb_distance(color, compare);
		if (tmp < value) {
			value   = tmp;
			closest = other;
		}
	}

	return closest;
}

/*
 *  call-seq:
 *     rgb.interpolate(to, steps) -> new_rgb
 *
 *  FIXME doesn't raise ArgumentError (pos 0..1)
 */
extern VALUE
rb_color_rgb_interpolate(VALUE self, VALUE r_other, VALUE r_pos)
{
	double pos = NUM2DBL(r_pos);
	
	cRGB *color1, *color2, *color3;
	Data_Get_Struct(self, cRGB, color1);
	Data_Get_Struct(r_other, cRGB, color2);
	VALUE rb_color = Data_Make_Struct(rb_cRGB, cRGB, NULL, free, color3);
	
	color_rgb_interpolate(color1, color2, color3, pos);
	
	return rb_color;
}

/*
 *  call-seq:
 *     rgb.distance(other) -> float
 *
 *  Returns the distance to another color of the same class.
 *  Distance is a Float between 0 and 1, where 1 is the maximum
 *  distance. Be aware that this is purely mathematical and human
 *  perception may differ.
 */
extern VALUE
rb_color_rgb_distance(VALUE self, VALUE other)
{
	cRGB *color1, *color2;
	Data_Get_Struct(self, cRGB, color1);
	Data_Get_Struct(other, cRGB, color2);
	return rb_float_new(color_rgb_distance(color1, color2));
}

/*
 *  call-seq:
 *     rgb.complement -> rgb
 *
 *  Returns the complementary rgb color.
 */
extern VALUE
rb_color_rgb_complement(VALUE self)
{
	cRGB *color1, *color2;
	cHSV hsv;
	Data_Get_Struct(self, cRGB, color1);
	VALUE rb_color = Data_Make_Struct(rb_cRGB, cRGB, NULL, free, color2);
	color_convert_rgb_to_hsv(color1, &hsv);
	hsv.h = fmodf(hsv.h+0.5, 1);
	color_convert_hsv_to_rgb(&hsv, color2);
	return rb_color;
}

/*
 *  call-seq:
 *     rgb.to_i -> integer
 *
 *  An integer representation of the RGB value in the form of
 *  0xaarrggbb. The alpha value is optional.
 *  Also see RGB::from_int
 */
extern VALUE
rb_color_rgb_to_i(int argc, VALUE *argv, VALUE self)
{
	cRGB *color;
	Data_Get_Struct(self, cRGB, color);
	VALUE alpha;
	rb_scan_args(argc, argv, "01", &alpha);

	if RTEST(alpha) {
		return UINT2NUM(
			(CHR2LONG(color->alpha) << 24) |
			(CHR2LONG(color->r) << 16) |
			(CHR2LONG(color->g) << 8) |
			(CHR2LONG(color->b))
		);
	} else {
		return UINT2NUM(
			(CHR2LONG(color->r) << 16) |
			(CHR2LONG(color->g) << 8) |
			(CHR2LONG(color->b))
		);
	}
}

/*
 *  call-seq:
 *     rgb.to_hsv -> hsv
 *
 *  Returns a HSV representation of this color.
 */
extern VALUE
rb_color_rgb_to_hsv(VALUE self)
{
	cRGB *rgb;
	cHSV *hsv;
	Data_Get_Struct(self, cRGB, rgb);
	VALUE rb_color = Data_Make_Struct(rb_cHSV, cHSV, NULL, free, hsv);
	color_convert_rgb_to_hsv(rgb, hsv);
	return rb_color;
}

/*
 *  call-seq:
 *     rgb.to_hsl -> hsl
 *
 *  Returns a HSL representation of this color.
 */
extern VALUE
rb_color_rgb_to_hsl(VALUE self)
{
	cRGB *rgb;
	cHSL *hsl;
	Data_Get_Struct(self, cRGB, rgb);
	VALUE rb_color = Data_Make_Struct(rb_cHSL, cHSL, NULL, free, hsl);
	color_convert_rgb_to_hsl(rgb, hsl);
	return rb_color;
}

/*
 *  call-seq:
 *     rgb.to_gray -> gray
 *
 *  Returns a Gray representation of this color.
 */
extern VALUE
rb_color_rgb_to_gray(VALUE self)
{
	cRGB *rgb;
	cGray *gray;
	Data_Get_Struct(self, cRGB, rgb);
	VALUE rb_color = Data_Make_Struct(rb_cGray, cGray, NULL, free, gray);
	color_convert_rgb_to_gray(rgb, gray);
	return rb_color;
}

/*
 *  call-seq:
 *     rgb.to_cmyk -> cmyk
 *
 *  Returns a CMYK representation of this color.
 */
extern VALUE
rb_color_rgb_to_cmyk(VALUE self)
{
	cRGB *rgb;
	cCMYK *cmyk;
	Data_Get_Struct(self, cRGB, rgb);
	VALUE rb_color = Data_Make_Struct(rb_cCMYK, cCMYK, NULL, free, cmyk);
	color_convert_rgb_to_cmyk(rgb, cmyk);
	return rb_color;
}

/*
 *  call-seq:
 *     rgb.eql?(other) -> true/false
 *
 *  Compares two RGB color instances for equality. Two RGB colors are eql? if
 *  their red, green, blue and alpha values are equal.
 */
extern VALUE
rb_color_rgb_eql(VALUE self, VALUE other)
{
	if (CLASS_OF(self) != CLASS_OF(other)) {
		return Qfalse;
	}
	cRGB *color1, *color2;
	Data_Get_Struct(self, cRGB, color1);
	Data_Get_Struct(other, cRGB, color2);
	return (
		color1->r     == color2->r &&
		color1->g     == color2->g &&
		color1->b     == color2->b &&
		color1->alpha == color2->alpha
	) ? Qtrue : Qfalse;
}

/*
 *  call-seq:
 *     rgb.hash   -> fixnum
 *
 *  Compute a hash-code for this color. Two colors with the same components
 *  will have the same hash code (and will compare using <code>eql?</code>).
 */
extern VALUE
rb_color_rgb_hash(VALUE self)
{
	cRGB *color;
	Data_Get_Struct(self, cRGB, color);

	return INT2FIX(
		(8) ^
		(CHR2LONG(color->alpha) << 28) ^
		(CHR2LONG(color->r) << 21) ^
		(CHR2LONG(color->g) << 14) ^
		(CHR2LONG(color->b << 7))
	);
}
