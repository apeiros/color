require 'color'

module Color # :nodoc:

	# == Description
	# this class is intended to make mixing colors simple
	# i.e. you can alter single attributes of a color, even
	# attributes that belong to another model (Mixer will convert
	# forth and back for the operation).
	# Example:
	#   red = Color.rgb(255,0,0)
	#   mix = red.to_mixer
	#   mix.hue += 1.0/3
	#   mix.color # => RGB: 0, 255, 0, 0
	#   mix.alpha += 128
	#   mix.color # => RGB: 0, 255, 0, 128
	#
	class Mixer
		class <<self
			alias from new
		end

		# The current state of the color - will always be of the same class
		# as the Mixer was generated from
		attr_reader :color

		# name will change - class of Color::Mixer#color
		attr_reader :klass
		def initialize(color)
			@color = color
			@klass = color.class
		end
		
		def complementary
			self.hue += 0.5
			self
		end
		
		def alpha=(value)
			@color = @color.with_alpha(value)
		end
		
		def red=(value)
			rgb    = @color.to_rgb
			@color = @klass.from(rgb.with(value, rgb.green, rgb.blue))
		end

		def green=(value)
			rgb    = @color.to_rgb
			@color = @klass.from(rgb.with(rgb.red, value, rgb.blue))
		end

		def blue=(value)
			rgb    = @color.to_rgb
			@color = @klass.from(rgb.with(rgb.red, rgb.green, value))
		end
		
		# unlike others, this one is not capped, but cyclic,
		# e.g. c.hue # => 0.8; c.hue += 0.4; c.hue # => 0.2
		def hue=(value)
			hsv    = @color.to_hsv
			@color = @klass.from(hsv.with(value%1, hsv.saturation, hsv.value))
		end

		def saturation=(value)
			hsv    = @color.to_hsv
			@color = @klass.from(hsv.with(hsv.hue, value, hsv.value))
		end
		
		def value=(value)
			hsv    = @color.to_hsv
			@color = @klass.from(hsv.with(hsv.hue, hsv.saturation, value))
		end
		
		def luminance=(value)
			hsl    = @color.to_hsl
			@color = @klass.from(hsl.with(hsl.hue, hsl.saturation, value))
		end
		
		def alpha
			@color.alpha
		end
		
		def red
			@color.to_rgb.red
		end
		
		def green
			@color.to_rgb.green
		end
		
		def blue
			@color.to_rgb.blue
		end
		
		def hue
			@color.to_hsv.hue
		end
		
		def saturation
			@color.to_hsv.saturation
		end
		
		def value
			@color.to_hsv.value
		end

		def luminance
			@color.to_hsl.luminance
		end
		
		def to_rgb
			@color.to_rgb
		end
		
		def to_hsv
			@color.to_hsv
		end
		
		def to_hsl
			@color.to_hsl
		end
		
		def to_cmyk
			@color.to_cmyk
		end
		
		def to_mixer
			dup
		end
	end
end
