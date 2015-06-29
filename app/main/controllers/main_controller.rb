# By default Volt generates this controller for your Main component
module Main
  class MainController < Volt::ModelController
    def index
    end

    def status
      self.model = store.coffee_machines.first_or_create
    end

    def calibrate
      self.model = Volt::Model.new({
        offset: 14,
        proportional: 3.0,
        error: 0.5,
        integral: 0.01,
        sum_error: -10.5,
        output: 6
      })
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
