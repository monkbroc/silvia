#!/usr/bin/env ruby

# Open the project in the Particle IDE with the Chrome dev tools open
# Copy the response of /apps/xxxxx.json into firmware.json run this tool
# `ruby save_firmware.rb < firmware.json`

require 'json'

app = JSON.parse ARGF.read

def save_file(name, content)
  File.open(name, 'w') do |f|
    f.write content
  end
end

save_file "#{app["title"]}.ino", app["content"]

app["tabs"].each do |tab|
  save_file "#{tab["title"]}.#{tab["extension"]}", tab["content"]
end

