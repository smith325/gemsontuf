#!/bin/bash

# If the below fail out and tell you to visit a stack overflow site, try
# running the following two lines of code to purge RVM from your system
# and then run the script again
#
# sudo apt-get --purge remove ruby-rvm
# sudo rm -rf /usr/share/ruby-rvm /etc/rvmrc /etc/profile.d/rvm.s

\curl -L https://get.rvm.io | bash -s stable
source ~/.rvm/scripts/rvm
type rvm | head -n 1

