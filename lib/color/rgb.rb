require 'color'

module Color # :nodoc:

	# === Description
	# RGB representation of color.
	class RGB
		include Common

		class <<self
			# used to load with Marshal.load
			def _load(marshalled) # :nodoc:
				new(*marshalled.unpack("C*"))
			end

			# === Synopsis
			#   Color::Named.from(Color::HSV.new(0,0,255)) # => <RGB: 255, 255, 255, 0 (#FFFFFF)>
			#
			# === Description
			# Coerces +value+ to RGB.
			# 
			def from(value)
				value.to_rgb
			end
			
			# === Synopsis
			#   Color::RGB.floats(0, 0.5, 0.75, 1) # => <RGB: 0, 128, 192, 255 (#FFFFFF)>
			#
			# === Description
			# Create an RGB color from float values. Counterpart to Color::RGB#to_a(true).
			# 
			def floats(red, green, blue, alpha=0)
				new((red*255).round, (green*255).round, (blue*255).round, (alpha*255).round)
			end			
			
			# === Synopsis
			#   Color::RGB.from_int(integer) -> RGB instance
			#
			# === Description
			# Create an RGB color from an integer of the form 0xaarrggbb.
			# See Color::RGB#to_i.
			#
			def from_int(value)
				new(value & 0xff, value >> 8 & 0xff, value >> 16 & 0xff, value >> 24)
			end

			# === Synopsis
			#   Color::RGB.from_html('#ff0099') # => <RGB: 255, 0, 153, 0 (#FF0099)>
			#
			# === Description
			# Create an RGB color from an HTML string, e.g. '#ff0099'.
			# Accepts strings with and without #, also accepts three digit notation ('#f09').
			# See Color::RGB#to_html.
			# 
			def from_html(string)
				case string.downcase
					when /\A#?([\da-f]{2})([\da-f]{2})([\da-f]{2})\Z/
						new($1.to_i(16), $2.to_i(16), $3.to_i(16))
					when /\A#?([\da-f])([\da-f])([\da-f])\Z/
						new(*[$1,$2,$3].map{|i|i.to_i(16)*17})
					else
						raise ArgumentError, "Invalid format #{string}, must be #hhh or #hhhhhh"
				end
			end
		end
		
		# The red portion of this color. A value between 0 and 255.
		attr_reader :red

		# The green portion of this color. A value between 0 and 255.
		attr_reader :green

		# The blue portion of this color. A value between 0 and 255.
		attr_reader :blue

		# The transparency of this color. A value between 0 and 255, where
		# 0 means opaque and 255 fully transparent.
		attr_reader :alpha

		# === Synopsis
		#   Color::RGB.new(red, green, blue[, alpha])
		# 
		# === Description
		# Create a new RGB instance. Red, green, blue and alpha are
		# Integers within 0 and 255 each, where for alpha 0 means
		# opaque and 255 fully transparent.
		def initialize(red, green, blue, alpha=0)
			@red   = red.round
			@green = green.round
			@blue  = blue.round
			@alpha = alpha.round

			unless [red, green, blue, alpha].all? { |v| v.between?(0,255) }
				raise ArgumentError, "Value must be between 0 and 255"
			end
		end

		# See Color::Common#blend
		# Color::RGB#blend adds the additional modes: :multiply, :negative_multiply
		def blend(with, with_alpha=nil, using=:interpolate)
			with_alpha ||= with.alpha
			opacity = (255-with_alpha)/255.0
			value   = proc { |v,a| v+a-a*v }
			case using
				when :interpolate
					self.class.new(
						(red+(with.red-red)*opacity).round,
						(green+(with.green-green)*opacity).round,
						(blue+(with.blue-blue)*opacity).round,
						alpha
					)
				when :multiply
					self.class.new(
						((red/255.0)*value[with.red/255.0, opacity]*255).round,
						((green/255.0)*value[with.green/255.0, opacity]*255).round,
						((blue/255.0)*value[with.blue/255.0, opacity]*255).round,
						alpha
					)
				when :negative_multiply
					self.class.new(
						((1-(1-red/255.0)*value[1-with.red/255.0, opacity])*255).round,
						((1-(1-green/255.0)*value[1-with.green/255.0, opacity])*255).round,
						((1-(1-blue/255.0)*value[1-with.blue/255.0, opacity])*255).round,
						alpha
					)
				else
					raise ArgumentError, "Unknown mode, #{using}"
			end
		end
		
		# === Synopsis
		#    rgb.to_i                                # => integer
		#    Color::RGB.from_int(rgb.to_i).eql?(rgb) # => true
		#
		# === Description
		# Returns an Integer representation of this color.
		# The format is 0xaarrggbb. If with_alpha is false/nil,
		# the aa part is 0.
		#
		def to_i(with_alpha=nil)
			(with_alpha ? (@alpha << 24) : 0) + (@red << 16) + (@green << 8) + @blue
		end
		
		# === Synopsis
		#    rgb.to_s # => string
		# 
		# === Description
		# Returns a String representation of this color.
		#
		def to_s
			"RGB: %d, %d, %d, %d (%s)" %  [red, green, blue, alpha, to_html]
		end

		# === Synopsis
		#   rgb.to_a # => array
		#
		# === Description
		# Returns all values in an array. If +as_floats+ is true, the values
		# are converted to float values between 0 and 1.
		#
		def to_a(as_floats=false)
			as_floats ?
				[red/255.0, green/255.0, blue/255.0, alpha/255.0] :
				[red, green, blue, alpha]
		end
		
		# === Synopsis
		#   rgb.to_hash # => hash
		#
		# === Description
		# Returns all values in a hash with keys :red, :green, :blue and :alpha.
		# If +as_floats+ is true, the values are converted to float
		# values between 0 and 1.
		#
		def to_hash(as_floats=false)
			as_floats ?
				{
					:red   => red/255.0,
					:green => green/255.0,
					:blue  => blue/255.0,
					:alpha => alpha/255.0
				} :
				{
					:red   => red,
					:green => green,
					:blue  => blue,
					:alpha => alpha
				}
		end
		
		def to_html # :nodoc:
			"#%06X" %  to_i(false)
		end

		def to_term # :nodoc:
			Term::Mapping[closest(Term::Mapping.keys)]
		end

		def to_named # :nodoc:
			Named.new(Named::Values[closest(Term::Values.keys)])
		end
		
		def to_rgb # :nodoc:
			dup
		end

		def to_cmyk # :nodoc:
			red, green, blue = @red/255.0, @green/255.0, @blue/255.0
			if red.in_delta(green) && green.in_delta(blue) then
				cyan, magenta, yellow = 0.0, 0.0, 0.0
				key     = 1.0-red
			else
				cyan    = 1.0-red
				magenta = 1.0-green
				yellow  = 1.0-blue
				key     = [cyan, magenta, yellow].min
				if key == 1.0 then
					cyan, magenta, yellow = 0.0, 0.0, 0.0
				else
					cyan    = (cyan-key)/(1.0-key)
					magenta = (magenta-key)/(1.0-key)
					yellow  = (yellow-key)/(1.0-key)
				end
			end
			CMYK.new(cyan, magenta, yellow, key, @alpha)
		end

		def to_hsv # :nodoc:
			red, green, blue = @red/255.0, @green/255.0, @blue/255.0
			max = [red, green, blue].max
			min = [red, green, blue].min

			# hue
			if max.in_delta(min) then
				hue = 0
			elsif (max == red) then
				hue = 1.0/6*(green-blue)/(max-min)
				hue += 1 if (green < blue)
			elsif (max == green) then
				hue = 1.0/6*(blue-red)/(max-min)+1.0/3
			elsif (max == @blue) then
				hue = 1.0/6*(red-green)/(max-min)+2.0/3
			end
			
			# saturation
			saturation = max =~ 0 ? 0 : (max-min)/max
			
			# value
			value      = max
			
			HSV.new(hue, saturation, value, @alpha)
		end

		def to_hsl # :nodoc:
			red, green, blue = @red/255.0, @green/255.0, @blue/255.0
			max = [red, green, blue].max
			min = [red, green, blue].min

			# hue
			if max.in_delta(min) then
				hue = 0
			elsif max == red then
				hue = 1.0/6*(green-blue)/(max-min)
				hue += 1 if (green < blue)
			elsif max == green then
				hue = 1.0/6*(blue-red)/(max-min)+1.0/3
			elsif max == blue then
				hue = 1.0/6*(red-green)/(max-min)+2.0/3
			end
			
			# luminance
			luminance = 0.5*(min+max)

			# saturation
			if max.in_delta(min) then
				saturation = 0
			elsif luminance <= 0.5 then
				saturation = (max-min)/(max+min)
			else
				saturation = (max-min)/(2-(max+min))
			end
			
			HSL.new(hue, saturation, luminance, @alpha)
		end
		
		def to_gray # :nodoc:
			Gray.new(((@red+@green+@blue)/3.0).round, @alpha)
		end

		def to_mixer # :nodoc:
			Mixer.new(self)
		end
		
		# Used with Marshal.dump to create a dump of this color.
		def _dump(*a) # :nodoc:
			[red, green, blue, alpha].pack("C*")
		end
	end
end
