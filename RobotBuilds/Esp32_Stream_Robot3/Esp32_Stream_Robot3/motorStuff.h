#pragma once

#include <Arduino.h>

void initMotors();

void controllMotors(float x, float y);

void motorLoop();
