module Main
  # Webhook endpoint for Particle device events
  class DeviceEventsController < Volt::HttpController
    def create
      if params._event == "coffee"
        data = JSON.parse params._data
        store.coffee_machines.first_or_create.then do |coffee_machine|
          coffee_machine._temperature = data["temp"]
          power = data["dc"]
          power_log = power > 1.0 ? 50 * Math.log10(power) : 0
          coffee_machine._power = power
          coffee_machine._power_log = power_log
          coffee_machine._error = data["e"]
          coffee_machine._p_part = data["p"]
          coffee_machine._i_part = data["i"]
        end.fail do |error|
          puts "Error saving event: #{error}"
        end
      end

      render json: { status: "success" }
    end
  end
end
