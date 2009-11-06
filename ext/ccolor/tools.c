#include <ruby.h>
#include <math.h>
#include "color.h"
#include "tools.h"
#include "rgb.h"
#include "hsv.h"
#include "hsl.h"
#include "cmyk.h"
#include "gray.h"

int
min2(int x, int y)
{
	return x < y ? x : y;
}

int
max2(int x, int y)
{
	return x > y ? x : y;
}

int
min3(int x, int y, int z)
{
	return x < y ? (z < x ? z : x) : (z < y ? z : y);
}

int
max3(int x, int y, int z)
{
	return x > y ? (z > x ? z : x) : (z > y ? z : y);
}

extern int
float_hash(float num)
{
	char *c;
	unsigned int i;
	int hash;
	
	if (num == 0) num = fabs(num);
	c = (char*)&num;
	for (hash=0, i=0; i<sizeof(float);i++) {
		hash += c[i] * 971;
	}
	if (hash < 0) hash = -hash;
	return hash;
}

extern int
color_cap(int value, int min, int max)
{
	if (value < min) {
		return min;
	} else if (value > max) {
		return max;
	} else {
		return value;
	}
}

extern float
color_capf(float value, float min, float max)
{
	if (value < min) {
		return min;
	} else if (value > max) {
		return max;
	} else {
		return value;
	}
}

extern float
color_rgb_distance(cRGB *color1, cRGB *color2)
{
	return sqrtf((
		powf(CHR2FLOAT(color1->r) - CHR2FLOAT(color2->r), 2) +
		powf(CHR2FLOAT(color1->g) - CHR2FLOAT(color2->g), 2) +
		powf(CHR2FLOAT(color1->b) - CHR2FLOAT(color2->b), 2) +
		powf(CHR2FLOAT(color1->alpha) - CHR2FLOAT(color2->alpha), 2)
	)/4);
}

extern void
color_rgb_interpolate(cRGB *color1, cRGB *color2, cRGB *color3, float pos)
{
	double r1,g1,b1,a1, r2,g2,b2,a2;
	
	r1 = CHR2FLOAT(color1->r);
	g1 = CHR2FLOAT(color1->g);
	b1 = CHR2FLOAT(color1->b);
	a1 = CHR2FLOAT(color1->alpha);

	r2 = CHR2FLOAT(color2->r);
	g2 = CHR2FLOAT(color2->g);
	b2 = CHR2FLOAT(color2->b);
	a2 = CHR2FLOAT(color2->alpha);

	color3->r     = FLOAT2CHR(r1+(r2-r1)*pos);
	color3->g     = FLOAT2CHR(g1+(g2-g1)*pos);
	color3->b     = FLOAT2CHR(b1+(b2-b1)*pos);
	color3->alpha = FLOAT2CHR(a1+(a2-a1)*pos);
}

extern void
color_convert_rgb_to_hsv(cRGB *rgb, cHSV *hsv)
{
	float red, green, blue, min, max;
	red   = CHR2FLOAT(rgb->r);
	green = CHR2FLOAT(rgb->g);
	blue  = CHR2FLOAT(rgb->b);
	max   = fmaxf(fmaxf(red, green), blue);
	min   = fminf(fminf(red, green), blue);
	
	// hue
	if (IN_DELTA(max, min)) {
		hsv->h = 0.0;
	} else if (max == red) {
		hsv->h = 1.0/6.0*(green-blue)/(max-min)+(green < blue ? 1 : 0);
	} else if (max == green) {
		hsv->h = 1.0/6.0*(blue-red)/(max-min)+1.0/3.0;
	} else if (max == blue) {
		hsv->h = 1.0/6.0*(red-green)/(max-min)+2.0/3.0;
	}
	
	// saturation
	hsv->s = IN_DELTA(max, 0) ? 0 : (max-min)/max;
	
	// value
	hsv->v = max;
	hsv->alpha = rgb->alpha;
}

extern void
color_convert_rgb_to_hsl(cRGB *rgb, cHSL *hsl)
{
	float red, green, blue, min, max;
	red   = CHR2FLOAT(rgb->r);
	green = CHR2FLOAT(rgb->g);
	blue  = CHR2FLOAT(rgb->b);
	max   = fmaxf(fmaxf(red, green), blue);
	min   = fminf(fminf(red, green), blue);
	
	// hue
	if (IN_DELTA(max, min)) {
		hsl->h = 0.0;
	} else if (max == red) {
		hsl->h = 1.0/6.0*(green-blue)/(max-min)+(green < blue ? 1 : 0);
	} else if (max == green) {
		hsl->h = 1.0/6.0*(blue-red)/(max-min)+1.0/3.0;
	} else if (max == blue) {
		hsl->h = 1.0/6.0*(red-green)/(max-min)+2.0/3.0;
	}

	// luminance
	hsl->l = 0.5*(min+max);
	
	// saturation
	if (IN_DELTA(max, min)) {
		hsl->s = 0;
	} else if (hsl->l <= 0.5) {
		hsl->s = (max-min)/(max+min);
	} else {
		hsl->s = (max-min)/(2-(max+min));
	}
	
	// alpha
	hsl->alpha = rgb->alpha;
}

extern void
color_convert_hsv_to_rgb(cHSV *hsv, cRGB *rgb)
{
	rgb->alpha = hsv->alpha;
	if (IN_DELTA(hsv->s, 0)) {
		float value = FLOAT2CHR(hsv->v);
		rgb->r      = value;
		rgb->g      = value;
		rgb->b      = value;
	} else {
		float hi, f, p, q, t;
		hi = ((int)(hsv->h*6.0))%6;
		f  = (hsv->h*6.0) - hi;
		p  = hsv->v*(1.0 - hsv->s);
		q  = hsv->v*(1.0 - f*hsv->s);
		t  = hsv->v*(1.0 - (1.0-f)*hsv->s);
		//rb_warn("Hi: %.3f %d, f: %f, p: %f, q: %f, t: %f", hi, (int)hi, f, p, q, t);
		// conversion depends on what sector out of 6 hue is in
		switch(((int)hi)%6) {
			case 0:
				rgb->r = FLOAT2CHR(hsv->v);
				rgb->g = FLOAT2CHR(t);
				rgb->b = FLOAT2CHR(p);
				break;
			case 1:
				rgb->r = FLOAT2CHR(q);
				rgb->g = FLOAT2CHR(hsv->v);
				rgb->b = FLOAT2CHR(p);
				break;
			case 2:
				rgb->r = FLOAT2CHR(p);
				rgb->g = FLOAT2CHR(hsv->v);
				rgb->b = FLOAT2CHR(t);
				break;
			case 3:
				rgb->r = FLOAT2CHR(p);
				rgb->g = FLOAT2CHR(q);
				rgb->b = FLOAT2CHR(hsv->v);
				break;
			case 4:
				rgb->r = FLOAT2CHR(t);
				rgb->g = FLOAT2CHR(p);
				rgb->b = FLOAT2CHR(hsv->v);
				break;
			case 5:
				rgb->r = FLOAT2CHR(hsv->v);
				rgb->g = FLOAT2CHR(p);
				rgb->b = FLOAT2CHR(q);
				break;
			default:
				rb_warn("troubles, my friend: %.2f", hi);
		}
	}
}

extern float
color_hue_to_rgb(float co_var1, float co_var2, float hue)
{
	if (hue < 0) {
		hue++;
	} else if (hue > 1) {
		hue--;
	}
	if (6*hue < 1) {
		return ((co_var1-co_var2)*6*hue+co_var2);
	} else if (2*hue < 1) {
		return co_var1;
	} else if (3*hue < 2) {
		return (co_var2+(co_var1-co_var2)*(2.0/3-hue)*6);
	}
	return co_var2;
}

extern void
color_convert_hsl_to_rgb(cHSL *hsl, cRGB *rgb)
{
	rgb->alpha = hsl->alpha;
	if (IN_DELTA(hsl->s, 0)) {
		unsigned char luminance = FLOAT2CHR(hsl->l);
		rgb->r = luminance;
		rgb->g = luminance;
		rgb->b = luminance;
	} else {
		float co_var1, co_var2;
		co_var1 = hsl->l < 0.5 ? (hsl->l * (1.0 + hsl->s)) : (hsl->l + hsl->s - hsl->l * hsl->s);
		co_var2 = 2*hsl->l - co_var1;
		rgb->r = FLOAT2CHR(color_hue_to_rgb(co_var1, co_var2, (hsl->h + 1.0/3.0)));
		rgb->g = FLOAT2CHR(color_hue_to_rgb(co_var1, co_var2, hsl->h));
		rgb->b = FLOAT2CHR(color_hue_to_rgb(co_var1, co_var2, (hsl->h - 1.0/3.0)));
	}
}

// "most-black, least color" conversion algorithm
extern void
color_convert_rgb_to_cmyk(cRGB *rgb, cCMYK *cmyk)
{
	cmyk->alpha = rgb->alpha;
	if (rgb->r == rgb->g && rgb->g == rgb->b) {
		cmyk->c = 0;
		cmyk->m = 0;
		cmyk->y = 0;
		cmyk->k = 255 - rgb->r;
	} else {
		cmyk->c = 255 - rgb->r;
		cmyk->m = 255 - rgb->g;
		cmyk->y = 255 - rgb->b;
		cmyk->k = min3(cmyk->c, cmyk->m, cmyk->y);
		if (cmyk->k == 255) {
			cmyk->c = 0;
			cmyk->m = 0;
			cmyk->y = 0;
		} else {
			unsigned char div = 255 - cmyk->k;
			cmyk->c = (unsigned char)(roundf(((float)(cmyk->c - cmyk->k) / (float)div) * 255));
			cmyk->m = (unsigned char)(roundf(((float)(cmyk->m - cmyk->k) / (float)div) * 255));
			cmyk->y = (unsigned char)(roundf(((float)(cmyk->y - cmyk->k) / (float)div) * 255));
		}
	}
}

// could probably be improved
extern unsigned char
color_cmyk_component_to_rgb(unsigned char c, unsigned char k)
{
	// 1-(@cyan*(1-@key)+@key)
	float cf = CHR2FLOAT(c);
	float kf = CHR2FLOAT(k);
	float rf = 1 - (cf * (1 - kf) + kf);
	return FLOAT2CHR(color_capf(rf, 0, 1));
}

extern void
color_convert_cmyk_to_rgb(cCMYK *cmyk, cRGB *rgb)
{
	rgb->r     = color_cmyk_component_to_rgb(cmyk->c, cmyk->k);
	rgb->g     = color_cmyk_component_to_rgb(cmyk->m, cmyk->k);
	rgb->b     = color_cmyk_component_to_rgb(cmyk->y, cmyk->k);
	rgb->alpha = cmyk->alpha;
}

extern void
color_convert_rgb_to_gray(cRGB *rgb, cGray *gray)
{
	float value = roundf(((float)(rgb->r + rgb->g + rgb->b))/3);
	gray->white = (unsigned char)(color_capf(value,0,255));
	gray->alpha = rgb->alpha;
}

extern void
color_convert_cmyk_to_gray(cCMYK *cmyk, cGray *gray)
{
	float black = roundf(((float)((cmyk->c + cmyk->m + cmyk->y)/3 + cmyk->k)));
	gray->white = 255 - (unsigned char)(color_capf(black,0,255));
	gray->alpha = cmyk->alpha;
}

extern void
color_convert_gray_to_rgb(cGray *gray, cRGB *rgb)
{
	rgb->r     = gray->white;
	rgb->g     = gray->white;
	rgb->b     = gray->white;
	rgb->alpha = gray->alpha;
}

extern void
color_convert_gray_to_cmyk(cGray *gray, cCMYK *cmyk)
{
	cmyk->c     = 0;
	cmyk->m     = 0;
	cmyk->y     = 0;
	cmyk->k     = 255 - gray->white;
	cmyk->alpha = gray->alpha;
}
