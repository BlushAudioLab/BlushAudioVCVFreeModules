#include "plugin.hpp"
#include "TrackAndField.hpp"

struct TrackAndField : Module {
	enum ParamId {
		MODE_SWITCH,
		PARAMS_LEN
	};
	enum InputId {
		SAMPLE_INPUT,
		TRIGGER_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		TF_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		STATUS_LIGHT,
		LIGHTS_LEN
	};

	TrackAndField() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(MODE_SWITCH, 0.0f,1.0f,0.0f,"Mode","",0.0f,1.0f,1.0f);
		configInput(SAMPLE_INPUT, "Sample");
		configInput(TRIGGER_INPUT, "Trigger");
		configLight(STATUS_LIGHT, "Status");
		configOutput(TF_OUTPUT, "Output");
	}

	void process(const ProcessArgs& args) override {

		sampleInput = inputs[SAMPLE_INPUT].getVoltage();
		triggerInput = inputs[TRIGGER_INPUT].getVoltage();
		
		mode = params[MODE_SWITCH].getValue() + 1;
		const float gain = 5.f / std::sqrt(2.f);
		
		//Track and Hold mode
		switch (mode){
			case 1:
				//regular old track and hold functionality
				if((inputs[SAMPLE_INPUT].isConnected()) && (triggerInput > threshold)){
				
					outputs[TF_OUTPUT].setVoltage(sampleInput);
					lights[STATUS_LIGHT].setBrightness(sampleInput);

				}
				//use internal white noise source if no sample input is connected
				else if(!(inputs[SAMPLE_INPUT].isConnected()) && (triggerInput > threshold)){
				
					outputs[TF_OUTPUT].setVoltage(white * gain);
					lights[STATUS_LIGHT].setBrightness(white * gain);

				}
				//set everything to 0 if nothing is connected
				else if(!(inputs[SAMPLE_INPUT].isConnected()) && (!(inputs[TRIGGER_INPUT].isConnected()))){
					outputs[TF_OUTPUT].setVoltage(0);
					lights[STATUS_LIGHT].setBrightness(0);
				}
			break;
			case 2:
				//regular old sample and hold functionality
				if ((inputs[SAMPLE_INPUT].isConnected()) && (schmittTrigger.process(triggerInput))){
					outputs[TF_OUTPUT].setVoltage(sampleInput);
					lights[STATUS_LIGHT].setBrightness(sampleInput);
				}
				//use internal white noise source if no sample input is connected
				if (!(inputs[SAMPLE_INPUT].isConnected()) && (schmittTrigger.process(triggerInput))){
					outputs[TF_OUTPUT].setVoltage(white * gain);
					lights[STATUS_LIGHT].setBrightness(white * gain);
				}
				//set everything to 0 if nothing is connected
				else if(!(inputs[SAMPLE_INPUT].isConnected()) && (!(inputs[TRIGGER_INPUT].isConnected()))){
					outputs[TF_OUTPUT].setVoltage(0);
					lights[STATUS_LIGHT].setBrightness(0);
				}
			break;
		}

	}		
};

struct ModeSwitch : app::SvgSwitch {
	ModeSwitch() {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/modebutton.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/modebutton.svg")));
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

		addChild(createParamCentered<ModeSwitch>(mm2px(Vec(8.89, 48.5)), module, TrackAndField::MODE_SWITCH));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.89, 21.269)), module, TrackAndField::SAMPLE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.182, 21.269)), module, TrackAndField::TRIGGER_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(8.89, 38.794)), module, TrackAndField::TF_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(22.182, 38.794)), module, TrackAndField::STATUS_LIGHT));
	}
};

Model* modelTrackAndField = createModel<TrackAndField, TrackAndFieldWidget>("TrackAndField");