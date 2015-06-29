module Main
  # Webhook endpoint for Particle device events
  class DeviceEventsController < Volt::HttpController
    def create
      #data = JSON.parse request.body

      render json: { status: "success" }
    end
  end
end
