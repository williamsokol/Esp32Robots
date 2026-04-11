
## changing server
In order to change the server there a 2 files that need changed ip address variables:

`Esp32Robots\RobotBuilds\Esp32-Stream-Robot2\NodeServer\Pages\client.js`
and 

`Esp32Robots\RobotBuilds\Esp32-Stream-Robot2\Esp32-Stream-Robot2\Esp32-Stream-Robot2.ino`


To run locally change both of those locations and call the command:

`node server 0.0.0.0`

change port in server.js to `const HTTP_PORT = 3000;` run locally also
