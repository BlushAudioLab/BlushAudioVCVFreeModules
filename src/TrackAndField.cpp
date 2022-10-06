#include "plugin.hpp"
#include "TrackAndField.hpp"

struct TrackAndField : Module {
	enum ParamId {
		MODE_SWITCHA,
		MODE_SWITCHB,
		PARAMS_LEN
	};
	enum InputId {
		SAMPLE_INPUTA,
		TRIGGER_INPUTA,
		SAMPLE_INPUTB,
		TRIGGER_INPUTB,
		INPUTS_LEN
	};
	enum OutputId {
		TF_OUTPUTA,
		TF_OUTPUTB,
		OUTPUTS_LEN
	};
	enum LightId {
		STATUS_LIGHTA,
		STATUS_LIGHTB,
		LIGHTS_LEN
	};

	TrackAndField() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(MODE_SWITCHA, 0.0f,1.0f,0.0f,"Mode A","",0.0f,1.0f,1.0f);
		configParam(MODE_SWITCHB, 0.0f,1.0f,0.0f,"Mode B","",0.0f,1.0f,1.0f);
		configInput(SAMPLE_INPUTA, "Sample A");
		configInput(TRIGGER_INPUTA, "Trigger A");
		configInput(SAMPLE_INPUTB, "Sample B");
		configInput(TRIGGER_INPUTB, "Trigger B");
		configLight(STATUS_LIGHTA, "Status A");
		configLight(STATUS_LIGHTB, "Status B");
		configOutput(TF_OUTPUTA, "Output A");
		configOutput(TF_OUTPUTA, "Output B");
	}

	void process(const ProcessArgs& args) override {

		sampleInputA = inputs[SAMPLE_INPUTA].getVoltage();
		triggerInputA = inputs[TRIGGER_INPUTA].getVoltage();
		sampleInputB = inputs[SAMPLE_INPUTB].getVoltage();
		triggerInputB = inputs[TRIGGER_INPUTB].getVoltage();
		
		modeA = params[MODE_SWITCHA].getValue() + 1;
		modeB = params[MODE_SWITCHB].getValue() + 1;

		white = random::normal();
		const float gain = 5.f / std::sqrt(2.f);
		
		//T&F Unit 1
		switch (modeA){
			case 1: //sample and hold
				//regular old sample and hold functionality
				if ((inputs[SAMPLE_INPUTA].isConnected()) && (schmittTriggerA.process(triggerInputA))){
					outputs[TF_OUTPUTA].setVoltage(sampleInputA);
					lights[STATUS_LIGHTA].setBrightness(sampleInputA);
				}
				//use internal white noise source if no sample input is connected
				if (!(inputs[SAMPLE_INPUTA].isConnected()) && (schmittTriggerA.process(triggerInputA))){
					outputs[TF_OUTPUTA].setVoltage(white * gain);
					lights[STATUS_LIGHTA].setBrightness(white * gain);
				}
				//set everything to 0 if nothing is connected
				else if(!(inputs[SAMPLE_INPUTA].isConnected()) && (!(inputs[TRIGGER_INPUTA].isConnected()))){
					outputs[TF_OUTPUTA].setVoltage(0);
					lights[STATUS_LIGHTA].setBrightness(0);
				}
			break;
			case 2: //track and hold
				//regular old track and hold functionality
				if((inputs[SAMPLE_INPUTA].isConnected()) && (triggerInputA > threshold)){
				
					outputs[TF_OUTPUTA].setVoltage(sampleInputA);
					lights[STATUS_LIGHTA].setBrightness(sampleInputA);

				}
				//use internal white noise source if no sample input is connected
				else if(!(inputs[SAMPLE_INPUTA].isConnected()) && (triggerInputA > threshold)){
				
					outputs[TF_OUTPUTA].setVoltage(white * gain);
					lights[STATUS_LIGHTA].setBrightness(white * gain);

				}
				//set everything to 0 if nothing is connected
				else if(!(inputs[SAMPLE_INPUTA].isConnected()) && (!(inputs[TRIGGER_INPUTA].isConnected()))){
					outputs[TF_OUTPUTA].setVoltage(0);
					lights[STATUS_LIGHTA].setBrightness(0);
				}
			break;
		}
		//T&F Unit 2
		switch (modeB){
			case 1: // sample and hold
				//regular old sample and hold functionality
				if ((inputs[SAMPLE_INPUTB].isConnected()) && (schmittTriggerB.process(triggerInputB))){
					outputs[TF_OUTPUTB].setVoltage(sampleInputB);
					lights[STATUS_LIGHTB].setBrightness(sampleInputB);
				}
				//use internal white noise source if no sample input is connected
				if (!(inputs[SAMPLE_INPUTB].isConnected()) && (schmittTriggerB.process(triggerInputB))){
					outputs[TF_OUTPUTB].setVoltage(white * gain);
					lights[STATUS_LIGHTB].setBrightness(white * gain);
				}
				//set everything to 0 if nothing is connected
				else if(!(inputs[SAMPLE_INPUTB].isConnected()) && (!(inputs[TRIGGER_INPUTB].isConnected()))){
					outputs[TF_OUTPUTB].setVoltage(0);
					lights[STATUS_LIGHTB].setBrightness(0);
				}
			break;
			case 2: //track and hold
				//regular old track and hold functionality
				if((inputs[SAMPLE_INPUTB].isConnected()) && (triggerInputB > threshold)){
				
					outputs[TF_OUTPUTB].setVoltage(sampleInputB);
					lights[STATUS_LIGHTB].setBrightness(sampleInputB);

				}
				//use internal white noise source if no sample input is connected
				else if(!(inputs[SAMPLE_INPUTB].isConnected()) && (triggerInputB > threshold)){
				
					outputs[TF_OUTPUTB].setVoltage(white * gain);
					lights[STATUS_LIGHTB].setBrightness(white * gain);

				}
				//set everything to 0 if nothing is connected
				else if(!(inputs[SAMPLE_INPUTB].isConnected()) && (!(inputs[TRIGGER_INPUTB].isConnected()))){
					outputs[TF_OUTPUTB].setVoltage(0);
					lights[STATUS_LIGHTB].setBrightness(0);
				}
			break;
		}

	}		
};

struct ModeSwitchA : app::SvgSwitch {
	ModeSwitchA() {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/NKK_2.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/NKK_0.svg")));
	}
};

struct ModeSwitchB : app::SvgSwitch {
	ModeSwitchB() {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/NKK_2.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/NKK_0.svg")));
	}
};

struct TrackAndFieldWidget : ModuleWidget {
	TrackAndFieldWidget(TrackAndField* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/TrackAndField.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addChild(createParamCentered<ModeSwitchA>(mm2px(Vec(8.89, 64.250)), module, TrackAndField::MODE_SWITCHA));
		addChild(createParamCentered<ModeSwitchB>(mm2px(Vec(22.182, 64.250)), module, TrackAndField::MODE_SWITCHB));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.89, 27.619)), module, TrackAndField::SAMPLE_INPUTA));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.182, 27.619)), module, TrackAndField::TRIGGER_INPUTA));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.89, 86.061)), module, TrackAndField::SAMPLE_INPUTB));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.182, 86.061)), module, TrackAndField::TRIGGER_INPUTB));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(8.89, 45.144)), module, TrackAndField::TF_OUTPUTA));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(8.89, 103.586)), module, TrackAndField::TF_OUTPUTB));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(22.182, 45.144)), module, TrackAndField::STATUS_LIGHTA));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(22.182, 103.586)), module, TrackAndField::STATUS_LIGHTB));
	}
};

Model* modelTrackAndField = createModel<TrackAndField, TrackAndFieldWidget>("TrackAndField");