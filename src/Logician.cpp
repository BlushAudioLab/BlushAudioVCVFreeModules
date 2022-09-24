#include "plugin.hpp"

struct Logician : Module {
	enum ParamId {
		MODE_SWITCH1,
		MODE_SWITCH2,
		PARAMS_LEN
	};
	enum InputId {
		INPUT1A_INPUT,
		INPUT1B_INPUT,
		INPUT2A_INPUT,
		INPUT2B_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUTPUT1_OUTPUT,
		OUTPUT2_OUTPUT,
		INVOUTPUT1_OUTPUT,
		INVOUTPUT2_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		OUTPUT1HIGHGREEN_LIGHT,
		OUTPUT1HIGHRED_LIGHT,
		OUTPUT2HIGHGREEN_LIGHT,
		OUTPUT2HIGHRED_LIGHT,
		AND_LIGHT1_LIGHT,
		OR_LIGHT1_LIGHT,
		XOR_LIGHT1_LIGHT,
		NOT_LIGHT1_LIGHT,
		AND_LIGHT2_LIGHT,
		OR_LIGHT2_LIGHT,
		XOR_LIGHT2_LIGHT,
		NOT_LIGHT2_LIGHT,
		LIGHTS_LEN
	};

	Logician() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(MODE_SWITCH1, 0.0f,2.0f,0.0f,"Mode","",0.0f,1.0f,1.0f);
		configParam(MODE_SWITCH2, 0.0f,2.0f,0.0f,"Mode","",0.0f,1.0f,1.0f);
		configInput(INPUT1A_INPUT, "Input 1A");
		configInput(INPUT1B_INPUT, "Input 1B");
		configInput(INPUT2A_INPUT, "Input 2A");
		configInput(INPUT2B_INPUT, "Input 2B");
		configOutput(OUTPUT1_OUTPUT, "Output 1");
		configOutput(OUTPUT2_OUTPUT, "Output 2");
		configOutput(INVOUTPUT2_OUTPUT, "Output 2 Inverted");
		configLight(OUTPUT1HIGHGREEN_LIGHT, "Output 1 High");
		configLight(OUTPUT2HIGHRED_LIGHT, "Output 2 High");
	}

	void process(const ProcessArgs& args) override {

		// get the mode from the first switch
		int modeA = params[MODE_SWITCH1].getValue() + 1;
		//get the mode from the second switch
		int modeB = params[MODE_SWITCH2].getValue() + 1;

		float input1a = inputs[INPUT1A_INPUT].getVoltage();
		float input1b = inputs[INPUT1B_INPUT].getVoltage();
		float input2a = inputs[INPUT2A_INPUT].getVoltage();
		float input2b = inputs[INPUT2B_INPUT].getVoltage();
		
		float comparator1 = 5.0f;
		float comparator2 = 5.0f;
		float hysterisisAmount = 0.5f;
		float hysterisisHigh1 = 5.0f + hysterisisAmount;
		float hysterisisLow1 = 5.0f - hysterisisAmount;
		float hysterisisHigh2 = 5.0f + hysterisisAmount;
		float hysterisisLow2 = 5.0f - hysterisisAmount;

		const bool low1a = ((input1a <= comparator1) && (input1a <= hysterisisLow1) && (input1a <= hysterisisHigh1));
		const bool high1a = ((input1a >= comparator1) && (input1a >= hysterisisLow1) && (input1a >= hysterisisHigh1));
		const bool low1b = ((input1b <= comparator1) && (input1b <= hysterisisLow1) && (input1b <= hysterisisHigh1));
		const bool high1b = ((input1b >= comparator1) && (input1b >= hysterisisLow1) && (input1b >= hysterisisHigh1));
		const bool low2a = ((input2a <= comparator2) && (input2a <= hysterisisLow2) && (input2a <= hysterisisHigh2));
		const bool high2a = ((input2a >= comparator2) && (input2a >= hysterisisLow2) && (input2a >= hysterisisHigh2));
		const bool low2b = ((input2b <= comparator2) && (input2b <= hysterisisLow2) && (input2b <= hysterisisHigh2));
		const bool high2b = ((input2b >= comparator2) && (input2b >= hysterisisLow2) && (input2b >= hysterisisHigh2));

//Logic Gate 1
		//All low if nothing connected 
		if (!(inputs[INPUT1A_INPUT].isConnected())){
		lights[AND_LIGHT1_LIGHT].setBrightness(0.0f);
		lights[OR_LIGHT1_LIGHT].setBrightness(0.0f);
		lights[XOR_LIGHT1_LIGHT].setBrightness(0.0f);
		lights[NOT_LIGHT1_LIGHT].setBrightness(0.0f);
				//set output voltage to 0
				outputs[OUTPUT1_OUTPUT].setVoltage(0.0f);
				outputs[INVOUTPUT1_OUTPUT].setVoltage(0.0f);
				//set LED Low
				lights[OUTPUT1HIGHGREEN_LIGHT].setBrightness(0.0f);	
				lights[OUTPUT1HIGHRED_LIGHT].setBrightness(0.0f);
		}
		else{
			
			if(!inputs[INPUT1B_INPUT].isConnected()){ //NOT1
				lights[AND_LIGHT1_LIGHT].setBrightness(0.0f);
				lights[OR_LIGHT1_LIGHT].setBrightness(0.0f);
				lights[XOR_LIGHT1_LIGHT].setBrightness(0.0f);
				lights[NOT_LIGHT1_LIGHT].setBrightness(1.0f);
				if ((input1a = low1a)){
					//set output voltage to high (10V)
					outputs[OUTPUT1_OUTPUT].setVoltage(10.0f);
					outputs[INVOUTPUT1_OUTPUT].setVoltage(0.0f);
					//set LED High
					lights[OUTPUT1HIGHGREEN_LIGHT].setBrightness(1.0f);
					lights[OUTPUT1HIGHRED_LIGHT].setBrightness(0.0f);
				}
				else if((input1a = high1a)){
					//set output voltage to 0
					outputs[OUTPUT1_OUTPUT].setVoltage(0.0f);
					outputs[INVOUTPUT1_OUTPUT].setVoltage(10.0f);
					//set LED Low
					lights[OUTPUT1HIGHGREEN_LIGHT].setBrightness(0.0f);
					lights[OUTPUT1HIGHRED_LIGHT].setBrightness(1.0f);
				}
			}
			else{
				switch(modeA){
					case 1: //AND1
						lights[AND_LIGHT1_LIGHT].setBrightness(1.0f);
						lights[OR_LIGHT1_LIGHT].setBrightness(0.0f);
						lights[XOR_LIGHT1_LIGHT].setBrightness(0.0f);
						lights[NOT_LIGHT1_LIGHT].setBrightness(0.0f);
						if ((input1a = (high1a) && (input1b = (high1b)))){
							//set output voltage to high (10V)
							outputs[OUTPUT1_OUTPUT].setVoltage(10.0f);
							outputs[INVOUTPUT1_OUTPUT].setVoltage(0.0f);
							//set LED High
							lights[OUTPUT1HIGHGREEN_LIGHT].setBrightness(1.0f);
							lights[OUTPUT1HIGHRED_LIGHT].setBrightness(0.0f);				
						}
						else if((input1a = (low1a) || (input1b = (low1b)))){
							//set output voltage to 0
							outputs[OUTPUT1_OUTPUT].setVoltage(0.0f);
							outputs[INVOUTPUT1_OUTPUT].setVoltage(10.0f);
							//set LED Low
							lights[OUTPUT1HIGHGREEN_LIGHT].setBrightness(0.0f);
							lights[OUTPUT1HIGHRED_LIGHT].setBrightness(1.0f);
						}
					break;
					case 2: //OR1
						lights[AND_LIGHT1_LIGHT].setBrightness(0.0f);
						lights[OR_LIGHT1_LIGHT].setBrightness(1.0f);
						lights[XOR_LIGHT1_LIGHT].setBrightness(0.0f);
						lights[NOT_LIGHT1_LIGHT].setBrightness(0.0f);
						if ((input1a = (high1a) || (input1b = (high1b)))){
							//set output voltage to high (10V)
							outputs[OUTPUT1_OUTPUT].setVoltage(10.0f);
							outputs[INVOUTPUT1_OUTPUT].setVoltage(0.0f);
							//set LED High
							lights[OUTPUT1HIGHGREEN_LIGHT].setBrightness(1.0f);
							lights[OUTPUT1HIGHRED_LIGHT].setBrightness(0.0f);				
						}
						else if((input1a = (low1a) && (input1b = (low1b)))){
							//set output voltage to 0
							outputs[OUTPUT1_OUTPUT].setVoltage(0.0f);
							outputs[INVOUTPUT1_OUTPUT].setVoltage(10.0f);
							//set LED Low
							lights[OUTPUT1HIGHGREEN_LIGHT].setBrightness(0.0f);
							lights[OUTPUT1HIGHRED_LIGHT].setBrightness(1.0f);
						}
					break;	
					case 3: //XOR1
						lights[AND_LIGHT1_LIGHT].setBrightness(0.0f);
						lights[OR_LIGHT1_LIGHT].setBrightness(0.0f);
						lights[XOR_LIGHT1_LIGHT].setBrightness(1.0f);
						lights[NOT_LIGHT1_LIGHT].setBrightness(0.0f);
						if ((input1a = (low1a) != (input1b = (low1b)))){
							//set output voltage to high (10V)
							outputs[OUTPUT1_OUTPUT].setVoltage(10.0f);
							outputs[INVOUTPUT1_OUTPUT].setVoltage(0.0f);
							//set LED High
							lights[OUTPUT1HIGHGREEN_LIGHT].setBrightness(1.0f);
							lights[OUTPUT1HIGHRED_LIGHT].setBrightness(0.0f);			
						}
						else if((input1a = (high1a) == (input1b = (high1b)))){
							//set output voltage to 0
							outputs[OUTPUT1_OUTPUT].setVoltage(0.0f);
							outputs[INVOUTPUT1_OUTPUT].setVoltage(10.0f);
							//set LED Low
							lights[OUTPUT1HIGHGREEN_LIGHT].setBrightness(0.0f);
							lights[OUTPUT1HIGHRED_LIGHT].setBrightness(1.0f);
						}
					break;		
				}
			}	
		}

//Logic Gate 2
		if (!(inputs[INPUT2A_INPUT].isConnected())){
			//set all lights off
			lights[AND_LIGHT2_LIGHT].setBrightness(0.0f);
			lights[OR_LIGHT2_LIGHT].setBrightness(0.0f);
			lights[XOR_LIGHT2_LIGHT].setBrightness(0.0f);
			lights[NOT_LIGHT2_LIGHT].setBrightness(0.0f);
				//set output voltage to 0
				outputs[OUTPUT2_OUTPUT].setVoltage(0.0f);
				outputs[INVOUTPUT2_OUTPUT].setVoltage(0.0f);
				//set LED Low
				lights[OUTPUT2HIGHGREEN_LIGHT].setBrightness(0.0f);
				lights[OUTPUT2HIGHRED_LIGHT].setBrightness(0.0f);		
		}
		else{
			if(!inputs[INPUT2B_INPUT].isConnected()){ //NOT
			//set NOT light active
			lights[AND_LIGHT2_LIGHT].setBrightness(0.0f);
			lights[OR_LIGHT2_LIGHT].setBrightness(0.0f);
			lights[XOR_LIGHT2_LIGHT].setBrightness(0.0f);
			lights[NOT_LIGHT2_LIGHT].setBrightness(1.0f);
			if ((input2a = low2a)){
				//set output voltage to high (10V)
				outputs[OUTPUT2_OUTPUT].setVoltage(10.0f);
				outputs[INVOUTPUT2_OUTPUT].setVoltage(0.0f);
				//set LED High
				lights[OUTPUT2HIGHGREEN_LIGHT].setBrightness(1.0f);
				lights[OUTPUT2HIGHRED_LIGHT].setBrightness(0.0f);
			}
			else if((input2a = high2a)){
				//set output voltage to 0
				outputs[OUTPUT2_OUTPUT].setVoltage(0.0f);
				outputs[INVOUTPUT2_OUTPUT].setVoltage(10.0f);
				//set LED Low
				lights[OUTPUT2HIGHGREEN_LIGHT].setBrightness(0.0f);
				lights[OUTPUT2HIGHRED_LIGHT].setBrightness(1.0f);
			}
			}
			else{
				switch(modeB){
					case 1: //AND2
						lights[AND_LIGHT2_LIGHT].setBrightness(1.0f);
						lights[OR_LIGHT2_LIGHT].setBrightness(0.0f);
						lights[XOR_LIGHT2_LIGHT].setBrightness(0.0f);
						lights[NOT_LIGHT2_LIGHT].setBrightness(0.0f);
						if ((input2a = high2a) && (input2b = high2b)){
							//set output voltage to high (10V)
							outputs[OUTPUT2_OUTPUT].setVoltage(10.0f);
							outputs[INVOUTPUT2_OUTPUT].setVoltage(0.0f);
							//set LED High
							lights[OUTPUT2HIGHGREEN_LIGHT].setBrightness(1.0f);
							lights[OUTPUT2HIGHRED_LIGHT].setBrightness(0.0f);
						}
						else if ((input2a = low2a) || (input2b = low2b)){
							//set output voltage to 0
							outputs[OUTPUT2_OUTPUT].setVoltage(0.0f);
							outputs[INVOUTPUT2_OUTPUT].setVoltage(10.0f);
							//set LED Low
							lights[OUTPUT2HIGHGREEN_LIGHT].setBrightness(0.0f);
							lights[OUTPUT2HIGHRED_LIGHT].setBrightness(1.0f);
						}
					break;
					case 2: //OR2
						lights[AND_LIGHT2_LIGHT].setBrightness(0.0f);
						lights[OR_LIGHT2_LIGHT].setBrightness(1.0f);
						lights[XOR_LIGHT2_LIGHT].setBrightness(0.0f);
						lights[NOT_LIGHT2_LIGHT].setBrightness(0.0f);
						if ((input2a = (high2a) || (input2b = (high2b)))){
							//set output voltage to high (10V)
							outputs[OUTPUT2_OUTPUT].setVoltage(10.0f);
							outputs[INVOUTPUT2_OUTPUT].setVoltage(0.0f);
							//set LED High
							lights[OUTPUT2HIGHGREEN_LIGHT].setBrightness(1.0f);
							lights[OUTPUT2HIGHRED_LIGHT].setBrightness(0.0f);				
						}
						else if((input2a = (low2a) && (input2b = (low2b)))){
							//set output voltage to 0
							outputs[OUTPUT2_OUTPUT].setVoltage(0.0f);
							outputs[INVOUTPUT2_OUTPUT].setVoltage(10.0f);
							//set LED Low
							lights[OUTPUT2HIGHGREEN_LIGHT].setBrightness(0.0f);
							lights[OUTPUT2HIGHRED_LIGHT].setBrightness(1.0f);
						}
					break;
					case 3: //XOR2
						lights[AND_LIGHT2_LIGHT].setBrightness(0.0f);
						lights[OR_LIGHT2_LIGHT].setBrightness(0.0f);
						lights[XOR_LIGHT2_LIGHT].setBrightness(1.0f);
						lights[NOT_LIGHT2_LIGHT].setBrightness(0.0f);
						if ((input2a = high2a) != (input2b = high2b)){
							//set output voltage to high (10V)
							outputs[OUTPUT2_OUTPUT].setVoltage(10.0f);
							outputs[INVOUTPUT2_OUTPUT].setVoltage(0.0f);
							//set LED High
							lights[OUTPUT2HIGHGREEN_LIGHT].setBrightness(1.0f);
							lights[OUTPUT2HIGHRED_LIGHT].setBrightness(0.0f);
						}
						else if ((input2a = low2a) == (input2b = low2b)){
							//set output voltage to 0
							outputs[OUTPUT2_OUTPUT].setVoltage(0.0f);
							outputs[INVOUTPUT2_OUTPUT].setVoltage(10.0f);
							//set LED Low
							lights[OUTPUT2HIGHGREEN_LIGHT].setBrightness(0.0f);
							lights[OUTPUT2HIGHRED_LIGHT].setBrightness(1.0f);
						}
					break;
				}		
			}			
		}

	}
};

struct ModeSwitch1 : app::SvgSwitch {
	ModeSwitch1() {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/modebutton.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/modebutton.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/modebutton.svg")));
	}
};

struct ModeSwitch2 : app::SvgSwitch {
	ModeSwitch2() {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/modebutton2.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/modebutton2.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/modebutton2.svg")));
	}
};


struct LogicianWidget : ModuleWidget {
	LogicianWidget(Logician* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Logician.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		
		addChild(createParamCentered<ModeSwitch1>(mm2px(Vec(8.89, 48.5)), module, Logician::MODE_SWITCH1));
		addChild(createParamCentered<ModeSwitch2>(mm2px(Vec(22.182, 48.5)), module, Logician::MODE_SWITCH2));

		addChild(createLightCentered<SmallLight<GreenLight>>(mm2px(Vec(6.89, 57.029)), module, Logician::AND_LIGHT1_LIGHT));
		addChild(createLightCentered<SmallLight<GreenLight>>(mm2px(Vec(6.89, 63.001)), module, Logician::OR_LIGHT1_LIGHT));
		addChild(createLightCentered<SmallLight<GreenLight>>(mm2px(Vec(6.89, 68.973)), module, Logician::XOR_LIGHT1_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(6.89, 74.945)), module, Logician::NOT_LIGHT1_LIGHT));

		addChild(createLightCentered<SmallLight<GreenLight>>(mm2px(Vec(24.182, 57.029)), module, Logician::AND_LIGHT2_LIGHT));
		addChild(createLightCentered<SmallLight<GreenLight>>(mm2px(Vec(24.182, 63.001)), module, Logician::OR_LIGHT2_LIGHT));
		addChild(createLightCentered<SmallLight<GreenLight>>(mm2px(Vec(24.182, 68.973)), module, Logician::XOR_LIGHT2_LIGHT));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(24.182, 74.945)), module, Logician::NOT_LIGHT2_LIGHT));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.89, 19.269)), module, Logician::INPUT1A_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.182, 19.269)), module, Logician::INPUT1B_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.89, 90.178)), module, Logician::INPUT2A_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.182, 90.178)), module, Logician::INPUT2B_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(8.89, 36.794)), module, Logician::OUTPUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.182, 36.794)), module, Logician::INVOUTPUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(8.89, 107.703)), module, Logician::OUTPUT2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.182, 107.703)), module, Logician::INVOUTPUT2_OUTPUT));

		addChild(createLightCentered<SmallLight<GreenRedLight>>(mm2px(Vec(15.536, 36.794)), module, Logician::OUTPUT1HIGHGREEN_LIGHT));
		addChild(createLightCentered<SmallLight<GreenRedLight>>(mm2px(Vec(15.536, 107.703)), module, Logician::OUTPUT2HIGHGREEN_LIGHT));
	}
};

Model* modelLogician = createModel<Logician, LogicianWidget>("Logician");