#include "plugin.hpp"


struct Coloratura : Module {
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
		BLINK_LIGHT,
		LIGHTS_LEN
	};

	Coloratura() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configInput(PITCH_INPUT, "");
		configOutput(SINE_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
	}
};


struct ColoraturaWidget : ModuleWidget {
	ColoraturaWidget(Coloratura* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Coloratura.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.89, 19.269)), module, Coloratura::PITCH_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(8.89, 36.794)), module, Coloratura::SINE_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(22.182, 36.794)), module, Coloratura::BLINK_LIGHT));
	}
};


Model* modelColoratura = createModel<Coloratura, ColoraturaWidget>("Coloratura");