class CalibrationTasks < Volt::Task
  FIELD_MAPPING = {
    "proportional" => "Kp",
    "integral" => "Ki",
    "offset" => "Ko",
    "target_temperature" => "sp",
    "integral_saturation" => "iSat"
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

  def set(field, value)
    mapped_field = FIELD_MAPPING[field]
    device.call "set", "#{mapped_field}=#{value}"
  end

  private

  def device
    @device ||= Particle::Client.new.device('silvia')
  end
end
