require 'color'

module Color # :nodoc:

	# == Description
	# HSV color model
	#
	class HSV
		include Color::Common

		class <<self
			# used to load with Marshal.load
			def _load(marshalled) # :nodoc:
				new(*marshalled.unpack("G3C"))
			end

			# === Synopsis
			#   Color::HSV.from(Color::RGB.new(255,255,255)) # => <HSV: 0°h 0%s, 100%v, 0>
			#
			# === Description
			# Coerces +value+ to HSV.
			# 
			def from(value)
				value.to_hsv
			end

			# === Synopsis
			#   Color::HSV.floats(0.5, 0.2, 0.3 0.75) # => <HSV: 180°h 20%s, 30%v, 192>
			#
			# === Description
			# Create a HSV color from float values. Counterpart to Color::HSV#to_a(true).
			# 
			def floats(hue, saturation, value, alpha=0)
				new(hue, saturation, value, (alpha*255).round)
			end			
		end

		# The hue of this color. A value between 0 and 1.
		attr_reader :hue

		# The saturation of this color. A value between 0 and 1.
		attr_reader :saturation

		# The value of this color. A value between 0 and 1.
		attr_reader :value

		# The transparency of this color. A value between 0 and 255, where
		# 0 means opaque and 255 fully transparent.
		attr_reader :alpha

		# === Synopsis
		#   Color::HSV.new(hue, saturation, value[, alpha])
		#
		# === Description
		# Create a new HSV instance. Hue, saturation, value are floats
		# between 0 and 1, alpha is an Integer within 0 and 255, where 0
		# means opaque and 255 fully transparent.
		#
		def initialize(hue, saturation, value, alpha=0)
			unless [hue, saturation, value].all? { |v| 0 <= v && v <= 1 } # between? fails at NaN
				raise ArgumentError, "Invalid Value, hue, saturation and value must be between 0 and 1"
			end
			raise ArgumentError, "Invalid alpha, must be between 0 and 255" unless alpha.between?(0,255)

			@hue        = (hue %  1).to_f
			@saturation = saturation.to_f
			@value      = value.to_f
			@alpha      = alpha.to_i
		end
		
		def to_rgb # :nodoc:
			if @saturation.in_delta(0) then
				RGB.new(@value*255, @value*255, @value*255, @alpha)
			else
				hi = (@hue*6.0).to_i % 6
				f  = (@hue*6.0)-hi
				p  = @value*(1.0-@saturation)
				q  = @value*(1.0-f*@saturation)
				t  = @value*(1.0-(1.0-f)*@saturation)
				r,g,b = *(case hi
					when 0: [@value, t, p]
					when 1: [q, @value, p]
					when 2: [p, @value, t]
					when 3: [p, q, @value]
					when 4: [t, p, @value]
					when 5: [@value, p, q]
					else
						raise "Error, #{hi} should be 0..5"
				end)
				RGB.new(r*255,g*255,b*255,@alpha)
			end
		end

		# === Synopsis
		#    hsv.to_s # => string
		# 
		# === Description
		# Returns a String representation of this color.
		#
		def to_s
			"HSV: %d°h %d%%s, %d%%v, %d" %  [(hue*360).round, (saturation*100).round, (value*100).round, alpha]
		end
		
		# === Synopsis
		#   hsv.to_a # => array
		#
		# === Description
		# Returns all values in an array. If +as_floats+ is true, the values
		# are converted to float values between 0 and 1.
		#
		def to_a(as_floats=false)
			[hue, saturation, value, as_floats ? alpha/255.0 : alpha]
		end
		
		# === Synopsis
		#   hsv.to_hash # => hash
		#
		# === Description
		# Returns all values in a hash with keys :hue, :saturation, :value and :alpha.
		# If +as_floats+ is true, the values are converted to float
		# values between 0 and 1.
		#
		def to_hash(as_floats=false)
			{ :hue => hue, :saturation => saturation, :value => value, :alpha => as_floats ? alpha/255.0 : alpha }
		end

		def to_hsv # :nodoc:
			dup
		end

		# Used with Marshal.dump to create a dump of this color.
		def _dump(*a) # :nodoc:
			[hue, saturation, value, alpha].pack("G3C")
		end
	end
end
