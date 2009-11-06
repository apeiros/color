require 'color/version'
require 'color/common'
require 'color/rgb'
require 'color/hsv'
require 'color/hsl'
require 'color/cmyk'
require 'color/gray'
require 'color/mixer'

# A module providing multiple color spaces, conversions and tools
module Color
	# Whether this is a native build (e.g. C) or plain ruby
	def native?
		false
	end

	# See Color::RGB::new
	# example:
	#   include Color
	#   orange = rgb(255, 128, 0)
	def rgb(*args)
		RGB.new(*args)
	end
	
	# See Color::CMYK::new
	# example:
	#   include Color
	#   orange = cmyk(0, 50, 100, 0)
	def cmyk(*args)
		CMYK.new(*args)
	end
	
	# See Color::HSV::new
	# example:
	#   include Color
	#   orange = hsv(30, 100, 100)
	def hsv(*args)
		HSV.new(*args)
	end
	
	# See Color::HSL::new
	# example:
	#   include Color
	#   orange = hsl(30, 100, 100)
	def hsl(*args)
		HSL.new(*args)
	end
	
	# See Color::Gray::new
	# example:
	#   include Color
	#   silver = gray(50)
	def gray(*args)
		Gray.new(*args)
	end
	
	def term(*args)
		Term.new(*args)
	end
	
	module_function :rgb
	module_function :cmyk
	module_function :hsv
	module_function :hsl
	module_function :gray
	module_function :term
end

begin
	# this is the C lib, all methods are also implemented in pure ruby, but the
	# C variant is 5-100x faster with MRI.
	require 'ccolor'
rescue LoadError
	warn "Could not load native extension for Color"
end
require 'color/term'
require 'color/named'

class Numeric
	def in_delta(other, delta=Float::EPSILON*4)
		delta ||= Float::EPSILON*4
		(self-other).abs < delta
	end
end

# HSI -> HSL
# HSB -> HSV

# FIXME, remove for release
include Color