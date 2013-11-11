require "rubygems"
require "rubypython"

RubyPython.start # start the Python interpreter

httplib = RubyPython.import("tuf.interposition")
httplib = RubyPython.import("urllib_tuf")
print httplib.urlopen("http://google.com")

RubyPython.stop # stop the Python interpreter