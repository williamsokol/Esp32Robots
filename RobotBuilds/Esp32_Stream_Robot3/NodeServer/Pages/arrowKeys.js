let KEYS_PRESSED = {}
const keyDown = (e) => {
  KEYS_PRESSED[e.key] = true;
}

const keyUp = (e) => {
  delete KEYS_PRESSED[e.key];
}

const keyPressed = (key) => {
  return KEYS_PRESSED[key]
}

document.addEventListener('keydown', keyDown)
document.addEventListener('keyup', keyUp)

document.addEventListener('keydown', function(e) {
  switch (e.key){
  case ('ArrowLeft'):
  case ('a'):
    document.querySelector('.left').classList.add('pressed');
    document.querySelector('.left').style.transform = 'translate(0, 2px)';
    break
  case('ArrowUp'):
  case ('w'):
    document.querySelector('.up').classList.add('pressed');
    break
      // keyDir.y = 100;
  case('ArrowRight') :
  case ('d'):
      // keyDiy.y =
    document.querySelector('.right').classList.add('pressed');
    document.querySelector('.right').style.transform = 'translate(0, 2px)';
    break
  case('ArrowDown') :
  case ('s'):
    document.querySelector('.down').classList.add('pressed');
    document.querySelector('.down').style.transform = 'translate(0, 2px)';
    break
  }
});

document.addEventListener('keyup', function(e) {
  switch (e.key){

    case('ArrowLeft'):
    case ('a'):
      document.querySelector('.left').classList.remove('pressed');
      document.querySelector('.left').style.transform = 'translate(0, 0)';
      break
    case('ArrowUp'):
    case ('w'):
      document.querySelector('.up').classList.remove('pressed');
      break
    case('ArrowRight'):
    case ('d'):
      document.querySelector('.right').classList.remove('pressed');
      document.querySelector('.right').style.transform = 'translate(0, 0)';
      break
    case('ArrowDown'):
    case ('s'):
      document.querySelector('.down').classList.remove('pressed');
      document.querySelector('.down').style.transform = 'translate(0, 0)';
      break
  }
});
function addMultipleEventListener(element, events, handler) {
  events.forEach(e => element.addEventListener(e, handler))
}

addMultipleEventListener(document.querySelector('.left'), ['mousedown','touchstart'], function() {
  document.querySelector('.left').style.transform = 'translate(0, 2px)';
  KEYS_PRESSED["ArrowLeft"] = true;
});

addMultipleEventListener(document.querySelector('.left'), ['mouseup','touchend'], function() {
  document.querySelector('.left').style.transform = 'translate(0, 0)';
  delete KEYS_PRESSED["ArrowLeft"];
});

addMultipleEventListener(document.querySelector('.right'), ['mousedown','touchstart'], function() {
  document.querySelector('.right').style.transform = 'translate(0, 2px)';
  KEYS_PRESSED["ArrowRight"] = true;
});

addMultipleEventListener(document.querySelector('.right'), ['mouseup','touchend'], function() {
  document.querySelector('.right').style.transform = 'translate(0, 0)';
  delete KEYS_PRESSED["ArrowRight"];
});

addMultipleEventListener(document.querySelector('.up'), ['mousedown','touchstart'], function() {
  document.querySelector('.left').style.transform = 'translate(0, 2px)';
  document.querySelector('.down').style.transform = 'translate(0, 2px)';
  document.querySelector('.right').style.transform = 'translate(0, 2px)';
  KEYS_PRESSED["ArrowUp"] = true;
});

addMultipleEventListener(document.querySelector('.up'), ['mouseup','touchend'], function() {
  delete KEYS_PRESSED["ArrowUp"];

});

addMultipleEventListener(document.querySelector('.down'), ['mousedown','touchstart'], function() {
  document.querySelector('.down').style.transform = 'translate(0, 2px)';
  KEYS_PRESSED["ArrowDown"] = true;
});

addMultipleEventListener(document.querySelector('.down'), ['mouseup','touchend'], function() {
  document.querySelector('.down').style.transform = 'translate(0, 0)';
  delete KEYS_PRESSED["ArrowDown"];
});
