namespace :firmware do
  def firmware_directory
    "firmware"
  end

  def device
    "silvia"
  end

  def device_type
    "photon"
  end

  task :account do
    sh "particle config personal"
  end

  task :compile => :account do
    sh "particle compile #{device_type} \"#{firmware_directory}\" --saveTo /tmp/firmware"
  end

  task :flash => :account do
    sh "particle flash #{device} \"#{firmware_directory}\""
  end
end

task :flash => "firmware:flash"
task :compile => "firmware:compile"
