#pragma once
#include <rack.hpp>
#include "RNBO.h" 
#include "widgets.hpp"
#include "utils.hpp"


using namespace rack;

// Declare the Plugin, defined in plugin.cpp
extern Plugin* pluginInstance;

// Declare each Model, defined in each module source file
// extern Model* modelMyModule;
extern Model* modelBuffMult;
extern Model* modelBuffMix;
extern Model* modelLogician;
extern Model* modelTrackAndField;
extern Model* modelColoratura;
extern Model* modelRnboModule;
