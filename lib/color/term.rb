require 'color'

module Color # :nodoc:

	# Ansi terminal colors
	# 
	class Term
		include Common

		class <<self
			# used to load with Marshal.load
			def _load(marshalled) # :nodoc:
				new(name.to_sym)
			end

			# === Synopsis
			#   Color::Term.from(Color::RGB.new(255,255,255)) # => <Named: White, 0>
			#
			# === Description
			# Coerces +value+ to Named.
			# 
			def from(value)
				value.to_term
			end
			
			# Uses Color::RGB::floats
			def floats(*values)
				RGB.floats(*values).to_term
			end
		end
		
		def initialize(name)
			@name = name
		end
		
		# The transparency of this color. A value between 0 and 255, where
		# 0 means opaque and 255 fully transparent.
		# Color::Term doesn't support alpha, so alpha is always 0
		def alpha
			0
		end
		
		# === Synopsis
		#    term.to_s # => string
		#    puts "#{term(:yellow).to_s}#{term(:black).to_s(true)}yellow on black\e[0m"
		#
		# === Description
		# Returns the ANSI sequence for this color in the foreground, or if +background+
		# is true, for the background.
		# Also see: Color::Term::StringColoring
		#
		def to_s(background=false)
			background ? "\e[#{Background[@name]}m" : "\e[#{Foreground[@name]}m"
		end

		# === Synopsis
		#   term.to_a # => array
		#
		# === Description
		# Returns all values in an array. If +as_floats+ is true, the values of
		# the rgb representation, converted to float values between 0 and 1 are
		# returned
		#
		def to_a(as_floats=false)
			as_floats ?
				to_rgb.to_a(true) :
				[@name, alpha]
		end
		
		# === Synopsis
		#   term.to_hash # => hash
		#
		# === Description
		# Returns all values in a hash with keys :name and :alpha.
		# If +as_floats+ is true, the values of the rgb representation, converted
		# to float values between 0 and 1 are returned. See Color::RGB#to_hash.
		#
		def to_hash(as_floats=false)
			as_floats ?
				to_rgb.to_hash(true) :
				{ :name => @name, :alpha => alpha }
		end
		
		def to_term # :nodoc:
			dup
		end
		
		def to_rgb # :nodoc:
			RGB.from_int(Value[@name])
		end
		
		def inspect # :nodoc:
			"<Term: #@name>"
		end
		
		# Used with Marshal.dump to create a dump of this color.
		def _dump # :nodoc:
			@name.to_s
		end

		Mapping = {
			RGB.from_int(0x000000) => :black,
			RGB.from_int(0xff0000) => :red,
			RGB.from_int(0x00ff00) => :green,
			RGB.from_int(0x0000ff) => :blue,
			RGB.from_int(0xffff00) => :yellow,
			RGB.from_int(0xff00ff) => :purple,
			RGB.from_int(0x00ffff) => :cyan,
			RGB.from_int(0xffffff) => :white,
		}
		Values = {
			:black  => 0x000000,
			:red    => 0xff0000,
			:green  => 0x00ff00,
			:blue   => 0x0000ff,
			:yellow => 0xffff00,
			:purple => 0xff00ff,
			:cyan   => 0x00ffff,
			:white  => 0xffffff,
		}
		Foreground = {
			:black  => 30,
			:red    => 31,
			:green  => 32,
			:yellow => 33,
			:blue   => 34,
			:purple => 35,
			:cyan   => 36,
			:white  => 37,
		}
		Background = {
			:black  => 40,
			:red    => 41,
			:green  => 42,
			:yellow => 43,
			:blue   => 44,
			:purple => 45,
			:cyan   => 46,
			:white  => 47,
		}

		# A helper module for using class Color::Term
		#
		# == Synopsis
		#   class String
		#     include Color::Term::StringColoring
		#   end
		#   puts "hello".bold.yellow.on_red
		#
		# == Notes
		# see Term::Foreground for names of foreground colors, Term::Background
		# for background colors (the methods for background colors are "on_" + name,
		# e.g. :red -> #on_red.
		#
		module StringColoring
			(Foreground.merge({
				:bold      => 1,
				:underline => 4,
				:blink     => 5,
				:invert    => 7,
			})).each { |name, value|
				define_method(name) { "\e[#{value}m#{self}\e[0m" }
			}
			Background.each { |name, value|
				define_method("on_#{name}") { "\e[#{value}m#{self}\e[0m" }
			}
		end
	end
end