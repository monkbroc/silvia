class CalibrationTasks < Volt::Task
  FIELD_MAPPING = {
    "proportional" => "Kp",
    "integral" => "Ki",
    "offset" => "Ko",
    "target_temperature" => "sp",
    "integral_saturation" => "iSat",
    "sleep" => "sleep",
    "wakeup_time" => "Twakeup"
  }

  def get
    cals = JSON.parse device.get('cals')

    store.coffee_machines.first_or_create.then do |coffee_machine|
      FIELD_MAPPING.each do |db_field, hw_field|
        coffee_machine.send "_#{db_field}=", cals[hw_field]
      end

      true
    end.fail do |error|
      puts "Couldn't do it: #{error}"
      false
    end
  end

  def get_wakeup
    device.call('get', 'Twakeup')
    twakeup = device.get('result').to_i
    wakeup_time = Time.at(twakeup).strftime("%-I:%M%P")

    store.coffee_machines.first_or_create.then do |coffee_machine|
      coffee_machine._wakeup_time = wakeup_time

      true
    end.fail do |error|
      puts "Couldn't do it: #{error}"
      false
    end
  end

  def set(field, value)
    # special case for wakeup time => get UNIX timestamp
    if field == "wakeup_time"
      value = timestamp_from_hour value
    end
    mapped_field = FIELD_MAPPING[field]
    device.call "set", "#{mapped_field}=#{value}"
  end

  private

  def device
    @device ||= Particle::Client.new.device('silvia')
  end

  def timestamp_from_hour(value)
    groups = /^\s*(\d+):(\d+)([ap]m)?/.match(value)
    if groups
      hour = groups[1].to_i
      min = groups[2].to_i
      pm = groups[3] ? groups[3].downcase == "pm" : false
      hour += 12 if pm

      now = Time.now
      t = Time.new(now.year, now.month, now.day, hour, min)
      if t < now
        t = Time.new(now.year, now.month, now.day + 1, hour, min)
      end

      t.to_i
    else
      raise "Invalid time"
    end
  end
end
