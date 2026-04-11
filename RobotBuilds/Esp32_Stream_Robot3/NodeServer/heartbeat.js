const dotenv = require('dotenv');
dotenv.config();

const { createClient } = require('@supabase/supabase-js');

// Use your actual environment variables or strings
const supabase = createClient(process.env.supabaseUrl, process.env.supabaseKey);

async function runHeartbeat() {
  console.log('--- Starting Daily Heartbeat ---');
  
  const { data, error } = await supabase
    .from('heartbeats') // Ensure this table exists in Supabase
    .insert([{ note: 'Daily wakeup call' }]);

  if (error) {
    console.error('Heartbeat failed:', error.message);
    process.exit(1);
  } else {
    console.log('Heartbeat successful.');
    process.exit(0);
  }
}

runHeartbeat();