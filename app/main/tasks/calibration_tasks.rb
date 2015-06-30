class CalibrationTasks < Volt::Task
  def get
    client = Particle::Client.new
    device = client.device('silvia')
    cals = JSON.parse device.variable('cals')

    store.coffee_machines.first_or_create.then do |coffee_machine|
      coffee_machine._proportional = cals["Kp"]
      coffee_machine._integral = cals["Ki"]
      coffee_machine._offset = cals["Ko"]
      coffee_machine._target_temperature = cals["sp"]
      coffee_machine._integral_saturation = cals["iSat"]

      true
    end.fail do |error|
      puts "Couldn't do it: #{error}"
      false
    end
  end
end
