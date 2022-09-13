#include "plugin.hpp"

struct Logician : Module {
	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		INPUTA1_INPUT,
		INPUTB1_INPUT,
		INPUTA2_INPUT,
		INPUTB2_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUTPUT1_OUTPUT,
		OUTPUT2_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		OUTPUT1HIGH_LIGHT,
		OUTPUT2HIGH_LIGHT,
		LIGHTS_LEN
	};

	Logician() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configInput(INPUTA1_INPUT, "Input 1A");
		configInput(INPUTB1_INPUT, "Input 1B");
		configInput(INPUTA2_INPUT, "Input 2A");
		configInput(INPUTB2_INPUT, "Input 2B");
		configOutput(OUTPUT1_OUTPUT, "Output 1");
		configOutput(OUTPUT2_OUTPUT, "Output 2");
		configLight(OUTPUT1HIGH_LIGHT, "Output 1 High");
		configLight(OUTPUT2HIGH_LIGHT, "Output 2 High");
	}

	void process(const ProcessArgs& args) override {
		float input1a = inputs[INPUTA1_INPUT].getVoltage();
		float input1b = inputs[INPUTB1_INPUT].getVoltage();
		float input2a = inputs[INPUTA2_INPUT].getVoltage();
		float input2b = inputs[INPUTB2_INPUT].getVoltage();
		float comparator1 = 5.0f;
		float comparator2 = 5.0f;

		const bool greaterThan1A = (input1a >= comparator1);
		const bool greaterThan1B = (input1b >= comparator1);
		const bool greaterThan2A = (input2a >= comparator2);
		const bool greaterThan2B = (input2b >= comparator2);

		//Logic Gate 1
		// int mode[4] = {1,2,3,4};
		int modeA = 6;
		switch(modeA){
			case 1: //AND
				if ((input1a = greaterThan1A) && (input1b = greaterThan1B)){
					//set output voltage to high (10V)
					outputs[OUTPUT1_OUTPUT].setVoltage(10.0f);
					//set LED High
					lights[OUTPUT1HIGH_LIGHT].setBrightness(1.0f);
				}
				else{
					//set output voltage to 0
					outputs[OUTPUT1_OUTPUT].setVoltage(0.0f);
					//set LED Low
					lights[OUTPUT1HIGH_LIGHT].setBrightness(0.0f);
				}
			break;
			case 2: //OR
				if ((input1a = greaterThan1A) || (input1b = greaterThan1B)){
					//set output voltage to high (10V)
					outputs[OUTPUT1_OUTPUT].setVoltage(10.0f);
					//set LED High
					lights[OUTPUT1HIGH_LIGHT].setBrightness(1.0f);
				}
				else{
					//set output voltage to 0
					outputs[OUTPUT1_OUTPUT].setVoltage(0.0f);
					//set LED Low
					lights[OUTPUT1HIGH_LIGHT].setBrightness(0.0f);
				}
			break;
			case 3: //NAND
				if (!(input1a = greaterThan1A) && !(input1b = greaterThan1B)){
					//set output voltage to high (10V)
					outputs[OUTPUT1_OUTPUT].setVoltage(10.0f);
					//set LED High
					lights[OUTPUT1HIGH_LIGHT].setBrightness(1.0f);
				}
				else{
					//set output voltage to 0
					outputs[OUTPUT1_OUTPUT].setVoltage(0.0f);
					//set LED Low
					lights[OUTPUT1HIGH_LIGHT].setBrightness(0.0f);
				}
			break;		
			case 4: //NOR
				if (!(input1a = greaterThan1A) || !(input1b = greaterThan1B)){
					//set output voltage to high (10V)
					outputs[OUTPUT1_OUTPUT].setVoltage(10.0f);
					//set LED High
					lights[OUTPUT1HIGH_LIGHT].setBrightness(1.0f);
				}
				else{
					//set output voltage to 0
					outputs[OUTPUT1_OUTPUT].setVoltage(0.0f);
					//set LED Low
					lights[OUTPUT1HIGH_LIGHT].setBrightness(0.0f);
				}
			break;
			case 5: //EXOR
				if ((input1a = greaterThan1A) != (input1b = greaterThan1B)){
					//set output voltage to high (10V)
					outputs[OUTPUT1_OUTPUT].setVoltage(10.0f);
					//set LED High
					lights[OUTPUT1HIGH_LIGHT].setBrightness(1.0f);
				}
				else{
					//set output voltage to 0
					outputs[OUTPUT1_OUTPUT].setVoltage(0.0f);
					//set LED Low
					lights[OUTPUT1HIGH_LIGHT].setBrightness(0.0f);
				}
			break;
			case 6: //EXNOR
				if ((input1a = greaterThan1A) == (input1b = greaterThan1B)){
					//set output voltage to high (10V)
					outputs[OUTPUT1_OUTPUT].setVoltage(10.0f);
					//set LED High
					lights[OUTPUT1HIGH_LIGHT].setBrightness(1.0f);
				}
				else{
					//set output voltage to 0
					outputs[OUTPUT1_OUTPUT].setVoltage(0.0f);
					//set LED Low
					lights[OUTPUT1HIGH_LIGHT].setBrightness(0.0f);
				}
			break;				
		}

		//Logic Gate 2
		// int mode[4] = {1,2,3,4};
		int modeB = 6;
		switch(modeB){
			case 1: //AND
				if ((input2a = greaterThan2A) && (input2b = greaterThan2B)){
					//set output voltage to high (10V)
					outputs[OUTPUT2_OUTPUT].setVoltage(10.0f);
					//set LED High
					lights[OUTPUT2HIGH_LIGHT].setBrightness(1.0f);
				}
				else{
					//set output voltage to 0
					outputs[OUTPUT2_OUTPUT].setVoltage(0.0f);
					//set LED Low
					lights[OUTPUT2HIGH_LIGHT].setBrightness(0.0f);
				}
			break;
			case 2: //OR
				if ((input2a = greaterThan2A) || (input2b = greaterThan2B)){
					//set output voltage to high (10V)
					outputs[OUTPUT2_OUTPUT].setVoltage(10.0f);
					//set LED High
					lights[OUTPUT2HIGH_LIGHT].setBrightness(1.0f);
				}
				else{
					//set output voltage to 0
					outputs[OUTPUT2_OUTPUT].setVoltage(0.0f);
					//set LED Low
					lights[OUTPUT2HIGH_LIGHT].setBrightness(0.0f);
				}
			break;
			case 3: //NAND
				if (!(input2a = greaterThan2A) && !(input2b = greaterThan2B)){
					//set output voltage to high (10V)
					outputs[OUTPUT2_OUTPUT].setVoltage(10.0f);
					//set LED High
					lights[OUTPUT2HIGH_LIGHT].setBrightness(1.0f);
				}
				else{
					//set output voltage to 0
					outputs[OUTPUT2_OUTPUT].setVoltage(0.0f);
					//set LED Low
					lights[OUTPUT2HIGH_LIGHT].setBrightness(0.0f);
				}
			break;		
			case 4: //NOR
				if (!(input2a = greaterThan2A) || !(input2b = greaterThan2B)){
					//set output voltage to high (10V)
					outputs[OUTPUT2_OUTPUT].setVoltage(10.0f);
					//set LED High
					lights[OUTPUT2HIGH_LIGHT].setBrightness(1.0f);
				}
				else{
					//set output voltage to 0
					outputs[OUTPUT2_OUTPUT].setVoltage(0.0f);
					//set LED Low
					lights[OUTPUT2HIGH_LIGHT].setBrightness(0.0f);
				}
			break;
			case 5: //EXOR
				if ((input2a = greaterThan2A) != (input2b = greaterThan2B)){
					//set output voltage to high (10V)
					outputs[OUTPUT2_OUTPUT].setVoltage(10.0f);
					//set LED High
					lights[OUTPUT2HIGH_LIGHT].setBrightness(1.0f);
				}
				else{
					//set output voltage to 0
					outputs[OUTPUT2_OUTPUT].setVoltage(0.0f);
					//set LED Low
					lights[OUTPUT2HIGH_LIGHT].setBrightness(0.0f);
				}
			break;
			case 6: //EXNOR
				if ((input2a = greaterThan2A) == (input2b = greaterThan2B)){
					//set output voltage to high (10V)
					outputs[OUTPUT2_OUTPUT].setVoltage(10.0f);
					//set LED High
					lights[OUTPUT2HIGH_LIGHT].setBrightness(1.0f);
				}
				else{
					//set output voltage to 0
					outputs[OUTPUT2_OUTPUT].setVoltage(0.0f);
					//set LED Low
					lights[OUTPUT2HIGH_LIGHT].setBrightness(0.0f);
				}
			break;				
		}

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

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.89, 19.269)), module, Logician::INPUTA1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.182, 19.269)), module, Logician::INPUTB1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.89, 90.178)), module, Logician::INPUTA2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.182, 90.178)), module, Logician::INPUTB2_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(8.89, 36.794)), module, Logician::OUTPUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(8.89, 107.703)), module, Logician::OUTPUT2_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(22.182, 36.794)), module, Logician::OUTPUT1HIGH_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(22.182, 107.703)), module, Logician::OUTPUT2HIGH_LIGHT));
	}
};

Model* modelLogician = createModel<Logician, LogicianWidget>("Logician");