# See https://github.com/voltrb/volt#routes for more info on routes

client '/about', action: 'about'

# Routes for login and signup, provided by user_templates component gem
client '/signup', component: 'user_templates', controller: 'signup'
client '/login', component: 'user_templates', controller: 'login'

# Webhook endpoint for Particle
post '/device_events', controller: 'device_events', action: 'create'

# The main route, this should be last. It will match any params not
# previously matched.
client '/', {}
