
var joy;
var controlType = "keys";
// <!-- this is the visual code to work around css -->
document.addEventListener('DOMContentLoaded', function () {
    // fix mainpage height:

    keyDiv.style.display = "none"
    // console.log(joyDiv.style.display)
    
    joyDiv.style.width = "95%"
    joyDiv.style.height = "95%"
    
    // console.log(joyDiv.scrollHeight > joyDiv.scrollWidth)
    let joyside = joyDiv.scrollHeight > joyDiv.scrollWidth? joyDiv.scrollWidth : joyDiv.scrollHeight; 
    joyDiv.style.width = joyside+"px"
    joyDiv.style.height = joyside+"px"
    joy = new JoyStick('joyDiv');

    joyDiv.style.margin = "auto";
    joyDiv.style.display = "none"
    keyDiv.style.display = "";
});

// Create JoyStick object into the DIV 'joyDiv'
var controls = document.getElementById('controls')
var joyDiv = document.getElementById('joyDiv')
var keyDiv = document.getElementById('keysDiv')

const arrowKeysButton = document.getElementById('arrowKeysButton');
const joystickButton = document.getElementById('joystickButton');
const arrowKeys = document.getElementById('arrowKeys');
const joystick = document.getElementById('joystick');

arrowKeysButton.addEventListener('click', function () {
    controlType = "keys";
    arrowKeysButton.classList.add('active');
    joystickButton.classList.remove('active');
    joyDiv.style.display = "none"
    keyDiv.style.display = ""

});

joystickButton.addEventListener('click', function () {
    controlType = "joystick";
    arrowKeysButton.classList.remove('active');
    joystickButton.classList.add('active');
    joyDiv.style.display = ""
    keyDiv.style.display = "none"
});
