#include "plugin.hpp"


struct TrackAndField : Module {
	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		PITCH_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		SINE_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		BLINK_LIGHT,
		LIGHTS_LEN
	};

	TrackAndField() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configInput(PITCH_INPUT, "");
		configOutput(SINE_OUTPUT, "");
		configOutput(SINE_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
	}
};


struct TrackAndFieldWidget : ModuleWidget {
	TrackAndFieldWidget(TrackAndField* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/TrackAndField.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.89, 21.269)), module, TrackAndField::PITCH_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(8.89, 38.794)), module, TrackAndField::SINE_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(22.182, 38.794)), module, TrackAndField::BLINK_LIGHT));
	}
};


Model* modelTrackAndField = createModel<TrackAndField, TrackAndFieldWidget>("TrackAndField");