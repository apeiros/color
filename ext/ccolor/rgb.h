extern VALUE rb_color_rgb__allocate(VALUE class);
extern VALUE rb_color_rgb__from_int(VALUE class, VALUE integer);
extern VALUE rb_color_rgb_initialize(int argc, VALUE *argv, VALUE self);
extern VALUE rb_color_rgb_initialize_copy(VALUE self, VALUE original);
extern VALUE rb_color_rgb_red(VALUE self);
extern VALUE rb_color_rgb_green(VALUE self);
extern VALUE rb_color_rgb_blue(VALUE self);
extern VALUE rb_color_rgb_alpha(VALUE self);
extern VALUE rb_color_rgb_add(VALUE self, VALUE other);
extern VALUE rb_color_rgb_sub(VALUE self, VALUE other);
extern VALUE rb_color_rgb_to_i(int argc, VALUE *argv, VALUE self);
extern VALUE rb_color_rgb_to_cmyk(VALUE self);
extern VALUE rb_color_rgb_to_gray(VALUE self);
extern VALUE rb_color_rgb_to_hsv(VALUE self);
extern VALUE rb_color_rgb_to_hsl(VALUE self);
extern VALUE rb_color_rgb_eql(VALUE self, VALUE other);
extern VALUE rb_color_rgb_hash(VALUE self);
extern VALUE rb_color_rgb_closest(VALUE self, VALUE r_ary_out_of);
extern VALUE rb_color_rgb_distance(VALUE self, VALUE other);
extern VALUE rb_color_rgb_complement(VALUE self);
extern VALUE rb_color_rgb_sequence(VALUE self, VALUE r_to, VALUE r_steps);
extern VALUE rb_color_rgb_interpolate(VALUE self, VALUE r_other, VALUE r_pos);
