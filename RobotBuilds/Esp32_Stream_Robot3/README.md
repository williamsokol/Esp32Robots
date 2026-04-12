
# Setting Up New Server

## firewall
you will probably need to change the iptables rules of your server to allow the http port and the WS_PORT you picked:
```
sudo iptables --list
sudo iptables --list INPUT -v
sudo iptables -I INPUT 6 -p tcp --dport 65080 -j ACCEPT
sudo iptables -I INPUT 6 -p tcp --dport 80 -j ACCEPT
```
Note: I am running the server with node in sudo because I am lazy to do nginx rn:
```
sudo /home/ubuntu/.nvm/versions/node/v24.14.1/bin/node server.js
```
## pm2

pm2 commnads (use sudo cause I run node in sudo):

```
sudo pm2 startup
sudo pm2 status
sudo pm2 list
sudo pm2 start server.js --name "ballybot-server"
sudo pm2 start heartbeat.js --name "supabase-heartbeat" --cron "0 0 * * *" --no-autorestart
sudo pm2 startup
sudo pm2 save
sudo pm2 list
```

## ip
2 files that need to change ip address variables: <br>
`Esp32Robots\RobotBuilds\Esp32-Stream-Robot3\NodeServer\.env`
<br> and <br>
`Esp32Robots\RobotBuilds\Esp32-Stream-Robot3\Esp32-Stream-Robot3\Esp32-Stream-Robot3.ino`


To run locally change both of those locations and call the command:

`node server 0.0.0.0`

change port in `.env` to `PORT=3000` to run locally also
