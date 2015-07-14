# By default Volt generates this controller for your Main component
module Main
  class MainController < Volt::ModelController
    def index
    end

    def status
      self.model = store.coffee_machines.first_or_create
    end

    def calibrate
      self.model = store.coffee_machines.first_or_create
      CalibrationTasks.get
    end

    def sleep
      self.model = store.coffee_machines.first_or_create
      CalibrationTasks.get_wakeup
    end

    def set(field, value)
      CalibrationTasks.set(field, value)
    end

    ENTER = 13

    def calibration_keypress(event, name)
      if event.key_code == ENTER
        event.prevent_default!
        calibration_blur(event, name)
      end
    end

    def calibration_blur(event, name)
      js_event = event.js_event
      value = `$(js_event.target).text()`
      set name, value
    end

    def sleep_on
      set 'wakeup_time', model._wakeup_time
      set 'sleep', '1'
    end

    def sleep_off
      set 'sleep', '0'
    end

    private

    # The main template contains a #template binding that shows another
    # template.  This is the path to that template.  It may change based
    # on the params._component, params._controller, and params._action values.
    def main_path
      "#{params._component || 'main'}/#{params._controller || 'main'}/#{params._action || 'index'}"
    end

    # Determine if the current nav component is the active one by looking
    # at the first part of the url against the href attribute.
    def active_tab?
      url.path.split('/')[1] == attrs.href.split('/')[1]
    end
  end
end
