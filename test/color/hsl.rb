require 'test/unit'
require 'color'

class TestHSV < Test::Unit::TestCase
	MaxDistance = 0.005
	Delta       = 1e9

	def setup
		@a = Color::HSL.new(1.0, 1.0, 1.0)
		@b = Color::HSL.new(0.5, 0.8, 0.3, 128)
		@c = Color::HSL.new(0.5, 0.8, 0.3, 128)
		@d = Color::HSL.new(0.5, 0.8, 0.3)
	end
	
	def test_initialize
		assert(@a)
		assert(@b)
		assert(@c)
		assert_in_delta(1.0, @a.hue, Delta)
		assert_in_delta(1.0, @a.saturation, Delta)
		assert_in_delta(1.0, @a.luminance, Delta)
		assert_in_delta(0.5, @b.hue, Delta)
		assert_in_delta(0.8, @b.saturation, Delta)
		assert_in_delta(0.3, @b.luminance, Delta)
		assert_equal(@b, @c)
		assert_not_equal(@a, @b)
		assert_not_equal(@b, @d)
		assert_not_equal(@a, @d)
		
		assert_raise(ArgumentError) { Color::HSL.new(-0.1,0,0) }
		assert_raise(ArgumentError) { Color::HSL.new(0,-0.1,0) }
		assert_raise(ArgumentError) { Color::HSL.new(0,0,-0.1) }
		assert_raise(ArgumentError) { Color::HSL.new(1.1,0,0) }
		assert_raise(ArgumentError) { Color::HSL.new(0,1.1,0) }
		assert_raise(ArgumentError) { Color::HSL.new(0,0,1.1) }
	end
	
	def test_conversion
		a = Color::Gray.new(97)
		@a = Color::HSL.new(0.0, 0.0, 1.0) # with luminance 100%, neither saturation nor hue matter
		assert_in_delta(@a.to_rgb.to_hsl.distance(@a), 0, MaxDistance)
		assert_in_delta(@a.to_hsv.to_hsl.distance(@a), 0, MaxDistance)
		assert_in_delta(@a.to_hsl.to_hsl.distance(@a), 0, MaxDistance)
		assert_in_delta(@a.to_cmyk.to_hsl.distance(@a), 0, MaxDistance)
		assert_in_delta(@b.to_rgb.to_hsl.distance(@b), 0, MaxDistance)
		assert_in_delta(@b.to_hsv.to_hsl.distance(@b), 0, MaxDistance)
		assert_in_delta(@b.to_hsl.to_hsl.distance(@b), 0, MaxDistance)
		assert_in_delta(@b.to_cmyk.to_hsl.distance(@b), 0, MaxDistance)
		assert_equal(@d.to_gray, a)
	end
	
	def test_hashing
		h = { @a => 1, @b => 2, @d => 3, :x => 4 }
		assert_equal(1, h[@a])
		assert_equal(2, h[@b])
		assert_equal(3, h[@d])
		assert_equal(2, h[@c])
		#assert_equal(nil, h[e])
	end
	
	def test_marshalling
		assert_equal(Marshal.load(Marshal.dump(@a)), @a)
		assert_equal(Marshal.load(Marshal.dump(@b)), @b)
		assert_equal(Marshal.load(Marshal.dump(@c)), @c)
		assert_equal(Marshal.load(Marshal.dump(@d)), @d)
	end
end