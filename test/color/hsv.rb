require 'test/unit'
require 'color'

class TestHSV < Test::Unit::TestCase
	MaxDistance = 0.005
	Delta       = 1e9

	def setup
		@a = Color::HSV.new(1.0, 1.0, 1.0)
		@b = Color::HSV.new(0.5, 0.8, 0.3, 128)
		@c = Color::HSV.new(0.5, 0.8, 0.3, 128)
		@d = Color::HSV.new(0.5, 0.8, 0.3)
	end
	
	def test_initialize
		assert(@a)
		assert(@b)
		assert(@c)
		assert_in_delta(1.0, @a.hue, Delta)
		assert_in_delta(1.0, @a.saturation, Delta)
		assert_in_delta(1.0, @a.value, Delta)
		assert_in_delta(0.5, @b.hue, Delta)
		assert_in_delta(0.8, @b.saturation, Delta)
		assert_in_delta(0.3, @b.value, Delta)
		assert_equal(@b, @c)
		assert_not_equal(@a, @b)
		assert_not_equal(@b, @d)
		assert_not_equal(@a, @d)
		
		assert_raise(ArgumentError) { Color::HSV.new(-0.1,0,0) }
		assert_raise(ArgumentError) { Color::HSV.new(0,-0.1,0) }
		assert_raise(ArgumentError) { Color::HSV.new(0,0,-0.1) }
		assert_raise(ArgumentError) { Color::HSV.new(1.1,0,0) }
		assert_raise(ArgumentError) { Color::HSV.new(0,1.1,0) }
		assert_raise(ArgumentError) { Color::HSV.new(0,0,1.1) }
	end
	
	def test_conversion
		a = Color::Gray.new(56)
		assert_in_delta(@a.to_rgb.to_hsv.distance(@a), 0, MaxDistance)
		assert_in_delta(@a.to_hsv.to_hsv.distance(@a), 0, MaxDistance)
		assert_in_delta(@a.to_hsl.to_hsv.distance(@a), 0, MaxDistance)
		assert_in_delta(@a.to_cmyk.to_hsv.distance(@a), 0, MaxDistance)
		assert_in_delta(@b.to_rgb.to_hsv.distance(@b), 0, MaxDistance)
		assert_in_delta(@b.to_hsv.to_hsv.distance(@b), 0, MaxDistance)
		assert_in_delta(@b.to_hsl.to_hsv.distance(@b), 0, MaxDistance)
		assert_in_delta(@b.to_cmyk.to_hsv.distance(@b), 0, MaxDistance)
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