WEBHOOK_URL = "http://monkbroc.dyndns.org:3000/device_events"

def webhook_exists?
  Particle.webhooks.any? do |webhook|
    webhook.url == WEBHOOK_URL
  end
end

def delete_webhook
  Particle.webhooks.each do |webhook|
    webhook.remove if webhook.url == WEBHOOK_URL
  end
end

def create_webhook
  Particle.webhook(
    event: "coffee",
    url: WEBHOOK_URL,
    mydevices: true
  ).create
  puts "### Created Particle webhook ###"
end

Particle.reset!

delete_webhook
create_webhook

